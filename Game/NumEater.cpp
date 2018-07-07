#include "NumEater.h"
#include "TheMathGame.h"

bool NumEater::Move()
{
	// Get my location
	Point myLocation = GetTopLeft();
	bool Died = false;

	//Adjust the direction based on the target
	AdjustDirection();

	//Check if something interfering on this way to the target
	list<Object *>ObjectsFound;
	CheckMovement(ObjectsFound);

	//If an object is on our way
	if (!ObjectsFound.empty())
	{//If its another num eater, kill him
		KillAnotherNumEater(ObjectsFound);
		Die(); // Then you need to die!
		Died = true;
	}
	// Else , our way is clean, if the number doesnt exist or I finally reached the number
	else if (target == myLocation.next(direction) || !theGame->IsPointNumber(target))
		setTarget(); // Delete the target from screen and get us a new one
	if (!Died)
		DrawMoveOScreen();


	return Died;
}

void NumEater::AdjustDirection()
{
	Point myLocation = GetTopLeft();
	if (target.GetX() != myLocation.GetX())
	{//If I havent reached the X coordinate yet, keep moving horizontal
		direction = Direction::RIGHT;
		ConfirmDirection();
	}
	else
	{//Reached the X coordinate, move vertical
		direction = Direction::UP;
		ConfirmDirection();
	}

}

void NumEater::setTarget(bool DeletePrevTarget)
{// Delete the prev target if needed, set a new target
	if (DeletePrevTarget)
		theGame->removeNum(target);
	// The target will be the median point of the closest number
	target = theGame->GetClosestNumMedianPoint(GetTopLeft());

}
void NumEater::KillAnotherNumEater(list<Object *>ObjectsFound)
{
	for (Object *object : ObjectsFound)
	{
		if (typeid(*object) == typeid(NumEater))
			object->Die();

	}
}