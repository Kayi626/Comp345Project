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
	vector<Territory*> countryInside;
public:
	//Constructors
	Continent(int continentID,string continentName);
	Continent();

	//Other class functions
	void display();
	int getID();
	string getName();
	vector<Territory*> getCountryInside();

};
