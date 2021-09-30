#include <iostream>
#include <cstring>
#include <vector>
#include "Map.h"
#include "Territory.h"

using namespace std;

//Constructors
Map::Map() {

}

//Copy constructor
Map::Map(const vector<vector<Territory*>> mapGraph) {
	
}

//Private helper function
// Helper function:: help to obtain the index of a country given its ID
int Map::getCountryIndex(int ID) {
	int index = -1;
	for (int x = 0; x < mapGraph.size(); x++) {
		Territory temp = *(mapGraph[x][0]);
		if (temp.getCountryID() == ID) {
			index = x;
			break;
	   }
	}
	return index;
}
//Helper function: help to iterate nodes of a graph in the depth-first principle
void Map::dfs(int i, vector<bool> visited) {
	visited[i] = true;
	for (Territory* elt : mapGraph[i]) {
		Territory temp = *(elt);
		//Obtain ID's of countries that are linked to maGraph[i][0]
		int tempID = temp.getCountryID();
		int index;
		if ((index=getCountryIndex(tempID)) != -1) {
			dfs(index,visited);
		}
		else {
			//something that doesn't belong to the country list appears,so it causes errors
			visited[i] = false;
			break;
		}
	}
}

//Helper function: help to check if the graph is connected
bool Map::is_connected() {

    //A bool vector with the same size as that of mapGraph
	int size = mapGraph.size();
	vector<bool> visited(size);
	dfs(0, visited);

	//It only returns true if all boolean values of "visited" are true
	for (int x = 0; x < visited.size(); x++) {
		if (!visited[x])
			return false;
	}
	return true;
}


//Other class functions
void Map::display() {
	int count = 1;
	//Display all countries and their adjacent countries
	for (vector<Territory*> v : mapGraph) {
		for (int x = 0; x < v.size()-1;x++) {
			Territory temp = *(v[x]);
			if (x == 0)
				cout << "Country No." << count << ": " << temp.getName() << endl << "Its adjacent countries includes: ";
			else
				cout << temp.getName()<<"  ";
		}
	    cout << endl;
	}
}

//Validate whether the map is a connected graph/continents are connected subgraphs/each country belongs to 1 and only 1 continent
bool Map::validate() {
	return 1;
}



bool Map::addCountry(Territory* t) {

	vector<Territory*> newAdjacencyList;
	int currentSize = mapGraph.size();

    //Add a new vector<Territory> into mapGraph and its index is currentSize
	mapGraph.push_back(newAdjacencyList);
    
	//Add the argument territory into the vector<Territory>
	try {
		mapGraph[currentSize].push_back(t);
		return true;
	}
	catch (exception& e) {

		cout << "Failed to add a country into the map! "<<e.what()<< endl;
		return false;
	}

}
bool Map::addContinent(Continent* c) {
	try {
		//Add a continent into continentGraph
		for (Continent* c : continentGraph) {
			Continent temp = *(c);
			if (temp.getName().compare(temp.getName()) == 0) {
				return true;
			}
		}
		continentGraph.push_back(c);
		return true;
	}
	catch (exception& e) {
		cout << e.what() << endl;

	}
}
bool Map::releaseMap() {
	try {
		//Release the memory of the mapGraph
		for (int x = 0; x < mapGraph.size(); x++) {
			for (int y = 0; y < mapGraph[x].size(); y++) {
				delete mapGraph[x][y];
				mapGraph[x][y] = NULL;
			}
		}
		//Release the memory of the continentGraph
		for (int x = 0; x < continentGraph.size(); x++) {
			Continent t = *(continentGraph[x]);
			vector<Territory*> temp = t.getCountryInside();
			for (int y = 0; y < temp.size(); y++) {
				delete temp[y];
				temp[x] = NULL;
			}
		}
		return true;
	}
	catch (exception& e) {
		cout << e.what() << endl;
		return false;	
	}
}