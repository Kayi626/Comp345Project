#include <iostream>
#include <cstring>
#include <vector>
#include "Map.h"
#include "Territory.h"

using namespace std;

//Constructors
Territory::Territory(string countryName, int armyNumber,int countryID,int belongToWhichContinent,int x,int y) {
	Territory::ctrName = countryName;
	Territory::armyNumber = armyNumber;
	Territory::ctrID = (countryID <0)? abs(countryID): countryID;
	Territory::contiBelong = (belongToWhichContinent < 0)? abs(belongToWhichContinent) : belongToWhichContinent;
	Territory::ctrAxisX = (x < 0) ? abs(x) : x;
	Territory::ctrAxisY = (y < 0) ? abs(y) : y;

}
Territory::Territory() {
	ctrAxisX = 0;
	ctrAxisY = 0;
	ctrID = 0;
	ctrName = "";
}

//Accessors
string Territory::getName() {

	return Territory::ctrName;
}
int Territory::getArmyNumber() {

	return Territory::armyNumber;
}

int Territory::getCountryID() {
	
	return Territory::ctrID;
}

int Territory::getBelongedContinentID() {
	return Territory::contiBelong;
}

int Territory::getCtrAxisX() {
	return Territory::ctrAxisX;
}
int Territory::getCtrAxisY() {
	return Territory::ctrAxisY;
}


//Mutators
void Territory::setName(string countryName) {

	Territory::ctrName = countryName;
	
}

void Territory::setArmyNumber(int armyNumber) {

	Territory::armyNumber = armyNumber;

}
void Territory::setCountryID(int countryID) {
	
	Territory::ctrID = countryID;
}

void Territory::setBelongedContinentID(int belongToWhichContinent) {
    Territory::contiBelong =belongToWhichContinent;
}

void Territory::setCtrAxisX(int x) {
	Territory::ctrAxisX = x;
};
void Territory::setCtrAxisY(int y) {
	Territory::ctrAxisY = y;
}

