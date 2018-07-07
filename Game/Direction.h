/*++++++++++++++++++++++++++++++++
Direction.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/
#ifndef _DIRECTION_H_
#define _DIRECTION_H_

enum class Direction { UP, RIGHT, DOWN, LEFT, STAY, TWIST };
const int DIRECTION_DB_SIZE = 4;
const Direction AllDirections[DIRECTION_DB_SIZE] = { Direction::DOWN, Direction::LEFT, Direction::RIGHT, Direction::UP };
#endif

