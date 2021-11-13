#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include "Orders.h"
#include "Map.h"

using namespace std;

extern int globalOrderID = 1;
//the global id .Every order have their unique id.

Orders::Orders() {
	this->orderID = globalOrderID;
	this->isDeployOrder  = false;
	globalOrderID++;
}
Orders::Orders(int playerID) {
	this->playerID = playerID;
	this->isDeployOrder = false;
	this->orderID = globalOrderID;

	globalOrderID++;
	//this will give a global unique order id to an order.
}
Orders::~Orders() {
}
Orders& Orders::operator= (const Orders& ord) {
	if (this == &ord) {
		return (Orders&)ord;
	}
	this->orderID = *new int(ord.orderID);
	this->playerID = *new int(ord.playerID);
	this->isDeployOrder = *new bool(ord.isDeployOrder);
	return *this;

}
Orders::Orders(const Orders& ord) {
	this->orderID = *new int(ord.orderID);
	this->playerID = *new int(ord.playerID);
	this->isDeployOrder = *new bool(ord.isDeployOrder);
}

ostream& operator<<(ostream& ost, const Orders& ord)
{
	ord.describe(ost);
	return ost;

}
void Orders::describe(std::ostream& output) const {
	std::cout << "The describe From the order base class." << endl;
}


int* Orders::getOrderID() {
	return &(Orders::orderID);
}
int Orders::getPlayerID() {
	return Orders::playerID;
}
std::unique_ptr<string> Orders::describingMessage() {
	std::unique_ptr<string> message = std::make_unique<string>("This is an empty order");
	return message;
}
string Orders::execute() {
	if (!validate())return "ERROR";
	return "ERROR";
}
bool Orders::validate() {
	return false;
}

OrderList::OrderList() {
	this->playerID = -1;
	vector<Orders*> ordersInside(100);
}
OrderList::OrderList(int playerID) {
	this->playerID = playerID;
	vector<Orders*> ordersInside(100);
}

OrderList& OrderList::operator= (const OrderList& ordlist) {
	if (this == &ordlist) {
		return (OrderList&)ordlist;
	}
	this->playerID = *new int(ordlist.playerID);
	this->ordersInside = *new vector<Orders*>(ordlist.ordersInside);
	return *this;

}
OrderList::OrderList(const OrderList& ordlist) {
	this->playerID = *new int(ordlist.playerID);
	this->ordersInside = *new vector<Orders*>(ordlist.ordersInside);
}

void OrderList::put(Orders* orderInsert) {
	ordersInside.push_back(orderInsert);
}
bool OrderList::move(int orderID, int indexmoving) {
	int size = ordersInside.size();
	for (int i = 0; i < size; ++i) {
		if (*(ordersInside[i]->getOrderID()) == orderID) {
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
		if (*(ordersInside[i]->getOrderID()) == orderID) {
			ordersInside.erase(std::next(ordersInside.begin(), i));
			return true;
		}
	}//gothrough the list and remove the order if the input orderID match.
	return false;
}

void OrderList::removeAll() {
	for (int i = 0; i < ordersInside.size(); ++i) {
			ordersInside.clear();		
	}

}


void OrderList::displayAll() {
	std::cout << "----- This is the orderlist hold by player with ID: " << this->playerID << "         -----" << endl;
	for (Orders* orders : ordersInside) {
		std::cout << ">>NO." << *(orders->getOrderID()) << " " << *(orders->describingMessage()) << endl;
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
vector<Orders*>  OrderList::getAllOrders(){
	return ordersInside;
}



DeployOrder::DeployOrder(int playerID, int numberOfArmies, Territory* targetTerritory)
	:Orders(playerID)
{
	this->numberOfArmies = numberOfArmies;
	this->targetTerritory = targetTerritory;
	this->isDeployOrder = true;
}

DeployOrder& DeployOrder::operator= (const DeployOrder& ord) {
	if (this == &ord) {
		return (DeployOrder&)ord;
	}
	Orders::operator=(ord);
	this->numberOfArmies = *new int(ord.numberOfArmies);
	this->targetTerritory = new Territory(*(ord.targetTerritory));
	return *this;

}
DeployOrder::DeployOrder(const DeployOrder& ord) {
	this->numberOfArmies = *new int(ord.numberOfArmies);
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
	if(!validate())return "ERROR";


	//TODO: this part leave for next assignment
	//REMINDER: will need to return the effect of the order after execute.
	return "You have successfuly deployed some armies.";
}
bool DeployOrder::validate() {
	return (this->getPlayerID() == targetTerritory->getcontrolledPlayerID());
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
	this->numberOfArmies = *new int(ord.numberOfArmies);
	this->targetTerritory = new Territory(*(ord.targetTerritory));
	this->fromTerritory = new Territory(*(ord.fromTerritory));
	this->isAdjacent = new bool(isAdjacent);
	return *this;

}
AdvanceOrder::AdvanceOrder(const AdvanceOrder& ord) {
	this->numberOfArmies = *new int(ord.numberOfArmies);
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
	if (!validate())return "ERROR";
	//TODO: this part leave for next assignment
	//REMINDER: will need to return the effect of the order after execute.
	return "You have successfuly moved some army.";
}
bool AdvanceOrder::validate() {

	std::cout << "DEBUG: this->getPlayerID() == fromTerritory->getcontrolledPlayerID(): " << (this->getPlayerID() == fromTerritory->getcontrolledPlayerID()) << "\n";
	std::cout << "DEBUG: isAdjacent: " << isAdjacent << "\n";
	std::cout << "DEBUG: getArmyNumber()>= numberOfArmies " << (fromTerritory->getArmyNumber() >= numberOfArmies) << "\n";
	std::cout << "DEBUG: fromTerritory->getArmyNumber() " << fromTerritory->getArmyNumber() << "\n";
	return (this->getPlayerID() == fromTerritory->getcontrolledPlayerID()
		&&isAdjacent
		&&fromTerritory->getArmyNumber()>= numberOfArmies);
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
	if (!validate())return "ERROR";
	//TODO: this part leave for next assignment
	//REMINDER: will need to return the effect of the order after execute.
	return "You destroied half of the armies.";
}
bool BombOrder::validate() {
	return (this->getPlayerID() != targetTerritory->getcontrolledPlayerID() && isAdjacent);
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
	if (!validate())return "ERROR";

	//TODO: this part leave for next assignment
	//REMINDER: will need to return the effect of the order after execute.
	return "You tripled the number of armies on a territry.";
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
	this->numberOfArmies = *new int(ord.numberOfArmies);
	this->targetTerritory = new Territory(*(ord.targetTerritory));
	this->fromTerritory = new Territory(*(ord.fromTerritory));
	return *this;

}
AirliftOrder::AirliftOrder(const AirliftOrder& ord) {
	this->numberOfArmies = *new int(ord.numberOfArmies);
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
	if (!validate())return "ERROR";
	//TODO: this part leave for next assignment
	//REMINDER: will need to return the effect of the order after execute.
	return "You advanced some army to the target terrtories.";
}
bool AirliftOrder::validate() {
	return (this->getPlayerID() == fromTerritory->getcontrolledPlayerID()
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
	if (!validate())return "ERROR";
	//TODO: this part leave for next assignment
	//REMINDER: will need to return the effect of the order after execute.
	return "The attacks between two players is prevented.";
}
bool NegotiateOrder::validate() {
	//just need to validate if it's negotiate with itself.
	return (!(this->getPlayerID() == this->targetTerritory->getcontrolledPlayerID()));
}
