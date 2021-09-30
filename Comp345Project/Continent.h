#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Territory.h"

using namespace std;

class Continent {
  
private:
	int continentID;
	string continentName;
	vector<Territory*> countryInside;
public:
	Continent();
	Continent(int continentID,string continentName);
	void display();
	int getID();
	string getName();
	vector<Territory*> getCountryInside();


};
