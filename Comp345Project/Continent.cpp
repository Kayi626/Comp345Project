#include <iostream>
#include <vector>
#include <cstring>
#include "Continent.h"

using namespace std;

Continent::Continent(int continentID, string continentName) {
	Continent::continentID = continentID;
	Continent::continentName = continentName;
}

Continent::Continent() {

}


void Continent::display() {
	cout <<endl << "Continent No." << continentID << ": " << continentName << endl << "It contains ";
	for (int x = 0; x < countryInside.size(); x++) {
		Territory temp = *(countryInside[x]);
		cout << temp.getName()<< " ";
	}
	cout << endl;
}
int Continent::getID() {
	return Continent::continentID;
}
string Continent::getName() {

	return Continent::continentName;
}
vector<Territory*> Continent::getCountryInside() {
	return Continent::countryInside;
}