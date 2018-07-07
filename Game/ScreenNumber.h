/*++++++++++++++++++++++++++++++++++
ScreenNumber.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/

#ifndef _SCR_H_
#define _SCR_H_

#include "Point.h"

const int NUM_OF_SPACES_TO_SCREENNUM_STRING = 3;

class ScreenNumber
{
	unsigned int ScreenNum;
	Color color;
	Point Place;

public:
	ScreenNumber() : color(Color::YELLOW) {}
	inline void SetPlace(int x, int y) { Place.set(x, y); }
	inline void SetScreenid(size_t screen_id) { ScreenNum = screen_id; }
	unsigned int GetID() { return ScreenNum; }
	//void UpdateAndDraw(unsigned int numscreen) { ScreenNum = numscreen; Draw(false); }

	void Draw();

};



#endif
