#pragma once
#include <iostream>
#include <cstring>

using namespace std;

class Territory {

private:
	string ctrName;
	int ctrID;
	int armyNumber;
	int controlledPlayerID;
	//the ID of player who's controlling this.
	//default set to -1, which means it's a neutral territory.
	int contiBelong;
	int ctrAxisX;
	int ctrAxisY;
public:
	//Constructors
	Territory(string countryName,int countryID,int belongToWhichContinent,int ctrAxisX,int ctrAxisY);
	Territory(const Territory& t);
	Territory();

	//Other functions
	 //Accessors
	 string getName();
	 int getArmyNumber();
	 int getCountryID();
	 int getBelongedContinentID();
	 int getcontrolledPlayerID();
	 int getCtrAxisX();
	 int getCtrAxisY();
	 //Mutators
	 void setName(string countryName);
	 void setArmyNumber(int armyNumber);
	 void setControlledPlayerID(int newPlayerID);
	 //void setCountryID(int countryID);
	 void setBelongedContinentID(int belongToWhichContinent);
	 void setCtrAxisX(int x);
	 void setCtrAxisY(int y);

};
