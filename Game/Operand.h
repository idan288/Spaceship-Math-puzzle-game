/*++++++++++++++++++++++++++++++++++
Operand.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/



#ifndef _OPERAND_H
#define _OPERAND_H

#include "Point.h"

class Operand
{
	int val;
	bool isKnown = false;
	Point Place;

public:

	Operand() {}
	Operand(int _val, bool _isKnown) :val(_val), isKnown(_isKnown) {}

	inline int GetVal() const { return val; }
	inline bool IsKnown() const { return isKnown; }
	inline void SetVal(int _val) { val = _val; }
	inline void SetPlace(int x, int y) { Place.set(x, y); }
	inline void SetisKnown(bool _isKnown) { isKnown = _isKnown; }

	void Display() const;
};
#endif
