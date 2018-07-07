#pragma once
#ifndef _GAME_MOVE_H
#define _GAME_MOVE_h


//A struct contains a game move for each important iteration
//Important iteration is an iteration when the player pressed on the keys and moved the game objects
struct GameMove {
	int IterationNum;
	char  FirstKey;
	char SecondKey;
	GameMove(int _iteration, char _FirstKey, char _SecondKey) : IterationNum(_iteration), FirstKey(_FirstKey), SecondKey(_SecondKey) {}
};

#endif