#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <random>
#include "Orders.h"
#include "Map.h"
#include "GameEngine.h"

using namespace std;

extern int globalOrderID = 1;
//the global id .Every order have their unique id.

Orders::Orders() {
	this->orderID = globalOrderID;
	globalOrderID++;
}
Orders::Orders(int playerID) {
	this->playerID = playerID;
	this->orderID = globalOrderID;

	globalOrderID++;
	//this will give a global unique order id to an order.
}
Orders::~Orders() {}
Orders& Orders::operator= (const Orders& ord) {
	if (this == &ord) {
		return (Orders&)ord;
	}
	this->orderID = ord.orderID;
	this->playerID = ord.playerID;
	return *this;
}
Orders::Orders(const Orders& ord) {
	this->orderID = ord.orderID;
	this->playerID = ord.playerID;
}

ostream& operator<<(ostream& ost, const Orders& ord) {
	ord.describe(ost);
	return ost;
}
void Orders::describe(std::ostream& output) const {
	std::cout << "The describe From the order base class." << endl;
}


int Orders::getOrderID() {
	return this->orderID;
}
int Orders::getPlayerID() {
	return this->playerID;
}
std::unique_ptr<string> Orders::describingMessage() {
	std::unique_ptr<string> message = std::make_unique<string>("This is an empty order");
	return message;
}
string Orders::execute() {
	notify(this);
	if (!validate())return "ERROR";
	return "ERROR";
}
bool Orders::validate() {
	return false;
}

std::string Orders::stringToLog() { return "Order Executed: [" + to_string(getOrderID()) + "]"; };


OrderList::OrderList() {
	this->playerID = -1;
}
OrderList::OrderList(int playerID) {
	this->playerID = playerID;
}

OrderList& OrderList::operator= (const OrderList& ordlist) {
	if (this == &ordlist) {
		return (OrderList&)ordlist;
	}
	this->playerID = ordlist.playerID;
	this->removeAll();
	for (auto ord : ordlist.ordersInside) {
		Orders *p = new Orders(*ord);
		for (auto &obs : this->observers)
			p->attach(obs);
		this->ordersInside.push_back(p);
	}
	return *this;
}
OrderList::OrderList(const OrderList& ordlist) {
	this->playerID = ordlist.playerID;
	this->removeAll();
	for (auto ord : ordlist.ordersInside) {
		Orders *p = new Orders(*ord);
		for (auto &obs : this->observers)
			p->attach(obs);
		this->ordersInside.push_back(p);
	}
}
OrderList::~OrderList() {
	this->removeAll();
}

void OrderList::addOrder(Orders* orderInsert) {
	for (auto &obs : this->observers)
		orderInsert->attach(obs);
	ordersInside.push_back(orderInsert);
	notify(this);
}
bool OrderList::move(int orderID, int indexmoving) {
	int size = ordersInside.size();
	for (int i = 0; i < size; ++i) {
		if (ordersInside[i]->getOrderID() == orderID) {
			if (indexmoving == 0) {
				//donothing
				return true;
			}
			else if (indexmoving > 0) {
				//if it's moving backwards
				if (i + indexmoving + 1 > size) {
					return false; //if it's out of range
				}
				ordersInside.push_back(ordersInside[i]);
				// move the target order to the back
				ordersInside.erase(std::next(ordersInside.begin(), i));
				//delete the order on origin position
				i += indexmoving;
				int moving = size - i - 1;
				while(moving > 0) {
					moving--;
					ordersInside.push_back(ordersInside[i]);
					ordersInside.erase(std::next(ordersInside.begin(), i));
				}
				return true;
			}
			else if(indexmoving < 0) {
				//if it's moving foward
				if (i + indexmoving < 0) {

					return false; //if it's out of range
				}
				int temp1 = abs(indexmoving);
				int temp2 = size + i - 1; //moves after the first move
				int temp3 = i+indexmoving;
				while (temp1>0) {
					ordersInside.push_back(ordersInside[temp3]);
					ordersInside.erase(std::next(ordersInside.begin(), temp3));
					temp1--;
				}
				i++;
				while (temp2 > 0) {
					ordersInside.push_back(ordersInside[i]);
					ordersInside.erase(std::next(ordersInside.begin(), i));
					temp2--;
				}
				return true;
				//example: move the order 6, 3 place forward.
				//1 2 3 4 5 6 7 8    Length = 8; move index:5 3 place foward.
				//1 2 6 7 8 3 4 5
				//1 2 6 3 4 5 7 8
			}
			return true;
		}
	}
	return false;
}
bool OrderList::remove(int orderID) {
	for (int i = 0; i < ordersInside.size(); ++i) {
		auto ord = this->ordersInside[i];
		//if (*(ordersInside[i]->getOrderID()) == orderID) {
		if (ord->getOrderID() == orderID) {
			ordersInside.erase(std::next(ordersInside.begin(), i));
			delete ord;
			return true;
		}
	}//gothrough the list and remove the order if the input orderID match.
	return false;
}

void OrderList::removeAll() {
	for (auto &ord : this->ordersInside)
		delete ord;
	ordersInside.clear();
}


void OrderList::displayAll() {
	std::cout << "----- This is the orderlist hold by player with ID: " << this->playerID << "         -----" << endl;
	for (Orders* orders : ordersInside) {
		if (!orders) continue;
		std::cout << ">>NO." << orders->getOrderID() << " " << *(orders->describingMessage()) << endl;
	}

	std::cout << "There are: " << this->ordersInside.size() << " orders in list." << endl;
}//just display all the order. by their describing Message.

int OrderList::getPlayerID() {
	return OrderList::playerID;
}

Orders* OrderList::getFirst() {
	return ordersInside.front();
}
Orders* OrderList::getLast() {
	return ordersInside.back();

}
Orders* OrderList::popFirst() {
	Orders* temp = ordersInside[0];
	//use a pointer save the order. then erase it.
	ordersInside.erase(std::next(ordersInside.begin(), 0));
	return temp;
}
Orders* OrderList::popLast() {
	Orders* temp = ordersInside.back();
	//use a pointer save the order. then erase it.
	ordersInside.pop_back();
	return temp;
}
vector<Orders*> OrderList::getAllOrders(){
	return ordersInside;
}

string Orders::getOrderName() { return "Order"; };
string DeployOrder::getOrderName() { return "DeployOrder"; };
string AdvanceOrder::getOrderName() { return "AdvanceOrder"; };
string BombOrder::getOrderName() { return "BombOrder"; };
string BlockadeOrder::getOrderName() { return "BlockadeOrder"; };
string AirliftOrder::getOrderName() { return  "AirliftOrder"; };
string NegotiateOrder::getOrderName() { return "NegotiateOrder"; };

std::string OrderList::stringToLog() { return "Order Issued: [" + getLast()->getOrderName() + "]"; };

std::string DeployOrder::stringToLog() { return "Order Executed: [" + to_string(getOrderID()) + "]["+ getOrderName() +"]"; };
std::string AdvanceOrder::stringToLog() { return "Order Executed: [" + to_string(getOrderID()) + "][" + getOrderName() + "]"; };
std::string BombOrder::stringToLog() { return "Order Executed: [" + to_string(getOrderID()) + "][" + getOrderName() + "]"; };
std::string BlockadeOrder::stringToLog() { return "Order Executed: [" + to_string(getOrderID()) + "][" + getOrderName() + "]"; };
std::string AirliftOrder::stringToLog() { return "Order Executed: [" + to_string(getOrderID()) + "][" + getOrderName() + "]"; };
std::string NegotiateOrder::stringToLog() { return "Order Executed: [" + to_string(getOrderID()) + "][" + getOrderName() + "]"; };





DeployOrder::DeployOrder(int playerID, int numberOfArmies, Territory* targetTerritory)
	:Orders(playerID)
{
	this->numberOfArmies = numberOfArmies;
	this->targetTerritory = targetTerritory;
}

DeployOrder& DeployOrder::operator= (const DeployOrder& ord) {
	if (this == &ord) {
		return (DeployOrder&)ord;
	}
	Orders::operator=(ord);
	this->numberOfArmies = ord.numberOfArmies;
	this->targetTerritory = new Territory(*(ord.targetTerritory));
	return *this;

}
DeployOrder::DeployOrder(const DeployOrder& ord) {
	this->numberOfArmies = ord.numberOfArmies;
	this->targetTerritory = new Territory(*(ord.targetTerritory));
}


void DeployOrder::describe(std::ostream& output) const {
	string temp1 = "[DEPLOY] Place ";
	string temp2 = to_string(this->numberOfArmies);
	string temp3 = " armies on terrtire: ";
	temp1.append(temp2);
	temp1.append(temp3);
	temp1.append(this->targetTerritory->getName());
	std::cout << temp1;
}

std::unique_ptr<string> DeployOrder::describingMessage() {
	string temp1 = "[DEPLOY] Place ";
	string temp2 = to_string(this->numberOfArmies);

	string temp3 = " armies on terrtire: ";
	temp1.append(temp2);
	temp1.append(temp3);
	temp1.append(this->targetTerritory->getName());
	std::unique_ptr<string> message = std::make_unique<string>(temp1);
	return message;
}

string DeployOrder::execute() {
	notify(this);
	if (!validate()) {
		string temp1 = "[Order Failed]Deploy Order: ";
		string temp2 = to_string(this->getOrderID());
		string temp3 = " from player: ";
		string temp4 = to_string(this->getPlayerID());
		string temp5 = " didn't execute! ";
		temp1.append(temp2);
		temp1.append(temp3);
		temp1.append(temp4);
		temp1.append(temp5);
		temp1.append(this->targetTerritory->getName());
		cout << temp1 << endl;
		return temp1;
	} 

	GameEngine::instance()->subtractPlayerPool(this->getPlayerID(),this->numberOfArmies);
	this->targetTerritory->setArmyNumber((this->targetTerritory->getArmyNumber()) + this->numberOfArmies);



	string temp1 = "[Order Executed]Player: ";
	string temp2 = to_string(this->getPlayerID());
	string temp3 = " deployed ";
	string temp4 = to_string(this->numberOfArmies);
	string temp5 = " armies on terrtire: ";
	temp1.append(temp2);
	temp1.append(temp3);
	temp1.append(temp4);
	temp1.append(temp5);
	temp1.append(this->targetTerritory->getName());

	cout << temp1 << endl;
	return temp1;
}
bool DeployOrder::validate() {
	bool haveEnoughArmies = GameEngine::instance()->checkPlayerPool(this->getPlayerID(), this->numberOfArmies);
	return (  (this->getPlayerID() == targetTerritory->getcontrolledPlayerID())      && haveEnoughArmies);
}

AdvanceOrder::AdvanceOrder(int playerID, int numberOfArmies, Territory* fromTerritory, Territory* targetTerritory, bool isAdjacent)
	:Orders(playerID)
{
	this->numberOfArmies = numberOfArmies;
	this->targetTerritory = targetTerritory;
	this->fromTerritory = fromTerritory;
	this->isAdjacent = isAdjacent;

}
AdvanceOrder& AdvanceOrder::operator= (const AdvanceOrder& ord) {
	if (this == &ord) {
		return (AdvanceOrder&)ord;
	}
	Orders::operator=(ord);
	this->numberOfArmies = ord.numberOfArmies;
	this->targetTerritory = new Territory(*(ord.targetTerritory));
	this->fromTerritory = new Territory(*(ord.fromTerritory));
	this->isAdjacent = new bool(isAdjacent);
	return *this;

}
AdvanceOrder::AdvanceOrder(const AdvanceOrder& ord) {
	this->numberOfArmies = ord.numberOfArmies;
	this->targetTerritory = new Territory(*(ord.targetTerritory));
	this->fromTerritory = new Territory(*(ord.fromTerritory));
	this->isAdjacent = new bool(isAdjacent);
}

void AdvanceOrder::describe(std::ostream& output) const {
	string temp1 = "[ADVANCE] Move ";
	string temp2 = to_string(this->numberOfArmies);
	string temp3 = " armies from terrtire: ";
	string temp4 = " to terrtire: ";
	temp1.append(temp2);
	temp1.append(temp3);
	temp1.append(this->fromTerritory->getName());
	temp1.append(temp4);
	temp1.append(this->targetTerritory->getName());
	std::cout << temp1;
}
std::unique_ptr<string> AdvanceOrder::describingMessage() {
	string temp1 = "[ADVANCE] Move ";
	string temp2 = to_string(this->numberOfArmies);
	string temp3 = " armies from terrtire: ";
	string temp4 = " to terrtire: ";
	temp1.append(temp2);
	temp1.append(temp3);
	temp1.append(this->fromTerritory->getName());
	temp1.append(temp4);
	temp1.append(this->targetTerritory->getName());
	std::unique_ptr<string> message = std::make_unique<string>(temp1);
	return message;
}

string AdvanceOrder::execute() {
	notify(this);
	if (!validate()) {
		string temp1 = "[Order Failed]Advance Order: ";
		string temp2 = to_string(this->getOrderID());
		string temp3 = " from player: ";
		string temp4 = to_string(this->getPlayerID());
		string temp5 = " didn't execute! ";
		temp1.append(temp2);
		temp1.append(temp3);
		temp1.append(temp4);
		temp1.append(temp5);
		temp1.append(this->targetTerritory->getName());
		cout << temp1 << endl;
		return temp1;
	}
	

	//this->targetTerritory->setArmyNumber((this->targetTerritory->getArmyNumber()) + this->numberOfArmies);

	if (targetTerritory->getcontrolledPlayerID() == fromTerritory->getcontrolledPlayerID()) {
		//if there's no battle between to terrtories
		this->targetTerritory->setArmyNumber((this->targetTerritory->getArmyNumber()) + numberOfArmies);
		this->fromTerritory->setArmyNumber((this->fromTerritory->getArmyNumber()) - numberOfArmies);

		string temp1 = "[Order Executed]Player: ";
		string temp2 = to_string(this->getPlayerID());
		string temp3 = " advanced ";
		string temp4 = to_string(this->numberOfArmies);
		string temp5 = " armies from terrtire: ";
		string temp6 = this->fromTerritory->getName();
		string temp7 = " to terrtire: ";
		temp1.append(temp2);
		temp1.append(temp3);
		temp1.append(temp4);
		temp1.append(temp5);
		temp1.append(temp6);
		temp1.append(temp7);
		temp1.append(this->targetTerritory->getName());

		cout << temp1 << endl;
		return temp1;
	}
	else {
		//simulate the attack between terrtories:
		bool battle = true;
		int currentRemainingArmies = numberOfArmies;
		int randomNumber = 0;
		while (battle) {
			currentRemainingArmies - 1;

			std::srand(std::time(nullptr));
			randomNumber = rand() % 10;
			//can be 0,1,2,3,4,5,6,7,8,9
			if (randomNumber < 6) {
				//attacking unit killing one defending army
				this->targetTerritory->setArmyNumber((this->targetTerritory->getArmyNumber()) - 1);
			}
			if (this->targetTerritory->getArmyNumber() > 0) {
				this->targetTerritory->setArmyNumber((this->targetTerritory->getArmyNumber()) - 1);
				std::srand(std::time(nullptr));
				randomNumber = rand() % 10;
				if (randomNumber < 7) {
					//defending unit killing one attacking army
					currentRemainingArmies - 1;
				}
			}
			if (currentRemainingArmies < 1) battle = false;
			if (this->targetTerritory->getArmyNumber() < 1) battle = false;
		}
		if (this->targetTerritory->getArmyNumber() == 0) {
			//attacker win the battle
			this->targetTerritory->setControlledPlayerID(this->getPlayerID());
			this->targetTerritory->setArmyNumber((this->targetTerritory->getArmyNumber()) + currentRemainingArmies);

			GameEngine::instance()->setPlayerConquered(this->getPlayerID());

			string temp1 = "[Order Executed]Player: ";
			string temp2 = to_string(this->getPlayerID());
			string temp3 = " advanced ";
			string temp4 = to_string(this->numberOfArmies);
			string temp5 = " armies from terrtire: ";
			string temp6 = this->fromTerritory->getName();
			string temp7 = " , and succesfully conquered terrtory: ";
			temp1.append(temp2);
			temp1.append(temp3);
			temp1.append(temp4);
			temp1.append(temp5);
			temp1.append(temp6);
			temp1.append(temp7);
			temp1.append(this->targetTerritory->getName());

			cout << temp1 << endl;
			return temp1;
		}
		else {

			string temp1 = "[Order Executed]Player: ";
			string temp2 = to_string(this->getPlayerID());
			string temp3 = " advanced ";
			string temp4 = to_string(this->numberOfArmies);
			string temp5 = " armies from terrtire: ";
			string temp6 = this->fromTerritory->getName();
			string temp7 = " , and failed to conquered terrtory: ";
			temp1.append(temp2);
			temp1.append(temp3);
			temp1.append(temp4);
			temp1.append(temp5);
			temp1.append(temp6);
			temp1.append(temp7);
			temp1.append(this->targetTerritory->getName());

			cout << temp1 << endl;
			return temp1;
		}
	}
}
bool AdvanceOrder::validate() {

	bool InNegotiateOrder = GameEngine::instance()->checkNegotiateOrderList(this->targetTerritory->getcontrolledPlayerID(), this->getPlayerID());
	if (targetTerritory->getcontrolledPlayerID() == fromTerritory->getcontrolledPlayerID()) {
		InNegotiateOrder = false;
	}

	return (this->getPlayerID() == fromTerritory->getcontrolledPlayerID()
		&&isAdjacent
		&&fromTerritory->getArmyNumber()>= numberOfArmies
		&& (!InNegotiateOrder));
}

BombOrder::BombOrder(int playerID, Territory* targetTerritory,bool isAdjacent)
	:Orders(playerID)
{
	this->targetTerritory = targetTerritory;
	this->isAdjacent = isAdjacent;
}
BombOrder& BombOrder::operator= (const BombOrder& ord) {
	if (this == &ord) {
		return (BombOrder&)ord;
	}
	Orders::operator=(ord);
	this->targetTerritory = new Territory(*(ord.targetTerritory));
	this->isAdjacent = new bool(isAdjacent);
	return *this;

}
BombOrder::BombOrder(const BombOrder& ord) {
	this->targetTerritory = new Territory(*(ord.targetTerritory));
	this->isAdjacent = new bool(isAdjacent);
}
void BombOrder::describe(std::ostream& output) const {
	string temp1 = "[BOMB] Destroy half of the armies located on Territory: ";
	temp1.append(this->targetTerritory->getName());
	std::cout << temp1;
}
std::unique_ptr<string> BombOrder::describingMessage() {
	string temp1 = "[BOMB] Destroy half of the armies located on Territory: ";
	temp1.append(this->targetTerritory->getName());
	std::unique_ptr<string> message = std::make_unique<string>(temp1);
	return message;
}

string BombOrder::execute() {
	notify(this);
	if (!validate()) {
		string temp1 = "[Order Failed]Bomb Order: ";
		string temp2 = to_string(this->getOrderID());
		string temp3 = " from player: ";
		string temp4 = to_string(this->getPlayerID());
		string temp5 = " didn't execute! ";
		temp1.append(temp2);
		temp1.append(temp3);
		temp1.append(temp4);
		temp1.append(temp5);
		temp1.append(this->targetTerritory->getName());
		cout << temp1 << endl;
		return temp1;
	}


	this->targetTerritory->setArmyNumber((this->targetTerritory->getArmyNumber()) /2);

	string temp1 = "[Order Executed]Player: ";
	string temp2 = to_string(this->getPlayerID());
	string temp3 = " destoryed half of the unit at Terrtory: ";
	string temp4 = this->targetTerritory->getName();
	temp1.append(temp2);
	temp1.append(temp3);
	temp1.append(temp4);

	cout << temp1 << endl;
	return temp1;
}
bool BombOrder::validate() {
	bool InNegotiateOrder = GameEngine::instance()->checkNegotiateOrderList(this->targetTerritory->getcontrolledPlayerID(), this->getPlayerID());
	return (this->getPlayerID() != targetTerritory->getcontrolledPlayerID() && isAdjacent && (!InNegotiateOrder));
}

BlockadeOrder::BlockadeOrder(int playerID, Territory* targetTerritory)
	:Orders(playerID)
{
	this->targetTerritory = targetTerritory;
}
BlockadeOrder& BlockadeOrder::operator= (const BlockadeOrder& ord) {
	if (this == &ord) {
		return (BlockadeOrder&)ord;
	}
	Orders::operator=(ord);
	this->targetTerritory = new Territory(*(ord.targetTerritory));
	return *this;

}
BlockadeOrder::BlockadeOrder(const BlockadeOrder& ord) {
	this->targetTerritory = new Territory(*(ord.targetTerritory));
}
void BlockadeOrder::describe(std::ostream& output) const {
	string temp1 = "[BLOCKADE] Triple the number of armies & make it neutral on Territory: ";
	temp1.append(this->targetTerritory->getName());
	std::cout << temp1;
}
std::unique_ptr<string> BlockadeOrder::describingMessage() {
	string temp1 = "[BLOCKADE] Triple the number of armies & make it neutral on Territory: ";
	temp1.append(this->targetTerritory->getName());
	std::unique_ptr<string> message = std::make_unique<string>(temp1);
	return message;
}

string BlockadeOrder::execute() {
	notify(this);
	if (!validate()) {
		string temp1 = "[Order Failed]Blockade Order: ";
		string temp2 = to_string(this->getOrderID());
		string temp3 = " from player: ";
		string temp4 = to_string(this->getPlayerID());
		string temp5 = " didn't execute! ";
		temp1.append(temp2);
		temp1.append(temp3);
		temp1.append(temp4);
		temp1.append(temp5);
		temp1.append(this->targetTerritory->getName());
		cout << temp1 << endl;
		return temp1;
	}

	this->targetTerritory->setControlledPlayerID(-1);
	this->targetTerritory->setArmyNumber((this->targetTerritory->getArmyNumber())*3);

	string temp1 = "[Order Executed]Player: ";
	string temp2 = to_string(this->getPlayerID());
	string temp3 = " Executed an blockade order. half of the armies has been destoryed at terrtory: ";
	string temp4 = this->targetTerritory->getName();
	temp1.append(temp2);
	temp1.append(temp3);
	temp1.append(temp4);

	cout << temp1 << endl;
	return temp1;
}
bool BlockadeOrder::validate() {
	return (this->getPlayerID() == targetTerritory->getcontrolledPlayerID());
}

AirliftOrder::AirliftOrder(int playerID, int numberOfArmies, Territory* fromTerritory, Territory* targetTerritory)
	:Orders(playerID)
{
	this->numberOfArmies = numberOfArmies;
	this->targetTerritory = targetTerritory;
	this->fromTerritory = fromTerritory;
}
AirliftOrder& AirliftOrder::operator= (const AirliftOrder& ord) {
	if (this == &ord) {
		return (AirliftOrder&)ord;
	}
	Orders::operator=(ord);
	this->numberOfArmies = ord.numberOfArmies;
	this->targetTerritory = new Territory(*(ord.targetTerritory));
	this->fromTerritory = new Territory(*(ord.fromTerritory));
	return *this;

}
AirliftOrder::AirliftOrder(const AirliftOrder& ord) {
	this->numberOfArmies = ord.numberOfArmies;
	this->targetTerritory = new Territory(*(ord.targetTerritory));
	this->fromTerritory = new Territory(*(ord.fromTerritory));
}

void AirliftOrder::describe(std::ostream& output) const {
	string temp1 = "[AIRLIFT] Advance ";
	string temp2 = to_string(this->numberOfArmies);
	string temp3 = " armies from terrtoire: ";
	string temp4 = " to terrtoire: ";
	temp1.append(temp2);
	temp1.append(temp3);
	temp1.append(this->fromTerritory->getName());
	temp1.append(temp4);
	temp1.append(this->targetTerritory->getName());
	std::cout << temp1;
}
std::unique_ptr<string> AirliftOrder::describingMessage() {
	string temp1 = "[AIRLIFT] Advance ";
	string temp2 = to_string(this->numberOfArmies);
	string temp3 = " armies from terrtoire: ";
	string temp4 = " to terrtoire: ";
	temp1.append(temp2);
	temp1.append(temp3);
	temp1.append(this->fromTerritory->getName());
	temp1.append(temp4);
	temp1.append(this->targetTerritory->getName());
	std::unique_ptr<string> message = std::make_unique<string>(temp1);
	return message;
}

string AirliftOrder::execute() {
	notify(this);
	if (!validate()) {
		string temp1 = "[Order Failed]Airlift Order: ";
		string temp2 = to_string(this->getOrderID());
		string temp3 = " from player: ";
		string temp4 = to_string(this->getPlayerID());
		string temp5 = " didn't execute! ";
		temp1.append(temp2);
		temp1.append(temp3);
		temp1.append(temp4);
		temp1.append(temp5);
		temp1.append(this->targetTerritory->getName());
		cout << temp1 << endl;
		return temp1;
	}
	this->targetTerritory->setArmyNumber((this->targetTerritory->getArmyNumber()) + numberOfArmies);
	this->fromTerritory->setArmyNumber((this->fromTerritory->getArmyNumber()) - numberOfArmies);

	string temp1 = "[Order Executed]Player: ";
	string temp2 = to_string(this->getPlayerID());
	string temp3 = " airlifted ";
	string temp4 = to_string(this->numberOfArmies);
	string temp5 = " armies from terrtire: ";
	string temp6 = this->fromTerritory->getName();
	string temp7 = " to terrtory: ";
	temp1.append(temp2);
	temp1.append(temp3);
	temp1.append(temp4);
	temp1.append(temp5);
	temp1.append(temp6);
	temp1.append(temp7);
	temp1.append(this->targetTerritory->getName());

	cout << temp1 << endl;
	return temp1;
}
bool AirliftOrder::validate() {
	return (this->getPlayerID() == fromTerritory->getcontrolledPlayerID()
		&& this->getPlayerID() == targetTerritory->getcontrolledPlayerID()
		&& fromTerritory->getArmyNumber() >=numberOfArmies);
}

NegotiateOrder::NegotiateOrder(int playerID, Territory* targetTerritory)
	:Orders(playerID)
{
	this->targetTerritory = targetTerritory;

}
NegotiateOrder& NegotiateOrder::operator= (const NegotiateOrder& ord) {
	if (this == &ord) {
		return (NegotiateOrder&)ord;
	}
	Orders::operator=(ord);
	this->targetTerritory = new Territory(*(ord.targetTerritory));
	return *this;

}
NegotiateOrder::NegotiateOrder(const NegotiateOrder& ord) {
	this->targetTerritory = new Territory(*(ord.targetTerritory));
}

void NegotiateOrder::describe(std::ostream& output) const {
	string temp1 = "[NEGOTIATE] Prevent attacks between the current player and player with ID:";
	string temp2 = to_string(this->targetTerritory->getcontrolledPlayerID());
	temp1.append(temp2);
	std::cout << temp1;
}
std::unique_ptr<string> NegotiateOrder::describingMessage() {
	string temp1 = "[NEGOTIATE] Prevent attacks between the current player and player with ID:";
	string temp2 = to_string(this->targetTerritory->getcontrolledPlayerID());
	temp1.append(temp2);
	std::unique_ptr<string> message = std::make_unique<string>(temp1);
	return message;
}

string NegotiateOrder::execute() {
	notify(this);
	if (!validate()) {
		string temp1 = "[Order Failed]Negotiate Order: ";
		string temp2 = to_string(this->getOrderID());
		string temp3 = " from player: ";
		string temp4 = to_string(this->getPlayerID());
		string temp5 = " didn't execute! ";
		temp1.append(temp2);
		temp1.append(temp3);
		temp1.append(temp4);
		temp1.append(temp5);
		temp1.append(this->targetTerritory->getName());
		cout << temp1 << endl;
		return temp1;
	}

	GameEngine::instance()->addToNegotiateOrderList(this->getPlayerID(), this->targetTerritory->getcontrolledPlayerID());

	string temp1 = "[Order Executed]An Negotiate Order between player: ";
	string temp2 = to_string(this->getPlayerID());
	string temp3 = " and player: ";
	string temp4 = to_string(this->targetTerritory->getcontrolledPlayerID());
	string temp5 = " is created! ";
	temp1.append(temp2);
	temp1.append(temp3);
	temp1.append(temp4);
	temp1.append(temp5);
	cout << temp1 << endl;
	return temp1;
}
bool NegotiateOrder::validate() {
	//just need to validate if it's negotiate with itself.
	return (!(this->getPlayerID() == this->targetTerritory->getcontrolledPlayerID()));
}
