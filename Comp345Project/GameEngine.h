#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Cards.h"
#include "CommandProcessing.h"
#include "Orders.h"
#include "Map.h"
#include "Player.h"

using namespace std;

class CommandProcessor;
class GameEngine {

private:

	CommandProcessor* cmdProcessor;
	Map* map;
	vector<Player*> playerList;
	Deck* deck;

	bool debugMode;
	//default to be false; when switch to true, 
	// - the game wont randomly seletct the first player.
	bool isStartup; 
	//the current game stage: on startup ,or on play stage
	int currentStage;
	int initialPlayerTerritoriesAmount;

	/*
	* currentStage - different value refers to different current stage.
	* 0 - default
	* 1 - start
	* 2 - map loaded
	* 3 - map validated
	* 4 - players added
	* 5 - assign reinforcement
	* 6 - issue orders
	* 7 - execute orders
	* 8 - win
	*/



public:
	//constructors
	GameEngine();
	GameEngine(CommandProcessor* inputCmdProcessor, int iniTerrAmount,bool isDebugmode);
	GameEngine(const GameEngine& ge);
	//Assignment Operator
	GameEngine& operator = (const GameEngine& ge);
	//Stream Insertion Operator
	friend ostream& operator << (ostream& ost, const GameEngine& ge);
	~GameEngine();


	void startup();
	//start the startup phases.
	void mainGameLoop(int startingPlayer);
	//start the play phases.

	void reinforcementPhase();
	int issueOrderPhase(int startingPlayer);
	void executreOrderPhase(int startingPlayer);


	void reset();
	//reset the variables and clear the memory.

	int switchCurrentPlayer(int current);
	//change to the next player, let him issue the order.


	void showState(std::ostream& output) const;
	void showState() ;
	//shows the current stage the game is on.

	//Accessors
	int getCurrentGameStage();
	bool getIsStartup();
};