#pragma once
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include "Territory.h"

using namespace std;


class Orders {
	private:
		int orderID;
		int playerID;
	public:
		Orders();
		Orders(int playerID);
		~Orders();
		virtual std::unique_ptr<string> describingMessage();
		//an message that describing the order. basicly a tostring method.
		virtual string execute();
		//execute the order. will validate first,but wont delete itself from order list.
		virtual bool validate();
		int* getOrderID();
		int getPlayerID();
		//return an unique order id.
};

class OrderList {
private:
	int playerID;//ID of the player hold this orderlist
	vector<Orders*> ordersInside;
public:
	OrderList();
	OrderList(int playerID);

	void put(Orders* orderInsert); //add an order into the list
	bool move(int orderID,int indexmoving);  //move an order around the orderlist
	//0 = do nothing. 3 = move it 3 place backward. -2 = move it 2 place forward.
	//return false: can't find the order / the moving is out of range.

	bool remove(int orderID);  //remove an order

	void displayAll();//print the current orderlist. will show the details of every order.
	int getPlayerID();//get the player that own this order list

	Orders* getFirst();		
	Orders* getLast();
	//getFirst & getLast: just return a pointer to the First/Last order in the list.

	Orders* popFirst();
	Orders* popLast();
	//popFirst & popLast: will remove the order from the list after return its. pointer.
};

class DeployOrder : public Orders {
private:
	int numberOfArmies;
	Territory* targetTerritory;
public:
	DeployOrder(int playerID,int numberOfArmies, Territory* targetTerritory);
	virtual std::unique_ptr<string> describingMessage();
	virtual string execute();
	virtual bool validate();
};

class AdvanceOrder : public Orders {
private:
	int numberOfArmies;
	Territory* fromTerritory;
	Territory* targetTerritory;
	bool isAdjacent;
public:
	AdvanceOrder(int playerID, int numberOfArmies, Territory* fromTerritory, Territory* targetTerritory, bool isAdjacent);
	virtual std::unique_ptr<string> describingMessage();
	virtual string execute();
	virtual bool validate();
};

class BombOrder : public Orders {
private:
	Territory* targetTerritory;
	bool isAdjacent;
public:
	BombOrder(int playerID, Territory* targetTerritory,bool isAdjacent);
	virtual std::unique_ptr<string> describingMessage();
	virtual string execute();
	virtual bool validate();
};

class BlockadeOrder : public Orders {
private:
	Territory* targetTerritory;
public:
	BlockadeOrder(int playerID, Territory* targetTerritory);
	virtual std::unique_ptr<string> describingMessage();
	virtual string execute();
	virtual bool validate();
};

class AirliftOrder : public Orders {
private:
	int numberOfArmies;
	Territory* fromTerritory;
	Territory* targetTerritory;
public:
	AirliftOrder(int playerID, int numberOfArmies, Territory* fromTerritory, Territory* targetTerritory);
	virtual std::unique_ptr<string> describingMessage();
	virtual string execute();
	virtual bool validate();
};


class NegotiateOrder : public Orders {
private:
	Territory* targetTerritory;
public:
	NegotiateOrder(int playerID, Territory* targetTerritory);
	virtual std::unique_ptr<string> describingMessage();
	virtual string execute();
	virtual bool validate();
};


