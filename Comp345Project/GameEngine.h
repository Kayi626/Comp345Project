#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Cards.h"
#include "CommandProcessing.h"
#include "Orders.h"
#include "Map.h"
#include "Player.h"
#include "LoggingObserver.h"

using namespace std;

class CommandProcessor;

class GameEngine: public ILoggable, public Subject {
private:

	static GameEngine* GE_instance;
	//using Singleton class design pettern.

	void init();
	void init(const GameEngine &);
	void destory();
	string stringToLog();
	LogObserver* logger;

	CommandProcessor* cmdProcessor;
	Map* map;
	vector<Player*> playerList;
	Deck* deck;
	string stageToString(int b);
	vector<vector<int>> negotiateOrderList;

	bool debugMode;
	//default to be false; when switch to true, 
	// - the game wont randomly seletct the first player.
	bool isStartup; 
	//the current game stage: on startup ,or on play stage
	int currentStage;

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
	* -1 - check win condition
	*/

	int initialPlayerTerritoriesAmount;
	
public:
	//change the state of the game
	void transition(int newState);
	//constructors
	GameEngine();
	GameEngine(CommandProcessor* inputCmdProcessor, int iniTerrAmount,bool isDebugmode);
	GameEngine(const GameEngine& ge);
	//Assignment Operator
	GameEngine& operator = (const GameEngine& ge);
	//Stream Insertion Operator
	friend ostream& operator << (ostream& ost, const GameEngine& ge);
	~GameEngine();


	static GameEngine* instance();
	static bool isDebugMode;
	static string fileLineReaderFilePath;
	static int defualtTerritoriesAmount;
	static bool useFileCommandProcessor;

	void startup();
	//start the startup phases.
	void mainGameLoop(int startingPlayer);
	//start the play phases.

	void reinforcementPhase();
	int issueOrderPhase(int startingPlayer);
	void executreOrderPhase(int startingPlayer);

	void subtractPlayerPool(int playerID, int amount);
	//subtract some amount of armies from target player's reinforcementpool.
	bool checkPlayerPool(int playerID, int amount);
	//if this player does't exist or he don't have enough armies, will return false.
	void addToNegotiateOrderList(int fromPlayerID, int toPlayerID);
	bool checkNegotiateOrderList(int fromPlayerID, int toPlayerID);
	void setPlayerConquered(int fromPlayerID);
	void removePlayerFromPlayerList(int playerID);
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
