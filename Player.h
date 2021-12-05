#pragma once
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include "Orders.h"
#include "Cards.h"
#include "Map.h"

using namespace std;

class PlayerStrategy;

class Player {
private:

	PlayerStrategy* ps;



	Hand* playerHandOfCards;

	bool hasEndThisIssueOrderTurn;
	int reinforcementpool;
	bool conqueredInThisTurn;
	int estimatePool;
	//this is use to showing player how many armies he have during issue order phase.
	int playerID;
	string playerName;



	bool addToControlledWithNoRepeatition(Territory* t);
	bool addToReachableWithNoRepeatition(Territory* t);
	//add to the verctor list by checking territory's Name. won't add two same territory to a vector.
	//return false if there's already exit a territory with same country id
public:
	vector<Territory*> controlledTerritories;
	//will return with (toDefend())
	vector<Territory*> reachcableTerritories;
	//will return with (toAttack())

	OrderList* playerOrderList;
	vector<vector<Territory*>>* mapGraph;
	//The mapGraph that this player is in.

	void attachToPlayerOrderList(list<Observer *> &);
	Player();
	Player(int playerID, string playerName, vector<vector<Territory*>> *mapGraph);
	//constructor will generate an empty 'ControlledTerrtories,HandOfCards,OrderList'.

	Player(const Player& p);
	Player& operator = (const Player& p);
	//Stream Insertion Operator
	friend ostream& operator << (ostream& ost, const Player& player);

	//Accessors
	string getName();
	int getPlayerID();
	OrderList* getOrderList();
	Hand* getHandsOfCard();

	PlayerStrategy* getPlayerStrategy();
	void setPlayerStrategy(PlayerStrategy* newPS);


	vector<Territory*> toDefend();
	//return a list of terrtiries to be defended
	vector<Territory*> toAttack();
	//return a list of terrtiries to be attacked
	

	 //Mutators
	bool checkAndResetConqueredInThisTurn();
	void setConqueredInThisTurn(bool input);

	void sethasEndThisIssueOrderTurn(bool input);
	bool gethasEndThisIssueOrderTurn();

	void addReinforcementpool(int amount);
	void setReinforcementpool(int amount);
	int getReinforcementpool();

	int getEstimatePool();
	void setEstimatePool(int amount);
	void substractEstimatePool(int amount);
	void updateEstimatePool();
	//will set the estimatePool equals to reinforcementpool

	void setName(string playerName);
	void setPlayerID(int newPlayerID);
	void addTerrtories(Territory* newTerritory);
	//will set the territory's controlledPlayerID to current player's and update the list.

	void update();
	//core function. will asign new values to ControlledTerritories and ReachcableTerritories

	void printPlayerTerrtories();
	//use 'std::cout' showing the result of toDefend() and toAttack()

	void issueOrder(int type, Territory* targetTerritory,int numberOfArmies, Territory* fromTerritory);
	/*creates an Orders objectand puts it in the player's list of orders.
	* type:
	* 0-DeployOrder, 3 args
	* 1-AdvanceOrder, 4 args
	* 2-BombOrder, 2 args
	* 3-BlockadeOrder, 2 args
	* 4-AirliftOrder, 4 args
	* 5-NegotiateOrder, 2 args : when selecting target player, just let the player select one of his territory.
	* 
	* all 4 arg is required for every kinds of order. BUT if an order only requires 3 or less argument, the arguments left behind can be whatever.
	* e.g. Bomborder - 3rd arg'number of armies' can be any integer. 4th argument 'from territory' can be any territory, you can just simply put the 2nd argument inside. it won't have any effect.
	*/

};

