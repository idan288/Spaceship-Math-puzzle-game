/*++++++++++++++++++++++++++++++++++
Operand.cpp

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/
#include "Operand.h"


/*-----------------------------------------------------------------------------------------------------------------------
This function go to the number place and in case the number is known the function print his value, else print underscope.
------------------------------------------------------------------------------------------------------------------------*/
void Operand::Display() const
{
	gotoxy(Place.GetX(), Place.GetY());

	if (isKnown)
	{
		cout << val;
	}
	else
	{
		cout << "_";
	}

}
