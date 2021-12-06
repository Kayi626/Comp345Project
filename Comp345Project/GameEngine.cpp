#include "GameEngine.h"
#include "Player.h"
#include "PlayerStrategies.h"
#include "Orders.h"
#include "Cards.h"
#include "CommandProcessing.h"
#include <string>
#include <regex>
#include <random>
#include <ctime>
#include <vector>
#include <iostream>
#include<map>

using namespace std;


GameEngine* GameEngine::GE_instance = 0;
bool GameEngine::isDebugMode = false;
bool GameEngine::useFileCommandProcessor = true;
string GameEngine::fileLineReaderFilePath = "../Comp345Project/commands_p4_t1.txt";
int GameEngine::defualtTerritoriesAmount = 2;
bool GameEngine::tournaMode = false;
map<string, string> GameEngine::mapfile_map = {{"1", "LOTR2.map"}, {"2","smallMap.map"}, {"3","bigeurope.map"}};
map<string, string> GameEngine::strategy_map = { {"A","Aggressive"},{"B","Benevolent"},{"N","Neutral"},{"C","Cheater"},{"H","Human"}};
int GameEngine::map_positioner = 0;
vector<vector<string>> GameEngine::tourna_paravec;
std::vector<vector<string>> GameEngine::winRecord;

void GameEngine::setFilePath(string str) {
	fileLineReaderFilePath = str;
}
GameEngine* GameEngine::instance() {
	if (!GE_instance) {
		if (useFileCommandProcessor) {
			FileLineReader* flr = new FileLineReader(GameEngine::fileLineReaderFilePath);
			FileCommandProcessorAdapter* fcomP = new FileCommandProcessorAdapter(flr);
			GE_instance = new GameEngine(fcomP, GameEngine::defualtTerritoriesAmount, GameEngine::isDebugMode);
		}
		else {
			CommandProcessor* comP = new CommandProcessor();
			GE_instance = new GameEngine(comP, GameEngine::defualtTerritoriesAmount, GameEngine::isDebugMode);
		}
	}
	return GE_instance;
}


//Constructor
void GameEngine::init() {
	this->isStartup = true;
	this->debugMode = false;
	this->currentStage = 0;
	this->cmdProcessor = new CommandProcessor();
	this->map = new Map();
	this->deck = new Deck();
	this->deck->original_vec_deck();
	this->initialPlayerTerritoriesAmount = 4;
	this->logger = new LogObserver();
	this->attach(logger);
	this->cmdProcessor->attach(this->logger);
}
void GameEngine::init(const GameEngine &ge) {
	this->isStartup = ge.isStartup;
	this->debugMode = ge.debugMode;
	this->currentStage = ge.currentStage;
	this->initialPlayerTerritoriesAmount = ge.initialPlayerTerritoriesAmount;
	this->cmdProcessor = new CommandProcessor(*(ge.cmdProcessor));
	this->map = new Map(*(ge.map));
	this->deck = new Deck(*(ge.deck));
	for (auto &player : ge.playerList) {
		Player *p = new Player(*player);

		p->setPlayerStrategy(new HumanPlayerStrategy(p));
		p->attachToPlayerOrderList(this->observers);
		this->playerList.push_back(p);
	}
	for (int x = 0; x < static_cast<int>(ge.negotiateOrderList.size()); x++) {
		for (int y = 0; y < ge.negotiateOrderList[x].size(); y++) {
			this->negotiateOrderList[x].push_back(ge.negotiateOrderList[x][y]);
		}
	}
	this->logger = new LogObserver();
	this->attach(logger);
	this->cmdProcessor->attach(this->logger);
}
void GameEngine::destory() {
	if (this->cmdProcessor && string(typeid(*this->cmdProcessor).name()) == "class CommandProcessor") {
		delete this->cmdProcessor;
		this->cmdProcessor = nullptr;
	}
	if (this->map) delete this->map;
	if (this->deck) delete this->deck;
	this->map = nullptr;
	this->deck = nullptr;
	for (int i = 0; i < playerList.size(); i++)
		delete this->playerList[i];
	this->playerList.clear();
	if (this->logger) delete this->logger;
	this->logger = nullptr;
}
GameEngine::GameEngine() {
	this->init();
}
GameEngine::GameEngine(CommandProcessor* inputCmdProcessor,int iniTerrAmount,bool isDebugMode) {
	this->init();
	delete this->cmdProcessor;
	this->cmdProcessor = inputCmdProcessor;
	this->cmdProcessor->attach(this->logger);
	this->initialPlayerTerritoriesAmount = iniTerrAmount;
	this->debugMode = isDebugMode;
}

GameEngine::GameEngine(const GameEngine& ge) {
	this->init(ge);
}

//Assignment Operator
GameEngine& GameEngine::operator= (const GameEngine& ge) {
	if (this == &ge) return *this;
	this->destory();
	this->init(ge);
	return *this;
}

//Destructor
GameEngine::~GameEngine() {
	this->destory();
}
string GameEngine::stageToString(int b) {
	switch (b) {
		case 0: return "default";
		case 1: return "gamestart";
		case 2: return "map loaded";
		case 3: return "map validated";
		case 4: return "players added";
		case 5: return "assign reinforcement";
		case 6: return "issue orders";
		case 7: return "execute orders";
		case 8: return "win";
		case -1: return "check win condition";
		default: return "-";
	}
}

//Other class functions
std::string GameEngine::stringToLog() {
	return logValue;
};

void GameEngine::setGameResult(string str) {
	logValue = str;
	winRecord[map_positioner].push_back(str);
	notify(this);
}
void GameEngine::setTournaMode(bool value){
	if (value == false && tournaMode == true) {
		logValue = "\n===============================Tournament Ends===============================";
		tournaMode = value;
		notify(this);
	}
	else if (value == true && tournaMode == false) {
		logValue = "===============================Tournament Starts===============================";
		tournaMode = value;
		notify(this);
	}
	
}

//Stream Insertion Operators
ostream& operator << (ostream& ost, const GameEngine& ge) {
	ge.showState(ost);
	return ost;
}
void GameEngine::showState(std::ostream& output) const {
	switch (currentStage)
	{
	case 0:
		//output << "[Current Game State: NONE, default, before entering start phase]" << endl;
		break;
	case 1:
		output << "[Current Game State: Start up, start]" << endl;
		break;
	case 2:
		output << "[Current Game State: Start up, map loaded]" << endl;
		break;
	case 3:
		output << "[Current Game State: Start up, map validated]" << endl;
		break;
	case 4:
		output << "[Current Game State: Start up, players added]" << endl;
		break;
	case 5:
		output << "[Current Game State: Play, assign reinforcement]" << endl;
		break;
	case 6:
		output << "[Current Game State: Play, issue orders]" << endl;
		break;
	case 7:
		output << "[Current Game State: Play, execute orders]" << endl;
		break;
	case 8:
		output << "[Current Game State: Play, win]" << endl;
		break;
	case -1:
		output << "[Current Game State: Play, check win condition]" << endl;
		break;
	default:
		output << "[Current Game State: ERROR - This should never happened. go check variable 'currentStage'.]" << endl;
		break;
	}

}
void GameEngine::showState(){
	switch (currentStage)
	{
	case 0:
		//cout << "[Current Game State: NONE, default, before entering start phase]" << endl;
		break;
	case 1:
		cout << "[Current Game State: Start up, start]" << endl;
		break;
	case 2:
		cout << "[Current Game State: Start up, map loaded]" << endl;
		break;
	case 3:
		cout << "[Current Game State: Start up, map validated]" << endl;
		break;
	case 4:
		cout << "[Current Game State: Start up, players added]" << endl;
		break;
	case 5:
		cout << "[Current Game State: Play, assign reinforcement]" << endl;
		break;
	case 6:
		cout << "[Current Game State: Play, issue orders]" << endl;
		break;
	case 7:
		cout << "[Current Game State: Play, execute orders]" << endl;
		break;
	case 8:
		cout << "[Current Game State: Play, win]" << endl;
		break;
	default:
		cout << "[Current Game State: ERROR - This should never happened. go check variable 'currentStage'.]" << endl;
		break;
	}
}


void GameEngine::startup() {
	vector<vector<Territory*>> connectedGraph ;
	int playerCount = 1;
	int theStartingPlayer = 0;
	if (currentStage == 0 && isStartup == true) {
		transition(1);
	}

	//Five different map files
	


	//keep looping until the actual game start.
	while (isStartup){

		cout << "================================================================================¨[" << endl;
		showState();
		//check the current game state by a switch statement. displays the message refers to that stage.
		switch (currentStage)
		{
		case 1:
			//Start Phase
			std::cout << "1. enter \"loadmap <mapfilepath>\" to load the map \nOR \"tournament - M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D < maxnumberofturns>\" to start tournament mode" << endl ;
			break;
		case 2:
			//map loaded Phase
			std::cout << "1. enter \"validatemap\" to validate the map" << endl;
			std::cout << "2. enter \"loadmap <mapfilepath>\" to reload the map " << endl;
			break;
		case 3:
			//map validated Phase
			std::cout << "1. enter \"addplayer <playername>\" to add players into the map" << endl;
			break;
		case 4:
			//players added Phase
			std::cout << "1. enter \"gamestart\" to assign countries to each player" << endl;
			std::cout << "2. enter \"addplayer <playername>\" to add more players" << endl;
			break;
		default:
			cout << "ERROR: Invaid state during startup()." << endl;
			break;
		}

		
		Command* command = NULL;	
		bool commandIsNotValid = true;
		while (commandIsNotValid && !tournaMode) {
			//read a command .
			command = cmdProcessor->getCommand();

			commandIsNotValid = !(cmdProcessor->validate(*command, currentStage));

			//*******************Validate tournament command + Runs****************************
			tourna_paravec = cmdProcessor->validateTourna(*command, currentStage);	
		
			tournaMode = !(tourna_paravec.size() == 0);
			//Turn on tournaMode and Run games under this condition
			if (tournaMode) {

				command->saveEffect("");
				int num_map = tourna_paravec[0].size();
				int num_games = tourna_paravec[2].size();
				// TODO: ADD MAX_NUMS_RUNS INTO THE RUN
				int max_num_runs = tourna_paravec[3].size();
				for (int x = 0; x < num_map; x++) {
					map_positioner = x;
					vector<string> temp;
					winRecord.push_back(temp);
					for (int y = 0; y < num_games; y++){
						this->startup();
				    }
				}
				return;
			}
			//**************************************************************************
			else if (commandIsNotValid) {
				cout << "Invaild Command! please re-enter the command: " << endl;
				command->saveEffect("Invalid Command");
			}
			else {
				command->saveEffect("");
		
			}
		}

		//********************************Tournament Command Initialization********************************************
		if (tournaMode) {
			switch (currentStage) {
			case 1:
			    command = cmdProcessor->saveCommand("loadmap " + mapfile_map.find(tourna_paravec[0][map_positioner])->second);
				command->saveEffect("");
				break;
			case 2:
				command = cmdProcessor->saveCommand("validatemap");
				command->saveEffect("");
				break;
			case 3:
				command = cmdProcessor->saveCommand("addplayer " + strategy_map.find(tourna_paravec[1][0])->second);
				command->saveEffect("");
				break;
			case 4:
				if (playerCount <= tourna_paravec[1].size()) {
					command = cmdProcessor->saveCommand("addplayer " + strategy_map.find(tourna_paravec[1][playerCount-1])->second);				
				}
				else {
					command = cmdProcessor->saveCommand("gamestart");
				}
				command->saveEffect("");
				break;
			default:
				break;
			}
		}
		//************************************************************************************************************
		
		//execute the command refers to different stage.
		cout << endl << "Executing command:  "<< command->getOriginalCommand()<< endl;
		vector<string> args = command->getArgs();
		switch (currentStage)
		{
		case 1:
			//Start Phase
			map = Map::mapCreater(args[1]);
			transition(2);
			std::cout << "The map: " << args[1] << " is successfully loaded!" << endl;
			break;
		case 2: {
			//map loaded Phase
			if (args[0].compare("validatemap") == 0) {
				if (map->validate()) {
					//successfully validated.
					transition(3);
					connectedGraph = (map->getMapGraph());
					std::cout << "The map is valid!" << endl;
				}
				else {
					//the map is not validate
					std::cout << "The map is not valid! please use command: loadmap <mapfile>   to load a different map!" << endl;
				}
			}
			else {
				//dont need to varifly if the 1st argument is 'only validatemap/loadmap'. this process is dnoe in cmdProcessor.validte();
				map = Map::mapCreater(args[1]);

				
				std::cout << "The map: " << args[1] << " is successfully loaded!" << endl;
			}
		}
			break;
		case 3: {
			//map validated Phase
			transition(4);
			Player* p1 = new Player(playerCount, args[1], &connectedGraph);
			if (!tournaMode || p1->getName().compare("Human") == 0) {
				p1->setPlayerStrategy(new HumanPlayerStrategy(p1));
			}
			else if (p1->getName().compare("Aggressive") == 0) {
				p1->setPlayerStrategy(new AggressivePlayerStrategy(p1));
			}
			else if (p1->getName().compare("Benevolent") == 0) {
				p1->setPlayerStrategy(new BenevolentPlayerStrategy(p1));
			}
			else if (p1->getName().compare("Cheater") == 0) {
				p1->setPlayerStrategy(new CheaterPlayerStrategy(p1));
			}
			else if (p1->getName().compare("Neutral") == 0) {
				p1->setPlayerStrategy(new NeutralPlayerStrategy(p1));
			}
			p1->attachToPlayerOrderList(this->observers);
			playerList.push_back(p1);
			
			playerCount++;
			std::cout << "Player: " << args[1] << " is successfully added!" << endl;
		}
			break;
		case 4: {
			//players added Phase
			if (args[0].compare("addplayer") == 0) {
				Player* p1 = new Player(playerCount, args[1], &connectedGraph);
				if (!tournaMode || p1->getName().compare("Human") == 0) {
					p1->setPlayerStrategy(new HumanPlayerStrategy(p1));
				}
				else if (p1->getName().compare("Aggressive") == 0) {
					p1->setPlayerStrategy(new AggressivePlayerStrategy(p1));
				}
				else if (p1->getName().compare("Benevolent") == 0) {
					p1->setPlayerStrategy(new BenevolentPlayerStrategy(p1));
				}
				else if (p1->getName().compare("Cheater") == 0) {
					p1->setPlayerStrategy(new CheaterPlayerStrategy(p1));
				}
				else if (p1->getName().compare("Neutral") == 0) {
					p1->setPlayerStrategy(new NeutralPlayerStrategy(p1));
				}
				p1->attachToPlayerOrderList(this->observers);
				playerList.push_back(p1);

				playerCount++;
				std::cout << "Player: " << args[1] << " is successfully added!" << endl;
			}   
			else if (args[0].compare("gamestart") == 0) {
				//game start!
				//distribute terrtories to the players - base on the initial number of terrtories. 
				if (connectedGraph.size() < initialPlayerTerritoriesAmount * playerList.size()) {
					std::cout << "ERROR: there's no enough terrtories for player's initial distribution!" << endl;
					exit(0);
				}
				int terrIndexCounter = 0;

				for (int j = 0; j<playerList.size(); j++)
				{
					Player* p1 = (playerList[j]);

					//give 50 initial arimies to each player
					p1->setReinforcementpool(50);

					//let each player draw 2 initial cards
					p1->getHandsOfCard()->set_vec_hand_cards(deck->draw());
					p1->getHandsOfCard()->set_vec_hand_cards(deck->draw());

					for (int i = 0; i < initialPlayerTerritoriesAmount; i++)
					{
						//add terrtories to each players. 
						p1->addTerrtories(connectedGraph[terrIndexCounter][0]);
						//p1.printPlayerTerrtories();
						terrIndexCounter++;
					}
				}
				//select the first player randomly (will skip this process if it's debug mode - then the first player will always be player #1 )
				if (!debugMode) {
					int max = playerList.size();
					std::srand(std::time(nullptr));
					theStartingPlayer = rand() % max;
				}

				//switch the game to the play phase:
				transition(5);
				isStartup = false;
			}
		}
			break;
		default:
			cout << "ERROR: Invaid state during startup()." << endl;
			break;
		}

		cout <<"================================================================================¨a" << endl << endl << endl;



	}


	mainGameLoop(theStartingPlayer);

}

void GameEngine::mainGameLoop(int startingPlayer) {

	int totalPlayer = playerList.size();
	int counter_runs = 0;
	cout << "¨X===============================================================================¨[" << endl;
	cout << "¨d~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~¨g" << endl;
	cout << "¨d                            WarZone: Game started                              ¨g" << endl;
	cout << "¨d~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~¨g" << endl;
	cout << "¨^===============================================================================¨a" << endl;


	//keep looping until the actual game start.
	while (counter_runs <= stoi(tourna_paravec[3][0]) && !isStartup) {
	    
		cout << "================================================================================¨[" << endl;
		//check the current game state by a switch statement. displays the message refers to that stage.
		switch (currentStage) {
		case 5:
			showState();
			//assign reinforcement Phase
			break;
		case 6: {
			//issue orders Phase
			if (!(playerList[startingPlayer]->gethasEndThisIssueOrderTurn())) {

				playerList[startingPlayer]->getOrderList()->displayAll();
				//display the current player's orderlist
				std::cout << endl;
				(playerList[startingPlayer]->getHandsOfCard())->print_vec_hand_cards();
				//display the current player's hand
				std::cout << endl;
				playerList[startingPlayer]->printPlayerTerrtories();
				std::cout << endl;
				//show the terrtories player own / can attack
				showState();
				if (!tournaMode || playerList[startingPlayer]->getName().compare("Human") == 0) {
					std::cout << "# " << playerList[startingPlayer]->getPlayerID() << " Player: [" << playerList[startingPlayer]->getName() << "] it's your turn to issue an order!" << endl;
					std::cout << "~=~ Order and their ordertype ID:   ~=~ " << endl;
					std::cout << "~=~ 0 - DeployOrder, 3 args         ~=~ " << endl;
					std::cout << "~=~ 1 - AdvanceOrder, 4 args        ~=~ " << endl;
					std::cout << "~=~ 2 - BombOrder, 2 args           ~=~ " << endl;
					std::cout << "~=~ 3 - BlockadeOrder, 2 args       ~=~ " << endl;
					std::cout << "~=~ 4 - AirliftOrder, 4 args        ~=~ " << endl;
					std::cout << "~=~ 5 - NegotiateOrder, 2 args      ~=~ " << endl;
					std::cout << "1. enter \"issueorder <ordertype ID> [TargetTerrtoryID] [numberOfArmies] [FromTerrtoryID]\" " << endl;
					std::cout << "to add an order into your order list " << endl;
					std::cout << "2. enter \"endissueorder\" to stop your turns of order issueing" << endl;
					std::cout << "You have: " << playerList[startingPlayer]->getEstimatePool() << " Armies in your reinforcement pool! (estimate)" << endl;
				}
			}
		}
			break;
		case 7:
			showState();
			//execute orders Phase
			break;
		case 8:
			//win Phase
			std::cout << "1. enter \"quit\" to stop the game." << endl;
			std::cout << "2. enter \"replay\" to replay the game." << endl;
			break;
		case -1:
			//check win condition Phase
			std::cout << "All the orders are being executed. Checking win condition...." << endl;
			break;
		default:
			cout << "ERROR: Invaid state during mainGameLoop()." << endl;
			break;
		}


		switch (currentStage) {
		case 5: {
			//assign reinforcement Phase
			reinforcementPhase();
			break;
		}
		case 6: {
			//issue orders Phase

			//if all the player has decide to end his turn, switch to next phase:
			bool allPlayerEndIssueingOrder = true;
			for (int i = 0; i < playerList.size(); i++)
			{
				if (!(playerList[i]->gethasEndThisIssueOrderTurn())) {
					allPlayerEndIssueingOrder = false;
				}
			}
			if (allPlayerEndIssueingOrder) {
				transition(7);
				for (int i = 0; i < playerList.size(); i++)
				{
					playerList[i]->sethasEndThisIssueOrderTurn(false);
				}
			}

			if (playerList[startingPlayer]->gethasEndThisIssueOrderTurn()||currentStage==7) {
				//if this player have set to end this turn of issuing order
				startingPlayer = switchCurrentPlayer(startingPlayer);
			
			}else {
				//if this player havn't set to end this turn of issuing order
				startingPlayer = issueOrderPhase(startingPlayer);
			}


			break;
		}
		case 7: {
			//execute orders Phase
			cout << "Executing orders... " << endl;
			executreOrderPhase(startingPlayer);
			counter_runs++;
			break;

		}
		case -1: {
			bool someoneWin = false;
			string winPlayerName = "";
			//check win Phase
			for (int j = playerList.size()-1; j >=0; j--)
			{
				bool controlledAll = true;
				bool stillControlSomeTerritory = false;
				for (size_t i = 0; i < map->getMapGraph().size(); i++)
				{

					if (!stillControlSomeTerritory && ((map->getMapGraph())[i][0]->getcontrolledPlayerID() )== playerList[j]->getPlayerID()) {
						//check if this player still controll any terrtories
						stillControlSomeTerritory = true;
					}
					if ((map->getMapGraph())[i][0]->getcontrolledPlayerID() != playerList[j]->getPlayerID()) {
						//if there's an territory that is not controlled by player 
						controlledAll = false;
					}
				}
				if (!stillControlSomeTerritory) {
					//remove the player if he dont control any terrtories.
					cout << "Player: " << playerList[j] ->getName()<<" has been removed since he dont control any terrtories anymore!" << endl;
					removePlayerFromPlayerList(playerList[j]->getPlayerID());
					startingPlayer = switchCurrentPlayer(startingPlayer);
				}
				if (controlledAll) {
					someoneWin = true;
					winPlayerName = playerList[j]->getName();
					
				}
			}

			if (someoneWin) {
				transition(8);
			}
			else {
				transition(5);
			}


			//if (true) {
			//	transition(8);
			//}
			//else {
			//	transition(5);
			//}
			//this is leave for quick access of testing restart function.

			break;
		}
		
		case 8: {
			//win Phase
			Command* command = NULL;
			bool commandIsNotValid = true;
			//Huamn player need to enter commnands
			if (!tournaMode || playerList[startingPlayer]->getName().compare("Human") == 0) {
				while (commandIsNotValid) {
					//read a command .
					command = cmdProcessor->getCommand();
					commandIsNotValid = !(cmdProcessor->validate(*command, currentStage));
					if (commandIsNotValid) {
						cout << "Invaild Command! please re-enter the command: " << endl;
						command->saveEffect("Invalid Command");
					}
					else {
						command->saveEffect("");

					}
				}
			}
			//AI auto-running
			else {
				reset();
				break;
			}

			//execute the command refers to different stage.
			cout << endl << "Executing command:  " << command->getOriginalCommand() << endl;
			vector<string> args = command->getArgs();

			if (args[0].compare("replay") == 0) {
				reset();
				startup();
			}
			else {
				cout << endl << "Thank you for playing! Game ends!"  << endl;
				exit(1);
			}



			break;
		}
		default:
			cout << "ERROR: Invaid state during mainGameLoop()." << endl;
			break;
		}

		cout << "================================================================================¨a" << endl << endl << endl;

		if (counter_runs > stoi(tourna_paravec[3][0])) {
		    
			cout << "Exceed the maximum turns. The game result of the current game run is DRAW." << endl;
		}

	}

}

void GameEngine::setPlayerConquered(int fromPlayerID) {
	for (int i = 0; i < this->playerList.size(); i++)
	{
		if ((this->playerList)[i]->getPlayerID() == fromPlayerID) {
			(this->playerList)[i]->setConqueredInThisTurn(true);
		}

	}

}

bool GameEngine::checkPlayerPool(int playerID, int amount) {
	for (int i = 0; i < this->playerList.size(); i++)
	{
		if ((this->playerList)[i]->getPlayerID() == playerID) {
			if ((this->playerList)[i]->getReinforcementpool() < amount) {
				return false;
			}
			return true;
		}

	}
	return false;
}
void GameEngine::subtractPlayerPool(int playerID, int amount) {
	for (int i = 0; i < this->playerList.size(); i++)
	{
		if ((this->playerList)[i]->getPlayerID() == playerID) {
			if ((this->playerList)[i]->getReinforcementpool() < amount) {
				return;
			}
			(this->playerList)[i]->setReinforcementpool((this->playerList)[i]->getReinforcementpool()  -amount);
		}

	}
}

void GameEngine::addToNegotiateOrderList(int fromPlayerID, int toPlayerID) {
	vector<int> newOrder;
	newOrder.push_back(fromPlayerID);
	newOrder.push_back(toPlayerID);
	this->negotiateOrderList.push_back(newOrder);
}
bool GameEngine::checkNegotiateOrderList(int fromPlayerID, int toPlayerID) {
	for (int i = 0; i < this->negotiateOrderList.size(); i++)
	{
		cout << "DEBUGGGGGGGGGGGGGGGG------------------------------------------------------------------------------------------------" << (this->negotiateOrderList)[i][0] << endl;
		cout << "DEBUGGGGGGGGGGGGGGGG-------------------------------------------------+++-----------------------------------------------" << (this->negotiateOrderList)[i][1] << endl;
		cout << "DEBUGGGGGGGGGGGGGGGG-------------------------------------------------+++-----------------------------------------------" << fromPlayerID << endl;
		cout << "DEBUGGGGGGGGGGGGGGGG-------------------------------------------------+++-----------------------------------------------" << toPlayerID << endl;
		if ((this->negotiateOrderList)[i][0] == fromPlayerID && (this->negotiateOrderList)[i][1] == toPlayerID) return true;
		if ((this->negotiateOrderList)[i][1] == fromPlayerID && (this->negotiateOrderList)[i][0] == toPlayerID) return true;
	}
	return false;
}

void GameEngine::removePlayerFromPlayerList(int playerID) {

	for (int i = 0; i < this->playerList.size(); i++)
	{
		if ((this->playerList)[i]->getPlayerID() == playerID) {
			auto player = (this->playerList)[i];
			playerList.erase(std::next(playerList.begin(), i));
			delete player;
			return;
		}

	}
}
//Reset state status
void GameEngine::reset() {
	this->isStartup = true;
	transition(0);

	if (string(typeid(*this->cmdProcessor).name()) == "class CommandProcessor") {
		delete this->cmdProcessor;
		this->cmdProcessor = nullptr;
	}
	delete map;
	map = new Map();
	delete deck;
	deck = new Deck();

	for (int i = 0; i < playerList.size(); i++)
		delete playerList[i];
	playerList.clear();
}

int GameEngine::switchCurrentPlayer(int current) {
	int size = playerList.size();
	int result = current + 1;
	if (result >= size) {
		result = 0;
	}

	return result;

}


//Accessors
int GameEngine::getCurrentGameStage() {
	return currentStage;
}
bool GameEngine::getIsStartup() {
	return isStartup;
}

void GameEngine::reinforcementPhase() {
	cout << "given every player different amount of arimies in to their reinforcement pool..." << endl;
	for (int i = 0; i < playerList.size(); i++)
	{
		int amount = 0;
		playerList[i]->update();
		amount = ((playerList[i]->toDefend()).size()) / 3;
		//add armies to player base on the amount of terrtory he controll.
		for (int j = 0; j < map->getContinentGraph().size(); j++)
		{
			int isAllControlledByPlayer = true;
			for (int k = 0; k < ((map->getContinentGraph())[j]->getCountryInside()).size(); k++)
			{
				if (((map->getContinentGraph())[j]->getCountryInside())[k]->getcontrolledPlayerID() != playerList[i]->getPlayerID()) {
					isAllControlledByPlayer = false;
				}
			};
			if (isAllControlledByPlayer) {
				//if this continment is all controlled by player, give them bonus
				int bonus = map->getContinentGraph()[j]->getBonus();
				cout << "Player:  " << playerList[i]->getName() << " controlled the entire " << map->getContinentGraph()[j]->getName() << " ! He get a bonus arimes of: " << bonus << endl;
				amount += bonus;
			}
		}
		if (amount < 3) {
			amount = 3;
			//the minimal number of reinforcement armies per turn for any player is 3
		}
		cout << "Player:  " << playerList[i]->getName() << " received a reinforcement of:  " << amount << " armies! " << endl;
		playerList[i]->addReinforcementpool(amount);
		playerList[i]->updateEstimatePool();

	}
	transition(6);
	//change stage
}



int GameEngine::issueOrderPhase(int startingPlayer) {
	int theCurrentPlayer = startingPlayer;
	Command* command = NULL;
	bool commandIsNotValid = true;
	//Human Player: it needs user's iteraction
	if (!tournaMode || playerList[startingPlayer]->getName().compare("Human") == 0 ) {
		while (commandIsNotValid) {
			//read a command .
			command = cmdProcessor->getCommand();
			commandIsNotValid = !(cmdProcessor->validate(*command, currentStage));
			if (commandIsNotValid) {
				cout << "Invaild Command! please re-enter the command: " << endl;
				command->saveEffect("Invalid Command");
			}
			else {
				command->saveEffect("");

			}
		}
	}
	//AI Player: auto-running commands
	else {
		command = cmdProcessor->saveCommand("AI Player's turn of issuing orders");
		command->saveEffect("");
		playerList[startingPlayer]->issueOrder(0,this->map->getMapGraph()[0][0],0,this->map->getMapGraph()[0][0]);
		//theCurrentPlayer = switchCurrentPlayer(startingPlayer);
		//return theCurrentPlayer;
		command = cmdProcessor->saveCommand("endissueorder");
		command->saveEffect("");
	}

	//execute the command refers to different stage.
	cout << endl << "Executing command:  " << command->getOriginalCommand() << endl;
	vector<string> args = command->getArgs();


	if (args[0].compare("endissueorder") == 0) {
		playerList[startingPlayer]->sethasEndThisIssueOrderTurn(true);
		theCurrentPlayer = switchCurrentPlayer(startingPlayer);
	}
	else if (args[0].compare("issueorder") == 0) {
		string type = args[1];
		if (type.compare("0") == 0) {
			//order type: Deploy order
			playerList[startingPlayer]->issueOrder(0,
				map->getTerrtoryById(atoi(args[2].c_str())),
				atoi(args[3].c_str()),
				NULL);
			playerList[startingPlayer]->substractEstimatePool(atoi(args[3].c_str()));
			cout << "You successfully placed an order!  " << endl;
			theCurrentPlayer = switchCurrentPlayer(startingPlayer);
		}
		else if (type.compare("1") == 0) {
			//order type: Advance Order
			playerList[startingPlayer]->issueOrder(1,
				map->getTerrtoryById(atoi(args[2].c_str())),
				atoi(args[3].c_str()),
				map->getTerrtoryById(atoi(args[4].c_str())));
			cout << "You successfully placed an order!  " << endl;
			theCurrentPlayer = switchCurrentPlayer(startingPlayer);
		}
		else if (type.compare("2") == 0) {
			//order type: Bomb Order

			if ((playerList[startingPlayer]->getHandsOfCard())->removeCardWithTypeID("bomb") || debugMode) {
				//if the player have this kind of card , or it's debug mode,
				//then remove this card, and create an order.
				playerList[startingPlayer]->issueOrder(2,
					map->getTerrtoryById(atoi(args[2].c_str())),
					0,
					NULL);
				cout << "You successfully placed an order!  " << endl;
				theCurrentPlayer = switchCurrentPlayer(startingPlayer);

			}
			else {
				//player dont have this card, the command issuing is failed and let the player issue the command again.
				cout << "You don't have that card in your hand! The order is deined.  " << endl;
			}


		}
		else if (type.compare("3") == 0) {
			//order type: Blockade Order


			if ((playerList[startingPlayer]->getHandsOfCard())->removeCardWithTypeID("blockade") || debugMode) {
				//if the player have this kind of card , or it's debug mode,
				//then remove this card, and create an order.
				playerList[startingPlayer]->issueOrder(3,
					map->getTerrtoryById(atoi(args[2].c_str())),
					0,
					NULL);
				cout << "You successfully placed an order!  " << endl;
				theCurrentPlayer = switchCurrentPlayer(startingPlayer);

			}
			else {
				//player dont have this card, the command issuing is failed and let the player issue the command again.
				cout << "You don't have that card in your hand! The order is deined.  " << endl;
			}
			

		}
		else if (type.compare("4") == 0) {
			//order type: Airlist Order
			if ((playerList[startingPlayer]->getHandsOfCard())->removeCardWithTypeID("airlift") || debugMode) {
				//if the player have this kind of card , or it's debug mode,
				//then remove this card, and create an order.
				playerList[startingPlayer]->issueOrder(4,
					map->getTerrtoryById(atoi(args[2].c_str())),
					atoi(args[3].c_str()),
					map->getTerrtoryById(atoi(args[4].c_str())));
				cout << "You successfully placed an order!  " << endl;
				theCurrentPlayer = switchCurrentPlayer(startingPlayer);

			}
			else {
				//player dont have this card, the command issuing is failed and let the player issue the command again.
				cout << "You don't have that card in your hand! The order is deined.  " << endl;
			}

		}
		else if (type.compare("5") == 0) {
			//order type: Negotiate Order
			if ((playerList[startingPlayer]->getHandsOfCard())->removeCardWithTypeID("diplomacy") || debugMode) {
				//if the player have this kind of card , or it's debug mode,
				//then remove this card, and create an order.
				playerList[startingPlayer]->issueOrder(5,
					map->getTerrtoryById(atoi(args[2].c_str())),
					NULL,
					map->getTerrtoryById(atoi(args[4].c_str())));
				cout << "You successfully placed an order!  " << endl;
				theCurrentPlayer = switchCurrentPlayer(startingPlayer);


			}
			else {
				//player dont have this card, the command issuing is failed and let the player issue the command again.
				cout << "You don't have that card in your hand! The order is deined.  " << endl;
			}
		}
	}
	return theCurrentPlayer;
}


void GameEngine::executreOrderPhase(int startingPlayer) {
	int currentPlayer = startingPlayer;
	//first execute all the deploy orders
	for (int i = 0; i < playerList.size(); i++) {
		auto ol = playerList[i]->getOrderList();
		auto cnmdb = ol->getAllOrders();
		for (int j = 0; j < cnmdb.size(); j++) {
			if (cnmdb[j] && "class DeployOrder" == string(typeid(*cnmdb[j]).name())) {
				cnmdb[j]->execute();
				int targetOrderID = cnmdb[j]->getOrderID();
				ol->remove(targetOrderID);
				//execute the deploy order we find and remove them from the list.
			}
		}
	}
	
	//then execute all the orders in the list with round-robin fashion
	bool isEmpty = false;
	while (!isEmpty) {
		isEmpty = true;
		for (int i = 0; i < playerList.size(); i++) {
			auto ol = playerList[currentPlayer]->getOrderList();
			int size = ol->getAllOrders().size();
			if (size != 0) {
				isEmpty = false;
				auto ord = ol->popLast();
				ord->execute();
				delete ord;
			}
			currentPlayer = switchCurrentPlayer(currentPlayer);
		}
	}

	//one card is given to a player if they conquer at least one territory in a turn

	for (int i = 0; i < this->playerList.size(); i++)
	{
		if ((this->playerList)[i]->checkAndResetConqueredInThisTurn()) {
			Player* p1 = ((this->playerList)[i]);
			//let player draw 1 cards
			p1->getHandsOfCard()->set_vec_hand_cards(deck->draw());

			cout << "Player: " <<p1->getName()<<" Conquered a terrtorie this turn. He received a card as boost!" << endl;
		}

	}

	transition(-1);
}



void GameEngine::transition(int newState) {
	this->currentStage = newState;
	logValue = "Current Game State: [" + stageToString(currentStage) + "]";
	notify(this);
}
