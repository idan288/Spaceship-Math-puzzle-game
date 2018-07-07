#ifndef FLYINGCREATURE_H_
#define FLYINGCREATURE_H_

#include "Object.h"

class FlyingCreature :public Object
{

public:
	FlyingCreature(const Point & Location, Symbols symbol, Color color, Direction dir) :Object(symbol, color) { SetDirection(dir); initializeBody(Location); }
	bool Move();
	
};


#endif