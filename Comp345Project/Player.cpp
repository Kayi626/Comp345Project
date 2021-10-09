#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "Cards.h"

using namespace std;

Player::Player() {

	this->playerID = -1;
	this->playerName = "NONAME";
}
Player::Player(int playerID, string playerName, vector<vector<Territory*>> *mapGraph) {
	this->playerID = playerID;
	this->playerName = playerName;
	this->mapGraph = mapGraph;

	vector<Territory*> controlledTerritories(10);
	vector<Territory*> reachcableTerritories(10);

	this->playerHandOfCards = new Hand();
	this->playerOrderList = new OrderList(playerID);

	this->update();
	// asign correct values to ControlledTerritories and ReachcableTerritories.
}

Player& Player::operator= (const Player& p) {
	if (this == &p) {
		return (Player&)p;
	}
	this->playerID = *new int(p.playerID);
	this->playerName = *new string(p.playerName);
	this->mapGraph = new vector<vector<Territory*>>(*(p.mapGraph));
	this->controlledTerritories = *new vector<Territory*>(p.controlledTerritories);
	this->reachcableTerritories = *new vector<Territory*>(p.reachcableTerritories);
	this->playerHandOfCards = new Hand(*(p.playerHandOfCards));
	this->playerOrderList = new OrderList(*(p.playerOrderList));
	return *this;

}
Player::Player(const Player& p) {
	this->playerID = *new int(p.playerID);
	this->playerName = *new string(p.playerName);
	this->mapGraph = new vector<vector<Territory*>>(*(p.mapGraph));
	this->controlledTerritories = *new vector<Territory*>(p.controlledTerritories);
	this->reachcableTerritories = *new vector<Territory*>(p.reachcableTerritories);
	this->playerHandOfCards = new Hand(*(p.playerHandOfCards));
	this->playerOrderList = new OrderList(*(p.playerOrderList));
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
vector<Territory*> Player::toDefend() {
	return this->controlledTerritories;
}
vector<Territory*> Player::toAttack() {
	return this->reachcableTerritories;
}

void Player::setName(string playerName) {
	this->playerName = playerName;
}
void Player::setPlayerID(int newPlayerID) {
	this->playerID= newPlayerID;
}
void Player::addTerrtories(Territory* newTerritory) {
	cout << endl<<newTerritory->getName() << "\n";
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

	cout << "----- These are the Terrtories to be defended by player: " << this->playerName << " -----" << endl;
	for (int i = 0; i < controlledTerritories.size();++i) {
		Territory temp = *(controlledTerritories[i]);
		cout << ">>Country: " << temp.getName() << " have armies: " << temp.getArmyNumber() << endl;
	}
	cout << "There are totally: " << this->controlledTerritories.size() << " captured by player." << endl<<endl;
	cout << "----- These are the Terrtories to be attacked by player: " << this->playerName << " -----" << endl;
	for (int i = 0; i < reachcableTerritories.size(); ++i) {
		Territory temp = *(reachcableTerritories[i]);
		cout << ">>Country: " << temp.getName() << " have armies: " << temp.getArmyNumber() << endl;
	}
	cout << "There are: " << this->reachcableTerritories.size() << " reachable by player." << endl;
}

void Player::issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory) {
	switch (type) {
		case 0: {
			//0 - DeployOrder, 3 args
			Orders* order = new DeployOrder(this->getPlayerID(), numberOfArmies, targetTerritory);
			this->getOrderList()->put(order);
			break;
		}
		case 1: {
			//1 - AdvanceOrder, 4 args
			bool isAdjacent = false;

			for (vector<Territory*> v : *mapGraph) {
				bool isHoldByPlayer = false;
				for (int x = 0; x < v.size(); x++) {
					Territory temp = *(v[x]);
					if (x == 0) {
						if (temp.getName().compare(fromTerritory->getName())== 0) {
							isHoldByPlayer = true;
						}
					}
					else if (isHoldByPlayer) {
						if (temp.getName().compare(targetTerritory->getName()) == 0) {
							isAdjacent = true;
						}
					}
				}
			} // check if the target terrtory and from territory is adjacent

			Orders* order = new AdvanceOrder(this->getPlayerID(), numberOfArmies, fromTerritory, targetTerritory,isAdjacent);
			this->getOrderList()->put(order);
			break;
		}
		case 2: {
			//2 - BombOrder, 2 args
			bool isAdjacent = false;
			for (Territory* ter : this->toAttack()) {
				if (ter->getName().compare(targetTerritory->getName()) == 0) {
					isAdjacent = true;
				}
			}
			Orders* order = new BombOrder(this->getPlayerID(),targetTerritory,isAdjacent);
			this->getOrderList()->put(order);
			break;
		}
		case 3: {
			//3 - BlockadeOrder, 2 args
			Orders* order = new BlockadeOrder(this->getPlayerID(), targetTerritory);
			this->getOrderList()->put(order);
			break;
		}
		case 4: {
			//4 - AirliftOrder, 4 args
			Orders* order = new AirliftOrder(this->getPlayerID(), numberOfArmies, fromTerritory, targetTerritory);
			this->getOrderList()->put(order);
			break;
		}
		case 5: {
			//5 - NegotiateOrder, 1 args
			Orders* order = new NegotiateOrder(this->getPlayerID(), targetTerritory);
			this->getOrderList()->put(order);
			break;
		}
	}

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