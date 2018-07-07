/*++++++++++++++++++++++++++++++++++
MathExercise.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/



#ifndef _MATHEXERCISE_
#define _MATHEXERCISE_

#include "Color.h"
#include "Operand.h"
#include "Point.h"
#include "CH_Symbols.h"



using namespace std;
const enum {
	NUM_OF_SPACES_FOR_THE_FIRST_NUMBER = 1, NUM_OF_SPACES_FOR_THE_FIRST_SECOND = 8, NUM_OF_SPACES_FOR_THE_RESULT = 16,
	 NUM_OF_SPACES_FOR_OPER = 5, NUM_OF_SPACES_FOR_EQULE = 13, LEN_OF_EXERICESE_STR = 8, NEXT_ROW = 1
};


class TheMathGame;


class MathExercise
{
	EMathOperation oper;
	Operand FirstNum;
	Operand SecondNum;
	Operand result;
	Point ExercisePlace;
	Color color = WHITE;
	TheMathGame * theGame;


private:
	int DoMathExercise();
	void MakeAsUnKnown(EUpdateInPlace place);
	ExState UpdateAndCheckforSolved(int _num);
	bool IsAlegalNumber(int num, EUpdateInPlace place) const;
	void SetNumber(char * line, Operand & num);

public:
	
	MathExercise(TheMathGame *_theGame) : color(Color::LIGHTGREEN), theGame(_theGame) {}
	void Draw(bool toFile = false) const;
	void DrawInSpeceficPlace(EUpdateInPlace place) const;
	void SetMathExerciseFromFile(ifstream &file, size_t LinesToSkip);
	void IntilizeXY(int x, int y);
	Symbols CheckThisNumber(int num);

};


#endif