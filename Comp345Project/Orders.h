#pragma once
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include "Map.h"
#include "LoggingObserver.h"

using namespace std;


class Orders: public ILoggable, public Subject {
	private:
		int orderID;
		int playerID;
	public:
		virtual string stringToLog();
		Orders();
		~Orders();
		Orders(int playerID);
		Orders(const Orders& ord);
		Orders& operator = (const Orders& ord);

		friend ostream& operator <<(ostream &ost, const Orders& ord);
		virtual void describe(std::ostream& output) const;
		//inorder to make stream insertion operator works on subclass, adding describe()method.
		//in the subclass, describe() is overrideed, and << will output the correct message when called.

		virtual std::unique_ptr<string> describingMessage();
		//an message that describing the order. basicly a tostring method.
		virtual string execute();
		//execute the order. will validate first,but wont delete itself from order list.
		virtual bool validate();
		int getOrderID();
		//return an unique order id.
		int getPlayerID();
		//return the player'id who holds this order..

		//bool isDeployOrder;
};

class OrderList: public ILoggable, public Subject {
private:
	int playerID;//ID of the player hold this orderlist
	vector<Orders*> ordersInside;
	//all the order inside of this vector list.
public:
	string stringToLog();
	OrderList();
	OrderList(int playerID);
	OrderList(const OrderList& ordlist);
	OrderList& operator = (const OrderList& ordlist);
	~OrderList();

	void addOrder(Orders* orderInsert); //add an order into the list
	bool move(int orderID,int indexmoving);  //move an order around the orderlist
	//0 = do nothing. 3 = move it 3 place backward. -2 = move it 2 place forward.
	//return false: can't find the order / the moving is out of range.

	bool remove(int orderID);  //remove an order
	void removeAll();//remove all orders

	void displayAll();//print the current orderlist. will show the details of every order.
	int getPlayerID();//get the player that own this order list

	Orders* getFirst();
	Orders* getLast();
	//getFirst & getLast: just return a pointer to the First/Last order in the list.

	Orders* popFirst();
	Orders* popLast();
	//popFirst & popLast: will remove the order from the list after return its pointer.
	vector<Orders*> getAllOrders();
	//return ordersInside
};

class DeployOrder : public Orders {
private:
	int numberOfArmies;
	Territory* targetTerritory;
public:
	DeployOrder(int playerID,int numberOfArmies, Territory* targetTerritory);
	DeployOrder(const DeployOrder& ord);
	DeployOrder& operator = (const DeployOrder& ord);
	virtual void describe(std::ostream& output) const override;

	virtual std::unique_ptr<string> describingMessage();
	virtual string execute();
	virtual bool validate();
	/*Conctructor: targetTerritory is the territories which player need to place his armies.
	* validate(): will check if the targetTerritory's holding player is belongs to players.
	*/
};

class AdvanceOrder : public Orders {
private:
	int numberOfArmies;
	Territory* fromTerritory;
	Territory* targetTerritory;
	bool isAdjacent;
public:
	AdvanceOrder(int playerID, int numberOfArmies, Territory* fromTerritory, Territory* targetTerritory, bool isAdjacent);
	AdvanceOrder(const AdvanceOrder& ord);
	AdvanceOrder& operator = (const AdvanceOrder& ord);
	virtual void describe(std::ostream& output) const override;

	virtual std::unique_ptr<string> describingMessage();
	virtual string execute();
	virtual bool validate();
	/*Conctructor: targetTerritory is the territories which player sending his armies to.
	*			fromTerritory: the territories player sending his armies  
	*
	* validate(): will check if the fromTerritory's holding player is belongs to players.
	*						if the targetTerritory is adjacent to fromTerritory.
	*						if the fromTerritory have enough armies inside.
	*/
};

class BombOrder : public Orders {
private:
	Territory* targetTerritory;
	bool isAdjacent;
public:
	BombOrder(int playerID, Territory* targetTerritory,bool isAdjacent);
	BombOrder(const BombOrder& ord);
	BombOrder& operator = (const BombOrder& ord);
	virtual void describe(std::ostream& output) const override;

	virtual std::unique_ptr<string> describingMessage();
	virtual string execute();
	virtual bool validate();
	/*Conctructor: targetTerritory is the territories which player wanna bomb.
	*
	* validate(): will check if the targetTerritory's is not holding by players.
	*						if the targetTerritory is adjacent to one of player's controlling territory.
	*/
};

class BlockadeOrder : public Orders {
private:
	Territory* targetTerritory;
public:
	BlockadeOrder(int playerID, Territory* targetTerritory);
	BlockadeOrder(const BlockadeOrder& ord);
	BlockadeOrder& operator = (const BlockadeOrder& ord);
	virtual void describe(std::ostream& output) const override;

	virtual std::unique_ptr<string> describingMessage();
	virtual string execute();
	virtual bool validate();
	/*Conctructor: set the targetTerritory.
	*
	* validate(): will check if the targetTerritory's is holding by players.
	*/
};

class AirliftOrder : public Orders {
private:
	int numberOfArmies;
	Territory* fromTerritory;
	Territory* targetTerritory;
public:
	AirliftOrder(int playerID, int numberOfArmies, Territory* fromTerritory, Territory* targetTerritory);
	AirliftOrder(const AirliftOrder& ord);
	AirliftOrder& operator = (const AirliftOrder& ord);
	virtual void describe(std::ostream& output) const override;

	virtual std::unique_ptr<string> describingMessage();
	virtual string execute();
	virtual bool validate();
	/*Conctructor: fromTerritory is the territories which player sending his armies.
	*			targetTerritory: the territories player sending his armies to
	*
	* validate(): will check if the fromTerritory's is holding by players.
	*			will check if the targetTerritory's is not holding by players.
	*/
};


class NegotiateOrder : public Orders {
private:
	Territory* targetTerritory;
public:
	NegotiateOrder(int playerID, Territory* targetTerritory);
	NegotiateOrder(const NegotiateOrder& ord);
	NegotiateOrder& operator = (const NegotiateOrder& ord);
	virtual void describe(std::ostream& output) const override;

	virtual std::unique_ptr<string> describingMessage();
	virtual string execute();
	virtual bool validate();
	/*Conctructor: targetTerritory: the territories hold by the target player
	* 
	* validate(): will check if the player's id is the same as target territory's holding player id.
	*/
};


