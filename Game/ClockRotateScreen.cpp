/*++++++++++++++++++++++++++++++++++
ClockRotateScreen.cpp

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/


#include "ClockRotateScreen.h"

const int NUM_OF_SPACE_OF_THE_CLOCKROTATE_STRING = 11;

/*-----------------------------------------------------------------------------------------------------------------------
This function get bool value that present if this is the first draw so we need to print the string "Rotate Num: "  before,
or just to print the Clock Rotate in his loaction.
-----------------------------------------------------------------------------------------------------------------------*/
void ClockRotateScreen::Draw(bool IsfirstTime)
{
	int x, y;

	setTextColor(color);
	Place.get(x, y);
	if (IsfirstTime)
	{
		gotoxy(x, y);
		//cout << "Rotate Num: ";
	}

	gotoxy(Place.GetX(), Place.GetY());   // go to the loaction of the number after the string.
	cout << NumClockRotates;
	setTextColor(WHITE);
}

