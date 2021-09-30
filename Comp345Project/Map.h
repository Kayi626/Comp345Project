#pragma once
#include<iostream>
#include<vector>
#include<cstring>
#include "Territory.h"
#include "Continent.h"

using namespace std;

class Map {

private:
	//A list of vectors of countries; each vector is an adjency list of the vector's first element.
	vector<vector<Territory*>> mapGraph;

	//A list of vectors of countries,serving as subgraphs of the mapGrph, represents all continents. Each element' reference links to the reference of the corresponding element.
	//For example, mapGrph[0][0] is Canada. ContinentGraph[0] is North America. Its vector<Territory> is supposed to contain tehe pointer to  mapGraph[0][0], which is Canada.
	vector<Continent*> continentGraph;

	//Helper function: help to create  continents and allocate countries into the corresponding continents
	bool addContinent(Continent* c);

	//Helper function: help to iterate nodes of a graph in the depth-first principle
	void dfs(int i, vector<bool> visited);

	//Helper function: help to check if the graph is connected
	bool is_connected();

	// Helper function: help to obtain the index of a country given its ID
	int getCountryIndex(int ID);

	//Helper function: help to obtain the index of a coun
public:
	//Constructors
	Map();
	Map(const vector<vector<Territory*>> countryList);
	
	//Other class functions
	void display();  

	bool addCountry(Territory* t);
	bool releaseMap();
	bool validate();
};
