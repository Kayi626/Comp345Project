#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include "Territory.h"

using namespace std;

class Continent {
  
private:
	int continentID;
	string continentName;
	string continentColor;
	int continentBonus;
	vector<Territory*> countryInside;
public:
	//Constructors
	Continent(int continentID,string continentName,int bonus, string color);
	Continent();
	Continent(const Continent& conti);

	//Other class functions
	void display();
	int getID();
	string getName();
	string getColor();
	int getBonus();
	vector<Territory*>& getCountryInside();


};
