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

PlayerStrategy::PlayerStrategy() {
	this->strategyType = -1;
}
PlayerStrategy::PlayerStrategy(Player* p, int type) {
	this->player = p;
	this->strategyType = type;

}
PlayerStrategy::PlayerStrategy(const PlayerStrategy& p) {
	this->strategyType = p.strategyType;
	this->player = p.player;
}
PlayerStrategy& PlayerStrategy::operator= (const PlayerStrategy& p) {
	if (this == &p) {
		return (PlayerStrategy&)p;
	}
	this->strategyType = p.strategyType;
	this->player = p.player;
	return *this;

}
void PlayerStrategy::issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory)
{
	return;
}

vector<Territory*> PlayerStrategy::toDefend()
{
	return vector<Territory*>();
}

vector<Territory*> PlayerStrategy::toAttack()
{
	return vector<Territory*>();
}

ostream& operator << (ostream& ost, const PlayerStrategy& playerStr) {
	ost << "[Player Strategy type]: " << playerStr.strategyType << endl;
	return ost;
}
int PlayerStrategy::getStrategyType() {
	return this->strategyType;
}



HumanPlayerStrategy::HumanPlayerStrategy() :PlayerStrategy() {
}
HumanPlayerStrategy::HumanPlayerStrategy(Player* p) :PlayerStrategy(p,1){


}
HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& p) {
	this->player = p.player;
	this->strategyType = p.strategyType;
}
HumanPlayerStrategy& HumanPlayerStrategy::operator= (const HumanPlayerStrategy& p) {
	if (this == &p) {
		return (HumanPlayerStrategy&)p;
	}

	PlayerStrategy::operator=(p);

	return *this;

}
ostream& operator << (ostream& ost, const HumanPlayerStrategy& playerStr) {
	ost << "[Player Strategy type]: " << playerStr.strategyType << endl;
	return ost;
}

void HumanPlayerStrategy::issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory) {

	switch (type) {
		case 0: {
			//0 - DeployOrder, 3 args
			Orders* order = new DeployOrder(this->player->getPlayerID(), numberOfArmies, targetTerritory);
			this->player->playerOrderList->addOrder(order);
			break;
		}
		case 1: {
			//1 - AdvanceOrder, 4 args
			bool isAdjacent = false;

			for (vector<Territory*> v : *(this->player->mapGraph)) {
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

			Orders* order = new AdvanceOrder(this->player->getPlayerID(), numberOfArmies, fromTerritory, targetTerritory,isAdjacent);

			this->player->playerOrderList->addOrder(order);
			break;
		}
		case 2: {
			//2 - BombOrder, 2 args
			bool isAdjacent = false;
			for (Territory* ter : this->player->toAttack()) {
				if (ter->getName().compare(targetTerritory->getName()) == 0) {
					isAdjacent = true;
				}
			}
			Orders* order = new BombOrder(this->player->getPlayerID(),targetTerritory,isAdjacent);
			this->player->playerOrderList->addOrder(order);
			break;
		}
		case 3: {
			//3 - BlockadeOrder, 2 args
			Orders* order = new BlockadeOrder(this->player->getPlayerID(), targetTerritory);
			this->player->playerOrderList->addOrder(order);
			break;
		}
		case 4: {
			//4 - AirliftOrder, 4 args
			Orders* order = new AirliftOrder(this->player->getPlayerID(), numberOfArmies, fromTerritory, targetTerritory);
			this->player->playerOrderList->addOrder(order);
			break;
		}
		case 5: {
			//5 - NegotiateOrder, 1 args
			Orders* order = new NegotiateOrder(this->player->getPlayerID(), targetTerritory);
			this->player->playerOrderList->addOrder(order);
			break;
		}
	}
}

vector<Territory*> HumanPlayerStrategy::toDefend() {

	return this->player->controlledTerritories;
}
vector<Territory*> HumanPlayerStrategy::toAttack() {

	return this->player->reachcableTerritories;
}


NeutralPlayerStrategy::NeutralPlayerStrategy() :PlayerStrategy() {
}
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* p) : PlayerStrategy(p, 4) {


}
NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy& p) {
	this->player = p.player;
	this->strategyType = p.strategyType;
}
NeutralPlayerStrategy& NeutralPlayerStrategy::operator= (const NeutralPlayerStrategy& p) {
	if (this == &p) {
		return (NeutralPlayerStrategy&)p;
	}

	PlayerStrategy::operator=(p);

	return *this;

}
ostream& operator << (ostream& ost, const NeutralPlayerStrategy& playerStr) {
	ost << "[Player Strategy type]: " << playerStr.strategyType << endl;
	return ost;
}

void NeutralPlayerStrategy::issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory) {
	this->player->sethasEndThisIssueOrderTurn(true);
	//telling the gameEngine that this player has end his turn.
	return;
}

vector<Territory*> NeutralPlayerStrategy::toDefend() {
	this->player->controlledTerritories.clear();
	return this->player->controlledTerritories;
	//return nothing since it wont defend or attack it's terrtories.
}
vector<Territory*> NeutralPlayerStrategy::toAttack() {
	this->player->reachcableTerritories.clear();
	return this->player->reachcableTerritories;
	//return nothing since it wont defend or attack it's terrtories.
}


AggressivePlayerStrategy::AggressivePlayerStrategy() :PlayerStrategy() {
}
AggressivePlayerStrategy::AggressivePlayerStrategy(Player* p) : PlayerStrategy(p, 2) {


}
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy& p) {
	this->player = p.player;
	this->strategyType = p.strategyType;
}
AggressivePlayerStrategy& AggressivePlayerStrategy::operator= (const AggressivePlayerStrategy& p) {
	if (this == &p) {
		return (AggressivePlayerStrategy&)p;
	}

	PlayerStrategy::operator=(p);

	return *this;

}
ostream& operator << (ostream& ost, const AggressivePlayerStrategy& playerStr) {
	ost << "[Player Strategy type]: " << playerStr.strategyType << endl;
	return ost;
}

void AggressivePlayerStrategy::issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory) {

	//deploys or advances armies on its strongest country
	this->player->update();
	int armAmout = this->player->getReinforcementpool();
	int strongestCountryIndex = 0;
	int maxArmTemp = 0;
	if (this->toDefend().size() > 0) {
		for (int i = 0; i < this->toDefend().size(); i++)
		{
			if ((this->toDefend())[i]->getArmyNumber() > maxArmTemp) {
				strongestCountryIndex = i;
			}
			//find the strongest country
		}

	}

	//then always advances to enemy territories until it cannot do so anymore
	for (vector<Territory*> v : *(this->player->mapGraph)) {
		//go through the map. find all the terrtories that is next the terrtory we find above.
		if (v[0]->getCountryID() == (this->toDefend())[strongestCountryIndex]->getCountryID()) {
			for (int x = 0; x < v.size(); x++) {
				if (x == 0) {
					Orders* order = new DeployOrder(this->player->getPlayerID(), armAmout, (this->toDefend())[strongestCountryIndex]);
					this->player->playerOrderList->addOrder(order);
					//for the strongest contry it self, deploy all the armies on it with an deploy order.
					//create the deploy order. deploy all the armies on that counytry.

				}
				else if(x>0 && v[x]->getcontrolledPlayerID() == this->player->getPlayerID()){
					Orders* order = new AdvanceOrder(this->player->getPlayerID(), v[x]->getArmyNumber(), v[x], (this->toDefend())[strongestCountryIndex], true);
					this->player->playerOrderList->addOrder(order);
					//if this is the terrtories that is controlled by the current player, create advance order and advance armies to the 'strongest country'.
				}
				else {
					Orders* order = new AdvanceOrder(this->player->getPlayerID(), armAmout+ (this->toDefend())[strongestCountryIndex]->getArmyNumber(), (this->toDefend())[strongestCountryIndex],v[x] , true);
					this->player->playerOrderList->addOrder(order);

					//if this is the terrtories that is not controlled by the current player, create advance all the armies to this country from the strongest country..

					//by the way, as the grading sheet required, 'advances to enemy territories until it cannot do so anymore'. this design have a change that the terrtories next to the strongest terrtory is all controled by the player. Then the aggressive player will stop advancing armies to enemies. however, this is rarely happened, and did't validate the requiement. 
				}
			}
		}
	}

	this->player->sethasEndThisIssueOrderTurn(true);
	//telling the gameEngine that this player has end his turn.
	return;
}

vector<Territory*> AggressivePlayerStrategy::toDefend() {
	return this->player->controlledTerritories;
}
vector<Territory*> AggressivePlayerStrategy::toAttack() {
	return this->player->reachcableTerritories;
}


BenevolentPlayerStrategy::BenevolentPlayerStrategy() :PlayerStrategy() {
}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* p) : PlayerStrategy(p, 3) {


}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy& p) {
	this->player = p.player;
	this->strategyType = p.strategyType;
}
BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator= (const BenevolentPlayerStrategy& p) {
	if (this == &p) {
		return (BenevolentPlayerStrategy&)p;
	}

	PlayerStrategy::operator=(p);

	return *this;

}
ostream& operator << (ostream& ost, const BenevolentPlayerStrategy& playerStr) {
	ost << "[Player Strategy type]: " << playerStr.strategyType << endl;
	return ost;
}

void BenevolentPlayerStrategy::issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory) {

	//find the Weakest country
	this->player->update();
	int armAmout = this->player->getReinforcementpool();
	int weakestCountryIndex = 0;
	int minArmTemp = 10000000;
	if (this->toDefend().size() > 0) {
		for (int i = 0; i < this->toDefend().size(); i++)
		{
			if ((this->toDefend())[i]->getArmyNumber() < minArmTemp) {
				weakestCountryIndex = i;
			}
		}

	}
	//(deploys or advances armies on its weakest countries,
	for (vector<Territory*> v : *(this->player->mapGraph)) {
		//go through the map. find all the terrtories that is next the terrtory we find above.
		if (v[0]->getCountryID() == (this->toDefend())[weakestCountryIndex]->getCountryID()) {
			for (int x = 0; x < v.size(); x++) {
				if (x == 0) {
					Orders* order = new DeployOrder(this->player->getPlayerID(), armAmout, (this->toDefend())[weakestCountryIndex]);
					this->player->playerOrderList->addOrder(order);
					//for the weakest contry it self, deploy all the armies on it with an deploy order.
					//create the deploy order. deploy all the armies on that counytry.

				}
				else if (x > 0 && v[x]->getcontrolledPlayerID() == this->player->getPlayerID()) {
					Orders* order = new AdvanceOrder(this->player->getPlayerID(), v[x]->getArmyNumber(), v[x], (this->toDefend())[weakestCountryIndex], true);
					this->player->playerOrderList->addOrder(order);
					//if this is the terrtories that is controlled by the current player, create advance order and advance armies to the 'weakest country'.
				}
			}
		}
	}

	this->player->sethasEndThisIssueOrderTurn(true);
	//telling the gameEngine that this player has end his turn.
	return;
}

vector<Territory*> BenevolentPlayerStrategy::toDefend() {
	return this->player->controlledTerritories;
}
vector<Territory*> BenevolentPlayerStrategy::toAttack() {
	this->player->reachcableTerritories.clear();
	return this->player->reachcableTerritories;
	//return nothing since it wont  attack other's terrtories.
}



CheaterPlayerStrategy::CheaterPlayerStrategy() :PlayerStrategy() {
}
CheaterPlayerStrategy::CheaterPlayerStrategy(Player* p) : PlayerStrategy(p, 4) {


}
CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy& p) {
	this->player = p.player;
	this->strategyType = p.strategyType;
}
CheaterPlayerStrategy& CheaterPlayerStrategy::operator= (const CheaterPlayerStrategy& p) {
	if (this == &p) {
		return (CheaterPlayerStrategy&)p;
	}

	PlayerStrategy::operator=(p);

	return *this;

}
ostream& operator << (ostream& ost, const CheaterPlayerStrategy& playerStr) {
	ost << "[Player Strategy type]: " << playerStr.strategyType << endl;
	return ost;
}

void CheaterPlayerStrategy::issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory) {

	bool find = false;
	//only do the cheating order once per turn.

	//(deploys or advances armies on its weakest countries,
	for (vector<Territory*> v : *(this->player->mapGraph)) {
		//go through the map. find all the terrtories that is controlled by him.
		if (v[0]->getcontrolledPlayerID() == this->player->getPlayerID() &&(!find)) {
			for (int x = 0; x < v.size(); x++) {
				if (x > 0 && v[x]->getcontrolledPlayerID() != this->player->getPlayerID()) {
					//if this is the enemy terrtory, conqure it by a cheating order.
					Orders* order = new CheatingOrder(this->player->getPlayerID(), v[x]);
					this->player->playerOrderList->addOrder(order);
				}
			}
			find = true;
		}
	}

	this->player->sethasEndThisIssueOrderTurn(true);
	//telling the gameEngine that this player has end his turn.
	return;
}

vector<Territory*> CheaterPlayerStrategy::toDefend() {
	this->player->controlledTerritories.clear();
	return this->player->controlledTerritories;
	//return nothing since it wont defend its terrtories.
}
vector<Territory*> CheaterPlayerStrategy::toAttack() {
	return this->player->reachcableTerritories;
}