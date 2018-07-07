/*++++++++++++++++++++++++++++++++++
TheMathGame.cpp

Idan Michael  301742060 Lesson  3
Matan Shabtay 204087134 Lesson  2
++++++++++++++++++++++++++++++++++*/


#include "TheMathGame.h"


void TheMathGame::doIteration(const list<char>& keyHits)
{

	int direction;

	//-------------------Get Directions from user--------------------------------------//

		//Keys pressed by the user, will be saved in a list
	char Keys[SHIPS_DB_SIZE + 1];

	//Get the keys for the ships
	for (int i = 0; i < SHIPS_DB_SIZE; i++)
	{
		Keys[i] = (char)Symbols::SPACE;						// Initialize the char

		if ((direction = Ships[i].GetDirection(keyHits)) != ERROR_CODE)
		{
			Ships[i].SetDirection((Direction)direction);		// Set the direction of the ship according to the pressed key found
			Keys[i] = Ships[i].GetKey(direction);				// Enter the char to the keys so it will be added to the moves list
		}
		Ships[i].InitializeMoveChecksum();						// Mark that the Ship hasnt moved yet
	}
	// If the user entered a new key for the first or second ship, add it to the moves list
	if (Keys[0] != (char)Symbols::SPACE || Keys[1] != (char)Symbols::SPACE)
		AddMove(GameMove(ClockRot.GetNumClockRotates(), Keys[0], Keys[1]));

	//-----------------END OF Get Directions from user--------------------------------------//

	//---------------------Time to Move----------------------------------------//
	//Move the objects
	MoveObjects(Objects);
	MoveObjects(FastObjects);
	//Move the ships
	if (!ShipHasBeenHit)
	{
		for (Spaceship &ship : Ships)
			ship.Move();
	}


	//-----------------END OF Time to Move ----------------------------------//


	//-----------------------Maintaine the database---------------------------//

	ClockRot.UpdateNumClockRotates();

	//-------------------END OF Maintane the database------------------------//


}


/////////////////////////////////////////////////////////////////////////////
//This function moves objects, fast or regular
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////

void TheMathGame::MoveObjects(list<ObjectHolder>& objects)
{
	list<ObjectHolder>::iterator iter = objects.begin();
	while (iter != objects.end() && !ShipHasBeenHit)
	{
		if (iter->Died()) //Remove all objects with a body in a size of 0
			iter = objects.erase(iter);
		else
			iter++->Move();
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions that initials the game
//------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TheMathGame::InitialGame(bool resetScreen)
{
	clear_screen();
	Moves.clear(); // Clear the list of moves for this round
	InitObjects(); // Initialize objects

	if (resetScreen)
		InitScreen();

	SetNumbersList();
	LevelDone = false;
	ShipHasBeenHit = false;
	initPtrsArray();
	initBoard();


}
void TheMathGame::InitScreen()
{
	ifstream file;

	OpenFile(MathBoards[CurrentLevel].GetScreenfilename(), ScreenGamesExtension, &file, nullptr);

	if (!file.good())
	{
		cerr << "Failed to open file in Init screen! ";
		return;
	}

	//Set the exercise
	Exercise.SetMathExerciseFromFile(file, (size_t)LinesToJump_BoardFile::Exercise);


	//SetBoard
	SetBoardFromFile(file, (size_t)LinesToJump_BoardFile::Board);

	//Set the clockrates
	ClockRot.ResetNumClockRotates();

	//Set strikes 
	if (strikes.GetNumStrikes() == 0 || !ShipHasBeenHit)
		strikes.Reset();
	// Conditions to clear strikes
	//A. Num of strikes is 0
	//B. Ship hasnt got hit but we anyway reached this function, meaning the player paused the game in the middle


//Set the screen number.
	ScreenNum.SetScreenid(MathBoards[CurrentLevel].GetScreenid());

	file.close();
}
void TheMathGame::initPtrsArray()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
			ObjectCordinates[i][j] = nullptr;
	}
}

void TheMathGame::SetNumbersList()
{
	Num num;
	Point p;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			p = Point(j, i);
			if (IsPointNumber(p))
			{
				num = Num(p);
				j = num.GetTopRight().GetX() + 1;
				numbers.push_back(num);

			}

		}

	}
}



void TheMathGame::handleObjectCreationFromBoard(int row, int col)
{
	Symbols symbolFound = (Symbols)board[row][col];
	Point p = Point(col, row);
	switch (symbolFound)
	{
	case Symbols::SMALL_SHIP:
		Ships[SMALL_SHIP].AddPointToBody(p);
		break;
	case Symbols::BIG_SHIP:
		Ships[BIG_SHIP].AddPointToBody(p);
		break;
	case Symbols::ARTIFACT:
		if (!isObject(p))
			Objects.push_back(ObjectHolder::CreateNewArtifact(p));
		break;
	case Symbols::MATH_EXERCISEPLACE:
		Exercise.IntilizeXY(col, row);
		break;
	case Symbols::NUM_EATER:
		FastObjects.push_back(ObjectHolder::CreateNewNumEater(p));
		break;
	case Symbols::FLYING_ROW:
		Objects.push_back(ObjectHolder::CreateNewFlyingCreature(p, Symbols::FLYING_ROW, Color::GREEN, Direction::RIGHT));
		break;
	case Symbols::FLYING_COL:
		FastObjects.push_back(ObjectHolder::CreateNewFlyingCreature(p, Symbols::FLYING_COL, Color::BROWN, Direction::UP));
		break;
	case Symbols::STRIKE_PLACE:
		ScreenNum.SetPlace(col, row);													//This three print in one row with spaces between.ScreenId is a unsigned intager upto 999.
		strikes.SetStrikesPlace(col + (int)ELocationSPace::NUM_SPACES_FOR_STRIKES, row);//strikes is 3 symbols.
		ClockRot.SetPlace(col + (int)ELocationSPace::NUM_SPACES_FOR_ROTATE_CLOCK, row);	//clocke rotate is a unsigned intager upto 99999 															
		break;
	default:
		break;

	}


}


void TheMathGame::initBoard()
{	// copy original board to actual board and build all the relevant objects
	// this shall be called any time we Start the level
	char ch;

	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			ch = board[i][j];
			DrawPoint(j, i, ch);
			handleObjectCreationFromBoard(i, j);

		}
	}
	printStrikeScreen();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  E                                           N                                                        D
////////////////////////////////////////////////////////////////////////////////////////////////////////////









////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions that prints the board , handling colors, and all related to printing to screen
//------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*-----------------------------------------------------------------
This function prints in a particular place the mathematical exercise,
strikes, number of clock rotate and the screen nubmer.
----------------------------------------------------------------*/
void TheMathGame::printStrikeScreen()
{
	Exercise.Draw();
	strikes.Draw();
	ClockRot.Draw(true);
	ScreenNum.Draw();
}
void TheMathGame::DrawBoard(bool isFile)
{
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			DrawPoint(j, i, board[i][j], isFile);
		}
		if (isFile) // If its a file, we use regular cout, not gotoxy, hence we need to print the space after each line
			cout << endl;
	}
}

void TheMathGame::DrawPoint(const int x, const int y, char ch, bool isFile)
{
	if (!isFile)
	{// If we are not printing to a file , need to handle colors and use gotoxy
		gotoxy(x, y);
		if (ch != (char)Symbols::SPACE)
			handleColor(ch);
	}
	cout << ch;
	cout.flush();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  E                                           N                                                        D
////////////////////////////////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------
//Functions that Updating the board status and reporting to the objects on game about the board status
//------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////








////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions  that finds and opens files
//------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TheMathGame::OpenFile(const char *FileName, const char *FileExtension, ifstream *read, ofstream *write)
{
	char File[buff_size + 1];
	strcpy(File, FileName);
	strcat(File, FileExtension);


	// Checks what the calling function wants, to read or write
	if (read != nullptr)
		read->open(File);
	else
		write->open(File);



}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------------------------------------
This function put into DataHolder all the filenames that in Path Directory and have the FileExtension.
----------------------------------------------------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TheMathGame::GetFilesFromFolder(vector<string>& DataHolder, const char* FileExtension)
{
	string directoryPathAndFormatSearch = "*";
	char filename[MAX_PATH];
	WIN32_FIND_DATA findData;
	HANDLE hFind;
	TCHAR fullPath[buff_size + 1] = TEXT("");

	//add the extension
	directoryPathAndFormatSearch.append(FileExtension);

	//convert to TCHAR
	copy(directoryPathAndFormatSearch.begin(), directoryPathAndFormatSearch.end(), fullPath);

	// search in fullSearchPath files acording to the format..
	hFind = FindFirstFile(fullPath, &findData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			//convert tchar/wchar to char *.
			wcstombs(filename, findData.cFileName, MAX_PATH);

			// Remove the file extension
			*(strstr(filename, FileExtension)) = TERMINATOR;
			DataHolder.push_back(filename);

		} while (FindNextFile(hFind, &findData) > 0);//while there is another file in this path that match to format search.

	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  E                                           N                                                        D
////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions responsible for saving and showing a record for a round
//------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TheMathGame::StoreRecord(const string&PlayerName)
{

	ofstream file;
	OpenRecordFile(nullptr, &file);		// Open the file


	//Failed to open the file!
	if (!file.good())
	{
		cerr << " Failed to open a record file in TheMathGame.cpp , function StoreRecord\n";
		return;
	}
	file << Scr_ID_Prefix << MathBoards[CurrentLevel].GetScreenid() << endl;	// Get the Screen ID
	file << Clck_Iter_Prefix << ClockRot.GetNumClockRotates() << endl;			// Copy the clock iterations
	file << Name_Solver_Prefix << PlayerName << endl;							// Copy the name of solver

	//Write down the iterations..
	for (GameMove &move : Moves)
		file << move.IterationNum << ": " << move.FirstKey << move.SecondKey << endl;

}
bool TheMathGame::isNewRecord()
{
	// This function checks if a new record is bigger than exisiting one
	//  If no existing record, it will return true as well

	ifstream file;
	char line[buff_size];
	char *temp;
	size_t record;

	OpenRecordFile(&file, nullptr);

	if (file.good())
	{//If file exist

		getLineInFile(RECORD_FILE_ITERATION_LOC, file, line, buff_size);
		//Close the file
		file.close();


		temp = strstr(line, SEPARATOR) + 1;
		record = atoi(temp);
		//If the new score is better than current - return true
		if (ClockRot.GetNumClockRotates() < record)
			return true;
		else
			return false;
	}
	else
		return true;


}


//Returns true if a record file exist for this screen ID
bool TheMathGame::isRecordExist(int _ScreenID) {

	WIN32_FIND_DATA data;
	HANDLE find;
	string ScreenID = to_string(_ScreenID);
	TCHAR filename[buff_size + 1] = TEXT("");
	ScreenID.append(RecordsExtension);

	copy(ScreenID.begin(), ScreenID.end(), filename);
	find = FindFirstFile(filename, &data);
	return (find != INVALID_HANDLE_VALUE);
}


bool TheMathGame::PlayFromRecord(list<GameMove>&moves)
{
	char line[buff_size + 1];

	ifstream file;
	OpenRecordFile(&file, nullptr);

	if (file.good())
	{
		// Skip the first line in the file (Screen ID)
		// Skip the line of the num of iterations
		// Skip the line of name of solver
		getLineInFile(RECORD_FILE_MOVES_LOC, file, line, buff_size);
		GetMovements(file, moves);
		file.close();
		//Returns true if record exist
		return true;

	}
	else
		// Returns false if record doesnt exist
		return false;

}
void TheMathGame::GetMovements(ifstream &file, list<GameMove>&moves)
{
	size_t iterationNum;
	char line[buff_size + 1], *token;
	char FirstKey, SecondKey;


	//Start reading movements
	file.getline(line, buff_size);
	while (!file.eof())
	{
		FirstKey = SecondKey = 0;
		// Get iteration number
		token = strtok(line, ": ");
		iterationNum = atoi(token);



		// Get keys
		token = strtok(NULL, " \n");
		FirstKey = token[0];
		if (token[1] != TERMINATOR) // If second key exists
			SecondKey = token[1];

		moves.push_back(GameMove(iterationNum, FirstKey, SecondKey));

		file.getline(line, buff_size); // Get next move

	}
}
void TheMathGame::OpenRecordFile(ifstream *read, ofstream *write)
{
	char _ScreenID[buff_size + 1];
	_itoa(ScreenNum.GetID(), _ScreenID, 10);
	OpenFile(_ScreenID, RecordsExtension, read, write);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  E                                           N                                                        D
////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions responsible for saving a game
//------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TheMathGame::SaveGame(string &FileName)
{
	streambuf *psbuf, *backup;
	ofstream file;
	if (*(FileName.c_str()) == ABORTION)
		return;

	OpenFile(FileName.c_str(), SavedGamesExtension, nullptr, &file);



	if (!file.good())
	{
		cerr << "Failed to open a file in SaveGame function!\n";
		_getch();
	}
	// On this function, we will change the cout to the file we opened, this is mandatory because printing the board and the math exercise
	// To the file can be done using existing functions that we used to print to the secreen

	backup = cout.rdbuf();     // back up cout's streambuf
	psbuf = file.rdbuf();
	cout.rdbuf(psbuf);         // assign streambuf to cout

	cout << Scr_ID_Prefix << ScreenNum.GetID() << endl;				    // Sets the ScreenID
	cout << Clck_Iter_Prefix << ClockRot.GetNumClockRotates() << endl; // Sets the number of clock iterations

	//Set the exercise
	cout << Exercise_Prefix;
	Exercise.Draw(true);

	cout << endl << Strikes_Prefix << strikes.GetNumStrikes() << endl;;
	DrawBoard(true);// Print the board to a file


	//Lets start putting all iterations
	for (GameMove &m : Moves)
	{
		file << endl;
		file << m.IterationNum << ": " << m.FirstKey << m.SecondKey;
	}
	cout.rdbuf(backup);        // restore cout's original streambuf
	file.close();


}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*-----------------------------------------------------------------------------------------------------------------------------------------------------
This function get Screenid and find his in the mathboards.
A)in case the screenid was found put into CurrentLevel,the level that we was savin, so when the user chose to replay the level form begining he could.
B)in case the screen if not found in MathBoards CurrentLevel go to the next level that shuld come after.
------------------------------------------------------------------------------------------------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool  TheMathGame::SetCurrentLevel(size_t ScreenId)
{

	size_t size = MathBoards.size();
	bool found = false;
	int NextCorrectLevel = (int)EStateCheckScreen::NOT_FOUND;//this var wiil save the last level that should come after.
	EStateGameScreen returnstate;												 //in case this level no in the array anymore.
	size_t i;

	for (i = 0; i < size && !found; ++i)
	{

		if (NextCorrectLevel == (int)EStateCheckScreen::NOT_FOUND && ScreenId < MathBoards[i].GetScreenid())
		{
			NextCorrectLevel = CurrentLevel;
		}

		if (ScreenId == MathBoards[i].GetScreenid())
			found = true;
		else
			++CurrentLevel;
	}


	//for cases that someone delete the original screen, so get the next valid screen.
	if (!found)
	{
		CurrentLevel = NextCorrectLevel;
		returnstate = (EStateGameScreen)GetScreenToPlay();//get the next valid screen.
		if (returnstate != EStateGameScreen::HAS_NEXT_LEVEL)//check if there is a next level.
			return false;
		else//found next level.
			return true;
	}
	else//the correct screen found
		return true;

}

bool TheMathGame::LoadGame(string &FileToLoad)
{// Loading a game based on a file name
	ifstream file;
	char line[buff_size + 1];																				// Reads the line of a file
	int screenID;																							// Holds the screen ID
	OpenFile(FileToLoad.c_str(), SavedGamesExtension, &file, nullptr);					// Open the requested file
	char *ptr;																								// Pointer to the important values
	if (!file.good())																						// If file didnt opened successfully
	{
		cerr << "Failed to load a file in LoadGame (TheMathGame!)\n";
		_getch();
	}

	//Get the Screen ID
	getLineInFile((size_t)GameLoading_ValuesLocation::ScreenID, file, line, buff_size);
	ptr = strstr(line, SEPARATOR) + 1;
	ScreenNum.SetScreenid(screenID = atoi(ptr));

	//Get the clock rotations
	getLineInFile((size_t)GameLoading_ValuesLocation::ClockIterations, file, line, buff_size);
	ptr = strstr(line, SEPARATOR) + 1;
	ClockRot.ResetNumClockRotates(atoi(ptr));

	//Get the life
	getLineInFile((size_t)GameLoading_ValuesLocation::Life, file, line, buff_size);
	ptr = strstr(line, SEPARATOR) + 1;
	strikes.SetStrikes(atoi(ptr));

	//Set Board
	SetBoardFromFile(file, (size_t)GameLoading_ValuesLocation::Board);

	//Set the math exercise
	Exercise.SetMathExerciseFromFile(file, (size_t)GameLoading_ValuesLocation::Exercise);

	//Initial Database of objects and cetera, dont initialze the screen (hence the false)
	InitialGame(false);

	//Get the movements stored
	getLineInFile((size_t)GameLoading_ValuesLocation::Moves - 1, file, line, buff_size);
	GetMovements(file, Moves);

	file.close();
	return SetCurrentLevel(screenID);


}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  E                                           N                                                        D
////////////////////////////////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                             CHECK SCREEN FUNCTIONS                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool TheMathGame::ThisIslegalCharAterScreenIdOrMathExe(char ch)
{
	if (ch == '=' || ch == ':' || ch == '-' || ch == '_')
		return true;
	else
		return false;
}
Symbols TheMathGame::CheckForValidNumber(char * & number)
{
	Symbols ReturnSymobl = Symbols::GOOD_NUMBER;
	size_t NumDigits = 0;

	if (number[0] == '_')
	{
		number++;
		ReturnSymobl = Symbols::UNDERSCOPE;
	}
	else
	{
		if (number[0] < '0' || number[0] > '9') //if this is not a digit  and  not under scope so this is worng input, return bad number.
		{
			ReturnSymobl = Symbols::BAD_NUMBER;
		}
		else
		{//while this is number go over all his digits.
			ReturnSymobl = Symbols::HAS_NUMBER;
			while (number[0] >= '0' && number[0] <= '9' && number[0] != '\0')
			{
				number++;
				++NumDigits;
			}
			if (NumDigits > 5)//this number should be at most 5 digits.
				ReturnSymobl = Symbols::BAD_NUMBER;
		}
	}


	return ReturnSymobl;
}
Symbols TheMathGame::CheckForValidOper(char * & number)
{
	EMathOperation oper = (EMathOperation)number[0];
	Symbols ReturnSym = Symbols::BAD_OPER;

	switch (oper)
	{
	case EMathOperation::PLUS:
		number++;
		ReturnSym = Symbols::GOOD_OPER;
		break;
	case EMathOperation::MINUS:
		number++;
		ReturnSym = Symbols::GOOD_OPER;
		break;
	case EMathOperation::MULT:
		number++;
		ReturnSym = Symbols::GOOD_OPER;
		break;
	case EMathOperation::DIV:
		number++;
		ReturnSym = Symbols::GOOD_OPER;
		break;
	}

	return ReturnSym;
}
EStateCheckScreen TheMathGame::CheckTheMathExerciseLine(char * MathExeLine)
{
	EStateCheckScreen ReturnState = EStateCheckScreen::GOOD;
	Symbols StateSymbol;
	char * ptr;
	bool first, second, res;

	first = second = res = false;

	ptr = strstr(MathExeLine, "Exercise");

	if (ptr == nullptr)
	{
		ReturnState = EStateCheckScreen::INVALID_MATHEXE;
	}
	else
	{
		ptr += (int)ELocationSPace::MATHEXE_STR_LEN;
		if (!ThisIslegalCharAterScreenIdOrMathExe(ptr[0]))
			ReturnState = EStateCheckScreen::INVALID_MATHEXE;
		else
		{
			ptr++;
			StateSymbol = CheckForValidNumber(ptr); //return symbol if this good first element on the equation.

			if (StateSymbol == Symbols::HAS_NUMBER)
				first = true;

			if (StateSymbol == Symbols::BAD_NUMBER)
				ReturnState = EStateCheckScreen::INVALID_MATHEXE;
			else
			{//go to oper.
				StateSymbol = CheckForValidOper(ptr);
				if (StateSymbol != Symbols::GOOD_OPER)
					ReturnState = EStateCheckScreen::INVALID_MATHEXE;
				else
				{//the second number.
					StateSymbol = CheckForValidNumber(ptr); //return symbol if this good second element on the equation.
					if (StateSymbol == Symbols::HAS_NUMBER)
						second = true;
					if (StateSymbol == Symbols::BAD_NUMBER)
						ReturnState = EStateCheckScreen::INVALID_MATHEXE;
					else
						if (ptr[0] != '=')
							ReturnState = EStateCheckScreen::INVALID_MATHEXE;
						else
						{//the answer part.
							ptr++;//go to answer part.
							StateSymbol = CheckForValidNumber(ptr); //return symbol if this good last element on the equation.
							if (StateSymbol == Symbols::HAS_NUMBER)
								res = true;
							if (StateSymbol == Symbols::BAD_NUMBER)
								ReturnState = EStateCheckScreen::INVALID_MATHEXE;
						}

				}
			}
		}
	}

	if (first && second && res)//if there is no unkowns its bad.
		return EStateCheckScreen::MATHEXE_WITHOUT_UNKONWNS;
	else
		return ReturnState;

}
EStateCheckScreen TheMathGame::CheckScreenBoard(ifstream & Screenfile, bool PrintToScreen)
{
	char tempBoard[ROWS][COLS] = {};
	Symbols ch;
	Spaceship BigSpaceship(Spaceship(BIG_SHIP_KEYS, Symbols::BIG_SHIP, Color::LIGHTGREEN));
	Spaceship SmallSpaceship(Spaceship(SMALL_SHIP_KEYS, Symbols::SMALL_SHIP, Color::LIGHTGREEN));
	Point XPoint;
	Point SPoint;
	EStateCheckScreen BigShipReturnState;
	EStateCheckScreen SmallShipReturnState;
	EStateCheckScreen XPlace = EStateCheckScreen::NO_X;
	EStateCheckScreen SPlace = EStateCheckScreen::NO_S;


	for (int i = 0; i < ROWS && !Screenfile.eof(); ++i)
	{

		for (int j = 0; j < COLS && !Screenfile.eof(); ++j)
		{
			ch = (Symbols)Screenfile.get();
			tempBoard[i][j] = (char)ch;

			switch (ch)
			{
			case Symbols::BIG_SHIP://collect all the big spaceship points.
				BigSpaceship.AddPointToBody(j, i, false);
				break;
			case Symbols::SMALL_SHIP://collect all the small spaceship points.
				SmallSpaceship.AddPointToBody(j, i, false);
				break;
			case Symbols::TAB://warn about tab in the screen.
				if (PrintToScreen)
					PrintErrorMassage(EStateCheckScreen::WARNINIG_TAB, i, j, Color::LIGHTMAGENTA);
				break;
			case Symbols::MATH_EXERCISEPLACE:
				if (XPlace == EStateCheckScreen::NO_X)
				{
					XPoint.set(j, i);
					XPlace = EStateCheckScreen::FOUND;
				}

				else
					XPlace = EStateCheckScreen::TOO_MANY_X_PLACE;
				break;
			case Symbols::STRIKE_PLACE:
				if (SPlace == EStateCheckScreen::NO_S)
				{
					SPoint.set(j, i);
					SPlace = EStateCheckScreen::FOUND;
				}
				else
					SPlace = EStateCheckScreen::TOO_MANY_S_PLACE;
				break;
			case Symbols::ENTER: //if the row finish before we get COLS input.
				while (j < COLS)
				{
					++j;
				}
				break;
			default:
				break;
			}
		}

		if (ch != Symbols::ENTER) // if the row not finish yet ignore the rest of the input in this row.
		{
			ch = (Symbols)Screenfile.get();

			while ((int)ch > 0 && i < ROWS && ch != Symbols::ENTER)
			{
				ch = (Symbols)Screenfile.get();
			}
		}
	}



	if (XPlace == EStateCheckScreen::FOUND)
	{
		XPlace = CheckForWallsAndSpaces(tempBoard, XPoint, (size_t)ELocationSPace::MATHEXERCISE_RECUIRE_SPACES, BigSpaceship, SmallSpaceship);
	}

	if (XPlace != EStateCheckScreen::GOOD)
	{
		if (PrintToScreen)
			PrintErrorMassage(XPlace, (int)ELocationSPace::NOT_A_ROW, (int)ELocationSPace::NOT_A_COL, Color::CYAN);
		else
			return  EStateCheckScreen::BAD;
	}


	if (SPlace == EStateCheckScreen::FOUND)
	{
		SPlace = CheckForWallsAndSpaces(tempBoard, SPoint, (size_t)ELocationSPace::SPLACE_RECUIRE_SPACES, BigSpaceship, SmallSpaceship);
	}

	if (SPlace != EStateCheckScreen::GOOD)
	{
		if (PrintToScreen)
			PrintErrorMassage(SPlace, (int)ELocationSPace::NOT_A_ROW, (int)ELocationSPace::NOT_A_COL, Color::BROWN);
		else
			return  EStateCheckScreen::BAD;
	}


	BigShipReturnState = PrintShipMsg(BigSpaceship, PrintToScreen);
	SmallShipReturnState = PrintShipMsg(SmallSpaceship, PrintToScreen);


	if (BigShipReturnState == EStateCheckScreen::GOOD && SmallShipReturnState == EStateCheckScreen::GOOD
		&& SPlace == EStateCheckScreen::GOOD && XPlace == EStateCheckScreen::GOOD)
		return EStateCheckScreen::GOOD;
	else
		return EStateCheckScreen::BAD;


}



//===========================================================Wall Checker================================================================================================//
bool TheMathGame::GetTheRightWall(const char(&tempboard)[ROWS][COLS], int row, int col, Point & PlaceRightWall, size_t & numSpaces)
{
	size_t i;
	char ch;
	ch = tempboard[row][col + 1];

	for (i = col + 1; i < COLS && (ch == (char)Symbols::TAB || ch == (char)Symbols::SPACE); ++i) //go right until you go to the wall or end of the line.
	{
		ch = tempboard[row][i];
		++numSpaces;
	}

	if (ch == (char)Symbols::WALL)
	{
		PlaceRightWall.set(--i, row);
		return true;
	}
	else
		return false;
}
bool TheMathGame::GetTheLeftWall(const char(&tempboard)[ROWS][COLS], int row, int col, Point & PlaceLeftWall)
{
	int i;
	char ch;
	ch = tempboard[row][col];

	for (i = col - 1; i >= 0 && ch != (char)Symbols::WALL; --i)
	{
		ch = tempboard[row][i];
	}

	if (ch == (char)Symbols::WALL)
	{
		PlaceLeftWall.set(++i, row);
		return true;
	}
	else
		return false;
}
bool TheMathGame::GetBottomWall(const char(&tempboard)[ROWS][COLS], int row, int col, Point & BottomPoint)
{
	int i;
	char ch = tempboard[row][col];
	char leftcharFromMeisWall = (char)EStateCheckScreen::NOT_FOUND;

	for (i = row + 1; i < ROWS && ch == (char)Symbols::WALL &&leftcharFromMeisWall == (char)EStateCheckScreen::NOT_FOUND; i++)
	{
		ch = tempboard[i][col];
		if (col - 1 >= 0)
			if (tempboard[i][col - 1] == (char)Symbols::WALL)
				leftcharFromMeisWall = tempboard[i][col - 1];
	}


	if (ch == (char)Symbols::WALL)
	{
		BottomPoint.set(col, --i);
		return true;
	}
	else
		return false;

}
bool TheMathGame::GetTopWall(const char(&tempboard)[ROWS][COLS], int row, int col, Point & topPoint, int limit)
{
	int i;
	char ch = tempboard[row][col];
	char leftcharFromMeisWall = (char)EStateCheckScreen::NOT_FOUND;


	for (i = row - 1; i >= 0 && i >= limit && ch == (char)Symbols::WALL && leftcharFromMeisWall == (char)EStateCheckScreen::NOT_FOUND; --i)
	{
		ch = tempboard[i][col];
		if (col - 1 >= 0)
			if (tempboard[i][col - 1] == (char)Symbols::WALL)
				leftcharFromMeisWall = tempboard[i][col - 1];
	}

	if (ch == (char)Symbols::WALL)
	{
		topPoint.set(col, ++i);
		return true;
	}
	else
		return false;
}
bool TheMathGame::CheckLeftWall(const char(&tempboard)[ROWS][COLS], int row, int col, int limit, Point & LeftPoint)
{
	int i;

	char ch = tempboard[row][col];

	for (i = col - 1; i >= limit && ch == (char)Symbols::WALL; --i)
	{
		ch = tempboard[row][i];
	}

	if (ch == (char)Symbols::WALL)
	{
		LeftPoint.set(++i, row);
		return true;
	}
	else
		return false;
}
bool TheMathGame::CheckIfThisPointInside(Point &TopLeftWall, Point &TopLeftSpaceShip, Point &BottomRightWall, Point &BottomRightWallSpaceship)
{

	if (TopLeftWall.GetX() <= TopLeftSpaceShip.GetX() && TopLeftWall.GetY() <= TopLeftSpaceShip.GetY() &&
		BottomRightWall.GetX() >= BottomRightWallSpaceship.GetX() && BottomRightWall.GetY() >= BottomRightWallSpaceship.GetY())
		return true;
	else
		return false;
}
EStateCheckScreen TheMathGame::CheckForWallsAndSpaces(const char(&tempboard)[ROWS][COLS], Point & Place, size_t NumSpaces, Spaceship &BigShip, Spaceship & SmallShip)
{
	bool MissingWall = false;
	Point PlaceofRightWall;
	Point PlaceofLeftWall;
	Point TopRightWall;
	Point TopLeftWall;
	Point BottomRight;
	Point BottomLeft;
	size_t SpacesAmount = 1;
	int row = Place.GetY();
	int col = Place.GetX();

	if (!GetTheRightWall(tempboard, row, col, PlaceofRightWall, SpacesAmount))//get the coordinate of the right border.
		MissingWall = true;
	else
		if (!GetTheLeftWall(tempboard, row, col, PlaceofLeftWall)) // get the coordinate of the left border.
			MissingWall = true;
		else
			if (!GetTopWall(tempboard, PlaceofRightWall.GetY(), PlaceofRightWall.GetX(), TopRightWall, -1))//walls and top left wall.
				MissingWall = true;
			else
				if (!GetBottomWall(tempboard, PlaceofRightWall.GetY(), PlaceofRightWall.GetX(), BottomRight)) //walls and bottom left wall.
					MissingWall = true;
				else
					if (!CheckLeftWall(tempboard, BottomRight.GetY(), BottomRight.GetX(), PlaceofLeftWall.GetX(), BottomLeft))
						MissingWall = true;
					else
						if (!GetTopWall(tempboard, BottomLeft.GetY(), BottomLeft.GetX(), TopLeftWall, TopRightWall.GetY()))
							MissingWall = true;
						else
							if (!CheckLeftWall(tempboard, TopRightWall.GetY(), TopRightWall.GetX(), TopLeftWall.GetX(), TopLeftWall))
								MissingWall = true;

	if (!MissingWall)//check for missing walls.
	{
		if (NumSpaces > SpacesAmount)//check if there is enough spaces.
		{//not enough spaces, return the right error state.
			if (tempboard[row][col] == (char)Symbols::MATH_EXERCISEPLACE)
				return EStateCheckScreen::MISSINGSPACES_FOR_X;
			else
				return EStateCheckScreen::MISSINGSPACES_FOR_S;
		}
	}
	else//there is missing walls,check who call me and give the right error.
		if (tempboard[row][col] == (char)Symbols::MATH_EXERCISEPLACE)
			return EStateCheckScreen::MISSING_WALL_X_PLACE;
		else
			return EStateCheckScreen::MISSING_WALL_S_PALCE;



	//Check if there is spaceship inside the block so its bad.
	//check this just if there is a spceship to check.
	if (BigShip.GetMyVolume() > 0 && SmallShip.GetMyVolume() > 0)
	{
		if ((CheckIfThisPointInside(TopLeftWall, BigShip.GetTopLeft(), BottomRight, BigShip.GetBottomRight()))
			|| (CheckIfThisPointInside(TopLeftWall, SmallShip.GetTopLeft(), BottomRight, SmallShip.GetBottomRight())))
		{
			if (tempboard[row][col] == (char)Symbols::MATH_EXERCISEPLACE)
				return EStateCheckScreen::SHIP_INSIDE_X_BLOCK;
			else
				return EStateCheckScreen::SHIP_INSIDE_S_BLOCK;
		}
		else//Yeah everything is good.
			return EStateCheckScreen::GOOD;
	}
	else//Yeah everything is good.
		return EStateCheckScreen::GOOD;
}
//==========================================================END Wall Checker============================================================================================//




//========================SHIPS CHECK======================================================//
EStateCheckScreen TheMathGame::CheckForValidBigShip(Spaceship & ship)
{

	Point TopLeftPoint = ship.GetTopLeft();
	Point SecondPoint;
	Point ThirdPoint;
	Point ForthPoint;
	Point FifthPoint;
	Point SixPoint;

	list<Point> ::iterator ShipPointPtr = ship.GetBody().begin();//pointer to the begin of the list.

	SecondPoint = *(++ShipPointPtr);//get the second point on the list.
	ThirdPoint = *(++ShipPointPtr);//get the Third point on the list.
	ForthPoint = *(++ShipPointPtr);//get the Forth point on the list.
	FifthPoint = *(++ShipPointPtr);//get the Fifth point on the list.
	SixPoint = *(++ShipPointPtr);//get the Six point on the list.

								 //Check the space ship on his regular shape, 4 Casese:
	if ((TopLeftPoint.next(Direction::RIGHT) == SecondPoint && TopLeftPoint.next(Direction::RIGHT).next(Direction::RIGHT) == ThirdPoint) &&							//Check to cases : 1) @@@  2) ===== <-screen border
		((TopLeftPoint.next(Direction::DOWN) == ForthPoint && TopLeftPoint.next(Direction::DOWN).next(Direction::RIGHT) == FifthPoint    && 						//				      @@@      @@@
			TopLeftPoint.next(Direction::DOWN).next(Direction::RIGHT).next(Direction::RIGHT) == SixPoint) ||															//												
			(TopLeftPoint.next(Direction::UP) == ForthPoint && TopLeftPoint.next(Direction::UP).next(Direction::RIGHT) == FifthPoint &&									//						       @@@
				TopLeftPoint.next(Direction::UP).next(Direction::RIGHT).next(Direction::RIGHT) == SixPoint)))															//							  =====
		return EStateCheckScreen::GOOD;
	else
		if (TopLeftPoint.next(Direction::RIGHT) == SecondPoint && TopLeftPoint.next(Direction::LEFT) == ThirdPoint &&
			TopLeftPoint.next(Direction::DOWN) == ForthPoint && TopLeftPoint.next(Direction::DOWN).next(Direction::RIGHT) == FifthPoint   &&							//The Second Check is about big spaceship : |@@    @|
			TopLeftPoint.next(Direction::DOWN).next(Direction::LEFT) == SixPoint)																					    //                                          |@@	   @|
			return EStateCheckScreen::GOOD;																															//|- Present the border of the screen 
		else
			if (TopLeftPoint.next(Direction::LEFT).next(Direction::LEFT) == SecondPoint && TopLeftPoint.next(Direction::LEFT) == ThirdPoint &&
				TopLeftPoint.next(Direction::DOWN) == ForthPoint && TopLeftPoint.next(Direction::LEFT).next(Direction::LEFT).next(Direction::LEFT) == FifthPoint &&  //The Third Check is about big spaceship : |@      @@|
				TopLeftPoint.next(Direction::DOWN).next(Direction::LEFT) == SixPoint)																			  //										    |@      @@|
				return EStateCheckScreen::GOOD;																													 // |- Present the border of the screen
			else																																															// Chceckes tow cases on big ship:
																																																			//Check the space ship on the twist shape, 4 Casese:																																					
				if ((TopLeftPoint.next(Direction::RIGHT) == SecondPoint && TopLeftPoint.next(Direction::DOWN) == ThirdPoint &&TopLeftPoint.next(Direction::DOWN).next(Direction::RIGHT) == ForthPoint) &&	// 1) : @@  2): =======  <-screen border
					((TopLeftPoint.next(Direction::DOWN).next(Direction::DOWN) == FifthPoint &&TopLeftPoint.next(Direction::DOWN).next(Direction::DOWN).next(Direction::RIGHT) == SixPoint) ||		  		//		@@	     @@																																																																																																																													 																																											                                
						(TopLeftPoint.next(Direction::UP) == FifthPoint && TopLeftPoint.next(Direction::UP).next(Direction::RIGHT) == SixPoint)))														  	//		@@	     @@
					return EStateCheckScreen::GOOD;																																					  		//				
				else																																												  		//               @@	
					if (TopLeftPoint.next(Direction::RIGHT) == SecondPoint && TopLeftPoint.next(Direction::UP).next(Direction::UP) == ThirdPoint &&                                                         //             ========
						TopLeftPoint.next(Direction::UP).next(Direction::UP).next(Direction::RIGHT) == ForthPoint && TopLeftPoint.next(Direction::UP) == FifthPoint &&
						TopLeftPoint.next(Direction::UP).next(Direction::RIGHT) == SixPoint)
						return EStateCheckScreen::GOOD;
					else
						if (TopLeftPoint.next(Direction::LEFT) == SecondPoint && TopLeftPoint.next(Direction::DOWN) == ThirdPoint &&
							TopLeftPoint.next(Direction::DOWN).next(Direction::LEFT) == ForthPoint && TopLeftPoint.next(Direction::DOWN).next(Direction::DOWN) == FifthPoint &&
							TopLeftPoint.next(Direction::DOWN).next(Direction::DOWN).next(Direction::LEFT) == SixPoint)
							return EStateCheckScreen::GOOD;
						else
							return EStateCheckScreen::INVALID_BIG_SPACESHIP;



}
EStateCheckScreen TheMathGame::CheckForValidSmallShip(Spaceship & ship)
{
	Point TopLeftPoint = ship.GetTopLeft();
	Point SecondPoint;
	Point ThirdPoint;


	list<Point> ::iterator ShipPointPtr = ship.GetBody().begin();//pointer to the begin of the list.

	SecondPoint = *(++ShipPointPtr);//get the second point on the list.
	ThirdPoint = *(++ShipPointPtr);//get the Third point on the list.

								   //Check the space ship on his regular shape, 3 Casese:

	if (TopLeftPoint.next(Direction::RIGHT) == SecondPoint && TopLeftPoint.next(Direction::RIGHT).next(Direction::RIGHT) == ThirdPoint)
		return EStateCheckScreen::GOOD;
	else
		if (TopLeftPoint.next(Direction::RIGHT) == SecondPoint && TopLeftPoint.next(Direction::LEFT) == ThirdPoint)
			return EStateCheckScreen::GOOD;
		else
			if (TopLeftPoint.next(Direction::LEFT) == SecondPoint && TopLeftPoint.next(Direction::LEFT).next(Direction::LEFT) == ThirdPoint)
				return EStateCheckScreen::GOOD;
			else
				//Check the space ship on his twist shape, 3 Casese:
				if ((TopLeftPoint.next(Direction::DOWN) == SecondPoint) && (TopLeftPoint.next(Direction::DOWN).next(Direction::DOWN) == ThirdPoint ||
					TopLeftPoint.next(Direction::UP) == ThirdPoint))
					return EStateCheckScreen::GOOD;
				else
					if (TopLeftPoint.next(Direction::UP) == SecondPoint && TopLeftPoint.next(Direction::UP).next(Direction::UP) == ThirdPoint)
						return EStateCheckScreen::GOOD;
					else
						return EStateCheckScreen::INVALID_SMALL_SPACESHIP;
}
EStateCheckScreen TheMathGame::PrintShipMsg(Spaceship & ship, bool PrintToScreen)
{

	if (ship.GetMySymbol() == Symbols::BIG_SHIP)//this is big ship.
	{
		if (ship.GetMyVolume() == (int)EStateCheckScreen::NO_BIG_SPACESHIP)//not found any big ships.
		{
			if (PrintToScreen)
				PrintErrorMassage(EStateCheckScreen::NO_BIG_SPACESHIP, (int)ELocationSPace::NOT_A_ROW, (int)ELocationSPace::NOT_A_COL, YELLOW);
			return EStateCheckScreen::BAD;
		}
		else
			if (ship.GetMyVolume() != (int)Symbols::BIG_SHIP_VOLUME) //volume of big ship is 6 so anything else is invalid.
			{
				if (PrintToScreen)
					PrintErrorMassage(EStateCheckScreen::INVALID_BIG_SPACESHIP, (int)ELocationSPace::NOT_A_ROW, (int)ELocationSPace::NOT_A_COL, YELLOW);
				return EStateCheckScreen::BAD;
			}
			else
				if (CheckForValidBigShip(ship) == EStateCheckScreen::INVALID_BIG_SPACESHIP)
				{
					if (PrintToScreen)
					{
						PrintErrorMassage(EStateCheckScreen::INVALID_BIG_SPACESHIP, (int)ELocationSPace::NOT_A_ROW, (int)ELocationSPace::NOT_A_COL, YELLOW);
					}
					return EStateCheckScreen::BAD;
				}
				else
					return EStateCheckScreen::GOOD;
	}
	else
	{//this is Small ship.
		if (ship.GetMyVolume() == (int)EStateCheckScreen::NO_SMALL_SPACESHIP - 1)//not found any big ships.
		{
			if (PrintToScreen)
			{
				PrintErrorMassage(EStateCheckScreen::NO_SMALL_SPACESHIP, (int)ELocationSPace::NOT_A_ROW, (int)ELocationSPace::NOT_A_COL, LIGHTBLUE);
			}

			return EStateCheckScreen::BAD;
		}
		else
			if (ship.GetMyVolume() != (int)Symbols::SMALL_SHIP_VOLUME) //volume of small ship is 3 so anything else is invalid.
			{
				if (PrintToScreen)
				{
					PrintErrorMassage(EStateCheckScreen::INVALID_SMALL_SPACESHIP, (int)ELocationSPace::NOT_A_ROW, (int)ELocationSPace::NOT_A_COL, LIGHTBLUE);
				}
				return EStateCheckScreen::BAD;
			}
			else
				if (CheckForValidSmallShip(ship) == EStateCheckScreen::INVALID_SMALL_SPACESHIP)
				{
					PrintErrorMassage(EStateCheckScreen::INVALID_SMALL_SPACESHIP, (int)ELocationSPace::NOT_A_ROW, (int)ELocationSPace::NOT_A_COL, LIGHTBLUE);
					return EStateCheckScreen::BAD;
				}
				else
					return EStateCheckScreen::GOOD;
	}
}
//======================== END SHIPS CHECK=================================================//


//========================SCREEN CHECHER========================================//
EStateCheckScreen TheMathGame::ScreenCheck(const char *  filename, bool PrintToScreen)
{
	ifstream ScreenFile;
	char line[MAX_PATH] = "";
	size_t id;
	EStateCheckScreen ReturnState;
	bool validScreen = true;
	bool HasDuplicate = false;
	vector <int> DupFilenamesIndex;

	OpenFile(filename, ScreenGamesExtension, &ScreenFile, nullptr);


	if (!ScreenFile.is_open()) // if the file for some reason didnt open.
	{
		PrintErrorMassage(filename, EFileState::FAILED_TO_OPEN);
		return EStateCheckScreen::BAD;
	}

	//SCREEN ID CHECK==>

	ScreenFile.getline(line, MAX_PATH);//get the line of the screen id.



	ReturnState = CheckTheScreenId(line, id, false); //check the screen id line.

	if (ReturnState == EStateCheckScreen::GOOD) //if the scrren id valid,check for duplicates.
	{
		ReturnState = SearchForDuplicateScreenId(filename, id, DupFilenamesIndex, PrintToScreen);
		if (ReturnState == EStateCheckScreen::DUPLICATE_ID)
		{
			HasDuplicate = true;
			if (PrintToScreen)
			{
				PrintErrorMassage(EStateCheckScreen::DUPLICATE_ID, (int)ELocationSPace::NOT_A_ROW, (int)ELocationSPace::NOT_A_COL, Color::LIGHTRED);
				PrintErrorMassage(DupFilenamesIndex, Color::LIGHTRED);
			}
			else
			{
				ScreenFile.close();
				return EStateCheckScreen::DUPLICATE_ID;
			}
		}
	}
	else
		if (PrintToScreen)
		{
			PrintErrorMassage(EStateCheckScreen::INVALID_SCREENID, (int)ELocationSPace::NOT_A_ROW, (int)ELocationSPace::NOT_A_COL, Color::RED);
			validScreen = false;
		}
		else
		{
			ScreenFile.close();
			return EStateCheckScreen::BAD;
		}

	//MATH EXE CHECK==>	

	ScreenFile.getline(line, MAX_PATH);//get the line of the Math Exercise.

	ReturnState = CheckTheMathExerciseLine(line);

	if (ReturnState != EStateCheckScreen::GOOD)//check if the Math Exercise line is vlaid.
	{
		if (PrintToScreen)
			PrintErrorMassage(ReturnState, (int)ELocationSPace::NOT_A_ROW, (int)ELocationSPace::NOT_A_COL, Color::LIGHTGREEN);
		else
		{
			ScreenFile.close();
			return EStateCheckScreen::BAD;
		}
		validScreen = false;
	}


	//BOARD CHECKS==>

	ReturnState = CheckScreenBoard(ScreenFile, PrintToScreen);
	if (ReturnState != EStateCheckScreen::GOOD)
		validScreen = false;


	//==========================END CHECKS==========================

	ScreenFile.close();

	if (HasDuplicate)
		return EStateCheckScreen::DUPLICATE_ID;
	else
		if (!validScreen)
			return EStateCheckScreen::BAD;
		else
			return EStateCheckScreen::GOOD;
}

//=================================SCREEN CHECHER========================================//


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          END  CHECK SCREEN FUNCTIONS                                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*-----------------------------------------------------------------------------------------------------------------------------------------------------
This function get ifstream by ref and copy the screen to to the board of the game.
the function get board with ROWS=24,COLS=80, so every thing after considered as comment.
if in the board there is a tab the function chage it to space.
if the line of the board end befor we get COLS charectares the fucntion put spaces on the rest of the line.
------------------------------------------------------------------------------------------------------------------------------------------------------*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TheMathGame::SetBoardFromFile(ifstream &inFile, size_t LinesToSkip)
{
	char line[MAX_PATH];
	char ch;
	int i;
	int j;

	getLineInFile(LinesToSkip - 1, inFile, line, MAX_PATH);

	for (i = 0; i < ROWS && !inFile.eof(); ++i)
	{

		for (j = 0; j < COLS && !inFile.eof(); ++j)
		{
			ch = inFile.get();

			if (ch == (char)Symbols::TAB)
			{
				board[i][j] = ' ';
			}
			else
				if (ch == '\n') //if the line finish before we get all the cols put spaces in the row.
				{
					while (j < COLS)
					{
						board[i][j] = (char)Symbols::SPACE;
						++j;
					}
				}
				else
				{
					board[i][j] = ch;
				}
		}

		if (ch != (char)Symbols::ENTER)//if the line not finish, keep input until get the end of the line.
		{
			ch = inFile.get();

			while (ch > 0 && i < ROWS && ch != (char)Symbols::ENTER)
			{
				ch = inFile.get();
			}
		}
	}


	if (i != ROWS)//in case the user make a screen with less then ROWS Lines.   
	{            //fill the rest of the lines with COLS spaces.
		for (; i < ROWS; ++i)
		{
			for (j = 0; j < COLS; ++j)
				board[i][j] = (char)Symbols::SPACE;
		}

	}

}


///////////////////////////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------------------------
This function find the first screen that valid.
if the function find vlaid scrren return HAS_NEXT_LEVEL, else return ERROR_NO_LEVELS.
-------------------------------------------------------------------------------------*/
///////////////////////////////////////////////////////////////////////////////////////
int TheMathGame::GetScreenToPlay()
{
	EStateCheckScreen ValidScreen = EStateCheckScreen::BAD;
	size_t size;
	size_t index;
	size = MathBoards.size();

	if (!hasNextLevel())
		return EStateGameScreen::FINISH_LEVELS;


	index = CurrentLevel;
	//Search for the first Screen that valid.
	while (index < size && ValidScreen != EStateCheckScreen::GOOD)
	{
		if (!MathBoards[index].GetisCheckValue())
		{

			ValidScreen = ScreenCheck(MathBoards[index].GetScreenfilename(), false);

			if (ValidScreen != EStateCheckScreen::GOOD)
			{
				index++;
				CurrentLevel++;
			}
			else
			{
				MathBoards[index].SetisChecked(true);
			}
		}
	}


	if (ValidScreen == EStateCheckScreen::GOOD)
	{
		return EStateGameScreen::HAS_NEXT_LEVEL;
	}
	else
		return EStateGameScreen::ERROR_NO_LEVELS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------------------------
This function Search duplicates ScreensId.
A)in case we in PrintErrorOfScreen mode, the function get all the duplicate file name and put them in PrintErrorOfScreen.
B)else we just want to know if there is a duplicate screen.
in case find duplicate The function return EStateCheckScreen::DUPLICATE_ID else return EStateCheckScreen::GOOD ;
----------------------------------------------------------------------------------------------------------------------*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EStateCheckScreen TheMathGame::SearchForDuplicateScreenId(const char * filename, size_t id, vector <int> & DupFilenames, bool PrintErrorOfScreen)
{
	bool found = false;
	size_t size = MathBoards.size();

	//Find if there is duplicates screen if, so we need to delete this screen.

	for (size_t i = 0; i < size && (!found || PrintErrorOfScreen); ++i) // if we here in case to save all the duplicates screen id file names, the loop goes over all the the file.
	{
		if (MathBoards[i].GetScreenid() == id && strcmp(filename, MathBoards[i].GetScreenfilename()) != 0)
		{
			if (PrintErrorOfScreen)                            //need to print the dup files, so just save the filename.
			{
				DupFilenames.push_back(i);                     //give me just the index of the other duplicates files.	
			}

			found = true;

		}//in case this screen dont duplicate screen with me,go to the next scren.

	}

	if (PrintErrorOfScreen && DupFilenames.size() == 0)
		return EStateCheckScreen::GOOD;

	if (found)
	{
		return EStateCheckScreen::DUPLICATE_ID;
	}

	else
		return EStateCheckScreen::GOOD;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------------------------
This function return true if can get the screen id from the file and put it in ScreenId else return false.
----------------------------------------------------------------------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EStateCheckScreen TheMathGame::CheckTheScreenId(char * ScreenIdRow, size_t & ScreenId, bool FromAddList)
{
	string line;
	size_t found;
	EStateCheckScreen ReturnState;

	line = ScreenIdRow;

	found = line.find("ScreenID");//return the first place of "ScreenId" in line.

	if (found == string::npos)//not found.
		ReturnState = EStateCheckScreen::INVALID_SCREENID;
	else
	{
		found += (int)ELocationSPace::SCREENID_LEN;//go to the next character.

		if (!ThisIslegalCharAterScreenIdOrMathExe(line[found]))//check if this char is legal.i.e this char is = or : or - or _
			ReturnState = EStateCheckScreen::INVALID_SCREENID;
		else
		{
			found += (int)ELocationSPace::NEXT_ROW;
			found = line.find_first_not_of(' ', found); // get the first index of char that he dosent white space.

			if (line[found] >= '0' && line[found] <= '9')//this is a digit.
			{
				ScreenId = (size_t)atoi(ScreenIdRow + found);
				if (!FromAddList && ScreenId > 1000)//screen id is 3 digits number.
					ReturnState = EStateCheckScreen::INVALID_SCREENID;
				else
					ReturnState = EStateCheckScreen::GOOD;
			}//not a digit so the screen id not valid.
			else
				ReturnState = EStateCheckScreen::INVALID_SCREENID;
		}
	}

	return ReturnState;
}

//////////////////////////////////////////////////////////////////////////
/*------------------------------------------------------------------------
This function Put into LastModified the last date that Modified this file.
------------------------------------------------------------------------*/
/////////////////////////////////////////////////////////////////////////
EStateCheckScreen TheMathGame::GetLastModified(TCHAR * filename, char * LastModified)
{
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	DWORD dwRet;

	HANDLE hFile;
	TCHAR Last[MAX_PATH];

	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		cout << "Could not open file, error " << GetLastError() << endl;
		return EStateCheckScreen::BAD;
	}

	// Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return EStateCheckScreen::BAD;

	// Convert the last-write time to local time.
	FileTimeToSystemTime(&ftWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	// Build a string showing the date and time.
	dwRet = StringCchPrintf(Last, MAX_PATH,
		TEXT("%02d/%02d/%d  %02d:%02d"),
		stLocal.wMonth, stLocal.wDay, stLocal.wYear,
		stLocal.wHour, stLocal.wMinute);

	wcstombs(LastModified, Last, MAX_PATH);//convert tchar/wchar to char *.

	CloseHandle(hFile);

	if (S_OK == dwRet)
		return EStateCheckScreen::GOOD;
	else
		return EStateCheckScreen::BAD;


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------------------------------------
This function get a file name and if he had a valid screen id, the function add this file to screen list.
---------------------------------------------------------------------------------------------------------*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////
EFileState TheMathGame::AddThisScreenToList(const char * filename, const char * LastModified, vector<string> & Update)
{
	size_t screenId;
	EStateCheckScreen hasValidScreenId;
	ifstream ScreenFile;
	char line[MAX_PATH] = "";

	OpenFile(filename, ScreenGamesExtension, &ScreenFile, nullptr);


	if (!ScreenFile.is_open())
		return EFileState::FAILED_TO_OPEN;

	ScreenFile.getline(line, MAX_PATH, (char)Symbols::ENTER);
	ScreenFile.close();

	hasValidScreenId = CheckTheScreenId(line, screenId, true);

	if (hasValidScreenId == EStateCheckScreen::INVALID_SCREENID)
		return EFileState::ILEGAL_FILE;

	Update.push_back(filename);       // get the new update screens name.
	Update.push_back(LastModified);  //get the new last date modified of the file. 

	if (ThisISaNewScreen(filename, LastModified, screenId))//add this screen just if he is a new screen
	{
		MathBoards.push_back(Boards(screenId, filename, LastModified)); //copy the new potnsial screen.
		return EFileState::SUCCEEDED;
	}
	else
		return EFileState::NOT_ADD_TO_LIST;



}

bool TheMathGame::ThisISaNewScreen(const char * filename, const  char * NewLastModifed, size_t id)
{
	size_t size;
	bool ISaNewScreen = true;

	size = MathBoards.size();

	for (size_t i = 0; i < size && ISaNewScreen; ++i)
	{
		if (strcmp(MathBoards[i].GetScreenfilename(), filename) == 0)
		{							// check if this a new file.
									// by checking file name
			ISaNewScreen = false;
			if (strcmp(MathBoards[i].GetScreenLastModified(), NewLastModifed) != 0)
			{
				MathBoards[i].SetisChecked(false);//check if this file was update so we need to check him again.
				MathBoards[i].SetScreenid(id);
			}
		}
	}

	return ISaNewScreen;
}

bool  ScreenidCompare(const Boards & FirstBoard, const Boards & SecondBoard)
{
	return (FirstBoard.GetScreenid() < SecondBoard.GetScreenid());
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------------------------------------------
									PRINTING ERROR FUNCTIONS
---------------------------------------------------------------------------------------------------------------*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TheMathGame::PrintErrorMassage(const char * filename, EFileState FilseState)
{
	if (FilseState == EFileState::FAILED_TO_OPEN)
		cout << "  ERROR The File:" << filename << "has failed to open!\n";
	else
		cout << "  The File:" << filename << "has ILgal Screen!\n";
}
void TheMathGame::PrintErrorMassage(vector <int> & DupFileNames, Color color)
{
	setTextColor(color);
	size_t size = DupFileNames.size();//DupFileNames is containing the indexs of the duplicate files in MathBoards.

	cout << "  #Duplicates ScreenID with:";

	for (size_t i = 0; i < size; ++i)
		cout << " " << MathBoards[DupFileNames[i]].GetScreenfilename();

	cout << ".\n";
	setTextColor(DEFAULT_COLOR);

}
void TheMathGame::PrintErrorMassage(EStateCheckScreen CheckState, int row, int col, Color color)
{
	setTextColor(color);

	switch (CheckState)
	{
	case EStateCheckScreen::DUPLICATE_ID:
		cout << "+ #Duplicates Screen Ids!!\n";
		break;
	case EStateCheckScreen::WARNINIG_TAB:
		cout << "+ #Warning in Row= " << row << ",Col= " << col << "\n" << "   Found TAB,The screen look different than it appears in the text editor!\n";
		break;
	case EStateCheckScreen::INVALID_SCREENID:
		cout << "+ #The ScreenID is invalid!!\n";
		break;
	case EStateCheckScreen::INVALID_MATHEXE:
		cout << "+ #The Math Exercises is invalid!!\n";
		break;
	case EStateCheckScreen::MATHEXE_WITHOUT_UNKONWNS:
		cout << "+ #Equation is required to have at least one unknown!!\n";
		break;
	case EStateCheckScreen::NO_BIG_SPACESHIP:
		cout << "+ #No Big Ship, How you forget?!!!\n";
		break;
	case EStateCheckScreen::NO_SMALL_SPACESHIP:
		cout << "+ #No Small Ship, How you forget?!!!\n";
		break;
	case EStateCheckScreen::NO_S:
		cout << "+ #Not Found Place for legend!!\n";
		break;
	case EStateCheckScreen::NO_X:
		cout << "+ #Not Found Place for Math Exercies!!\n";
		break;
	case EStateCheckScreen::TOO_MANY_S_PLACE:
		cout << "+ #Too Many Places For legend!!\n";
		break;
	case EStateCheckScreen::TOO_MANY_X_PLACE:;
		cout << "+ #Too Many Placess for Mathe Exerciese!!\n";
		break;
	case EStateCheckScreen::MISSING_WALL_X_PLACE:
		cout << "+ #Math Exericses Place Missing Walls!!\n";
		break;
	case EStateCheckScreen::MISSING_WALL_S_PALCE:
		cout << "+ #Legened Place Missing Walls!!\n";
		break;
	case EStateCheckScreen::MISSINGSPACES_FOR_X:
		cout << "+ #Math Exericses Place Missing Spaces, Recuire 22 Spaces!!\n";
		break;
	case EStateCheckScreen::MISSINGSPACES_FOR_S:
		cout << "+ #Legened Place Missing Spaces, Recuire 16 Spaces!!!!\n";
		break;
	case EStateCheckScreen::SHIP_INSIDE_S_BLOCK:
		cout << "+ #There is a Ship Inside Legend Block!!\n";
		break;
	case EStateCheckScreen::SHIP_INSIDE_X_BLOCK:
		cout << "+ #There is a Ship Inside Math Exercise Block!!\n";
		break;
	case EStateCheckScreen::INVALID_BIG_SPACESHIP:
		cout << "+ #Shame on you! The Big Ship is invalid!!\n";
		break;
	case EStateCheckScreen::INVALID_SMALL_SPACESHIP:
		cout << "+ #Shame on you! The Small Ship is invalid!!\n";
		break;
	}

	setTextColor(DEFAULT_COLOR);
}
void TheMathGame::PrintErrorMassage(int & indexToPrintErrorMsg)
{
	gotoxy((int)ELocationSPace::LOCATION_X_TO_PRINT, (int)ELocationSPace::LOACATION_Y_TO_PRINT + (int)ELocationSPace::NEXT_ROW + indexToPrintErrorMsg);
	cout << "Please Try Again and Now with legal number or -1.\n";
	++indexToPrintErrorMsg;
	gotoxy((int)ELocationSPace::LOCATION_X_TO_PRINT, (int)ELocationSPace::LOACATION_Y_TO_PRINT + (int)ELocationSPace::NEXT_ROW + indexToPrintErrorMsg);
	++indexToPrintErrorMsg;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------------------------------------------
								   END  PRINTING ERROR FUNCTIONS
--------------------------------------------------------------------------------------------------------------*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////





void TheMathGame::SetAllFileToUnChecked()
{
	size_t size = MathBoards.size();

	for (size_t i = 0; i < size; ++i)
	{
		MathBoards[i].SetisChecked(false);
	}
}
void TheMathGame::CheckIfNeedToDeleteSomeOldScreens(vector <string> & Update)
{
	ofstream ScreenFiles;
	size_t NewScreensize;
	size_t oldScreensize;
	size_t i, index;
	bool found;
	const char * filename;
	ifstream file;

	NewScreensize = Update.size();
	oldScreensize = MathBoards.size();

	//find the screen that need to erase.
	for (i = index = 0; i < oldScreensize; ++i)
	{
		found = false;
		filename = MathBoards[index].GetScreenfilename();
		for (size_t j = 0; j < NewScreensize && !found; j += 2)
		{
			if (strcmp(filename, Update[j].c_str()) == 0) //check if this screen is active.
				found = true;
		}

		if (!found)
		{//erase this screen from MathBoards.
			MathBoards.erase(MathBoards.begin() + index);
		}
		else
			index++;
	}



}

/////////////////////////////////////////////////////////
/*-------------------------------------------------------
This function create the data base of the screen files.
-------------------------------------------------------*/
/////////////////////////////////////////////////////////
bool TheMathGame::getBoards()
{
	EFileState FileState;
	char LastModified[MAX_PATH];
	TCHAR TCHARfileName[MAX_PATH];
	size_t oldsizeofMathBoards;
	vector <string> Update;
	vector<string> Filenames;
	bool ItemsDeltedDoSort;
	size_t size;

	oldsizeofMathBoards = MathBoards.size();

	GetFilesFromFolder(Filenames, ScreenGamesExtension);//put into Filenames all the file names of ScreenGamesExtension in the directory.

	size = Filenames.size();

	for (size_t i = 0; i < size; ++i)
	{

		mbstowcs(TCHARfileName, Filenames[i].c_str(), MAX_PATH);     //converte filename char * to TCHAR.

		wcscat_s(TCHARfileName, TCHARScreenGamesExtension);

		GetLastModified(TCHARfileName, LastModified);               //put into LastModified the last date modified of the file.

		FileState = AddThisScreenToList(Filenames[i].c_str(), LastModified, Update);
	}


	if (MathBoards.size() > Filenames.size())
	{//so we need to delete some screens.
		CheckIfNeedToDeleteSomeOldScreens(Update);
		ItemsDeltedDoSort = true;
	}

	if (MathBoards.size() != oldsizeofMathBoards || ItemsDeltedDoSort)// made changes in MathBoards do sort.
	{
		SetAllFileToUnChecked();
		sort(MathBoards.begin(), MathBoards.end(), ScreenidCompare);
	}


	if (MathBoards.size() > 0)
		return true;
	else
		return false;



}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------------------
	   This function running the ScreenCheck function in PrintErrorOfScreen, so all the Error print to the screen.
----------------------------------------------------------------------------------------------------------------*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int TheMathGame::CheckThisGame(int option, bool FromPlaySpecificScreen)
{
	EStateCheckScreen ReturnState;
	char ch;

	if (FromPlaySpecificScreen)
		gotoxy((int)ELocationSPace::LOCATION_X_TO_PRINT, (int)ELocationSPace::LOACATION_Y_TO_PRINT);
	else
		gotoxy((int)ELocationSPace::LOCATION_X_IN_SCREENCHECK, (int)ELocationSPace::LOCATION_Y_IN_SCREENCHECK);

	cout << MathBoards[option].GetScreenfilename() << " is checking Now.....\n\n";

	ReturnState = ScreenCheck(MathBoards[option].GetScreenfilename(), !FromPlaySpecificScreen);
	fflush(stdin);

	if (!FromPlaySpecificScreen)
	{

		if (ReturnState == EStateCheckScreen::GOOD)
		{
			cout << "\n    This screen valid and that Excites Me!!!\n\n";
		}
		cout << "\n    Press Any Key To Back To Main Menu.........\n";


		ch = _getch();
	}



	return (int)ReturnState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------------------
This function get input from the user until the user input a legal input, and return his choice.
----------------------------------------------------------------------------------------------------------------*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int TheMathGame::InputFromTheUser(int indexToPrintErrorMsg)
{
	int option;
	int size = (int)MathBoards.size();

	fflush(stdin);

	gotoxy((int)ELocationSPace::LOCATION_X_TO_PRINT, (int)ELocationSPace::LOACATION_Y_TO_PRINT + (int)ELocationSPace::NEXT_ROW + indexToPrintErrorMsg++);

	//Check for legal input, in case the user input integer number
	//that in the range of 0 up to size-1 or -1 to exit =.
	//any other option make the user input again.
	while (!(cin >> option) || (option >= size || option < -1))
	{
		cin.clear();                                          //clear the flag.
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard the bad input.
		PrintErrorMassage(indexToPrintErrorMsg);

	}

	return option;

}

/////////////////////////////////////////////////////////////
/*----------------------------------------------------------
This function Print all the Screen file name to the screen
----------------------------------------------------------*/
////////////////////////////////////////////////////////////
int TheMathGame::PrintGamesName()
{
	int size = MathBoards.size();
	int i;
	// Presents to the user a list of saved games  , which received by the game

	gotoxy((int)ELocationSPace::LOCATION_X_TO_PRINT, (int)ELocationSPace::LOACATION_Y_TO_PRINT);

	cout << "Please select number file or -1 to Exit:\n";
	for (i = 0; i < size; i++)
	{
		gotoxy((int)ELocationSPace::LOCATION_X_TO_PRINT, (int)ELocationSPace::LOACATION_Y_TO_PRINT + (int)ELocationSPace::NEXT_ROW + i);
		cout << "[" << i << "] " << MathBoards[i].GetScreenfilename();
		
		if (isRecordExist(MathBoards[i].GetScreenid()))
			cout << " - Has solution available";
		cout << endl;
	}

	// Get the option from the user
	return InputFromTheUser(i);

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------------------
This function get the option that the user input and if this screen has valid, begin the game from this level,
else print massage that this screen is a invalid screen and back to main menu.
----------------------------------------------------------------------------------------------------------------*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool TheMathGame::PlayFromSpecificScreen(int  option)
{
	EStateCheckScreen Valid;
	char ch;
	bool State = true;
	CurrentLevel = (size_t)option;

	Valid = (EStateCheckScreen)CheckThisGame(option, true);

	if (Valid == EStateCheckScreen::GOOD)
	{
		gotoxy((int)ELocationSPace::LOCATION_X_TO_PRINT, (int)ELocationSPace::LOACATION_Y_TO_PRINT + (int)ELocationSPace::NEXT_TOW_ROW);
		cout << "This screen valid!!!\n\n";
		gotoxy((int)ELocationSPace::LOCATION_X_TO_PRINT, (int)ELocationSPace::LOACATION_Y_TO_PRINT + (int)ELocationSPace::NEXT_THREE_ROW);
		cout << "Press Any Key To Play.........\n";

	}
	else
	{
		State = false;
		gotoxy((int)ELocationSPace::LOCATION_X_TO_PRINT, (int)ELocationSPace::LOACATION_Y_TO_PRINT + (int)ELocationSPace::NEXT_TOW_ROW);
		cout << "This screen invalid!!!\n\n";
		gotoxy((int)ELocationSPace::LOCATION_X_TO_PRINT, (int)ELocationSPace::LOACATION_Y_TO_PRINT + (int)ELocationSPace::NEXT_THREE_ROW);
		cout << "Press Any Key To Back To Main Menu.........\n";
	}

	ch = _getch();

	return State;

}

// Receives by ref a point on screen, finds the closest number to this point and returns its median point
Point TheMathGame::GetClosestNumMedianPoint(const Point&p)
{
	Num &_temp = numbers.begin()._Ptr->_Myval;
	Point res, temp;

	_temp.GetNumMedianPoint(res);

	for (list<Num>::iterator iter = numbers.begin(); iter != numbers.end(); iter++)
	{
		iter->GetNumMedianPoint(temp);
		if (temp.GetDistance(p) < res.GetDistance(p))
			res = temp;
	}
	return res;
}

//Clears a number from screen and from the numbers list , receiving a point belongs to the number, finds the whole number and deletes it
int TheMathGame::removeNum(const Point& p)
{
	int val;
	Num num2del(p);
	val = num2del.GetVal();
	numbers.remove(num2del);
	ClearNumFromScreen(num2del);
	return val;
}


// Clears a number from screeen
void TheMathGame::ClearNumFromScreen(const Num& Num2Clear)
{
	int X, Y, RightX;
	X = Num2Clear.GetTopLeft().GetX();
	Y = Num2Clear.GetTopRight().GetY();
	RightX = Num2Clear.GetTopRight().GetX();

	while (X <= RightX)
	{
		gotoxy(X, Y);
		RemoveDigFromBoard(X, Y);											//update the board that there is no number in this coordinate.
		cout << (char)(Symbols::SPACE);									   //print space instead of the number.
		++X;
	}

}

