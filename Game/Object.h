/*+++++++++++++++++++++++++++++++++
Object.h 

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/


#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <iostream>
#include "io_utils.h"
#include "Direction.h"
#include "Point.h"
#include "CH_Symbols.h"
#include <list>

const enum { ERROR_CODE = -1 };

class TheMathGame;


class Object
{
	Symbols Symbol;																		// Will hold the object's character on board
	Direction previousDirection = Direction::STAY;										// Will hold the  previous direction of the object
	Color color;																		// Will hold the object's color

protected:
	list<Point> body;																	// Will hold the body's coordinates array
	static TheMathGame *theGame;														// Will hold a pointer to the game
	Direction direction = Direction::STAY;												// Will hold the current direction of the object

public:

//------------------------------------------------------
//Functions that responsible for setting up an object
//------------------------------------------------------
	Object(Symbols _Symbol = Symbols::ARTIFACT, Color _color = Color::WHITE) : Symbol(_Symbol), color(_color) {} // Object ctor
	static void setGame(TheMathGame* _theGame); // sets the static pointer to the game for all objects

	//Clean the object if requested and remove all points, basically brings the object to a brand new object status
	virtual void Init() { body.clear(); direction = Direction::STAY; }


//------------------------------------------------------
//Functions that responsible for an object's movement
//------------------------------------------------------
	void DrawMoveOScreen(); // Basic move of an object on the board
	void CheckMovement(list<Object *>& ObjectsFound, const Object* IgnoredObject = nullptr); // Checks if an object can move based on his next direction coordinates
	void CheckForNumbers();																	 // Checks for numbers on the board on the next move location of the object
	void EraseObject(); // Deletes an object from screen
	void DrawObject();	// Prints object to the screen


//----------------------------------------------------
//Functions that sets and gets the object's direction
//--------------------------------------------------

	inline void SetDirection(Direction _direction)
	{ // Sets an object direction
		previousDirection = direction;
		direction = _direction;
	}
	Direction getDirection() { return direction; }					// Get my current direction
	Direction getPreviousDirection() { return previousDirection; }	// Get my previous direction
	void RestoreDirection() { direction = previousDirection; }		// Restore's the direction
	Direction GetOppositeDirection();								// Get the opposite direction

	inline void Stop() {		// Functions that stops the object
		SetDirection(Direction::STAY);
	}
	bool Stopped() { return direction == Direction::STAY; }			// Returns true if stopped
	bool isFalling() { return (direction == Direction::DOWN&& previousDirection==Direction::DOWN); }       // Returns true if object is on falling mode

//---------------------------------------------------------------
//Functions that gets and sets parameters of the body in general
//---------------------------------------------------------------

	inline Point GetTopLeft() { return body.front(); }											// Get my top left coordinate
	inline Point GetBottomRight() { return body.back(); }										// Get my bottom right coordinate
	inline list<Point>& GetBody() { return body; }											   	// Get the list of the body

	//Function overloading - adding a point to the body's list
	void AddPointToBody(int X = 1, int Y = 1,bool updateBoard=true);
	void AddPointToBody(const Point &p, bool updateBoard = true);

	void initializeBody(const Point &p);														 // Function that initializes the artifact's body, it receives one point on the screen and looks for the other connected to it



	inline void SetSymbol(Symbols Symbol2Set) { Symbol = Symbol2Set; }
	inline int GetMyVolume() { return body.size(); }
	inline Symbols GetMySymbol() { return Symbol; }




//-----------------------------------------------
// class virtuals:
//---------------------------------------------
virtual bool Move() = 0; // Moves of inherited objects
virtual void Die() { EraseObject();  body.clear(); }



};



#endif