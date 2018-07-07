/*++++++++++++++++++++++++++++++++++
Spaceship.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/

#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_
#include "Object.h"
#include <list>

// Number of times to blink- we want to ship to blink incase lose a strike due to hit a wrong number
const enum { NUM_OF_TIMES_2_BLINK = 4, KEYS_DB_SIZE = 6};

class Spaceship : public Object {
	char ArrowKeys[KEYS_DB_SIZE];		// Keys controls the ship
	int Blinking;      					// Will hold the times left us to blink - ship will blink when it picks up a bad number
	bool AlreadyMoved;       			// Will hold the acknowledgement if the ship already moved on each iteration

public:

	Spaceship(const char *_ArrowKeys, Symbols _Symbol, Color _color) : Object(_Symbol, _color), Blinking(0),AlreadyMoved(false) { setArrowKeys(_ArrowKeys); }
	Spaceship() = default;

	bool Move();																	// Move the ship

	void Die();		                                                               // Function that kill the ship, stops the round and restarts the level
	void Init() { Blinking = 0; Object::Init(); AlreadyMoved = false; }            // Intializes  the ship if needed
	void InitializeMoveChecksum() { AlreadyMoved = false; }         //Initialize the AlreadyMoved boolean


//Keys handling
	int GetDirection(const list<char>& keyHits);
	int GetDirection(const char key);
	int CheckKey(char &key)
	{
		int i;
		i = GetDirection(key);
		if (i != ERROR_CODE)
			return i;
	}
	char GetKey(int index) { return ArrowKeys[index]; }

private:
	void setArrowKeys(const char* keys);	// Sets the arrow keys of the ship

	void MoveToDirection(list<Object *>&ObjectsFound);			     		// Checks if I can move to the requested direction
	bool CheckForArtifactAbove(list<Object *>&ObjectsFound);				// Helper of MoveToDirection (more details at the function itself)
	bool DragArtifact(Object *artifact, bool DisregardVolume = false);		// Function that drags an artifact
	bool DragShip(Object* Ship);											// Function that drags a ship if needed
	void CheckForNumbers();													// Once a move has been validated, check for numbers on our same direction
	inline int MaxVolumePush(int Volume) { return (2 * Volume) / 3; }		// Calculates the maximal volume a ship can push
	void Twist();															// Function that responsible for twisting the ship
	void TwistShipBody();													// Helper of Twist (more details at the fnction itself)
	Object* isFullThrottle(Object *ArtifactToMove);							// Functions that checks if two ships push together
	void Spaceship::GetDiameter(int &Rows, int &Cols);						// Function that calculate the diamter of a spaceship
	void Spaceship::isSomeOneAboveMe(list<Object*>&ObjectsFound);			// Function that checks if there is an object above the spaceship

	void StartBlinking() { Blinking = 2 * NUM_OF_TIMES_2_BLINK; }			// Function that causes a ship to blink once picked a wrong number

	void PerformMove();														// Perform an actual move, if the ship hits a wrong number - we enter to blinking mode
	void RollBackEveryone(list<Object*>&Objects, Object *stuckObject);      // Returns objects that already moved before who met an object that couldnt move - Helper of MoveToDirection

	bool isExistInList(list<Object *>Objects, Object *object);

};


#endif
