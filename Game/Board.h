/*++++++++++++++++++++++++++++++++++
Board.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/


#ifndef _BOARD_H_
#define _BOARD_H_

#define _CRT_SECURE_NO_WARNINGS
#include <list>
#include <iostream>
#include "Direction.h"
#include <string>
#include <fstream>
#include "ScreenSize.h"
#include "MathExercise.h"
#include "GameMove.h"


class Boards
{
	size_t screen_id;
	char filename[MAX_PATH];
	bool isChecked = false;
	char LastModified[MAX_PATH];
public:
	void SetScreenid(size_t _screen_id) { screen_id = _screen_id; }
	void SetisChecked(bool changeto) { isChecked = changeto; }
	bool GetisCheckValue() { return isChecked; }
	size_t GetScreenid() const { return screen_id; }
	const char * GetScreenfilename() const { return filename; }
	const char * GetScreenLastModified()const { return LastModified; }

	Boards(size_t ScreenId, const char * name, const char * ModifiedDate)
	{
		screen_id = ScreenId;
		strcpy_s(filename, name);
		strcpy_s(LastModified, ModifiedDate);
	}

};







#endif



