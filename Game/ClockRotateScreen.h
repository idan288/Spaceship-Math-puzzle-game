/*++++++++++++++++++++++++++++++++
ClockRotateScreen.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
+++++++++++++++++++++++++++++++++*/



#ifndef _CLOCKROTATE_H_
#define _CLOCKROTATE_H_

#include "Point.h"


class ClockRotateScreen
{
	unsigned int NumClockRotates;
	Point Place;
	Color color;


public:
	ClockRotateScreen() : color(Color::LIGHTCYAN) {}
	void Draw(bool IsfirstTime);

	inline void SetPlace(int x, int y) { Place.set(x, y); }
	inline void UpdateNumClockRotates() { ++NumClockRotates; Draw(false); }
	inline unsigned int GetNumClockRotates() const { return NumClockRotates; }
	inline int GetX() { return Place.GetX(); }
	inline void ResetNumClockRotates(int Val = 0) { NumClockRotates = Val; }
	inline int GetY() { return Place.GetY(); }
};

#endif