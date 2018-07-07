/*++++++++++++++++++++++++++++++++++
ScreenNumber.cpp

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/


#include "ScreenNumber.h"



/*----------------------------------------------------------------------
This function print the Screen Number in the first time in his loaction.
-----------------------------------------------------------------------*/
void ScreenNumber::Draw()
{
	int x, y;
	Place.get(x, y);
	setTextColor(color);

	gotoxy(x, y);
	cout << "ID: ";


	gotoxy(x + NUM_OF_SPACES_TO_SCREENNUM_STRING, y);
	cout << ScreenNum;
	setTextColor(WHITE);

}

