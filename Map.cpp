#include <iostream>
#include <cstring>
#include <vector>
#include "Map.h"
#include "MapLoader.h"
#include "Territory.h"

using namespace std;

//Constructors
Map::Map() {
	//A list of vectors of countries; each vector is an adjency list of the vector's first element.
	vector<vector<Territory*>> mapGraph(10);

	//A list of vectors of countries,serving as subgraphs of the mapGrph, represents all continents. Each element' reference links to the reference of the corresponding element.
	//For example, mapGrph[0][0] is Canada. ContinentGraph[0] is North America. Its vector<Territory> is supposed to contain tehe pointer to  mapGraph[0][0], which is Canada.
	vector<Continent*> continentGraph(5);
}

//Copy constructor
Map::Map(const Map &map) {

	//Copy mapgraph
	for (int x = 0; x < static_cast<int>(map.mapGraph.size()); x++) {
		
		for (int y = 0; y < map.mapGraph[x].size(); y++) {

			Territory* temp = map.mapGraph[x][y];
			this->mapGraph[x].push_back(temp);
		}
	}

	//Copy continentgraph
	for (int x = 0; x < static_cast<int>(map.continentGraph.size()); x++) {

		Continent* tempC = map.continentGraph[x];
		this->continentGraph.push_back(tempC);
	}
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
void Map::dfs(int i, vector<bool>& visited) {

	visited[i] = true;
	for (int x = 1; x < mapGraph[i].size();x++) {
		Territory temp = *(mapGraph[i][x]);
		//Obtain ID's of countries that are linked to maGraph[i][0]
		int tempID = temp.getCountryID();	
		int index;
		if (((index=getCountryIndex(tempID)) != -1) && (visited[index] !=true)) {

			dfs(index,visited);
		}
		else {
			continue;
		}
	}
}


//Helper function: help to find the connectivity of continents
void Map::ccs(vector<bool>& visited) {
	for (int x = 0; x < mapGraph.size(); x++) {

		//Obtain ID's of countries that are linked to maGraph[i][0]
		Territory comparedC = *(mapGraph[x][0]);
		int tempID1 = comparedC.getBelongedContinentID();
	
		for (int y = 1; y < mapGraph[x].size(); y++) {

			Territory temp = *(mapGraph[x][y]);
			int tempID2 = temp.getBelongedContinentID();		
			//cout << "Index: " << ;
			//When at least a country from another continent connects to the current country, it will break the loop
			if (visited[tempID1 - 1] != true && tempID1 != tempID2) {

				visited[tempID1- 1] = true;
				break;
				
			}

		}
	}
}

//Helper function: help to check if the graph is connected
bool Map::is_connected() {

    //A bool vector with the same size as that of mapGraph
	int size = static_cast<int> (mapGraph.size());
	vector<bool> visited(size);
	dfs(0, visited);

	//It only returns true if all boolean values of "visited" are true
	for (int x = 0; x < visited.size(); x++) {
		if (!visited[x])
			return false;
	}
	return true;
}

bool Map::countryMatched(Territory* territory) {
	bool matched = false;
	for (int x = 0; x < mapGraph.size(); x++) {
		Territory* temp = mapGraph[x][0];
		matched = matched || (territory == temp);
	}
	return matched;
}

bool Map::is_subgraphs() {
	bool is_subgraphs = true;
	bool is_connected = true;
	//if each country of continentGraph is included in mapGraph, it will be connected(checked by is_connected() function). Otherwise, it is not connected.
	for (int x = 0; x < continentGraph.size(); x++) {
		Continent temp = *(continentGraph[x]);
		vector<Territory*> tempCountryInside = temp.getCountryInside();
		for (int y = 0; y < tempCountryInside.size(); y++) {
			is_subgraphs = is_subgraphs && countryMatched(tempCountryInside[y]);
		}
	}

	//If each continent is connected to at least one continent, boolean value is_connected will be true.
	vector<bool> visited (static_cast <int>(continentGraph.size()));
	ccs(visited);
	//It only returns true if all boolean values of "visited" are true
	for (int x = 0; x < visited.size(); x++) {
		if (!visited[x]) {
			is_connected = false;
			break;
		}
			
	}

	return is_subgraphs && is_connected;

}

//Helper function: help to check whether each country belongs to one continent and only one continent
bool Map::belongTo_OneContinent() {
	bool belongTo = true;
	for (int x = 0; x < mapGraph.size(); x++) {
		Territory temp = *(mapGraph[x][0]);
		int tempID = temp.getBelongedContinentID();
		//A false value will fail the checking.
		belongTo = belongTo && continentMatched(tempID);		
	}
	return belongTo;
}

//Helper function: help to check whether the given belongedContinentID mathces any of continents of continentGraph
bool Map::continentMatched(int continentID) {
	bool matched = false;
	if (continentID < 0) {
		return matched;
	}
	//if no ID mathced for the given one, it will return false(It means that the country does not belong to any existing continent)
	for (int x = 0; x < continentGraph.size(); x++) {
		Continent temp = *(continentGraph[x]);
		int tempID = temp.getID();
		matched = matched || (continentID == tempID);
	}
	return matched;
}

//Helper function: help to find the index of the continent with the same ID as that of the given ID
int Map::continentMatched2(int continentID) {
	int index = -1;
	if (continentID < 0) {
		return index;
	}
	//if no ID mathced for the given one, it will return false(It means that the country does not belong to any existing continent)
	for (int x = 0; x < continentGraph.size(); x++) {
		Continent temp = *(continentGraph[x]);
		int tempID = temp.getID();
		if (tempID == continentID)
			index = x;
	}
	return index;
}

//Accessors
vector<Continent*> Map::getContinentGraph() {
	return Map::continentGraph;
}
vector<vector<Territory*>> Map::getMapGraph() {
	return Map::mapGraph;
}




//Other class functions
void Map::displayLink() {
	int count = 1;
	//Display all countries and their adjacent countries
	for (vector<Territory*> v : mapGraph) {
		for (int x = 0; x < v.size();x++) {
			Territory temp = *(v[x]);
			if (x == 0)
				cout << "Country " << count << ": [" << temp.getName() << "] has adjacent countries: " << endl;
			else if (x == v.size() - 1)
			    cout << temp.getName() << endl;
			else
				cout << temp.getName()<<",  ";
		}
		count++;
	    cout << endl;
	}
}

void Map::displayAllContinents() {
	for (Continent* c : continentGraph) {
		c->display();
	}
}

//Validate whether the map is a connected graph/continents are connected subgraphs/each country belongs to 1 and only 1 continent
bool Map::validate() {

	//First to check the entire graph is connected. Second to check if continents are subgraphs(if they are subgraphs, they are connected as well). Third to check if each country belongs to 1 continent
	bool stageCheck1 = is_connected();
	bool stageCheck2 = is_subgraphs();
	bool stageCheck3 = belongTo_OneContinent();

	//Dispay status info for each checkpoint
	cout << "--------------Validating----------------------" << endl<<endl;
	cout << "Map Graph is Connected: " << static_cast<string>(((stageCheck1)? "[true]" : "[false]")) << endl;
	cout << "Continents are connected subrgaphs: " << static_cast<string>(((stageCheck2) ? "[true]" : "[false]")) << endl;
	cout << "Each country has oen continent: " << static_cast<string>(((stageCheck3) ? "[true]" : "[false]")) << endl;
	return stageCheck1 && stageCheck2 && stageCheck3;

}



bool Map::addCountry( Territory* t) {

	vector<Territory*> newAdjacencyList;
	int currentSize = static_cast<int>(mapGraph.size());

	//If country ID / continent ID is -1(uninitialized),no corresponding continent ID, and duplicate country ID's, the country will not be added
	if ((t->getCountryID() == -1) || (getCountryIndex(t->getCountryID()) != -1) || !(continentMatched(t->getBelongedContinentID()))) {
		return false;
	}
	
    
	//Add the argument territory into the vector<Territory>
	try {
		//Add a new vector<Territory> into mapGraph and its index is currentSize
		mapGraph.push_back(newAdjacencyList);
		mapGraph[currentSize].push_back(t);
        
		
		Territory temp = *(t);
		int tempID = temp.getBelongedContinentID();

		//To allocate the given country to the corresponding continent
		continentGraph[continentMatched2(tempID)]->getCountryInside().push_back(t);
		return true;
	}
	catch (exception& e) {

		cout << "Failed to add a country into the map! "<<e.what()<< endl;
		return false;
	}

}
bool Map::addContinent( Continent* conti1) {
	
	//If there are duplicate continent ID's, the continent wont be added
	if (continentMatched(conti1->getID())) {
		return false;
	}
	try {
		//Add a continent into continentGraph
		for (Continent* conti2 : continentGraph) {
			Continent temp1 = *(conti1);
			Continent temp2 = *(conti2);
			//The given continent already exists. It wont be added any more
			if (temp1.getID() == temp2.getID()) {
				return false;
			}
		}
		continentGraph.push_back(conti1);
		return true;
	}
	catch (exception& e) {
		cout << "Failed to add a continent into the map"<<e.what() << endl;
		return false;
	}
}
//Add an edge for two ajacent countries
bool Map::addEdge(int id1, int id2) {
	int index1, index2;

	//If either of the ID arguments is invalid, it will return false.
	if ((index1 = getCountryIndex(id1)) == -1 || (index2 = getCountryIndex(id2)) == -1) {
		return false;
	}
	mapGraph[index1].push_back(mapGraph[index2][0]);
	return true;
}

//Release memory of a map object
bool Map::releaseMap() {
	try {

		//Release the memory of the mapGraph
		for (int x = 0; x < mapGraph.size(); x++) {
			for (int y = 0; y < mapGraph[x].size(); y++) {
				delete mapGraph[x][y];
			
			}
		}

		//Release the memory of the continentGraph
		for (int x = 0; x < continentGraph.size(); x++) {
			Continent* tp = continentGraph[x];
			Continent t = *(tp);
			
			vector<Territory*> temp = t.getCountryInside();
			for (int y = 0; y < temp.size(); y++) {
				delete temp[y];
			}

			temp.clear();

			delete tp;
		    tp= nullptr;
		}

		//Clear two vector variables
		mapGraph.clear();
		continentGraph.clear();


		return true;
	}
	catch (exception& e) {
		cout << e.what() << endl;
		return false;	
	}
}


Map* Map::mapCreater(string mapPath) {
	bool edgeAdded = true;
	auto* mb_loader = new MapLoader();
	mb_loader->mapReader(mapPath);
	mb_loader->validate();

	//Create a map object whose only two private variables are mapGraph and continentGraph
	Map* newMap = new Map();
	vector<vector<string>> temp1 = mb_loader->getCtiVec();
	vector<vector<string>> temp2 = mb_loader->getCtrVec();
	vector<vector<string>> temp3 = mb_loader->getBorderVec();
	vector<vector<string>> temp4 = mb_loader->getFileVec();

	//Fill the continentGraph (to build continents)
	for (int x = 0; x < temp1.size(); x++) {
		// check data type correct;
		if (MapLoader::isStrInt(temp1[x][0]) || !MapLoader::isStrInt(temp1[x][1])) {
			cerr << "Warning: some elements within Continent data has wrong type!" << endl;
			delete newMap;
			newMap = NULL;
			exit(0);
		}
		Continent* temp = new Continent((x + 1), temp1[x][0], stoi(temp1[x][1]), temp1[x][2]);
		newMap->addContinent(temp);
	}

	//Fill the mapGraph (to build countries and allocate them to the corresponding continents)
	for (int x = 0; x < temp2.size(); x++) {
		// check data type correct;
		if (!MapLoader::isStrInt(temp2[x][0]) || MapLoader::isStrInt(temp2[x][1]) || !MapLoader::isStrInt(temp2[x][2]) || !MapLoader::isStrInt(temp2[x][3]) || !MapLoader::isStrInt(temp2[x][4])) {
			cerr << "Warning: some elements within country data has wrong type!" << endl;
			delete newMap;
			newMap = NULL;
			exit(0);
		}
		Territory* temp = new Territory(temp2[x][1], stoi(temp2[x][0]), stoi(temp2[x][2]), stoi(temp2[x][3]), stoi(temp2[x][4]));
		if (newMap->addCountry(temp))
			cout << temp2[x][1] << " added " << endl;
		else
			cout << temp2[x][1] << " NOT added, It might be because of duplicate territory ID's, non-exsiting continent ID, and missing territory ID's" << endl;
	}

	//Fill the adjacency list 
	for (int x = 0; x < temp3.size(); x++) {
		// check data type correct;
		if (!MapLoader::isStrInt(temp3[x][0])) {
			cerr << "Warning: some elements within border data has wrong type!" << endl;
			delete newMap;
			newMap = NULL;
			exit(0);
		}
		//the ID of the target country
		int temp = stoi(temp3[x][0]);

		for (int y = 1; y < temp3[x].size(); y++) {
			// check data type correct;
			if (!MapLoader::isStrInt(temp3[x][y])) {
				cerr << "Warning: some elements within border data has wrong type!" << endl;
				delete newMap;
				newMap = NULL;
				exit(0);
			}
			if (!newMap->addEdge(temp, stoi(temp3[x][y]))) {
				edgeAdded = false;
			}
		}
	}
	if (edgeAdded) {
		cout << endl << "---Edge added---" << endl << endl;
	}
	else {
		cerr << endl << "Failed to add the edge" << endl << endl;
		delete newMap;
		newMap = NULL;
		exit(0);
	}
	return newMap;
}
