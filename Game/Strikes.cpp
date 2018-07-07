/*++++++++++++++++++++++++++++++++++
Strikes.cpp

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/

#include "Strikes.h"

const int NUM_OF_SPACES_TO_STRIKE_STRING = 8;

/*------------------------------------------------
This function print the strikes in their loaction.
-------------------------------------------------*/
void Strikes::Draw()
{
	int x, y;

	setTextColor(color);

	StrikesPlace[0].get(x, y);
	gotoxy(x, y);

	gotoxy(StrikesPlace[0].GetX(), StrikesPlace[0].GetY());

	for (int i = 0; i < NumStrikes; ++i)                           //print all the strike to the screen.
	{
		cout << strikes[i];
	}

	setTextColor(WHITE);
}

/*--------------------------------------------------------------
This function get a Symbol and color and seting up the strikes.
---------------------------------------------------------------*/
void Strikes::SetStrikesChar(Symbols symbol)
{

	for (int i = 0; i < NumStrikes; ++i)
	{
		strikes[i] = (char)symbol;
	}

}


/*-------------------------------------------------------------------------
This function get intager x and y and seting up the location of each strike.
---------------------------------------------------------------------------*/
void Strikes::SetStrikesPlace(int x, int y)
{
	for (int i = 0; i < NumStrikes; ++i)
	{
		StrikesPlace[i].set(x + i, y);   //the places set acoording to the string "Strikes : "  that prints before. 
	}
}


/*-----------------------------------------------------------------------------------
This function clear one strike from the screen and update the new number of strikes.
-------------------------------------------------------------------------------------*/
void Strikes::ClearStrike()
{
	int x, y;

	StrikesPlace[NumStrikes - 1].get(x, y);
	gotoxy(x, y);

	--NumStrikes;
	cout << (char)(Symbols::SPACE);

}
