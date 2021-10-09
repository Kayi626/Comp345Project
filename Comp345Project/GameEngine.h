#pragma once

#include <iostream>
#include <string>

using namespace std;

class GameEngine {

private:
	bool start;
	bool map_loaded;
	bool map_validated;
	bool players_added;
	bool assign_reinforcement;
	bool issue_orders;
	bool execute_orders;
	bool win;

public:
	//constructors
	GameEngine();
	GameEngine(const GameEngine& ge);
	//other class functions
	void gameFlow();
	void reset();
	//Stream Insertion Operator
	friend ostream& operator << (ostream& ost, const GameEngine& ge);
	void checkState(std::ostream& output) const;

	//Accessors
	bool getStart();
	bool getMap_Loaded();
	bool getMap_Validated();
	bool getPlayers_Added();
	bool getAssign_Reinforcement();
	bool getIssue_Orders();
	bool getExecute_Orders();
	bool getWin();
};