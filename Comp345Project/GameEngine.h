#pragma once

#include <iostream>
#include <string>

using namespace std;

class CommandProcessor;
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
	//Assignment Operator
	GameEngine& operator = (const GameEngine& ge);
	//Stream Insertion Operator
	friend ostream& operator << (ostream& ost, const GameEngine& ge);
	~GameEngine();
	//other class functions
	void gameFlow(CommandProcessor& comP);
	void reset();
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