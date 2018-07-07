#pragma once
#ifndef _NUM_H_
#define _NUM_H
#include "Point.h"
class TheMathGame;
class Num {
	static TheMathGame *theGame;
	Point Left, Right;
	int val;

private:
	//Finds the left and right borders of the number on the board (screen)
	void CreateNumberBorderLeftAndRight(Point const & point);
	//This function crate the number form its border.
	int GetNum();
public:
	static void setGame(TheMathGame* _theGame); // sets the static pointer to the game for all objects
	Num() = default;
	Num(const Point&p) {
		CreateNumberBorderLeftAndRight(p); val = GetNum();}		// Builds a number on board only from one point
	bool operator==(const Num &p) {									// compeares two numbers, if the p.LeftX is bigger than my Left and the p.RightX is smaller than mine and our Y's location is the same, its mean the number is within range, and its me!
		if (Left.GetX() <= p.Left.GetX() && p.Right.GetX()<=Right.GetX() && Left.GetY() == p.Left.GetY()) return true; else return false; }

	void GetNumMedianPoint(Point &res)const { res.set((Left.GetX() + Right.GetX()) / 2, Left.GetY()); }		// Returns the median point of the number
	Point GetTopRight()const { return Right; }
	Point GetTopLeft()const { return Left; }
	int GetVal() const { return val; }

};

#endif