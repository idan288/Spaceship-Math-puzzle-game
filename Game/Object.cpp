/*++++++++++++++++++++++++++++++++++
Object.cpp

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/

#include "Object.h"
#include "TheMathGame.h"

TheMathGame* Object::theGame = nullptr;
void Object::setGame(TheMathGame* _theGame) { theGame = _theGame; }

//------------------------------------------------------
//Functions that responsible for drawing the object
//------------------------------------------------------
void Object::EraseObject()
{// Delete the object from screen

	for (Point &p : body)							// Run on the body's list
	{

		p.draw(Symbols::SPACE);						// Delete my coordinate
		theGame->UpdateBoardCordinate(nullptr, p);	// Update the board that I am not longer there
		p.Move(direction);							// Move to the next direction and be prepered for Draw() to come!

	}

}

void Object::DrawObject()
{// Draw the object on screen

	setTextColor(color);						// Set the color of the printed the text
	for (Point &p : body)						// Run on the body's array
	{
		p.draw(Symbol);									// Draw the coordinate
		theGame->UpdateBoardCordinate(this, p);			// Update the board that we have taken over this coordinate

	}

	setTextColor(DEFAULT_COLOR);						// Restore color

}
void Object::DrawMoveOScreen()
{// Function that basically moves the object

	if (!Stopped())		                   // If my direction is not stay
	{
		EraseObject();						// Delete me
	}
	DrawObject();							// Print me on the next coordinates

}

//-----------------------------------------------------------------------
// E    N    D         O      F
//------------------------------------------------------
//Functions that responsible for drawing the object
//------------------------------------------------------
// E    N    D         O      F
//------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------------------------
//Functions that responsible for manage and control of an object's movement - validate an object movement, 
//identify objects on the same coordinates
//--------------------------------------------------------------------------------------------------------------

void Object::CheckMovement(list<Object *>& ObjectsFound, const Object* IgnoredObject)
{// Functions that tell if my next step is legal, IgnoredObject can be received by the calling function
 // Ignored Object - If the calling function dont want to get the addresss of something specfically, through IgnoredObject I will know what it is


	Object* object = nullptr;	// Will hold the address returned from the pointer's board located in TheMathGame class


	for (Point &p : body)
	{
		object = theGame->isObject(p.next(direction));							// Check if there is any object location in one of the coordinates the object is heading to
		if (object != nullptr && object != this && object != IgnoredObject)		// If the returned address is not null, not me, and not an object I would like to ignore from, its something worth checking
		{
			// If the object is an artifact, push it to the end
			// If the object is a spacship, push it to the front, meeting spaceship is more important than artifact

			if (typeid(*object) == typeid(Artifact))
				ObjectsFound.push_back(object);
			else // Object is a spaceship
				ObjectsFound.push_front(object);
		}

		else if (theGame->isThisSymbol(p.next(direction), Symbols::WALL))	// Here we check for a wall on the same iteration
		{
			if (IgnoredObject == nullptr)		// If ignoredObject is not nullptr, it means someone called this function in order to
				ObjectsFound.clear();			// Know what objects are infront of this object (meaning the calling object is not this)
												// Meaning he needs to all the pointers found to gather information from the object's status
			ObjectsFound.push_back(this);
			return;
			// If a wall has been found, returning 'this' will be the sign that we hit a wall
			// No need to keep running once we hit the wall.
		}

	}
	ObjectsFound.unique();		// Clean the list from duplications
}

void Object::CheckForNumbers()
{
	for (Point &p : body)
	{
		if (theGame->IsPointNumber(p.next(direction)))
			theGame->removeNum(p.next(direction));
	}
}
Direction Object::GetOppositeDirection()
{ // Get the opposite direction of my current direction

	switch (direction)
	{
	case(Direction::LEFT) :
		return (Direction::RIGHT);
		break;
	case(Direction::RIGHT) :
		return (Direction::LEFT);
		break;
	case(Direction::UP) :
		return (Direction::DOWN);
		break;
	case(Direction::DOWN) :
		return(Direction::UP);
		break;
	default:
		return direction;
		break;
	}

}

//--------------------------------------------------------------------------------------------------------------
// E    N    D         O      F
//-------------------------------------------------------------------------------------------------------------
//Functions that responsible for manage and control of an object's movement - validate an object movement, 
//identify objects on the same coordinates
//--------------------------------------------------------------------------------------------------------------
// E    N    D         O      F
//--------------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------
//Functions that manages the object's body list
//--------------------------------------------------------------------------------------------------------------


void Object::AddPointToBody(int X, int Y,bool updateBoard)
{ // Get another coordinate of the body from the board and save it
	body.push_back(Point(X, Y));
	theGame->UpdateBoardCordinate(this, Point(X, Y));
	return;
}
void Object::AddPointToBody(const Point &p, bool updateBoard)
{ // Get another coordinate of the body from the board and save it
	body.push_back(p);
	if(updateBoard)
	theGame->UpdateBoardCordinate(this, p);
	return;
}


void Object::initializeBody(const Point &p) // Function that initializes the artifact's body, it receives one point on the screen and looks for the other connected to it
{
	if (theGame->isObject(p) != nullptr || !theGame->isThisSymbol(p, Symbol)) // Point is already taken
		return;
	else // Point is not taken, lets take it!
	{
		AddPointToBody(p);
		for (Direction direction : AllDirections)
			initializeBody(p.next(direction));

	}
}
//--------------------------------------------------------------------------------------------------------------
// E    N    D         O      F
//-------------------------------------------------------------------------------------------------------------
//Functions that manages the object's body list
//--------------------------------------------------------------------------------------------------------------
// E    N    D         O      F
//--------------------------------------------------------------------------------------------------------------

