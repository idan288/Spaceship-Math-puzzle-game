/*++++++++++++++++++++++++++++++++++
ISpecificGame.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/


#ifndef _ISPECIFIC_GAME_H_
#define _ISPECIFIC_GAME_H_

#include <vector>
#include <list>
#include "GameMove.h"
#include <iostream>
using namespace std;


enum EStateGameScreen { HAS_NEXT_LEVEL, ERROR_NO_LEVELS, FINISH_LEVELS };


// this is a base class for actual games
// actual games will inherit from this class and implement the required methods
class ISpecificGame
{
public:
	// virtual fucnction with =0 is called an "abstract method"
	// abstract method must be implemented by non-abstract inherited class
	virtual bool isLevelDone() = 0;
	virtual bool isGameOver() const = 0;
	virtual bool hasNextLevel()const = 0;
	virtual void UpCurrentLelvel() = 0;
	virtual void startLevel() = 0;
	virtual void printBoard()= 0;
	virtual bool isRoundOver()	= 0;
	virtual bool isNewRecord() = 0;
	virtual void StoreRecord(const string &name) = 0;
	virtual bool PlayFromRecord(list<GameMove>&moves) = 0;
	virtual unsigned int GetIteration() = 0;
	virtual void GetSavedGames(vector<string>&SavedGames) = 0;
	virtual bool LoadGame(string &s)=0;
	virtual bool getBoards() = 0;
	virtual void ReseteCurrentLevel() = 0;
	virtual int GetScreenToPlay() = 0;
	virtual void SaveGame(string &FileName) = 0;
	virtual int CheckThisGame(int option, bool FromPlaySpecificScreen) = 0;
	virtual bool PlayFromSpecificScreen(int option) = 0;
	virtual int PrintGamesName() = 0;


	// get a list with keyHits and returns a list with the keys that were used
	virtual void doIteration(const list<char>& keyHits) = 0;
	//virtual void doIteration() = 0;

	virtual void doSubIteration() = 0;
};

#endif