#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include "Player.h"
#include "PlayerStrategies.h"
#include "Map.h"
#include "Orders.h"
#include "Cards.h"

using namespace std;

Player::Player() {

	this->playerID = -1;
	this->playerName = "NONAME";
	this->reinforcementpool = 30;
	this->conqueredInThisTurn = false;
	this->estimatePool = 30;
	this->hasEndThisIssueOrderTurn = false;
	this->playerHandOfCards = new Hand();
}
Player::Player(int playerID, string playerName, vector<vector<Territory*>> *mapGraph) {
	this->playerID = playerID;
	this->reinforcementpool = 30;
	this->estimatePool = 30;
	this->conqueredInThisTurn = false;
	this->hasEndThisIssueOrderTurn = false;
	this->playerName = playerName;
	this->mapGraph = mapGraph;

	this->controlledTerritories = vector<Territory*>(1000);
	this->reachcableTerritories = vector<Territory*>(1000);

	this->playerHandOfCards = new Hand();
	this->playerOrderList = new OrderList(playerID);
	//this->playerOrderList->attach()


	this->update();
	// asign correct values to ControlledTerritories and ReachcableTerritories.
}

Player& Player::operator= (const Player& p) {
	if (this == &p) {
		return (Player&)p;
	}
	this->playerID = p.playerID;
	this->reinforcementpool = p.reinforcementpool;
	this->estimatePool = p.estimatePool;
	this->conqueredInThisTurn = p.conqueredInThisTurn;
	this->hasEndThisIssueOrderTurn = p.hasEndThisIssueOrderTurn;
	this->playerName = string(p.playerName);
	this->mapGraph = p.mapGraph;
	this->controlledTerritories = vector<Territory*>(p.controlledTerritories);
	this->reachcableTerritories = vector<Territory*>(p.reachcableTerritories);
	this->playerHandOfCards = new Hand(*(p.playerHandOfCards));
	this->playerOrderList = new OrderList(*(p.playerOrderList));
	this->ps = p.ps;
	return *this;

}
Player::Player(const Player& p) {
	this->playerID = p.playerID;
	this->reinforcementpool = p.reinforcementpool;
	this->conqueredInThisTurn = p.conqueredInThisTurn;
	this->estimatePool = p.estimatePool;
	this->hasEndThisIssueOrderTurn = p.hasEndThisIssueOrderTurn;
	this->playerName = string(p.playerName);
	this->mapGraph = p.mapGraph;
	this->controlledTerritories = vector<Territory*>(p.controlledTerritories);
	this->reachcableTerritories = vector<Territory*>(p.reachcableTerritories);
	this->playerHandOfCards = new Hand(*(p.playerHandOfCards));
	this->playerOrderList = new OrderList(*(p.playerOrderList));
	this->ps = p.ps;
}

void Player::attachToPlayerOrderList(list<Observer*> &observers) {
	for (auto obs : observers) {
		this->playerOrderList->attach(obs);
	}
}

//Stream Input Operator
ostream& operator << (ostream& ost, const Player& player) {
	ost << "[Player ID]: " << player.playerID << "  [Player Name]: " << player.playerName<<endl;
	return ost;
}

string Player::getName() {
	return this->playerName;
}
int Player::getPlayerID() {
	return this->playerID;
}
OrderList* Player::getOrderList() {
	return this->playerOrderList;
}
Hand* Player::getHandsOfCard() {
	return this->playerHandOfCards;
}

PlayerStrategy* Player::getPlayerStrategy() {
	return this->ps;
}

void Player::setPlayerStrategy(PlayerStrategy* newPS) {
	this->ps = newPS;
}

vector<Territory*> Player::toDefend() {

	return this->ps->toDefend();
}
vector<Territory*> Player::toAttack() {

	return this->ps->toAttack();
}




bool Player::checkAndResetConqueredInThisTurn() {
	bool temp = this->conqueredInThisTurn;
	this->conqueredInThisTurn = false;
	return temp;
}
void Player::setConqueredInThisTurn(bool input) {
	this->conqueredInThisTurn = input;
}
bool Player::gethasEndThisIssueOrderTurn() {
	return this->hasEndThisIssueOrderTurn;
}
void Player::sethasEndThisIssueOrderTurn(bool input) {
	this->hasEndThisIssueOrderTurn = input;
}
void Player::addReinforcementpool(int amount) {
	this->reinforcementpool += amount;

}
void Player::setReinforcementpool(int amount) {
	this->reinforcementpool = amount;
}
int Player::getReinforcementpool() {
	return this->reinforcementpool;
}


int Player::getEstimatePool() {
	return this->estimatePool;
}
void Player::setEstimatePool(int amount) {
	this->estimatePool = amount;
}
void Player::substractEstimatePool(int amount) {
	this->estimatePool = this->estimatePool - amount;
}
void Player::updateEstimatePool() {
	this->estimatePool = this->reinforcementpool;
}

void Player::setName(string playerName) {
	this->playerName = playerName;
}
void Player::setPlayerID(int newPlayerID) {
	this->playerID= newPlayerID;
}
void Player::addTerrtories(Territory* newTerritory) {
	newTerritory->setControlledPlayerID(playerID);

	this->update();
}
void Player::update() {

	controlledTerritories.clear();
	reachcableTerritories.clear();

	//add all territroy that is controlled by player(and the territories next to these territories) into the vector
	for (vector<Territory*> v : *mapGraph) {
		bool isHoldByPlayer = false;
		//add all nearby territories into 'reachcableTerritories' if the above flag is find.
		for (int x = 0; x < v.size(); x++) {
			Territory temp = *(v[x]);
			if (x == 0) {
				if (temp.getcontrolledPlayerID() == playerID) {
					//add the territory to the list if the territory's id is match.
					if (addToControlledWithNoRepeatition(v[x])) {
						isHoldByPlayer = true; 
					}
				}
			}	
			else if (isHoldByPlayer){
				addToReachableWithNoRepeatition(v[x]);
			}
		}
	}
	//remove every territory that's already exit in 'controlledTerritories' from ' reachcableTerritories'
	for (int i = reachcableTerritories.size() - 1; i >= 0; --i) {
		int tempID = reachcableTerritories[i]->getCountryID();
		for (int j = 0; j < controlledTerritories.size(); ++j) {
			if (tempID == controlledTerritories[j]->getCountryID()) {
				reachcableTerritories.erase(std::next(reachcableTerritories.begin(), i));
				break;
			}
		}
	}
}

void Player::printPlayerTerrtories() {

	std::cout << "----- These are the Terrtories to be defended by player: " << this->playerName << " -----" << endl;
	for (int i = 0; i < controlledTerritories.size();++i) {
		Territory temp = *(controlledTerritories[i]);
		std::cout << "ID: " << temp.getCountryID()<< " Territory: " << temp.getName() << " have armies: " << temp.getArmyNumber() << endl;
	}
	std::cout << "There are totally: " << this->controlledTerritories.size() << " captured by player." << endl;
	std::cout << "----- These are the Terrtories to be attacked by player: " << this->playerName << " -----" << endl;
	for (int i = 0; i < reachcableTerritories.size(); ++i) {
		Territory temp = *(reachcableTerritories[i]);
		std::cout << "ID: " << temp.getCountryID() << " Territory: "<< temp.getName() << " have armies: " << temp.getArmyNumber() << endl;
	}
	std::cout << "There are: " << this->reachcableTerritories.size() << " reachable by player." << endl;
}

void Player::issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory) {


	this->ps->issueOrder(type, targetTerritory, numberOfArmies, fromTerritory);



}

bool Player::addToControlledWithNoRepeatition(Territory* t) {
	for (Territory* terr : controlledTerritories)
	{

		if (terr->getName().compare(t->getName()) == 0) {
			return false;
		}
	}
	controlledTerritories.push_back(t);
	return true;
}
bool Player::addToReachableWithNoRepeatition(Territory* t) {
	for (Territory* terr : reachcableTerritories)
	{
		if (terr->getName().compare(t->getName()) == 0) {
			return false;
		}
	}
	reachcableTerritories.push_back(t);
	return true;
}

