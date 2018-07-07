/*+++++++++++++++++++++++++++++++++
CH_Symbols.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/


#ifndef _CH_SYMBOLS_H_
#define _CH_SYMBOLS_H_

enum class Symbols
{
	SPACE = ' ', BIG_SHIP = '@', SMALL_SHIP = '#', NUM_EATER='%',ARTIFACT = '*', WALL = '+', MATH_EXERCISEPLACE = 'X',
	STRIKE_PLACE = 'S', STRIKE_CH = 251, BAD_NUMBER = -1, GOOD_NUMBER, ENTER = '\n', TAB = '\t', UNDERSCOPE = '_',
	GOOD_OPER, BAD_OPER, HAS_NUMBER, BIG_SHIP_VOLUME = 6, SMALL_SHIP_VOLUME = 3,FLYING_ROW='!',FLYING_COL='$'
};


enum EUpdateInPlace { FIRST, SECONDARY, ANSWER };



enum EMathOperation
{
	PLUS = '+',
	MINUS = '-',
	MULT = '*',
	DIV = '/'
};
enum ExState { CONTINUE, STRIKE, SOLVED };

enum EFileState { SUCCEEDED, FAILED_TO_OPEN, ILEGAL_FILE, NOT_ADD_TO_LIST };


#endif