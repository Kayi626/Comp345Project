#pragma once
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include "Territory.h"
#include "Orders.h"
#include "Cards.h"

using namespace std;

class Player {
private:

	vector<Territory*> controlledTerritories;
	//will return with (toDefend())
	vector<Territory*> reachcableTerritories;
	//will return with (toAttack())
	Hand* playerHandOfCards;
	OrderList* playerOrderList;

	int playerID;
	string playerName;
	vector<vector<Territory*>>* mapGraph;
	//The mapGraph that this player is in.

	bool addToControlledWithNoRepeatition(Territory* t);
	bool addToReachableWithNoRepeatition(Territory* t);
	//add to the verctor list by checking territory's Name. won't add two same territory to a vector.
	//return false if there's already exit a territory with same country id
public:
	Player();
	Player(int playerID,string playerName, vector<vector<Territory*>> *mapGraph);
	//constructor will generate an empty 'ControlledTerrtories,HandOfCards,OrderList'.
	
	//Accessors
	string getName();
	int getPlayerID();
	OrderList* getOrderList();
	Hand* getHandsOfCard();

	vector<Territory*> toDefend();
	//return a list of terrtiries to be defended
	vector<Territory*> toAttack();
	//return a list of terrtiries to be attacked
	

	 //Mutators
	void setName(string playerName);
	void setPlayerID(int newPlayerID);
	void addTerrtories(Territory* newTerritory);
	//will set the territory's controlledPlayerID to current player's and update the list.

	void update();
	//core function. will asign new values to ControlledTerritories and ReachcableTerritories

	void printPlayerTerrtories();

	void issueOrder(int type, Territory* targetTerritory,int numberOfArmies, Territory* fromTerritory);
	/*creates an Orders objectand puts it in the player's list of orders.
	* type:
	* 0-DeployOrder, 3 args
	* 1-AdvanceOrder, 4 args
	* 2-BombOrder, 2 args
	* 3-BlockadeOrder, 2 args
	* 4-AirliftOrder, 4 args
	* 5-NegotiateOrder, 2 args : when selecting target player, just let the player select one of his territory.
	*/

};
