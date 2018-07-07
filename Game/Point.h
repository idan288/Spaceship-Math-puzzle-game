/*++++++++++++++++++++++++++++++++++
Point.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/

#ifndef _POINT_H_
#define _POINT_H_

#include "Direction.h"
#include "io_utils.h"
#include <iostream>
#include "CH_Symbols.h"
#include "ScreenSize.h"
using namespace std;

class Point {
	int x, y; // initializee values just to avoid bad cordinations.
public:

	Point(int _x=0, int _y=0) { set(_x, _y); } // constructor
	Point(const Point&point) { x = point.x; y = point.y; } // copy ctor
	Point& operator=(const Point& p) { x = p.x; y = p.y; return *this; } // copy assigment ctor
	bool operator==(const Point& p)const { if (x == p.x && y == p.y) return true; else return false; } // checks if two points are equal
	~Point() = default;



	bool set(int _x, int _y);
	void Move(Direction direction); // move to a specific direction
	void get(int &_x, int &_y) const // get coordinates and set X and Y
	{
		_x = x;
		_y = y;
	}
	void draw(Symbols Symbol) { // draw a point
		gotoxy(x, y);
		cout << (char)Symbol << endl;
	}
	Point next(Direction direction)const {
		Point next = *this;
		next.Move(direction);
		return next;
	}

	int GetX() const { return x; }
	int GetY() const { return y; }
	void Go() { gotoxy(x, y); }

	double GetDistance(const Point &p)
	{
		int A, B;
		double result;
		A = p.x - x;
		B = p.y - y;

		result = pow((double)A, 2.) + pow((double)B, 2.);
		return(sqrt(result));

	}


};

#endif


