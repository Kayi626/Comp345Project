#include <iostream>
#include <vector>
#include <cstring>
#include "Continent.h"

using namespace std;

Continent::Continent(int continentID, string continentName, int bonus,string color) {
	Continent::continentID = continentID;
	Continent::continentName = continentName;
	Continent::continentColor = color;
	Continent::continentBonus = bonus;

}

Continent::Continent() {
	continentID = -1;
	continentName = "";
	string continentColor="";
	int continentBonus=0;
	vector<Territory*> countryInside(10);
}

Continent::Continent(const Continent& conti) {
	this->continentName = conti.continentName;
	this->continentColor = conti.continentColor;
	this->continentID = conti.continentID;
	this->countryInside = conti.countryInside;
}


void Continent::display() {
	cout <<endl << "Continent " << continentID << ": " << continentName << endl << "Has Countries: ";
	if (countryInside.size() > 0) {
		for (int x = 0; x < countryInside.size(); x++) {
			Territory temp = *(countryInside[x]);
			cout << "|"<<temp.getName() << "|  ";
		}
	}
	else
		cout << "It contains no countries.";
	cout << endl<<endl;
}
int Continent::getID() {
	return Continent::continentID;
}
string Continent::getName() {

	return Continent::continentName;
}
vector<Territory*>& Continent::getCountryInside() {
	return Continent::countryInside;
}

string Continent::getColor() {
	return continentColor;
}
int Continent::getBonus() {
	return continentBonus;
}