/*++++++++++++++++++++++++++++++++++
GameManager.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/


#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "ISpecificGame.h"
#include <string>
#include <list>
#include <vector>
#include <stdlib.h>
#include "GameMove.h"
#include "ISpecificGame.h"
#include "io_utils.h"
#pragma comment(lib,"Winmm.lib") // include the LIB file for PlaySound(). 
const int EXIT = -1;
const string Nums = "0123456789";
using namespace std;

class GameManager
{
	ISpecificGame& actualGame;
	unsigned int currentLevel = 0;
	unsigned int clockCycleInMillisec;
	unsigned int iterationsPerClockCycle;
	const static unsigned int KEYBOARD_HIT_LOOP = 10;
	const static char ESC = 27; // the ESC key
public:



	GameManager(ISpecificGame& theSpecificGame, unsigned int clockCycleLengthInMillisec, unsigned int numIterationsPerClockCycle)
		:actualGame(theSpecificGame),clockCycleInMillisec(clockCycleLengthInMillisec), iterationsPerClockCycle(numIterationsPerClockCycle) {}
	void run();

private:
	// helper methods
	void init()
	{
		currentLevel = 0;
	}
	char mainMenu()const;
	bool playGame(bool fromLoad = false, bool GameFromSpecificScreen = false,bool HasNextLevel=true);
	char playNextLevel();
	char doLevelIterations();
	bool doIteration() {
		PerformSubIterations(clockCycleInMillisec);
		return doInputIteration();
	}
	bool doInputIteration();
	char playLevel(bool fromload = false, bool HasNextLevel = true);
	void PerformSubIterations(int ClockCycleSpeed);

	// General inputs and outputs for game features
	bool PrintSavedGames(string &FileToLoad);
	void Save();
	bool SolveRound(char &action);
	char ShowSolution(list<GameMove>&moves);
	char GetActionFromSubMenu();

	//General error msgs
	void PrintScrIDError() { cout << "Sorry Cant Replay the level!!\nBecause the screen ID of the saved level not found and there is no other valid next level!!\n"; }
	void PrintNoLevels() {
		clear_screen();
		cout << "There is no screen to play with, the game cannot run!!!\n";
		system("pause");
	}



	void ChangeColor(const char*Screen)const;				// changing the text color based on the color number defined on the screen (color nums can be viewed in screens.h)
	void PrintScreen(const char *ScreenToPrint[])const;		// function that prints a screen

public:
	class MainMenuOptions
	{
	public:
		static bool isValidOption(char option) {
			string validOptions = "124593";
			return (validOptions.find(option) != string::npos);
		}
		enum
		{
			PRESENT_INSTRUCTIONS		=	'1',
			PLAY_GAME					=	'2',
			CHECK_SCREEN				=	'3',
			LOAD_GAME					=   '4',
			PLAY_FROM_SELECTED_SCREEN	=	'5',
			EXIT_APPLICATION			=	'9'
		};
	};
	class LevelOptions
	{
	public:
		static bool isValidOption(char option) {
			string validOptions = "123465";
			return (validOptions.find(option) != string::npos);
		}
		enum
		{
			EXIT_APPLICATION			=	'1',
			BACK_TO_MAIN_MENU			=	'2',
			CONTINUE					=	'3',
			REPLAY_LEVEL				=	'4',
			SHOW_LEVEL_SOLUTION			=   '6',
			NEXT_LEVEL					=	'9',
			SAVE_GAME					=   '5'
		};
	};
	


};

#endif