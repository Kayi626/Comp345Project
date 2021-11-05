#include "GameEngine.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "CommandProcessing.h"
#include <string>
#include <regex>
#include <iostream>
using namespace std;



//Constructor
GameEngine::GameEngine() {
	this->start = true;
	this->map_loaded = false;
	this->map_validated=false;
	this->players_added=false;
	this->assign_reinforcement=false;
	this->issue_orders=false;
	this->execute_orders=false;
	this->win=false;
}

GameEngine::GameEngine(const GameEngine& ge) {
	this->start = *new bool(ge.start);
	this->map_loaded = *new bool(ge.map_loaded);
	this->map_validated = *new bool(ge.map_validated);
	this->players_added = *new bool(ge.players_added);
	this->assign_reinforcement = *new bool(ge.assign_reinforcement);
	this->issue_orders = *new bool(ge.issue_orders);
	this->execute_orders = *new bool(ge.execute_orders);
	this->win = *new bool(ge.win);
}

//Assignment Operator
GameEngine& GameEngine::operator= (const GameEngine& ge) {
	if (this == &ge) {
		return *this;
	}
	this->start = *new bool(ge.start);
	this->map_loaded = *new bool(ge.map_loaded);
	this->map_validated = *new bool(ge.map_validated);
	this->players_added = *new bool(ge.players_added);
	this->assign_reinforcement = *new bool(ge.assign_reinforcement);
	this->issue_orders = *new bool(ge.issue_orders);
	this->execute_orders = *new bool(ge.execute_orders);
	this->win = *new bool(ge.win);

	return *this;
}

//Destructor
GameEngine::~GameEngine() {

}

//Other class functions

//Stream Insertion Operators
ostream& operator << (ostream& ost, const GameEngine& ge) {
	ge.checkState(ost);
	return ost;
}
void GameEngine::checkState(std::ostream& output) const {

	if (start)
		output << "[Current State: Start]" << endl;
	else if (map_loaded)
		output << "[Current State: Map_Loaded]" << endl;
	else if (map_validated)
		output << "[Current State: Map_Validated]" << endl;
	else if (players_added)
		output << "[Current State: Players_Added]";
	else if (assign_reinforcement)
		output << "[Current State: Assign_Reinforcement]" << endl;
	else if (issue_orders)
		output << "[Current State: Issue Orders]" << endl;
	else if (execute_orders)
		output << "[Current State: Execute Orders]" << endl;
	else if (win)
		output << "[Current State: Win]" << endl;

}

//define other global variables like file path
string mapfilepath = "";
string playername = "";
void GameEngine::gameFlow() {

	//define variables for logic control
	string input;
	bool lock = false;
	bool lock2 = true;
	bool lock3 = true;
	bool executed = false;
	bool playStage = false;
	bool run = true;
	bool Reinforce_assigned = false;

	//define objects of classes like Player,Cards,Orders,and Map
	Map* newMap=new Map();
	Deck* newDeck = new Deck;
	newDeck->original_vec_deck();
	int IDgenerator = 1;
	vector<Player> playerList;
	vector<vector<Territory*>> currentMapGraph;
	int issueTime = 1;

	//Two targest interface linked to Gameengine
	CommandProcessor& comP = *(new CommandProcessor());
	FileCommandProcessorAdapter fcomP;


	
 
	while (run) {

		//Navigate all states in the console
		

		//Display state information and prompt users to enter commands plus implementations
		if (start) {
			if(lock3)
			std::cout <<endl<< "---------------WELCOME TO WARZONE!---------------" << endl;
			std::cout << endl << "[Stage: Start]" << endl;
			std::cout << endl << "Please enter \"loadmap <mapfilepath>\" to load the map: " << endl << endl;
		}
		else if (map_loaded && lock) {
			std::cout << endl << "---------------Map Loading Start---------------" << endl;
			//Code to load map
		
			newMap=Map::mapCreater(mapfilepath);
			currentMapGraph = (newMap->getMapGraph());
			std::cout << *(newMap);

			std::cout << endl << "---------------Map Loading Done----------------" << endl; 
			std::cout << endl << "[Stage: Map_Loaded]" << endl; 
			std::cout << endl << "1. enter \"validatemap\" to validate the map" << endl;
            std::cout << endl << "2. enter \"loadmap <mapfilepath>\" to reload map: " << endl << endl;
		}
		else if (map_validated && lock) {
			std::cout << endl << "---------------Map Validating Start---------------" << endl;
			//Code to load map
			newMap->validate();

			std::cout << endl << "---------------Map Validating Done----------------" << endl;
            std::cout << endl << "[Stage: Map_Validated]" << endl; 
			std::cout << endl << "1. enter \"addplayer <playername>\" to add players into the map"<< endl << endl;
		}
		else if (players_added && lock) {
			std::cout << endl << "---------------Adding Players Start---------------" << endl;
			//Code to load map
			if (IDgenerator < 6) {
				vector<vector<Territory*>> currentMapGraph = (newMap->getMapGraph());
				Player player=*(new Player(IDgenerator, playername, &currentMapGraph));
				Hand* newHand = player.getHandsOfCard();
				for (int i = 0; i < 5; i++) {
					newHand->set_vec_hand_cards(newDeck->draw());
				}
				playerList.push_back(player);
				std::cout << endl;
				player.getHandsOfCard()->print_vec_hand_cards();
				IDgenerator++;
				std::cout << endl<< player;
			}
			else {
				std::cout << endl<<"Exceed the maximum number of players!" << endl;
			}

			std::cout << endl << "---------------Adding Players Done----------------" << endl;
            std::cout << endl << "[Stage: Player_Added]" << endl;
            std::cout << endl << "1. enter \"gamestart\" to assign countries to each player" << endl;
            std::cout << endl << "2. enter \"addplayer <playername>\" to add more players " << endl << endl;
		}

        //Get command inputs
		Command& com = comP.getCommand();
		input = com.getCommand();
	

		//Start up starts; Start Stage + Map Loaded Stage
		if((comP.validate(com,"start") || comP.validate(com,"maploaded")) && com.getEffect().compare("maploaded") == 0) {

			//Catch the path of map file
			mapfilepath=comP.extractName(com);
			start = false;
			map_loaded = true;
			lock = true;
			continue;
		}
		
		//Map Validated Stage
		else if (comP.validate(com,"maploaded")) {

			map_loaded = false;
			map_validated = true;
			lock = true;
			continue;
		}
		
		//Add players Stage
		else if ((comP.validate(com,"playersadded") || comP.validate(com,"mapvalidated")) && com.getEffect().compare("playersadded") == 0) {

			playername = comP.extractName(com);
			players_added = true;
			map_validated = false;
			lock = true;
			continue;

		}

		//assign reinforcement state
	    else if (comP.validate(com,"playersadded")) {
          
			players_added = false;
			assign_reinforcement = true;
			playStage = true;
			
		}
		else {

			if (start) {
				std::cout << endl << "---------------Invalid commands detected At [Start] stage---------------" << endl;
				//std::cout << endl << "[Stage: Start] Please enter \"loadmap\" to load the map: " << endl << endl;
				lock3 = false;
			} else if(map_loaded){
				std::cout << endl << "---------------Invalid commands detected At [Map_Loaded] stage---------------" << endl;
				std::cout << endl << "[Stage: Map_Loaded] Please enter \"validatemap\" to validate the map: " << endl << endl;
			} else if (map_validated) {
				std::cout << endl << "---------------Invalid commands detected At [Map_validated] stage---------------" << endl;
				std::cout << endl << "[Stage: Map_Validated] Please enter \"addplayer <playername>\" to add players into the map " << endl << endl;
			} else if (players_added) {
				std::cout << endl << "---------------Invalid commands detected At [Players_Added] stage---------------" << endl;
				std::cout << endl << "[Stage: Player_Added] Please enter \"gamestart\" to assign countries to each player " << endl << endl;
			}
			//No state transition
			lock = false;
			continue;	
			
		}

		//Play stage starts
		while (playStage) {

            //Display state information and prompt users to enter commands
			if (assign_reinforcement && lock2) {
				std::cout << endl << "----------------Assigning Reinforcement Starts---------------" << endl;
				//code to assign reinforcement
				if (!Reinforce_assigned) {
					for (int x = 0; x < playerList.size(); x++) {

						playerList[x].addTerrtories(currentMapGraph[x][0]);
						currentMapGraph[x][0]->setArmyNumber(5);
						playerList[x].printPlayerTerrtories();
						std::cout << playerList[x];

					}
				}
				else {

					//Remove orders from each player's order list
					for (int x = 0; x < playerList.size(); x++) {
						OrderList* ordL = playerList[x].getOrderList();
						ordL->removeAll();
						
					}

					for (int x = 0; x < playerList.size(); x++) {
						currentMapGraph[x][0]->setArmyNumber(5);
						playerList[x].printPlayerTerrtories();
						std::cout << playerList[x];

					}

				}
				
				std::cout << endl << "---------------Assigning Reinforcement Done---------------" << endl;
				std::cout << endl <<"[Stage: Assign Reinforcement]" << endl;
				std::cout << endl <<	"1. enter \"issueorder\" to issue orders for players: " << endl << endl;
			}
			else if(issue_orders && lock2) {
				std::cout << endl << "---------------Issuing Orders Starts---------------" << endl;
				//code to issue orders
				for (int x = 0; x < playerList.size(); x++) {
					playerList[x].issueOrder(issueTime%4, currentMapGraph[1+x][0], 2, currentMapGraph[x][0]);
					playerList[x].getOrderList()->displayAll();
				}
				issueTime++;
				

				std::cout << endl << "---------------Issuing Orders Done-----------------" << endl;
                std::cout << endl << "[Stage: Issue Orders]" << endl;
                std::cout << endl <<	"1. enter \"issueorder\" to issue orders for players" << endl;
				std::cout << endl <<	"2. enter \"endissueorders\" to end: " << endl << endl;
			}
			else if (execute_orders && lock2) {
				if (executed) {
					std::cout << endl << "---------------Executing Orders Starts---------------" << endl;
					//code to execute orders
					for (int x = 0; x < playerList.size(); x++) {
						Orders* ord = playerList[x].getOrderList()->popFirst();
						if (ord->validate()) {
							std::cout << *ord << "----VALID ORDER EXECUTED----"<< endl;
						}
						else {
							std::cout << *ord << "----INVALID ORDER NOT EXECUTED----" << endl;;
						}
					}
				}

				std::cout<< endl << "[Stage: Execute Orders]" << endl;
                std::cout<< endl << "1. enter \"execorder\" to issue orders for players" << endl;
			    std::cout<< endl << "2. enter \"endexecorders\" to issue orders for players" << endl;
                std::cout<< endl << "3. enter \"win\" to win the game: " << endl << endl;
			}
			else if (win && lock2) {
				std::cout << endl << "---------------Generating the Winner Starts---------------" << endl;
				//code for generating the winner
				std::cout << endl << "---------------Generating the Winner Done---------------" << endl;
                std::cout << endl << "[Stage: Win]" << endl;
				std::cout << endl <<	"1. enter \"replay\" to replay the game: " << endl;
				std::cout << endl <<	"1. enter \"quit\" to end the game: " << endl << endl;
			}
	
			
			//Get command inputs
			Command& com = comP.getCommand();
			input = com.getCommand();

			//Issue orders
			if ((assign_reinforcement || issue_orders) && input.compare("issueorder") == 0) {

				assign_reinforcement = false;
				Reinforce_assigned = true;
				issue_orders = true;
				lock2 = true;
				continue;
			}

			//End issuing orders
			 else if (issue_orders && input.compare("endissueorders") == 0) {

				issue_orders = false;
				execute_orders = true;
				lock2 = true;
				continue;
			}
			

			//Execute orders(win the game)
		    else if (execute_orders && input.compare("win") == 0) {

				execute_orders = false;
				win = true;
				lock2 = true;
				continue;
			}
			
			//Execute orders(restart the play stage)
			else if (execute_orders && input.compare("endexecorders") == 0) {
				execute_orders = false;
				assign_reinforcement = true;
				lock2 = true;
				continue;

			}

			//continue to execute orders
			else if (execute_orders && input.compare("execorder") == 0) {

				executed = true;
				continue;

			}
				
            //Win(Re-play the game)
			 else if (comP.validate(com,"win") && com.getEffect().compare("start") == 0) {

				reset();
				playStage = false;
				Reinforce_assigned = false;
				break;

			}
			

			//Win(End the game)
			else if (comP.validate(com,"win") && com.getEffect().compare("exist program") == 0) {

				run = false;
				break;

			}

			//Invalid inputs
			else {

				if(assign_reinforcement)
				std::cout << endl << "---------------Invalid commands detected at [Assign reinforcement] state---------------" << endl << "[Stage: Assign Reinforcement] Enter \"issueorder\" to issue orders for players: " << endl;
				else if(issue_orders)
				std::cout << endl << "---------------Invalid commands detected at [Issue Orders] state---------------" << endl << "[Stage: Issue Orders] Enter \"issueorder\" to issue orders for players OR \"endissueorder\" to end: " << endl;
				else if(execute_orders)
				std::cout << endl << "---------------Invalid commands detected at [Execute Orders] state---------------" << endl << "[Stage: Execute Orders] Enter \"execorder\" to execute orders for players OR \"win\" to win the game: " << endl;
				else if(win)
				std::cout << endl << "---------------Invalid commands detected at [Win] state---------------" << endl << "[Stage: Win] Enter \"play\" to replay the game OR \"end\" to end the game: " << endl;
				lock2 = false;
				continue;
				
			}
						

		}


	}

	std::cout << endl << "---------------Game OVer. Thank you for playing the warzone!---------------";

}

//Reset state status
void GameEngine::reset() {
	this->start = true;
	this->map_loaded = false;
	this->map_validated = false;
	this->players_added = false;
	this->assign_reinforcement = false;
	this->issue_orders = false;
	this->execute_orders = false;
	this->win = false;
}

//Accessors
bool GameEngine::getStart() {
	return start;
}
bool GameEngine::getMap_Loaded() {
	return map_loaded;
}
bool GameEngine::getMap_Validated() {
	return map_validated;
}
bool GameEngine::getPlayers_Added() {
	return players_added;
}
bool GameEngine::getAssign_Reinforcement() {
	return assign_reinforcement;
}
bool GameEngine::getIssue_Orders() {
	return issue_orders;
}
bool GameEngine::getExecute_Orders() {
	return execute_orders;
}
bool GameEngine::getWin() {
	return win;
}

