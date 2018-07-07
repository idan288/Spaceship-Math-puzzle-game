/*++++++++++++++++++++++++++++++++++
Strikes.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/

#ifndef STRIKES_H_
#define STRIKES_H_

#include "Point.h"
#include "Color.h"

const int NUMOFSTRIKES = 3;

class Strikes
{
	char strikes[NUMOFSTRIKES];
	int NumStrikes;
	Point StrikesPlace[NUMOFSTRIKES];
	Color color;

	void SetStrikesChar(Symbols symbol);

public:
	Strikes() : color(Color::WHITE),NumStrikes(NUMOFSTRIKES) { SetStrikesChar(Symbols::STRIKE_CH); }
	void Draw();
	void ClearStrike();
	void Reset() { NumStrikes = NUMOFSTRIKES; }
	void SetStrikesPlace(int x, int y);
	inline void SetStrikes(int _Numstrikes) { NumStrikes = _Numstrikes; }
	inline int  GetNumStrikes() const { return NumStrikes; }
};

#endif