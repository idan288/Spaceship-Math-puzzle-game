
#include "FlyingCreature.h"
#include "TheMathGame.h"

bool FlyingCreature::Move()
{
	list<Object *>ObjectsFound;
	bool ShipIsDie = false;

	CheckMovement(ObjectsFound);


	for (Object *object : ObjectsFound)
	{
		if (object != this)		// If we touch something that its not a wall, kill it
			object->Die();
	}
	CheckForNumbers();
	DrawMoveOScreen();

	return ShipIsDie;
}