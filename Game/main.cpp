/*++++++++++++++++++++++++++++++++++
main.cpp

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/



#include <iostream>
#include "io_utils.h"
#include "TheMathGame.h"
#include "GameManager.h"



//==========================
//  M  A  I  N
//==========================
int main()
{
	TheMathGame game;

	clear_screen();
	hideCursor();

	GameManager(game, 130, 2).run();


}
//==========================
// E n d   o f   M  A  I  N
//==========================
