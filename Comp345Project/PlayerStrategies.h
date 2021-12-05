#pragma once
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include "Orders.h"
#include "Cards.h"
#include "Map.h"

using namespace std;

class Player;

class PlayerStrategy {
	public:
		Player* player;
		int strategyType;
		PlayerStrategy();
		PlayerStrategy(Player* p,int type);

		PlayerStrategy(const PlayerStrategy& p);
		PlayerStrategy& operator = (const PlayerStrategy& p);
		//Stream Insertion Operator
		friend ostream& operator << (ostream& ost, const PlayerStrategy& playerStr);


		virtual void issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory);

		virtual vector<Territory*> toDefend();
		virtual vector<Territory*> toAttack();

		int getStrategyType();
		/*
		* -1: defafult, ERROR
		* 1: Human Player
		* 2: Aggressive Player
		* 3: Benevolent Player
		* 4: Neutral Player
		* 5: Cheater Player
		*/
};



class HumanPlayerStrategy : public PlayerStrategy {
	public:
		HumanPlayerStrategy();
		HumanPlayerStrategy(Player* p);

		HumanPlayerStrategy(const HumanPlayerStrategy& p);
		HumanPlayerStrategy& operator = (const HumanPlayerStrategy& p);
		//Stream Insertion Operator
		friend ostream& operator << (ostream& ost, const HumanPlayerStrategy& playerStr);


		void issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory);
		vector<Territory*> toDefend();
		vector<Territory*> toAttack();

};


class NeutralPlayerStrategy : public PlayerStrategy {
public:
	NeutralPlayerStrategy();
	NeutralPlayerStrategy(Player* p);

	NeutralPlayerStrategy(const NeutralPlayerStrategy& p);
	NeutralPlayerStrategy& operator = (const NeutralPlayerStrategy& p);
	//Stream Insertion Operator
	friend ostream& operator << (ostream& ost, const NeutralPlayerStrategy& playerStr);


	void issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory);
	vector<Territory*> toDefend();
	vector<Territory*> toAttack();

};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
	AggressivePlayerStrategy();
	AggressivePlayerStrategy(Player* p);

	AggressivePlayerStrategy(const AggressivePlayerStrategy& p);
	AggressivePlayerStrategy& operator = (const AggressivePlayerStrategy& p);
	//Stream Insertion Operator
	friend ostream& operator << (ostream& ost, const AggressivePlayerStrategy& playerStr);


	void issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory);
	vector<Territory*> toDefend();
	vector<Territory*> toAttack();

};



class BenevolentPlayerStrategy : public PlayerStrategy {
public:
	BenevolentPlayerStrategy();
	BenevolentPlayerStrategy(Player* p);

	BenevolentPlayerStrategy(const BenevolentPlayerStrategy& p);
	BenevolentPlayerStrategy& operator = (const BenevolentPlayerStrategy& p);
	//Stream Insertion Operator
	friend ostream& operator << (ostream& ost, const BenevolentPlayerStrategy& playerStr);


	void issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory);
	vector<Territory*> toDefend();
	vector<Territory*> toAttack();

};


class CheaterPlayerStrategy : public PlayerStrategy {
public:
	CheaterPlayerStrategy();
	CheaterPlayerStrategy(Player* p);

	CheaterPlayerStrategy(const CheaterPlayerStrategy& p);
	CheaterPlayerStrategy& operator = (const CheaterPlayerStrategy& p);
	//Stream Insertion Operator
	friend ostream& operator << (ostream& ost, const CheaterPlayerStrategy& playerStr);


	void issueOrder(int type, Territory* targetTerritory, int numberOfArmies, Territory* fromTerritory);
	vector<Territory*> toDefend();
	vector<Territory*> toAttack();

};