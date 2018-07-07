/*+++++++++++++++++++++++++++++++++
TheMathGame.h

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/

#ifndef _THE_MATH_GAME_H
#define _THE_MATH_GAME_H

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include <iostream>
#include "Spaceship.h"
#include "Artifact.h"
#include "NumEater.h"
#include "ISpecificGame.h"
#include "ScreenSize.h"
#include "Strikes.h"
#include "ClockRotateScreen.h"
#include "ScreenNumber.h"
#include "Board.h"
#include "GameMove.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <strsafe.h>
#include <limits>
#include <tchar.h>
#include <list>
#include "ObjectHolder.h"
#include "Num.h"
using namespace std;


////////////////////////////////////////////////////////////////////
//General consts
//------------------------------------------------------------------
////////////////////////////////////////////////////////////////////
const enum { buff_size = 1024, ESC = 27, TERMINATOR = '\0' };
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//Consts for the spaceships
//------------------------------------------------------------------
////////////////////////////////////////////////////////////////////
const char BIG_SHIP_KEYS[KEYS_DB_SIZE + 1] = "wdxasz";
const char SMALL_SHIP_KEYS[KEYS_DB_SIZE + 1] = "ilmjkn";
const enum { SHIPS_DB_SIZE = 2 ,SMALL_SHIP=0,BIG_SHIP=1 };
const char SEPARATOR[] = "=";
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//Consts prefixes for loading and storing records/ saves games
//------------------------------------------------------------------
////////////////////////////////////////////////////////////////////
const string Scr_ID_Prefix = "ScreenID=";
const string Clck_Iter_Prefix = "ClockIterations=";
const string Name_Solver_Prefix = "NameOfSolver=";
const string Exercise_Prefix = "Exercise=";
const string Strikes_Prefix = "Life=";

const char SavedGamesExtension[] = ".spp";

const char RecordsExtension[] = ".spd";

const char ScreenGamesExtension[] = ".spg";

const TCHAR TCHARScreenGamesExtension[]= TEXT(".spg");


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Consts for screen checker, file handling
//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class EStateCheckScreen
{
	GOOD = -5, BAD, DUPLICATE_ID, INVALID_SCREENID, NOT_FOUND, NO_BIG_SPACESHIP, NO_SMALL_SPACESHIP, TOO_MANY_S_PLACE,
	TOO_MANY_X_PLACE, INVALID_BIG_SPACESHIP, INVALID_SMALL_SPACESHIP, NO_X, NO_S, MISSING_WALL_X_PLACE, MISSING_WALL_S_PALCE,
	MATHEXE_WITHOUT_UNKONWNS, WARNINIG_TAB, FOUND, MISSINGWALLS, MISSINGSPACES, MISSINGSPACES_FOR_X, MISSINGSPACES_FOR_S, INVALID_MATHEXE,
	SHIP_INSIDE_S_BLOCK, SHIP_INSIDE_X_BLOCK
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                    Consts for Loacations And Space
//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum class ELocationSPace {
	NUM_SPACES_FOR_STRIKES = 7, MATHEXE_STR_LEN,NUM_SPACES_FOR_ROTATE_CLOCK = 11, MATHEXERCISE_RECUIRE_SPACES = 22, SPLACE_RECUIRE_SPACES = 16, SCREENID_LEN = 8,
	LOCATION_X_TO_PRINT = 8, LOACATION_Y_TO_PRINT = 5, LOCATION_Y_IN_SCREENCHECK = 8, LOCATION_X_IN_SCREENCHECK = 3, PATH_INDEX = 2, THERE_IS_NEW_PATH = 3,
	NOT_A_COL = -1, NOT_A_ROW = -1, NEXT_ROW = 1, NEXT_TOW_ROW, NEXT_THREE_ROW
};

enum class LinesToJump_BoardFile { Board = 3, Exercise = 2 };
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                  END Consts
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/***********************************TEMP******************************/


class TheMathGame : public ISpecificGame
{
	Spaceship Ships[SHIPS_DB_SIZE];				// Database of the ships
	list<ObjectHolder> Objects;					// The artifact on the level
	list <ObjectHolder> FastObjects;            // Container that save the Flying Cols and the NumEater, that should move faster then others objects. 
	char board[ROWS][COLS + 1];					// this is the actual board we play on, i.e. changes on board are done here
	Object *ObjectCordinates[ROWS][COLS];		// Pointers array of the entire objects on the board, every object that wants to move on the board, ,must to update this array
	bool LevelDone;								// will hold the acknowledgement if the level is done or not
	size_t CurrentLevel;						// game's num of levels
	ScreenNumber ScreenNum;						// will hold the current screen number
	ClockRotateScreen ClockRot;					// will hold the current clock rotation
	Strikes strikes;							// will hold the current amount of strikes
	MathExercise Exercise;	                	// will handle the math exercise
	bool ShipHasBeenHit = false;				// will hold the acknowledgement if the round is over or not due to a ship it by an artifact
	vector <Boards> MathBoards;				    // this container get for every level its board,its screen num and its math exercise.
	list<GameMove>Moves;						// Holds all the iterations taken by the user for a certain round
	list<Num> numbers;
private:
	void InitialGame(bool resetScreen = true);
	void InitScreen();
	void initBoard();
	void initPtrsArray();
	void handleObjectCreationFromBoard(int row, int col);
	void printStrikeScreen();
	bool SetCurrentLevel(size_t ScreenId);
	void SetNumbersList();



	///////////////////////////////////////////////////////////////////////////////
	//      Print Error Functions
	//-----------------------------------------------------------------------------
	///////////////////////////////////////////////////////////////////////////////
	void PrintErrorMassage(vector <int> & DupFileNames, Color color);
	void PrintErrorMassage(const char * filename, EFileState FilseState);
	void PrintErrorMassage(EStateCheckScreen CheckState, int row, int col, Color color);
	void PrintErrorMassage(int & indexToPrintErrorMsg);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                                             CHECK SCREEN FUNCTIONS                                                                        //
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	EStateCheckScreen ScreenCheck(const char * filename, bool PrintToScreen);
	//===========================================================ScreenId Checker================================================================================================//
	EStateCheckScreen CheckTheScreenId(char * ScreenIdRow, size_t & ScreenId, bool FromAddList);
	bool ThisIslegalCharAterScreenIdOrMathExe(char ch);
	//===========================================================Math Exercise Checker================================================================================================//
	EStateCheckScreen CheckTheMathExerciseLine(char * line);
	Symbols CheckForValidNumber(char * & number);
	Symbols CheckForValidOper(char * & number);
	//===========================================================Board Checker================================================================================================//
	EStateCheckScreen CheckScreenBoard(ifstream & Screenfile, bool PrintToScreen);
	//===========================================================Ships Checker================================================================================================//
	EStateCheckScreen CheckForValidSmallShip(Spaceship & ship);
	EStateCheckScreen CheckForValidBigShip(Spaceship & ship);
	EStateCheckScreen PrintShipMsg(Spaceship & ship, bool PrintToScreen);
	//===========================================================Wall Checker================================================================================================//
	EStateCheckScreen CheckForWallsAndSpaces(const char(&board)[ROWS][COLS], Point & Place, size_t NumSpaces, Spaceship &BigShip, Spaceship & SmallShip);
	bool GetBottomWall(const char(&tempboard)[ROWS][COLS], int row, int col, Point & BottomPoint);
	bool GetTheRightWall(const char(&tempboard)[ROWS][COLS], int row, int col, Point & PlaceRightWall, size_t & numSpaces);
	bool GetTopWall(const char(&tempboard)[ROWS][COLS], int row, int col, Point & topPoint, int limit);
	bool GetTheLeftWall(const char(&tempboard)[ROWS][COLS], int row, int col, Point & PlaceLeftWall);
	bool CheckLeftWall(const char(&tempboard)[ROWS][COLS], int row, int col, int limit, Point & LeftPoint);
	bool CheckIfThisPointInside(Point &TopLeftWall, Point &TopLeftSpaceShip, Point &BottomRightWall, Point &BottomRightWallSpaceship);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                                          Screens List Handling                                                                   //
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetAllFileToUnChecked();
	void CheckIfNeedToDeleteSomeOldScreens(vector <string> & Update);
	bool ThisISaNewScreen(const char * filename, const char * LastModified, size_t  index);
	EFileState AddThisScreenToList(const char * filename, const char * LastModified, vector<string> & Update);
	EStateCheckScreen SearchForDuplicateScreenId(const char * filename, size_t id, vector <int> &, bool);
	EStateCheckScreen GetLastModified(TCHAR * filename, char * LastMod);
	void SetBoardFromFile(ifstream &inFile, size_t LinesToSkip = (size_t)LinesToJump_BoardFile::Board);
	int  InputFromTheUser(int indexToPrintErrorMsg);


public:

	//ctor
	TheMathGame() :Exercise(this), Ships{ Spaceship(SMALL_SHIP_KEYS, Symbols::SMALL_SHIP, Color::LIGHTGREEN), Spaceship(BIG_SHIP_KEYS, Symbols::BIG_SHIP, Color::LIGHTGREY) } {
		Object::setGame(this); Num::setGame(this); // Set the game pointer to all objects on game
	}
	//virtual functions, for the manager to manage the game
	virtual bool isGameOver() const { return (strikes.GetNumStrikes() <= 0); }
	virtual bool isLevelDone() { return LevelDone; }
	virtual bool isRoundOver() { return ShipHasBeenHit; }
	virtual bool hasNextLevel()const { return CurrentLevel < MathBoards.size(); }
	virtual bool PlayFromSpecificScreen(int option);
	virtual bool getBoards();
	virtual void startLevel() { InitialGame(); }
	virtual void doIteration(const list<char>& keyHits);
	virtual void doSubIteration() { MoveObjects(FastObjects); }
	virtual unsigned int GetIteration() { return ClockRot.GetNumClockRotates(); }

	// vritual functions that communicates between game and game manager for special features
	virtual void ReseteCurrentLevel() { CurrentLevel = 0; }
	virtual int GetScreenToPlay();
	virtual int CheckThisGame(int option, bool FromSpecificGame = false);
	virtual int PrintGamesName();
	virtual void GetSavedGames(vector<string>&SavedGames) { GetFilesFromFolder(SavedGames, SavedGamesExtension); }



	// general functions for file handling
	void GetFilesFromFolder(vector<string>& DataHolder, const char* FileExtension);			// Returns a list files
	void OpenFile(const char *FileName, const char *FileExtension, ifstream *read = nullptr, ofstream *write = nullptr); // Function that opens a file


	//level handling
	void UpCurrentLelvel() { ++CurrentLevel; }
	inline void SetLevelDone() { LevelDone = true; }
	void ClearStrikeFromBoard() { strikes.ClearStrike(); }				    // Reduces strike from the board
	void ShipGotHit() { if (!ShipHasBeenHit) ClearStrikeFromBoard(); ShipHasBeenHit = true; } // changing to true when the ship got hit by an object
	void MoveObjects(list<ObjectHolder>& objects);

	//Numbers on screen handling
	int removeNum(const Point& p);							//Clears a number from screen and from the numbers list , receiving a point belongs to the number, finds the whole number and deletes it
	Point GetClosestNumMedianPoint(const Point&p);			// Receives by ref a point on screen, finds the closest number to this point and returns its median point
	void ClearNumFromScreen(const Num& Num2Clear);			// Clears a number from screeen

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions responsible for saving and showing a record for a round
//------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const enum { RECORD_FILE_MOVES_LOC = 3, RECORD_FILE_ITERATION_LOC = 2 };
	inline bool isDigit(char c) { return(c <= '9' && c >= '0'); }		// Returns true if the char is a digit
	inline size_t toDig(char c) { return(c - '0'); }					// Converts a char to a digit
	void AddMove(const GameMove& move) { Moves.push_back(move); }		// Adding a move to the Moves list data member
	virtual void StoreRecord(const string&PlayerName);					// Store a new record found
	virtual bool isNewRecord();											// Returns true if a new record has been received
	bool isRecordExist(int _ScreenID);									// Returns true if a record file exists without opening the file
	virtual bool PlayFromRecord(list<GameMove>&moves);					// Returns a list of games moves from a record file
	void OpenRecordFile(ifstream *read = nullptr, ofstream *write = nullptr);	// Opens a record file
	void GetMovements(ifstream &file, list<GameMove>&moves);					// Fills in a game moves list received by Ref from file
////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions responsible for saving a game
//------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    enum { ABORTION = '!' };	// If the user abort the file saving
	enum class GameLoading_ValuesLocation { ScreenID = 1, ClockIterations = 2, Exercise = 3, Life = 4, Board = 5, Moves = 30 };	// Location of each value in a save game file (.spp)
	virtual void SaveGame(string &FileName);																	// Function that saves the game
	virtual bool LoadGame(string &FileToLoad);																	// Function that loads a save

////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions that manages the objects database
//------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void InitObjects()
	{// Init the objects database as needed
		for (Spaceship &ship : Ships)
			ship.Init();
		Objects.clear();
		FastObjects.clear();
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////




	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Functions that Updating the board status and reporting to the objects on game about the board status
	//------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline char GetCharFromTheBoard(int x, int y) { return board[y][x]; }	// Get a character from the board
	inline void RemoveDigFromBoard(int x, int y) { board[y][x] = (char)Symbols::SPACE; }	  // Removes a digit from board





/*--------------------------------------------------------------------
This function return true if this point is a number else return false.
----------------------------------------------------------------------*/
	bool IsPointNumber(Point const & point)
	{
		int x, y;
		point.get(x, y);

		if (board[y][x] <= '9' && board[y][x] >= '0')
			return true;
		else
			return false;
	}
	bool isThisSymbol(const Point& p, Symbols SymbolToCheck)
	{//returns if the coordinate is a certain symbol
		int x, y;
		p.get(x, y);
		return (board[y][x] == (char)SymbolToCheck);

	}

	//return the address of the object taking the coordinate x,y on screen
	Object* isObject(const Point& p)
	{// returns if object exist on this coordinate
		int x, y;
		p.get(x, y);
		return ObjectCordinates[y][x];

	}


	void UpdateBoardCordinate(Object* object, const Point& Cordinate)
	{// update the pointer's board that this coordinate is taking by the calling object
		int x, y;

		Cordinate.get(x, y);
		ObjectCordinates[y][x] = object;
		if (object == nullptr)
			board[y][x] = (char)Symbols::SPACE;
		else
			board[y][x] = (char)object->GetMySymbol();
	}
	bool isShip(Point const &p) { return (isThisSymbol(p, Symbols::SMALL_SHIP) || isThisSymbol(p, Symbols::BIG_SHIP)); }

	/*---------------------------------------------------------------------------------------------------------
	This function get a const point by ref and check if this number is relvent soloution for the math exercise.
	and return:
	A)in case this point is a number the function return BAD_NUMBER if the number is bad solution,
	or if the number solved the exercise the function return GOOD_NUMBER.
	B)in case this point not a number the function return SPACE.
	----------------------------------------------------------------------------------------------------------*/
	Symbols isNumber(const Point & point)
	{

		if (IsPointNumber(point))
		{
		  return Exercise.CheckThisNumber(removeNum(point));
		}
		else
			return Symbols::SPACE;

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  E                                           N                                                        D
	////////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Functions that prints the board , handling colors, and all related to printing to screen
	//------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void DrawBoard(bool isFile = false);
	void DrawPoint(const int x, const int y, char ch, bool isFile = false);
	virtual void printBoard()
	{
		DrawBoard();
		printStrikeScreen();
	}

	void  handleColor(char ch)
	{//  handling the numbers and wall's number while printing the board
		if (ch == (char)Symbols::WALL)		// its a wall
			setTextColor(Color::LIGHTRED);
		else if (ch <= '9' && ch >= '0')  // it's a digit
			setTextColor(Color::LIGHTCYAN);
		else
			setTextColor(Color::WHITE);

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////


};
#endif