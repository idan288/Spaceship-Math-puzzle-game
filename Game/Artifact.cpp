#include "Artifact.h"
#include "TheMathGame.h"

bool Artifact::Move()
{// isPushing will hold the acknowledgement if someone is pushing us
 //If someone pushes us, his volume will be sent through here

	list<Object *>ObjectsFound;							         	// Will hold the list of objects on our way
	bool InputFromShip = false, MeetWall = false;	                // Validates if the objects we are meeting are able to move or not


	isArtifactsAroundMe();											// Check for Artifacts around this one

	if (!Pushed)													// If someone forced the artifact to move, this will be true
		SetDirection(Direction::DOWN);								// So dont change your direction back to down (becuase someone is pushing)

	CheckMovement(ObjectsFound);			// Start checking whos infront of our way

	for (Object *object : ObjectsFound) 	// Run on the objects found
	{
		if (typeid(*object) == typeid(Spaceship))		   // if its a spaceship
			InputFromShip = HandleShipAhead(object);	  //Handle it as needed, returns true if ship cannot move

		else if (typeid(*object) == typeid(NumEater))	// If its a num eater,It should cease to exist
			object->Die();	

		else if(object==this)		// Otherwise the other object is this, sign of a wall for us
			MeetWall = true;	   	// Need to change to stop


	}
	AdjustDirection(InputFromShip, MeetWall);	// Last Validation if we should move or not
	DrawMoveOScreen();

	Pushed = false;											// Init pushed

	return(Stopped());
	
}





bool Artifact::HandleShipAhead(Object *ShipBody)
{// Handles a situation when artifact meets a spaceship
	bool ShouldChangeToStay = false;
	Point p= GetTopLeft();
	if (!Pushed && isFalling()) // We are on dropping mode, no one forced us to move, kill the ship!
		ShipBody->Die();

	else if (Pushed &&  ShipBody->getDirection() == direction)    // If the ship and me heading to the same location,and someone forced the artifact to move
	{	    														// then we will move the ship now and she will skip her iteration as we are moving her now
		ShouldChangeToStay = ShipBody->Move();				     	// Moving the ship...	Checks if the ship is facing something - if it is, will return to ShouldChangeToStay									  							
		                                                            //                                                                                                                                             *************
		if (!(GetTopLeft() == p))									// This check we are doing here is because there is a special case, when 2 ships dragging an artifact together , for example above their heads   ###   @@@
			ShouldChangeToStay = true;								// The first one tries to move the artifact, it tells the artifact to move down. during the artifact movement,the artifact meets the second ship, it tries to move it down as well. When  the second ships tries to move,it is now trying to drag down 
	}																// The very same artifact ! (because it is also above her head), the result is that same artifact being dragged down twice - each time by each ship
																	// Solution is: if my top left has changed during the movement of the ship im trying to drag  it means i moved by the ship, dont move again...
	else
		ShouldChangeToStay = true;									// Else  - we shouldnt kill the ship, its not aheading to the same direction as us, need to stop!

	return ShouldChangeToStay;
}








void Artifact::AdjustDirection(bool InputFromShip, bool MeetWall)
{

	// Last validation if the artifact can move or not.
	// If the artifact wasnt on falling mode and it lands on a number, it should stop
	// If the artifact met a wall or it met a ship which it coudlnt move

	if (getPreviousDirection() == Direction::STAY&& direction == Direction::DOWN && isSomethingBelow(WhatToCheck::NUMBERS)) // If my previous direction was stay and now im falling and there is a number below,
		Stop();						 								                                                      //  it means i was not on dropping mode on last iteration,Dont remove the number from the screen
	else
		CheckForNumbers();
	if (MeetWall || InputFromShip)		 // If we met a ship or a wall
		Stop();
}






bool Artifact::isSomethingBelow(WhatToCheck object) 
{// Functions that only check if there is a number below the artifact or a spaceship, depands on the calling function request
	Point pDown;

	for (Point &p : body)
	{
		pDown = p.next(Direction::DOWN);
		if (object == WhatToCheck::SHIPS)
			return(theGame->isShip(pDown));
		else
			return(theGame->IsPointNumber(pDown));
	}
	return false;
}




void Artifact::isArtifactsAroundMe()
{//Function that merges nearby artifacts
	list <Artifact *>ArtifactsFound;						// Holds all the nearby artifacts found
	Object *object;
	bool ShipBelow = isSomethingBelow(WhatToCheck::SHIPS);	// Validates if we are sitting on a spaceship
	for (Direction d : AllDirections)
	{
		for (Point &p : body)
		{
			object = theGame->isObject(p.next(d));											 // Take the address of the object nearby us
			if (object != nullptr && typeid(*object) == typeid(Artifact) && object != this)  // If an artifact located on this point and its not me
			{
				ArtifactsFound.push_back(dynamic_cast<Artifact *>(object));                  // Add it to the list
			}

		}

	}
	ArtifactsFound.unique(); // Remove duplications to avoid double checks.
	for (Artifact *a : ArtifactsFound)
	{
		if (ShipBelow || !a->isSomethingBelow(WhatToCheck::SHIPS))			// If I have a Ship below me,forcing me to move
			body.splice(body.begin(), a->body);		                       // Merge the artifacts
	}
}
