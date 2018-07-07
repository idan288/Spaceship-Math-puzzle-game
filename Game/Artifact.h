#ifndef _ARTIFACT_H_
#define _ARTIFACT_H_
#include "Object.h"
enum WhatToCheck { SHIPS,NUMBERS};
class Artifact : public Object {
	bool Pushed=false;				// Will acknowledge is someone is pushing this artifact by force
public:
	Artifact(const Point&p) : Object(Symbols::ARTIFACT, Color::YELLOW) { initializeBody(p); }
	bool operator==(const Artifact& p) { if (this == &p) return true; else return false; } // checks if artifact's are equal based on their addresses, we need this function for managing the list of artifact's, functions such as list.remove(x) requires it
	bool Move();
	void setPushed() { Pushed = true; }

private:
	void AdjustDirection(bool InputFromShip = false, bool MeetWall = false);        // function that gathers all the reasons for a artifact to stop moving and examines all of them together, by using this function we can detect bugs better when artifact stops
	bool HandleShipAhead(Object *ShipBody);											// Handles a situation when artifact meets a spaceship
	void isArtifactsAroundMe();

	//Chceck if a certain object is below this artifact
	bool isSomethingBelow(WhatToCheck object);


};


#endif