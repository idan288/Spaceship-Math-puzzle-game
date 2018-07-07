#include "Num.h"
#include "TheMathGame.h"

TheMathGame* Num::theGame = nullptr;
void Num::setGame(TheMathGame* _theGame) { theGame = _theGame; }

void Num::CreateNumberBorderLeftAndRight(Point const & point)
{

		Left = point;
		Right = point;

		while (theGame->IsPointNumber(Left))
		{
			Left.Move(Direction::LEFT);
		}
		Left.Move(Direction::RIGHT);


		while (theGame->IsPointNumber(Right))
		{
			Right.Move(Direction::RIGHT);
		}
		Right.Move(Direction::LEFT);

}

/*-------------------------------------------------------------------------------------------
This function get tow points that present the left and the right borders of number,
the function go from the left boarder to the right and create intager number and return him.
--------------------------------------------------------------------------------------------*/
int Num::GetNum() 
{
	int X, Y;
	int num = 0;
	int mul = 10;
	char ch;
	Left.get(X, Y);

	while (X <= Right.GetX())
	{
		ch = theGame->GetCharFromTheBoard(X, Y);
		num = num*mul + (ch - '0');
		X++;
	}

	return num;
}