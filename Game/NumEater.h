#pragma once
#ifndef _NUM_EATER_H
#define _NUM_EATER_H

#include "Object.h"

class NumEater : public Object {
	Point target;

	void AdjustDirection();										// Adjust the direction of the object
	void setTarget(bool DeletePrevTarget = true);				// Set a new target
	void KillAnotherNumEater(list<Object *>ObjectsFound);		// If I hit another object and its an num eater, kill it

	void ConfirmDirection()										// Validates the direction , if the direction chosen was correct
	{															// If the direction this object is heading is the wrong one, go to the other side
		Point myLocation = GetTopLeft();
		if (target.GetDistance(myLocation) < target.GetDistance(myLocation.next(direction)))
			direction = GetOppositeDirection();
	}

public:
	NumEater(Point _Location) : Object(Symbols::NUM_EATER, Color::LIGHTMAGENTA) { initializeBody(_Location); setTarget(false); }
	bool Move();

};

#endif