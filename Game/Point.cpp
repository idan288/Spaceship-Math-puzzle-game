/*++++++++++++++++++++++++++++++++++
Point.cpp

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/

#include "Point.h"

bool Point::set(int _x, int _y)
{
	if ((_x >= BOARD_MIN_VALUE&& _x <= COLS) && (_y >= BOARD_MIN_VALUE && _y <= ROWS))
	{
		x = _x;
		y = _y;
		return true; // succeed setting the coordinates
	}
	else
	{// Invalid parameters, enter the default to avoid the program from failing
		x = BOARD_MIN_VALUE;
		y = BOARD_MIN_VALUE;
	}
	return false;
}
void Point::Move(Direction dir)
{
	switch (dir) {
	case Direction::UP:
		--y;
		if (y < BOARD_MIN_VALUE) {
			y = ROWS - 1;
		}
		break;
	case Direction::DOWN:
		++y;
		if (y > ROWS - 1) {
			y = BOARD_MIN_VALUE;
		}
		break;
	case Direction::LEFT:
		--x;
		if (x < BOARD_MIN_VALUE) {
			x = COLS - 1;
		}
		break;
	case Direction::RIGHT:
		++x;
		if (x > COLS - 1) {
			x = BOARD_MIN_VALUE;
		}
		break;
	case Direction::STAY:
		break;

	}
}
