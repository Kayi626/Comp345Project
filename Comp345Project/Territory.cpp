#include <iostream>
#include <cstring>
#include <vector>
#include "Map.h"
#include "Territory.h"

using namespace std;

//Constructors
Territory::Territory(string countryName, int armyNumber,int countryID) {
	Territory::countryName = countryName;
	Territory::armyNumber = armyNumber;
	Territory::countryID = countryID;
}
Territory::Territory() {
	
}

//Accessors
string Territory::getName() {

	return Territory::countryName;
}
int Territory::getArmyNumber() {

	return Territory::armyNumber;
}

int Territory::getCountryID() {
	
	return Territory::countryID;
}

//Mutators
void Territory::setName(string countryName) {

	Territory::countryName = countryName;
	
}

void Territory::setArmyNumber(int armyNumber) {

	Territory::armyNumber = armyNumber;

}
void Territory::setCountryID(int countryID) {
	
	Territory::countryID = countryID;
}

