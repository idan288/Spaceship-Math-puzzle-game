/*++++++++++++++++++++++++++++++++++
Spaceship.cpp

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/




#include "Spaceship.h"
#include "TheMathGame.h"
//------------------------------------------------------
//Functions that responsible to get the pressed keys and directions from the user
//------------------------------------------------------
int Spaceship::GetDirection(const char key)
{
	int i;
	for (i = 0; i < KEYS_DB_SIZE; i++)
	{
		if (key == ArrowKeys[i])
			return i;
	}
	return ERROR_CODE;

}

int Spaceship::GetDirection(const list<char>& keyHits)
{
	int i;
	for (char key : keyHits)
	{
		i = GetDirection(key);
		if (i != ERROR_CODE)
			return i;
	}

	return ERROR_CODE;
}

void Spaceship::setArrowKeys(const char* keys) {
	for (int i = 0; i < KEYS_DB_SIZE; i++)
		ArrowKeys[i] = keys[i];
	return;
}
// E    N    D         O      F
//------------------------------------------------------
//Functions that responsible to get the pressed keys and directions from the user
//------------------------------------------------------
// E    N    D         O      F


void Spaceship::Twist()
{ // Responsible for twisting the ship once pressed by user

	list<Object *>ObjectsFound;
	isSomeOneAboveMe(ObjectsFound);
	if (ObjectsFound.empty())						        // If there is no artifact above my head, keep checking
	{
		EraseObject();										// First delete the current location
		TwistShipBody();									// Twist the ship's body

		CheckMovement(ObjectsFound);						// Will check if something is interfering the ship to twist
		if (!ObjectsFound.empty())			                // If the list is not empty, something is interfering, rollback.
			TwistShipBody();
	}

	PerformMove();								//Perform moving
	RestoreDirection();							//Restore the direction before player pressed twist
	return;
}

void Spaceship::TwistShipBody()
{ // Peforms the actual twist of the object (the ship)
	int x, y;		// Will hold the coordinates of the ship's top left coordinate
	int Rows, Cols; // Will hold the current ship's rows and cols based on the distance from the top left to the bottom right coordinate

	GetDiameter(Cols, Rows);										// Get the ships diameter, but swap between Cols and Rows (we sent Cols on Row's location and same for the rows , the signature of GetDiameter is: void GetDiameter(int &Rows, int &Cols).
	auto *p = body.begin()._Ptr;								// Get the list head


	GetTopLeft().get(x, y);											// Get the top left coordinate, this will remain the top left coordinate also after the twist
	for (int i = 0; i < Rows; i++)							        // Set all the ship's coordinate by relying on the top left coordinate, the next coordinate in the ship's body will be x+i,y+j  
	{
		for (int j = 0; j < Cols; j++)
		{
			p->_Myval.set(x + j, y + i);
			p = p->_Next;
		}
	}
}
void Spaceship::GetDiameter(int &Rows, int &Cols)
{//Function that returns the diameter of a rectangled spaceship

	int x1, y1, x2, y2;


	GetTopLeft().get(x1, y1);
	GetBottomRight().get(x2, y2);

	Rows = y2 - y1 + 1;
	Cols = x2 - x1 + 1;

}
// E    N    D         O      F
//------------------------------------------------------
//Functions that responsible for twisting the ship
//------------------------------------------------------
// E    N    D         O      F




//------------------------------------------------------
//Functions that responsible for general movement iteration of the ship
//------------------------------------------------------
bool Spaceship::Move()
{
	bool AreWeFacingABlocker = true;		// Will return true if we hit something
	Direction HoldDirection = direction;
	if (!AlreadyMoved)     					// If we havent moved yet or someone is forcing us to move (example of force moving is when the ship was on stuck mode and now dragged (by the second ship) artifact hits it)
	{
		AlreadyMoved = true;

		list <Object *>ObjectsFound;
		if (direction == Direction::TWIST)
			Twist();
		else
		{
			MoveToDirection(ObjectsFound);														// Set the next direction based on the board status (allow the ship to move to the requested direction or stop it)
																					// Moreover, if an artifact is on my direction, move it, then move me! (it will happen in MoveToDirection() )

			if (direction == Direction::UP || !CheckForArtifactAbove(ObjectsFound))				// If artifact hasnt been found above my head, need to move the ship, otherwise (artifact has been found) , the ship will move at IsSomeoneAboveMe()
				PerformMove();														// Moreover, if my direction is up, I dont need to check above my head

			AreWeFacingABlocker = Stopped();                        			// Right before you move, initialize the boolean
			direction = HoldDirection;											// Restore my previous direction (if it has been changed to STAY @ MoveToDirection() )
		}

	}
	return (AreWeFacingABlocker);												// Return true if we couldnt move

}

void Spaceship::MoveToDirection(list<Object *>&ObjectsFound)
{// Function that responsible for validating our move (that we can go ahead to the next direction) if the functions meets an artifact, it will drag it as well
 // Objects Found Will hold the returned object address (if there is any object) that is located on of the coordinates that I am heading to at the board

	bool ShouldStop = false;
	CheckMovement(ObjectsFound);					// Check if there is any object or non-moving object located on one of the coordinates I am heading to
	Object *stuckObject = nullptr;					// If one of the objects couldnt move, need to rollback the ones who already moved
	for (Object *object : ObjectsFound)				 // If object!=nullptr, means there is something on my direction (wall, artifact or a ship)
	{

		// Check what we hit:

		if (typeid(*object) == typeid(Artifact))
			// Object is an artifact:
			ShouldStop = DragArtifact(object);			                                // 2 terms for a ship to stop, in case we are facing an artifact:
																						//   A.Were moving an artifact and the artifact's volume is bigger than my volume
																						//   B.The artifact is facing a wall or something, moving him can cause  a bug
	
		else if (typeid(*object) == typeid(NumEater))									// If the object is a num eater, need to kill this damn create right now!
			object->Die();																// Kill him!
		
		else if (typeid(*object) == typeid(Spaceship) && object!=this)		            // Dragging another ship (happens only when other ship hasnt moved yet and its direction is the same as mine)															      							       
			ShouldStop = DragShip(object);												// Othership cant move at this momment will return true										    

		else                                                                            //Otherwise the object is ourselves,meaning its a wall..
			Stop();
		if (ShouldStop)																	// If one of the dragged objects didnt move, then we need to stop too.
		{
			stuckObject = object;
			break;
		}
	}
	if (stuckObject != nullptr && stuckObject != this)// If there's an object that got stuck (and its not me, meaning its not a a wall), need to rollback all the ones who already moved successfully before it
	{
		RollBackEveryone(ObjectsFound, stuckObject);
		Stop();
	}



}
void Spaceship::RollBackEveryone(list<Object*>&Objects, Object *stuckObject)
{// Returns objects that already moved before who met an object that couldnt move - Helper of MoveToDirection

	direction = GetOppositeDirection();
	Object *object;
	for (auto itr = Objects.begin(); itr._Ptr->_Myval != stuckObject; itr++)
	{
		object = itr._Ptr->_Myval;
		if (typeid(*object) == typeid(Artifact))
			DragArtifact(object, true);
		else
			DragShip(object);

	}
}
bool Spaceship::DragArtifact(Object *artifact, bool DisregardVolume)
{
	Artifact *art = dynamic_cast<Artifact *>(artifact); // Will hold the casting to the artifact if needed to do casting
	int MyVolume;								    	// Will hold the ships volume
	Object* HelpingShipAddress = nullptr;		    	// Will check if any ship is helping us to drag
	bool cantMove = true;						        // Will return true if the dragging object didnt move we are not authorized to move
	Direction ArtifactDirection = artifact->getDirection();	// Will temporarly hold the artifact's direction to restore it after proccess

	if (!DisregardVolume)
	{// If we are dragging an artifact infront of the ship, we care about our volume, hence the check below
		MyVolume = GetMyVolume();

		if ((HelpingShipAddress = isFullThrottle(artifact)) != nullptr)// If another ship has been found that drags the artifact
			MyVolume += HelpingShipAddress->GetMyVolume();		 	   // Take its volume and add it to mine
		MyVolume = MaxVolumePush(MyVolume);							   // Calculate the volume alltogether based on the formula given
	}

	else // If we drag above our the ship's head, we do not care from the volume , take something that will be sufficient for the inequation below to be true
		MyVolume = artifact->GetMyVolume();

	//If our volume is sufficient to push the artifact and the casting went successfully
	if (MyVolume >= artifact->GetMyVolume()&& art)
	{
		art->setPushed();
		art->SetDirection(direction);				     		// Set the object direction to my direction
		cantMove = art->Move();									// Try to move the object

		art->SetDirection(ArtifactDirection);			    // Restore the object's direction
		if (HelpingShipAddress != nullptr && !cantMove)	  	// If we are authorized to move, force the helping ship to move now as well
			DragShip(HelpingShipAddress);
	}
	return cantMove;
}

void Spaceship::PerformMove() {
	// Perform an actual move, if the ship hits a wrong number - we enter to blinking mode

	CheckForNumbers(); // Check for numbers on our direction before perform an actual move
					   //Do a normal move of the ship

	if (Blinking % 2 == 0)				// %2 because this give us the effect that the ship is blinking (one iteration - dont draw the ship, the next one- draw it) , all the odd indexes will be the times that the ship is beling erased only
		DrawMoveOScreen();
	else                               // Its a time to blink! draw a space instead of a ship
	{
		Symbols temp = GetMySymbol();
		SetSymbol(Symbols::SPACE);
		DrawMoveOScreen();
		SetSymbol(temp);

	}
	if (Blinking > 0)
		Blinking--;
}


//Function that drags a ship by force 
//These function is needed for cases like when 2 ships pushing an artifact together. Because on the doIteration , the ships moved one after one
//and not on the same time, it is likely that one ship will try to move an artifact and it will succeed, but the second ship will be now far in one point from the artifact
//As this ship and the dragged artifact already moved, so it will not see it.
//Hence the first ship that touchs the artifact will move the second one, and then the second one will NOT try to move the other.

//Another reason to move a ship is if this ship hits the second one, and the second ship is heading the same direction as me, this ship will force the second one to move, then it will make space for this ship to move

bool Spaceship::DragShip(Object* Ship)
{
	bool cantMove = false;
	Spaceship *ship= dynamic_cast<Spaceship *>(Ship);
	if (Ship->getDirection() == direction && ship)	// If the ship's direction is the same as this one and succeed to do casting, move it now
	{
		ship->AlreadyMoved = false;					// Init the AlreadyMoved boolean of the ship because we want to force it to move
		cantMove = Ship->Move();
	}
	else// Our directions isnt the same, then dont move
		cantMove = true;

	return cantMove;
}
void Spaceship::CheckForNumbers()
{
	//Check if digits located on my next coordinates
	for (Point &p : body)				// run on my body's coordinate and check for numbers on the direction i am aheading to, treat them as well
	{
		if (theGame->isNumber(p.next(direction)) == Symbols::BAD_NUMBER)
			StartBlinking();
	}
}


Object* Spaceship::isFullThrottle(Object *ArtifactToMove) {
	// Function that checks if two ships are pushing an artifact or not, if another ship is helping this ship to push, will return the volume of the helping ship

	list<Object*>ObjectsFound;										// Will hold the objects found locaed on the opposite direction of the artifct
	Object* SpaceshipFound = nullptr;
	Direction ArtifactDir = ArtifactToMove->getDirection();			// Save the artifact's direction

	ArtifactToMove->SetDirection(GetOppositeDirection());					// Set the direction of the artifact to be the opposite of this ship
	ArtifactToMove->CheckMovement(ObjectsFound, this);				        // Now that the artifact's next direction is where I am at, check if someone is one his way excpet me, CheckMovement will ignore my spaceship address in case it will find me
	for (Object *object : ObjectsFound)
	{
		if (typeid(*object) == typeid(Spaceship) && object->getDirection() == direction) // If a ship has been found and we are heading to the same locaton
			SpaceshipFound = object;

	}


	ArtifactToMove->SetDirection(ArtifactDir);											    // Restore the artifact's direction
	return (SpaceshipFound);																// Return nullptr or the ship's volume
}
// E    N    D         O      F
//------------------------------------------------------
//Functions that responsible for general movement iteration of the ship
//------------------------------------------------------
// E    N    D         O      F

bool Spaceship::CheckForArtifactAbove(list<Object *>&ObjectsFound)
{// Function that checks the area above the ship, if there is any object there that we need to carry
 // ObjectsFound Will hold the list of artifacts that already found in MoveToDirection
	bool MovedOnce = false;							// If an artifact has been found, I want to move first, then I will give all the artifacts above me to move
	list<Object*> ObjectsFoundSecond;				// Create a list of objects above my head
	isSomeOneAboveMe(ObjectsFoundSecond);			//  Check if someone is above me and return me his address		


	for (Object *object : ObjectsFoundSecond)									    	//	If object an artifact has been found
	{
		if (!isExistInList(ObjectsFound, object))										// If the object was exist on the list found by MoveToDirection, no need to check here again as already know this object.
		{																				// We know that this checking method is not very efficient, but since the board size is constant, everything is constat, in addition we didnt learn STL yet so we dont know all tricks, only google.


			if (object->isFalling())		                    							//	If the artifact is on movement, facing down and no one (by no one we mean the spaceships),
				Die();																//  forced the artifact to move down, then it means the artifact fell on our head and we lose a strike :(


			else																			//  The artifact is not facing down, we shouldn't lose a strike,
																							//  if my direction is not up then I didnt move the artifact yet on function MoveToDirection()
			{//Its an artifact and it shouldnt kill us, move him
				if (!MovedOnce)// If I havent move yet
				{
					PerformMove();															//	The ship will move first (this because when moving down, ship must move first)
					MovedOnce = true;														//	Anounce that the ship moved once, if there is another artifact to move, dont move again
				}
				DragArtifact(object, true);								     			    //	Move the object above me to the same location that I move

			}
		}
	}

	return MovedOnce;	// Return true if you moved
}
bool Spaceship::isExistInList(list<Object *>Objects, Object *object)
{// Checks if an object already exist in a certain list
	std::list<Object *>::iterator findIter = std::find(Objects.begin(), Objects.end(), object);
	if (findIter != Objects.end())
		return true;		// Object exist
	else
		return false;	// Object doesnt exist
}
void Spaceship::isSomeOneAboveMe(list<Object*>&ObjectsFound)
{// Function that returns an address of an artifact (if there is any) that is above this object


	int Y;							// Y will hold the Y value of the top left's coordinate
	Object *object;					// Will hold the returned value from isObject, by that we will know if anything is above the ship,
	Y = GetTopLeft().GetY();		// Because if there is anything above the ship, isObject will return its adddress

	for (Point &p : body)			// Run on the body's list
	{
		if (p.GetY() == Y)	// Each coordinate on the body's array that share with top left the same Y value, its a coordinate we need to check above it
		{
			object = theGame->isObject(p.next(Direction::UP));			                // Check above the coordinate
			if (object != nullptr && typeid(*object) == typeid(Artifact))		       // If there is artifact over there, return its address
				ObjectsFound.push_back(object);
		}

	}
	ObjectsFound.unique();	// Clear duplications
	return;

}
void Spaceship::Die()
{
	EraseObject();
	GetTopLeft().Go();
	setTextColor(Color::LIGHTRED);
	cout << "BOOM!";
	Sleep(500);
	theGame->ShipGotHit();
}