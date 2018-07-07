/*++++++++++++++++++++++++++++++++++
io_utils.cpp

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/



#include "io_utils.h"
#include "Color.h"

using namespace std;

#ifndef WINDOWS
	void gotoxy(int x, int y) {}
	int _getch(void){return 0;}
	int _kbhit(void){return 0;}
	void Sleep(unsigned long){}
	void setTextColor(Color color){}
	void hideCursor(){}
	void clear_screen(){}
#else
void gotoxy(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	cout<<flush;
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput,dwCursorPosition);
}
void getLineInFile(size_t lineNumber,ifstream &file,char *line,size_t buff_size)
{
	file.seekg(ios::beg);
	for (size_t i = 0; i < lineNumber; i++)
		file.getline(line, buff_size);
	
}

void setTextColor(Color colorToSet){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)colorToSet);
}

void hideCursor()
{
	 HANDLE myconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	 CONSOLE_CURSOR_INFO CURSOR;
	 CURSOR.dwSize = 1;
	 CURSOR.bVisible = FALSE;
	 SetConsoleCursorInfo(myconsole, &CURSOR);//second argument need pointer
}

void clear_screen()
{
	system("cls");
}

#endif


