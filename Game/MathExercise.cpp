/*++++++++++++++++++++++++++++++++++
MathExercise.cpp

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/


#include "MathExercise.h"
#include "io_utils.h"
#include "TheMathGame.h"
#include <cstring>
#include <string>


/*---------------------------------------------------------------------------------------------------
This Function Draw for the first time the Math Exercise on the screen, in specified location.

** for this Ex we assume that the numbers At most Three-digit, this assume effect only on the spaces.**
-----------------------------------------------------------------------------------------------------*/
void MathExercise::Draw(bool toFile) const
{
	setTextColor(color);

	gotoxy(ExercisePlace.GetX(), ExercisePlace.GetY());

	FirstNum.Display();

	gotoxy(ExercisePlace.GetX() + NUM_OF_SPACES_FOR_OPER, ExercisePlace.GetY());

	switch (oper)
	{
	case PLUS:
		cout << "+";
		break;
	case MINUS:
		cout << "-";
		break;
	case MULT:
		cout << "*";
		break;
	case DIV:
		cout << "/";
		break;
	default:
		break;
	}

	SecondNum.Display();
	gotoxy(ExercisePlace.GetX() + NUM_OF_SPACES_FOR_EQULE, ExercisePlace.GetY());
	cout << "=";
	result.Display();

	setTextColor(WHITE);
}


/*--------------------------------------------------------------------------------------------------------------------------------------
This Function get a new number update him in the correct place (FirstNum/SecondNum/result).
A) in case there is at least one unknown number,the fuction prints the new number to the secreen in the correct place and return CONTINUE.
B)in case there is no unknown numbers the Math Exercise calculated.
and if the new number solved the Exercise the function return SOLVED else Return STRIKE.
----------------------------------------------------------------------------------------------------------------------------------------*/
ExState MathExercise::UpdateAndCheckforSolved(int Newnumber)
{
	int res;
	EUpdateInPlace place;

	if (!FirstNum.IsKnown())
	{
		FirstNum.SetisKnown(true);
		FirstNum.SetVal(Newnumber);
		place = FIRST;
	}
	else
		if (!SecondNum.IsKnown())
		{
			SecondNum.SetisKnown(true);
			SecondNum.SetVal(Newnumber);
			place = SECONDARY;
		}
		else
		{
			result.SetisKnown(true);
			result.SetVal(Newnumber);
			place = ANSWER;
		}

	if (!result.IsKnown() || place == ANSWER || IsAlegalNumber(Newnumber, place))
	{
		if (!SecondNum.IsKnown() || !result.IsKnown())
		{
			DrawInSpeceficPlace(place);
			return CONTINUE;
		}
		else
		{
			res = DoMathExercise();

			if (res == result.GetVal())
			{
				DrawInSpeceficPlace(place);
				return SOLVED;
			}
			else
			{
				MakeAsUnKnown(place);
				return STRIKE;
			}
		}
	}
	else
	{
		MakeAsUnKnown(place);
		return STRIKE;
	}


}


/*-----------------------------------------------------------------------------------------
This Function calculated the Math Exercise according to the operator and return the result.
------------------------------------------------------------------------------------------*/
int MathExercise::DoMathExercise()
{
	int res;

	switch (oper)
	{
	case PLUS:
		res = FirstNum.GetVal() + SecondNum.GetVal();
		break;
	case MINUS:
		res = FirstNum.GetVal() - SecondNum.GetVal();
		break;
	case MULT:
		res = FirstNum.GetVal() * SecondNum.GetVal();
		break;
	case DIV:
		res = FirstNum.GetVal() / SecondNum.GetVal();
	default:
		break;
	}

	return res;
}


/*---------------------------------------------------------------------------------------------------------------------------------------
This Function get enum EUpdateInPlace place that pesent the place of the member (FirstNum/SecondNum/result), and print him in his location.
-----------------------------------------------------------------------------------------------------------------------------------------*/
void MathExercise::DrawInSpeceficPlace(EUpdateInPlace place) const
{
	setTextColor(color);

	if (place == FIRST)
		FirstNum.Display();
	else
		if (place == SECONDARY)
			SecondNum.Display();
		else
			result.Display();

	setTextColor(WHITE);
}

/*----------------------------------------------------------------------------------------------------------------------------------
This Function get enum EUpdateInPlace place that pesent the place of the member (FirstNum/SecondNum/result),and update him to unknown.
-----------------------------------------------------------------------------------------------------------------------------------*/
void MathExercise::MakeAsUnKnown(EUpdateInPlace place)
{
	if (place == FIRST)
		FirstNum.SetisKnown(false);
	else
		if (place == SECONDARY)
			SecondNum.SetisKnown(false);
		else
			result.SetisKnown(false);
}

/*--------------------------------------------------------------------------------------------------------------------
This function get point that prsent a number on the screen.
A)In case the number solved the Math Exercise the function set that the level is done and return a Symbols::GOOD_NUMBER.
B)In case the number didnt solved the Math Exercise decrease the strikes and return Symbols::BAD_NUMBER;
Also the function clear the number from the board and the screen.
---------------------------------------------------------------------------------------------------------------------*/
Symbols MathExercise::CheckThisNumber(int num)
{
	Symbols symbol = Symbols::GOOD_NUMBER;
	ExState res;

	res = UpdateAndCheckforSolved(num);
	
	if (res == STRIKE)
	{
		theGame->ClearStrikeFromBoard();
		symbol = Symbols::BAD_NUMBER;
	}
	else
		if (res == SOLVED)
			theGame->SetLevelDone();

	return symbol;
}




/*-----------------------------------------------------------------------------------------
This function get new number that update the Exercises and his place (first num/second num)
and check if he a legal number for this Exercises.
-----------------------------------------------------------------------------------------*/
bool MathExercise::IsAlegalNumber(int num, EUpdateInPlace place) const
{
	bool ans = true;
	int res = result.GetVal();

	if ((oper == PLUS || oper == MULT) && num > res)
		ans = false;
	else
		if ((oper == MINUS || oper == DIV) && ((place == FIRST && num < res) || (place == SECONDARY && num > res)))
			ans = false;

	return ans;
}


/*---------------------------------------------------------------------------------
This function get two intager numbers x,y that present the Math Exercise loaction,
and acroding that Intilize the loaction of FirstNum , SecondNum and result.
---------------------------------------------------------------------------------*/
void MathExercise::IntilizeXY(int x, int y)
{
	ExercisePlace.set(x, y);
	FirstNum.SetPlace(x, y);
	SecondNum.SetPlace(x + NUM_OF_SPACES_FOR_THE_FIRST_SECOND, y);
	result.SetPlace(x + NUM_OF_SPACES_FOR_THE_RESULT, y);
}

void MathExercise::SetNumber(char * str, Operand & num)
{
	if (str[0] == '_')
		num.SetisKnown(false);
	else
	{
		num.SetVal(stoi(str));
		num.SetisKnown(true);
	}
}

void MathExercise::SetMathExerciseFromFile(ifstream &file, size_t LinesToSkip)
{
	char line[MAX_PATH];
	char * pline;

	getLineInFile(LinesToSkip, file, line, MAX_PATH);

	pline = strstr(line, "Exercise");

	pline += LEN_OF_EXERICESE_STR;

	SetNumber(pline + NEXT_ROW, FirstNum);//set the first number..

	pline = strpbrk(pline, "+-/*");//go to the oper in the line.

	oper = (EMathOperation)pline[0]; //set the oper.

	SetNumber(pline + NEXT_ROW, SecondNum); //set the second number.

	pline = strpbrk(pline, "=");

	SetNumber(pline + NEXT_ROW, result);//set the result.


}
