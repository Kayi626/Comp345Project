#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include "Orders.h"

using namespace std;

extern int globalOrderID = 1;
//the global id .Every order have their unique id.

Orders::Orders() {
	this->orderID = globalOrderID;
	globalOrderID++;
}
Orders::~Orders() {
}

int* Orders::getOrderID() {
	return &(Orders::orderID);
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
	vector<Orders*> ordersInside(10);
}
OrderList::OrderList(int playerID) {
	this->playerID = playerID;
	vector<Orders*> ordersInside(10);
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
	}
	return false;
}


void OrderList::displayAll() {
	cout << "----- This is the orderlist hold by player with ID: " << this->playerID << " -----" << endl;
	for (Orders* orders : ordersInside) {
		cout << ">>NO." << *(orders->getOrderID()) << " " << *(orders->describingMessage()) << endl;
	}
	cout << "There are: " << this->ordersInside.size() << " orders in list." << endl;
}

inline int OrderList::getPlayerID() {
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
	ordersInside.erase(std::next(ordersInside.begin(), 0));
	return temp;
}
Orders* OrderList::popLast() {
	Orders* temp = ordersInside.back();
	ordersInside.pop_back();
	return temp;

}


DeployOrder::DeployOrder(int numberOfArmies, Territory* targetTerritory)
	:Orders()
{
	this->numberOfArmies = numberOfArmies;
	this->targetTerritory = targetTerritory;
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
	//TODO: this part leave for next assignment
	return true;
}

AdvanceOrder::AdvanceOrder(int numberOfArmies, Territory* fromTerritory, Territory* targetTerritory)
	:Orders()
{
	this->numberOfArmies = numberOfArmies;
	this->targetTerritory = targetTerritory;
	this->fromTerritory = fromTerritory;
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
	//TODO: this part leave for next assignment
	return true;
}

BombOrder::BombOrder(Territory* targetTerritory)
	:Orders()
{
	this->targetTerritory = targetTerritory;
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
	//TODO: this part leave for next assignment
	return true;
}

BlockadeOrder::BlockadeOrder(Territory* targetTerritory)
	:Orders()
{
	this->targetTerritory = targetTerritory;
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
	//TODO: this part leave for next assignment
	return true;
}

AirliftOrder::AirliftOrder(int numberOfArmies, Territory* fromTerritory, Territory* targetTerritory)
	:Orders()
{
	this->numberOfArmies = numberOfArmies;
	this->targetTerritory = targetTerritory;
	this->fromTerritory = fromTerritory;
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
	//TODO: this part leave for next assignment
	return true;
}

NegotiateOrder::NegotiateOrder()
	:Orders()
{
}
std::unique_ptr<string> NegotiateOrder::describingMessage() {
	string temp1 = "[NEGOTIATE] Prevent attacks between the current player and another player unitl the end of the turn ";
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
	//TODO: this part leave for next assignment
	return true;
}
