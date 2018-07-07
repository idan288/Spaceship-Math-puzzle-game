/*++++++++++++++++++++++++++++++++++++
 GameManager.cpp

 Idan Michael  301742060
 Matan Shabtay 204087134
 ++++++++++++++++++++++++++++++++++++*/


#include "Screens.h"
#include "GameManager.h"
using namespace std;



char GameManager::mainMenu()const
{
	PrintScreen(MainMenuScr);
	char selection = 0;
	do {
		selection = _getch();
	} while (!GameManager::MainMenuOptions::isValidOption(selection));
	return selection;
}

void GameManager::run()
{
	// If all boards are invaid, this boolean will remain false
	bool SucceedToGetBoardsFromDirectory = false;

	char menuSelection;
	bool userWantsToPlay = true;
	bool HasNextLevel = true;
	// we run as long as the user wants
	while (userWantsToPlay) {
		menuSelection = mainMenu();
		SucceedToGetBoardsFromDirectory = actualGame.getBoards();
		switch (menuSelection)
		{

		case GameManager::MainMenuOptions::PLAY_GAME:
			if (SucceedToGetBoardsFromDirectory)
			{
				init();
				userWantsToPlay = playGame();
			}
			else
				PrintNoLevels();
			break;
		case GameManager::MainMenuOptions::PRESENT_INSTRUCTIONS:
			PrintScreen(Instructions);
			fflush(stdin);
			_getch();
			break;
		case GameManager::MainMenuOptions::CHECK_SCREEN:
		{
			int option;
			PrintScreen(ScreenListChose);
			option = actualGame.PrintGamesName();

			if (option != EXIT)
			{
				PrintScreen(GameCheckScreen);
				actualGame.CheckThisGame(option, false);
			}
		}

		break;
		case GameManager::MainMenuOptions::LOAD_GAME:
		{
			string FileToLoad;

			if (PrintSavedGames(FileToLoad))
			{
				cout << "Loading Complete,Press Any Key to start...." << endl;
				_getch();
				HasNextLevel = actualGame.LoadGame(FileToLoad);
				userWantsToPlay = playGame(true, false, HasNextLevel);
			}
			break;
		}
		case GameManager::MainMenuOptions::PLAY_FROM_SELECTED_SCREEN:
		{
			int option;
			bool ValidGame;
			PrintScreen(ScreenListChose);
			option = actualGame.PrintGamesName();
			if (option != EXIT)
			{
				PrintScreen(ScreenListChose);
				ValidGame = actualGame.PlayFromSpecificScreen(option);
				if (ValidGame)
					userWantsToPlay = playGame(false, true);
			}
		}
		break;
		case GameManager::MainMenuOptions::EXIT_APPLICATION:
			userWantsToPlay = false;
			clear_screen();
			break;
		default: // normally we shouldn't get to here...
			userWantsToPlay = false;
		};
	}
}

bool GameManager::playGame(bool fromLoad, bool GameFromSpecificScreen, bool HasNextLevel)
{
	// we assume that we have multiple levels so there is a need to loop through levels
	// till all levels were played or till user quits - if user quits we do not continue to next level
	//-------------------------------------------------------------
	// this is the game LEVELS loop
	//-------------------------------------------------------------
	char action = GameManager::LevelOptions::NEXT_LEVEL;

	EStateGameScreen ReturnState = EStateGameScreen::FINISH_LEVELS;
	if (fromLoad)
		action = playLevel(fromLoad, HasNextLevel);

	if (!GameFromSpecificScreen)
		actualGame.ReseteCurrentLevel();

	while (action == GameManager::LevelOptions::NEXT_LEVEL && (EStateGameScreen::HAS_NEXT_LEVEL == (ReturnState = (EStateGameScreen)actualGame.GetScreenToPlay())))
	{
		action = playNextLevel();
	}


	if (ReturnState == EStateGameScreen::ERROR_NO_LEVELS && currentLevel == 0)
		PrintNoLevels();


	//-------------------------------------------------------------
	// END of game LEVELS loop
	//-------------------------------------------------------------

	// return true if the user wants to keep playing
	return (action != GameManager::LevelOptions::EXIT_APPLICATION);
}

// return action to take in case of ESC
char GameManager::playNextLevel()
{
	++currentLevel;
	actualGame.startLevel();
	char action = playLevel();


	//------------------------------------------
	// the actions that may get out from this method are: BACK_TO_MAIN_MENU and EXIT_APPLICATION
	return action;
}


char GameManager::playLevel(bool fromload, bool HasNextLevel)
{
	//------------------------------------------------------------------------------
	// here we control the ESC menu
	//------------------------------------------------------------------------------
	bool keepRunning = true;
	char action = 0;
	char recheck = false;
	while (keepRunning)
	{

		//=============================================================================================
		// this is the actual call to play game iterations
		// if there is a need to check the switchcase for action again before doing an iteration, it will not go to doLevelIterations
		// This boolean is needed for times that the user chosed an option from the ESC menu and then he needs to choose another one before continuing playing
		if (!recheck)
			action = doLevelIterations();
		//=============================================================================================
		recheck = false;
		// check action based on game ended (action==BACK_TO_MAIN_MENU) or input from user on ESC menu
		switch (action) {
		case GameManager::LevelOptions::SAVE_GAME:
			Save();
			action = GetActionFromSubMenu();
			recheck = true;
			break;
		case GameManager::LevelOptions::SHOW_LEVEL_SOLUTION:
			recheck = SolveRound(action);
			break;

		case GameManager::LevelOptions::CONTINUE:
			// keepRunning is true, so we only need to set thing right and then - keepRunning!
			//--------------------------------------------------------------------------------
			actualGame.printBoard();
			break;
		case GameManager::LevelOptions::REPLAY_LEVEL:
			// keepRunning is true, so we only need to set thing right and then - keepRunning!
			//--------------------------------------------------------------------------------
			if (fromload && HasNextLevel || !fromload)
				actualGame.startLevel();
			else
			{
				clear_screen();
				PrintScrIDError();
				_getch();
				keepRunning = false;
			}
			break;

		case GameManager::LevelOptions::BACK_TO_MAIN_MENU:

		case GameManager::LevelOptions::EXIT_APPLICATION:
			// get out from the loop
			clear_screen();
			keepRunning = false;
			break;


		case GameManager::LevelOptions::NEXT_LEVEL:
			// get out from the loop so we get to the next level
			keepRunning = false;
			actualGame.UpCurrentLelvel();
			break;
		}
		//---------------------------------------
		// END of sub menu action switch
		//---------------------------------------
	}
	//------------------------------------------
	// END of Level keepRunning loop (sub menu)
	return action;
}

// return action to take in case of ESC
char GameManager::doLevelIterations()
{
	char action;
	bool escapePressed = false;
	//---------------------------------------
	// this is the game iterations loop
	//---------------------------------------
	while (!actualGame.isRoundOver() && !actualGame.isLevelDone() && !actualGame.isGameOver() && !escapePressed) {
		escapePressed = !doIteration();
	}
	//-----------------------------------------------------
	// END of game iterations loop or exit on ESC
	//-----------------------------------------------------

	// check why we are here
	if (actualGame.isLevelDone())
	{
		//PlayingFromRecord = false;
		PrintScreen(WinningScr);
		PlaySound(TEXT("winning.wav"), 0, 0);
		if (actualGame.isNewRecord())
		{
			string name;
			PrintScreen(NewRecord);
			gotoxy((int)RECORD_INPUT_LOC::COL, (int)RECORD_INPUT_LOC::ROW);
			cin >> name;
			actualGame.StoreRecord(name);
		}
		action = GameManager::LevelOptions::NEXT_LEVEL;
	}
	else if (actualGame.isGameOver())
	{
		clear_screen();
		PrintScreen(GameOverScr);
		PlaySound(TEXT("failure.wav"), 0, 0);
		action = GameManager::LevelOptions::BACK_TO_MAIN_MENU;
	}
	else if (actualGame.isRoundOver())
	{
		PrintScreen(LostRoundScr);
		PlaySound(TEXT("failure.wav"), 0, 0);
		action = GameManager::LevelOptions::REPLAY_LEVEL;
	}
	else
		if (escapePressed)
			action = GetActionFromSubMenu();
	return action;
}
void GameManager::PerformSubIterations(int ClockCycleSpeed)
{
	unsigned int sleepLength = ClockCycleSpeed / iterationsPerClockCycle;
	unsigned int i;
	for (i = 0; i < iterationsPerClockCycle - 1; ++i)
	{
		Sleep(sleepLength);
		actualGame.doSubIteration();
	}

	
}

bool GameManager::doInputIteration()
{
	list<char> keyHits(KEYBOARD_HIT_LOOP); // set vector initialize size to the max key strokes that we may collect
	bool shouldContinue = true;
	// get keyboard input in a loop, to get enough keyboard hits
	unsigned int sleepLength = clockCycleInMillisec / iterationsPerClockCycle / KEYBOARD_HIT_LOOP;
	for (unsigned int i = 0; i < KEYBOARD_HIT_LOOP; ++i) {
		Sleep(sleepLength);
		if (_kbhit()) {
			char ch = _getch();
			if (ch == ESC) {
				// ESC pressed
				shouldContinue = false;
			}
			else {
				// we got keyHits byref, so we actually set input into the original list sent here
				keyHits.push_front(ch); // we want recent keys to come first, thus we use push_front
			}
		}
	}
	// send the keystrokes to the game
	// (even if ESC was hit, we may still have something in the keystrokes vector and should use it)
	actualGame.doIteration(keyHits);

	return shouldContinue;
}



char GameManager::GetActionFromSubMenu()
{
	char action = 0;
	clear_screen();
	PrintScreen(LeveloptionsMenu);
	do {
		action = _getch();
	} while (!GameManager::LevelOptions::isValidOption(action));

	return action;
}
bool GameManager::PrintSavedGames(string &FileToLoad)
{
	// Receives by the game a list of relevant files
	const int ABORT = 0;
	vector <string>SavedGames;
	actualGame.GetSavedGames(SavedGames);
	size_t i = 0;
	string _option;
	int option = ABORT - 1;

	// Print the screen for this feature
	clear_screen();
	PrintScreen(SavedGamesScreen);

	// Presents to the user a list of saved games  , which received by the game
	int x = (int)SAVED_GAMES_LIST_LOC::X, y = (int)SAVED_GAMES_LIST_LOC::Y;
	for (i = 0; i < SavedGames.size(); i++)
	{
		gotoxy(x, y + i);
		cout << "[" << i + 1 << "] " << SavedGames[i] << endl;

	}
	// Get the option from the user
	// If option is within range, continue
	while (option < ABORT || option >(int)SavedGames.size())
	{
		gotoxy(x, y + i++);
		cin >> _option;
		if (_option.find_first_not_of(Nums) == std::string::npos)
			option = stoi(_option);
		//If its a valid option
		if (option > 0 && option <= (int)SavedGames.size())
			FileToLoad = SavedGames[option - 1];
		//Not a valid Option, user hasnt pressed abort
		else if (option != ABORT)
		{
			gotoxy(x, y + i++);
			cout << "Please check your input dude , dont try to fool us" << endl;
		}

	}
	gotoxy(x, y + i++);


	return(option != ABORT);

}

char GameManager::ShowSolution(list<GameMove>&moves)
{
	const char cont = GameManager::LevelOptions::CONTINUE;
	const int SLOWMO = (int)(clockCycleInMillisec * 1.5);
	list <char> keyHits;
	char action = cont;
	char ch;

	actualGame.startLevel();

	while (!actualGame.isLevelDone() && action == cont)
	{//While the level isnt done and the user still want to the see solution

		Sleep(SLOWMO); // Slow mo Sleep
		keyHits.clear();//Clear the prev key hits

		if (!moves.empty() && actualGame.GetIteration() == moves.front().IterationNum)
		{//If there is still key hits and the iteration num fits
			keyHits.push_back(moves.front().FirstKey);
			keyHits.push_back(moves.front().SecondKey);
			moves.pop_front();
		}
		actualGame.doIteration(keyHits);
		PerformSubIterations(clockCycleInMillisec);

		if (_kbhit() && (ch = _getch()) == ESC)
		{//If the user pressed ESC ,show him the level menu
			action = GetActionFromSubMenu();

			if (action == cont) // User still wants to see the solution, keep showing it
				actualGame.printBoard();
		}
	}

	return action;
}

bool GameManager::SolveRound(char &action)
{
	//Function returns true if the calling menu need go to the switchcase again and check it again before going to doLevelIteration again
	//Returns false otherwise

	//Shows the solution to the user
	list<GameMove> moves;
	if (!actualGame.PlayFromRecord(moves))
	{//If no record exist for this level
		PrintScreen(NoRecordExist);
		action = _getch();
		action = GetActionFromSubMenu();
		return true;		//Recheck the action switchcase
	}
	//Else - record exist

	if ((action = ShowSolution(moves)) == GameManager::LevelOptions::CONTINUE) // If the user saw the solution and havent pressed on the ESC menu and chose something else
		action = GetActionFromSubMenu();							// Then get next choice form user as the solution presentation is over

																	//If action is still continue - private case
	if (action == GameManager::LevelOptions::CONTINUE)		// This is a private case for 'Continue' - according to the course instructor (Amir) on Forum,
		actualGame.startLevel();							// Once user pressed continue, need to restart the level. However since automatic solver has been initiated

	else													// Need to restart the level from here, because at the end of this function, LevelDone will be true
		return true;	// Else, Continue wasnt chosen, all other choices by user are fine with us, tell the switch case to check again											

	return false;		// Continue was chosen , dont go to the switchcase again , just perform another do iteration as the game restarted

}

void GameManager::Save()
{
	string FileName;
	PrintScreen(SaveGame);												// Print the relevant screen for this feature
	gotoxy((int)RECORD_INPUT_LOC::COL, (int)RECORD_INPUT_LOC::ROW);
	cin >> FileName;													// Get the file name
	actualGame.SaveGame(FileName);										// Send it to the game

}

void GameManager::PrintScreen(const char *ScreenToPrint[])const
{
	clear_screen();
	bool readingMode = false;		//if we are in a middle of reading a number
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (!readingMode)
			{
				if (ScreenToPrint[i][j] == colorSign)
				{
					ChangeColor(ScreenToPrint[i] + j);
					readingMode = true;					/// color sign has been detected, start reading the number in ChangeColor()
				}
				else
				{
					gotoxy(j, i);
					cout << ScreenToPrint[i][j];
				}

			}
			else if (ScreenToPrint[i][j] == colorSign) // check if the number has been terminated
				readingMode = false;
		}


	}
	setTextColor(DEFAULT_COLOR);
}
void GameManager::ChangeColor(const char*Screen)const
{
	int read = 1;
	int num = 0;
	while (Screen[read] != colorSign)
	{
		num *= 10;
		num += Screen[read] - '0';
		read++;
	}
	setTextColor((Color)num);
}