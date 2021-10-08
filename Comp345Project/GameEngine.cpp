#include "GameEngine.h"
#include <string>
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
	this-> start = ge.start;
	this->map_loaded = ge.map_loaded;
	this->map_validated = ge.map_validated;
	this->players_added = ge.players_added;
	this->assign_reinforcement = ge.assign_reinforcement;
	this->issue_orders = ge.issue_orders;
	this->execute_orders = ge.execute_orders;
	this->win = ge.win;
}

//Other class functions
void GameEngine::gameFlow() {

	//define variables for logic control
	string input;
	bool lock = false;
	bool lock2 = true;
	bool executed = false;
	bool playStage = false;
	bool run = true;
	
	cout << "---------------WELCOME TO WARZONE!---------------" << endl; 
    cout << endl << "[Stage: Start]" << endl;
	cout << endl << "Please enter \"loadmap\" to load the map: " << endl << endl;

	while (run) {

		//Navigate all states in the console

		//Display state information and prompt users to enter commands plus implementations
		if (map_loaded && lock) {
			cout << endl << "---------------Map Loading Start---------------" << endl;
			//Code to load map
			cout << endl << "---------------Map Loading Done----------------" << endl; 
			cout << endl <<"[Stage: Map_Loaded]" << endl; 
			cout << endl << "1. enter \"validatemap\" to validate the map" << endl;
            cout << endl << "2. enter \"loadmap\" to reload map: " << endl << endl;
		}
		else if (map_validated && lock) {
			cout << endl << "---------------Map Validating Start---------------" << endl;
			//Code to load map
			cout << endl << "---------------Map Validating Done----------------" << endl;
            cout << endl << "[Stage: Map_Validated]" << endl; 
			cout << endl << "1. enter \"addplayer\" to add players into the map"<< endl << endl;
		}
		else if (players_added && lock) {
			cout << endl << "---------------Adding Players Start---------------" << endl;
			//Code to load map
			cout << endl << "---------------Adding Players Done----------------" << endl;
            cout << endl << "[Stage: Player_Added]" << endl;
            cout << endl << "1. enter \"assigncountries\" to assign countries to each player" << endl;
            cout << endl << "2. enter \"addplayer\" to add more player " << endl << endl;
		}

		cin >> input;
		
		//Start up starts; Start Stage
		if((start || map_loaded)&& input.compare("loadmap") == 0) {

			start = false;
			map_loaded = true;
			lock = true;
			continue;
		}
		
		//Map Loaded Stage
		else if (map_loaded && input.compare("validatemap") == 0) {

			map_loaded = false;
			map_validated = true;
			lock = true;
			continue;
		}
		

		//Map validated Stage
		else if ((map_validated || players_added) && input.compare("addplayer") == 0) {


			players_added = true;
			map_validated = false;
			lock = true;
			continue;
		}

		//Player_added state
	    else if (players_added && input.compare("assigncountries") == 0) {
          
			players_added = false;
			assign_reinforcement = true;
			playStage = true;
			
		}
		else {

			if (start) {
				cout << endl << "---------------Invalid commands detected At [Start] stage---------------" << endl;
				cout << endl << "[Stage: Start] Please enter \"loadmap\" to load the map: " << endl << endl;
			} else if(map_loaded){
				cout << endl << "---------------Invalid commands detected At [Map_Loaded] stage---------------" << endl;
				cout << endl << "[Stage: Map_Loaded] Please enter \"validatemap\" to validate the map: " << endl << endl;
			} else if (map_validated) {
				cout << endl << "---------------Invalid commands detected At [Map_validated] stage---------------" << endl;
				cout << endl << "[Stage: Map_Validated] Please enter \"addplayer\" to add players into the map " << endl << endl;
			} else if (players_added) {
				cout << endl << "---------------Invalid commands detected At [Players_Added] stage---------------" << endl;
				cout << endl << "[Stage: Player_Added] Please enter \"assigncountries\" to assign countries to each player " << endl << endl;
			}
			//No state transition
			lock = false;
			continue;	
			
		}

		//Play stage starts
		while (playStage) {

            //Display state information and prompt users to enter commands
			if (assign_reinforcement && lock2) {
				cout << endl << "----------------Assigning Reinforcement Starts---------------" << endl;
				//code to assign reinforcement
				cout << endl << "---------------Assigning Reinforcement Done---------------" << endl;
				cout << endl <<"[Stage: Assign Reinforcement]" << endl;
				cout << endl <<	"1. enter \"issueorder\" to issue orders for players: " << endl << endl;
			}
			else if(issue_orders && lock2) {
				cout << endl << "---------------Issuing Orders Starts---------------" << endl;
				//code to issue orders
				cout << endl << "---------------Issuing Orders Done-----------------" << endl;
                cout << endl << "[Stage: Issue Orders]" << endl;
                cout << endl <<	"1. enter \"issueorder\" to issue orders for players" << endl;
				cout << endl <<	"2. enter \"endissueorders\" to end: " << endl << endl;
			}
			else if (execute_orders && lock2) {
				if (executed) {
					cout << endl << "---------------Executing Orders Starts---------------" << endl;
					//code to execute orders
					cout << endl << "---------------Executing Orders Done-----------------" << endl;
				}
				cout<< endl << "[Stage: Execute Orders]" << endl;
                cout<< endl << "1. enter \"execorder\" to issue orders for players" << endl;
			    cout<< endl << "2. enter \"endexecorders\" to issue orders for players" << endl;
                cout<< endl << "3. enter \"win\" to win the game: " << endl << endl;
			}
			else if (win && lock2) {
				cout << endl << "---------------Generating the Winner Starts---------------" << endl;
				//code for generating the winner
				cout << endl << "---------------Generating the Winner Done---------------" << endl;
                cout << endl << "[Stage: Win]" << endl;
				cout << endl <<	"1. enter \"play\" to replay the game: " << endl;
				cout << endl <<	"1. enter \"end\" to end the game: " << endl << endl;
			}
	
			
			//user inputs
			cin >> input;

			//Issue orders
			if ((assign_reinforcement || issue_orders) && input.compare("issueorder") == 0) {

				assign_reinforcement = false;
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
			 else if (win && input.compare("play")==0) {

				reset();
				playStage = false;
                cout << endl << "[Stage: Start]" << endl;
				cout << endl << "Please enter \"loadmap\" to load the map: " << endl << endl;
				break;

			}
			

			//Win(End the game)
			else if (win && input.compare("end") == 0) {

				run = false;
				break;

			}

			//Invalid inputs
			else {

				if(assign_reinforcement)
				cout << endl << "---------------Invalid commands detected at [Assign reinforcement] state---------------" << endl << "[Stage: Assign Reinforcement] Enter \"issueorder\" to issue orders for players: " << endl;
				else if(issue_orders)
				cout << endl << "---------------Invalid commands detected at [Issue Orders] state---------------" << endl << "[Stage: Issue Orders] Enter \"issueorder\" to issue orders for players OR \"endissueorder\" to end: " << endl;
				else if(execute_orders)
				cout << endl << "---------------Invalid commands detected at [Execute Orders] state---------------" << endl << "[Stage: Execute Orders] Enter \"execorder\" to execute orders for players OR \"win\" to win the game: " << endl;
				else if(win)
				cout << endl << "---------------Invalid commands detected at [Win] state---------------" << endl << "[Stage: Win] Enter \"play\" to replay the game OR \"end\" to end the game: " << endl;
				lock2 = false;
				continue;
				
			}
						

		}


	}

	cout << endl << "---------------Game OVer. Thank you for playing the warzone!---------------";

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

