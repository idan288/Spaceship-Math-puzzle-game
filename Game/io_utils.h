/*++++++++++++++++++++++++++++++++++
io_utils.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/


#ifndef _IO_UTILS_H_
#define _IO_UTILS_H_
// you must have a config file, both for windows and for compiling on Linux (MAMA)
// BUT, you need to change the content of the config!
#include "config.h"
#include "Color.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
using namespace std;
void gotoxy(int x, int y);
void getLineInFile(size_t lineNumber, ifstream &file, char *line, size_t buff_size);
void setTextColor(Color);
void hideCursor();
void clear_screen();

	#ifndef WINDOWS	
		int _getch(void);
		int _kbhit(void);
		void Sleep(unsigned long);
		void _itoa_s(unsigned int, char s[], unsigned int, int);
	#else
		#include <stdlib.h>
		#include <conio.h>
		#include <windows.h>
        #ifdef max 
        #undef max
        #endif
	#endif

#endif