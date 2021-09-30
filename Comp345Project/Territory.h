#pragma once
#include <iostream>
#include <cstring>

using namespace std;

class Territory {

private:
	string countryName;
	int armyNumber;
	int countryID;
public:
	//Constructors
	Territory(string countryName,int armyNumber,int countryID);
	Territory();

	//Other functions
	 //Accessors
	 string getName();
	 int getArmyNumber();
	 int getCountryID();
	 //Mutators
	 void setName(string countryName);
	 void setArmyNumber(int armyNumber);
	 void setCountryID(int countryID);

};
