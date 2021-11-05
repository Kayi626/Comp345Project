#include "Map.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

#pragma region MapLoader
// read through provided address and collect corresponding data
void MapLoader::mapReader(const string& mapName) {
  std::cout << endl << "---Loading Map File---" << endl << endl;
  mapPath = mapName;
  int lineFlag;
  string line;
  ifstream mapFile;
  mapFile.open(mapName.c_str(), ios::in);

  if (!mapFile.is_open()) {
    std::cout << "Invalid MapFile: " << mapPath << " can not find!" << endl;
    exit(0);
  } else if (mapFile.peek() == EOF) {
    std::cout << "Invalid MapFile: there is nothing to read within " << mapPath
         << endl;
    exit(0);
  }

  while (getline(mapFile, line) && !mapFile.eof()) {
    if (line.empty() || line.at(0) == ';') {
      lineFlag = 0;
      continue;
    } else if (line.find("[files]") != string::npos) {
      lineFlag = 1;
      continue;
    } else if (line.find("[continents]") != string::npos) {
      lineFlag = 2;
      continue;
    } else if (line.find("[countries]") != string::npos) {
      lineFlag = 3;
      continue;
    } else if (line.find("[borders]") != string::npos) {
      lineFlag = 4;
      continue;
    }

    // 1 for file, 2 for continents, 3 for countries
    switch (lineFlag) {
      case 1:
        file.push_back(line);
        break;
      case 2:
        continents.push_back(line);
        break;
      case 3:
        countries.push_back(line);
        break;
      case 4:
        borders.push_back(line);
        break;
      default:
        break;
    }
  }
  mapFile.close();

  // split each line into the 2d vector
  for (const string& element : file) {
    vector<string> borderLine = split(element, ' ');
    fileVec.push_back(borderLine);
  }
  for (const string& element : countries) {
    vector<string> ctrLine = split(element, ' ');
    ctrVec.push_back(ctrLine);
  }
  for (const string& element : continents) {
    vector<string> ctrLine = split(element, ' ');
    ctiVec.push_back(ctrLine);
  }
  for (const string& element : borders) {
    vector<string> borderLine = split(element, ' ');
    borderVec.push_back(borderLine);
  }

  std::cout << endl << "---Map File Loaded---" << endl << endl;
}

// print loaded data
void MapLoader::toString() {
  // test result
  std::cout << endl << "borders" << endl;
  for (const string& element : borders) {
    std::cout << element << endl;
  }
  std::cout << endl << "file" << endl;
  for (const string& element : file) {
    std::cout << element << endl;
  }
  std::cout << endl << "continents" << endl;
  for (const string& element : continents) {
    std::cout << element << endl;
  }
  std::cout << endl << "countries" << endl;
  for (const string& element : countries) {
    std::cout << element << endl;
  }
}

// A method to split string line by provided charater
vector<string>& MapLoader::split(const string& s, char delim,
                                 vector<string>& elements) {
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    if (item.length() > 0) {
      elements.push_back(item);
    }
  }
  return elements;
}
vector<string> MapLoader::split(const string& text, char delim) {
  vector<string> elements;
  split(text, delim, elements);
  return elements;
}

// verify imported data
void MapLoader::validate() {
  bool hasNoBorder = false;
  vector<string> ctrName;
  vector<string> ctrNumber;
  vector<string> brdNumber;

  // push required data into the checker vector
  for (int i = 0; i < ctrVec.size(); i++) {
    ctrName.push_back(ctrVec[i][1]);
    ctrNumber.push_back(ctrVec[i][0]);
  }

  for (int i = 0; i < borderVec.size(); i++) {
    brdNumber.push_back(borderVec[i][0]);
    // border check has already done here;
    if (borderVec[i].size() == 1) {
      hasNoBorder = true;
    }
  }

  // 1. what if there are two or more country has same name?
  if (!checkUnique(ctrName)) {
    cerr << "Invalid MapFile: the [country] data within " << mapPath
         << " has two or more identical country name" << endl;
  }

  // 2. what if there are two or more same country number?
  if (!checkUnique(ctrNumber)) {
    cerr << "Invalid MapFile: the [country] data within " << mapPath
         << " has two or more identical country numbers" << endl;
  }

  if (!checkUnique(brdNumber)) {
    cerr << "Invalid MapFile: the [border] data within " << mapPath
         << " has two or more identical country numbers" << endl;
  }

  // 3. what if there is a country has no border?
  if (hasNoBorder) {
    cerr << "Invalid MapFile: the [border] data within " << mapPath
         << " has no border" << endl;
  }
}

// check if there is a data among vector is not unique
bool MapLoader::checkUnique(vector<string> vec) {
  sort(vec.begin(), vec.end());
  auto it = unique(vec.begin(), vec.end());
  bool wasUnique = (it == vec.end());
  return wasUnique;
}

// Accessors
vector<vector<string>> MapLoader::getFileVec() { return fileVec; }
vector<vector<string>> MapLoader::getCtrVec() { return ctrVec; }
vector<vector<string>> MapLoader::getCtiVec() { return ctiVec; }
vector<vector<string>> MapLoader::getBorderVec() { return borderVec; }

// Mutators
MapLoader::MapLoader() = default;

// check whether a data is int or string
bool MapLoader::isStrInt(string str) {
  regex reg("^[0-9]+$");
  return regex_match(str, reg);
}
#pragma endregion MapLoader
#pragma region Continent
Continent::Continent(int continentID, string continentName, int bonus,
                     string color) {
  Continent::continentID = continentID;
  Continent::continentName = continentName;
  Continent::continentColor = color;
  Continent::continentBonus = bonus;
}

Continent::Continent() {
  continentID = -1;
  continentName = "";
  string continentColor = "";
  int continentBonus = 0;
  vector<Territory*> countryInside(10);
}

Continent::Continent(const Continent& conti) {
  this->continentName = conti.continentName;
  this->continentColor = conti.continentColor;
  this->continentID = conti.continentID;
  vector<Territory*> tempInside = conti.countryInside;
  // creat deep copy of countryInside
  for (int x = 0; x < static_cast<int>(conti.countryInside.size()); x++) {
    Territory* temp = tempInside[x];
    this->countryInside.push_back(temp);
  }
}

//Assignment Operator
Continent& Continent::operator=(const Continent& t) {
    if (this == &t) {
        return *this;
    }
    this->continentName = t.continentName.c_str();
    this->continentColor = t.continentColor;
    this->continentID = t.continentID;
    vector<Territory*> tempInside = t.countryInside;
    // creat deep copy of countryInside
    for (int x = 0; x < static_cast<int>(tempInside.size()); x++) {
        Territory* temp = tempInside[x];
        this->countryInside.push_back(temp);
    }
}
//Stream Insertion Operator
ostream& operator<<(ostream& ost, const Continent& t) {
    ost << "[Continent ID]: " << t.continentID << "  [Continent Name]: " << t.continentName << "  [Continent Color]: " << t.continentColor << "  [Continent Bonus]: " << t.continentBonus << endl;
    return ost;
}

//Destructor
Continent::~Continent() {

}

void Continent::display() {
  std::cout << endl
       << "Continent " << continentID << ": " << continentName << endl
       << "Has Countries: ";
  if (countryInside.size() > 0) {
    for (int x = 0; x < countryInside.size(); x++) {
      Territory temp = *(countryInside[x]);
      std::cout << "|" << temp.getName() << "|  ";
    }
  } else
    std::cout << "It contains no countries.";
  std::cout << endl << endl;
}
int Continent::getID() { return Continent::continentID; }
string Continent::getName() { return Continent::continentName; }
vector<Territory*>& Continent::getCountryInside() {
  return Continent::countryInside;
}

string Continent::getColor() { return continentColor; }
int Continent::getBonus() { return continentBonus; }

#pragma endregion Continent
#pragma region Territory

using namespace std;

// Constructors
Territory::Territory(string countryName, int countryID,
                     int belongToWhichContinent, int x, int y) {
  Territory::ctrName = countryName;
  Territory::armyNumber = 0;
  Territory::ctrID = (countryID < 0) ? abs(countryID) : countryID;
  Territory::contiBelong = (belongToWhichContinent < 0)
                               ? abs(belongToWhichContinent)
                               : belongToWhichContinent;
  Territory::ctrAxisX = (x < 0) ? abs(x) : x;
  Territory::ctrAxisY = (y < 0) ? abs(y) : y;

  controlledPlayerID = -1;
}
Territory::Territory() {
  ctrAxisX = 0;
  ctrAxisY = 0;
  ctrID = -1;
  ctrName = "";
  controlledPlayerID = -1;
}

Territory::Territory(const Territory& t) {
  this->ctrID = t.ctrID;
  this->armyNumber = t.armyNumber;
  this->contiBelong = t.contiBelong;
  this->ctrAxisX = t.ctrAxisX;
  this->ctrAxisY = t.ctrAxisY;
  this->ctrName = t.ctrName.c_str();
  this->controlledPlayerID = t.controlledPlayerID;
}

//Assignment Operator
Territory& Territory::operator=(const Territory& t) {
    if (this == &t) {
        return *this;
    }
    this->ctrID = t.ctrID;
    this->armyNumber = t.armyNumber;
    this->contiBelong = t.contiBelong;
    this->ctrAxisX = t.ctrAxisX;
    this->ctrAxisY = t.ctrAxisY;
    this->ctrName = t.ctrName.c_str();
    this->controlledPlayerID = t.controlledPlayerID;
    
    return *this;

}
//Stream Insertion Operator
ostream& operator<<(ostream& ost, const Territory& t) {
    ost << "[Territory ID]: " << t.ctrID << "  [Territory Name]: " << t.ctrName << "  [Territory BelongedContiID]: " << t.contiBelong << "  [Territory ArmyNumber]: " << t.armyNumber << "  [Territory X-Axis]: " << t.ctrAxisX << " [Territory Y-Axis]: " << t.ctrAxisY << "  [Territory ControledPlayer ID]: " << t.controlledPlayerID << endl;
    return ost;
}

//Destructor
Territory::~Territory() {

}

// Accessors
string Territory::getName() { return Territory::ctrName; }
int Territory::getArmyNumber() { return Territory::armyNumber; }

int Territory::getCountryID() { return Territory::ctrID; }

int Territory::getBelongedContinentID() { return Territory::contiBelong; }

int Territory::getcontrolledPlayerID() { return Territory::controlledPlayerID; }

int Territory::getCtrAxisX() { return Territory::ctrAxisX; }
int Territory::getCtrAxisY() { return Territory::ctrAxisY; }

// Mutators
void Territory::setName(string countryName) {
  Territory::ctrName = countryName;
}

void Territory::setArmyNumber(int armyNumber) {
  Territory::armyNumber = armyNumber;
}
void Territory::setControlledPlayerID(int newPlayerID) {
  Territory::controlledPlayerID = newPlayerID;
}
/*
void Territory::setCountryID(int countryID) {

        Territory::ctrID = countryID;
}
*/

void Territory::setBelongedContinentID(int belongToWhichContinent) {
  Territory::contiBelong = belongToWhichContinent;
}

void Territory::setCtrAxisX(int x) { Territory::ctrAxisX = x; };
void Territory::setCtrAxisY(int y) { Territory::ctrAxisY = y; }

#pragma endregion Territory
#pragma region Map
// Constructors
Map::Map() {
  // A list of vectors of countries; each vector is an adjency list of the
  // vector's first element.
  vector<vector<Territory*>> mapGraph(10);

  // A list of vectors of countries,serving as subgraphs of the mapGrph,
  // represents all continents. Each element' reference links to the reference of
  // the corresponding element. For example, mapGrph[0][0] is Canada.
  // ContinentGraph[0] is North America. Its vector<Territory> is supposed to
  // contain tehe pointer to  mapGraph[0][0], which is Canada.
  vector<Continent*> continentGraph(5);
}

// Copy constructor
Map::Map(const Map& map) {
  // Copy mapgraph
  for (int x = 0; x < static_cast<int>(map.mapGraph.size()); x++) {
    for (int y = 0; y < map.mapGraph[x].size(); y++) {
      Territory* temp = map.mapGraph[x][y];
      this->mapGraph[x].push_back(temp);
    }
  }

  // Copy continentgraph
  for (int x = 0; x < static_cast<int>(map.continentGraph.size()); x++) {
    Continent* tempC = map.continentGraph[x];
    this->continentGraph.push_back(tempC);
  }
}

//Assignment Operator
Map& Map::operator=(const Map& map) {
    if (this == &map) {
        return *this;
    }
        // Copy mapgraph
    for(int x = 0; x < static_cast<int>(map.mapGraph.size()); x++) {
        for (int y = 0; y < map.mapGraph[x].size(); y++) {
                Territory* temp = map.mapGraph[x][y];
                this->mapGraph[x].push_back(temp);
         }
     }

    // Copy continentgraph
    for (int x = 0; x < static_cast<int>(map.continentGraph.size()); x++) {
        Continent* tempC = map.continentGraph[x];
        this->continentGraph.push_back(tempC);
    }

    return *this;

}

//Stream Insertion Operator
ostream& operator<<(std::ostream& ost, const Map& map) {
    ost << endl << "---Displaying all countries and their adjacent countries---" << endl;
    map.displayLink(ost);
    ost << endl << "---Displaying all continent---" << endl;
    map.displayAllContinents(ost);
    ost << endl << "---End of Display---" << endl;
    return ost;
}

//Destructor
Map::~Map() {
}

// Private helper function
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
// Helper function: help to iterate nodes of a graph in the depth-first
// principle
void Map::dfs(int i, vector<bool>& visited) {
  visited[i] = true;
  for (int x = 1; x < mapGraph[i].size(); x++) {
    Territory temp = *(mapGraph[i][x]);
    // Obtain ID's of countries that are linked to maGraph[i][0]
    int tempID = temp.getCountryID();
    int index;
    if (((index = getCountryIndex(tempID)) != -1) && (visited[index] != true)) {
      dfs(index, visited);
    } else {
      continue;
    }
  }
}

// Helper function: help to find the connectivity of continents
void Map::ccs(vector<bool>& visited) {
  for (int x = 0; x < mapGraph.size(); x++) {
    // Obtain ID's of countries that are linked to maGraph[i][0]
    Territory comparedC = *(mapGraph[x][0]);
    unsigned int tempID1 = comparedC.getBelongedContinentID();

    for (int y = 1; y < mapGraph[x].size(); y++) {
      Territory temp = *(mapGraph[x][y]);
      unsigned int tempID2 = temp.getBelongedContinentID();
      // std::cout << "Index: " << ;
      // When at least a country from another continent connects to the current
      // country, it will break the loop
      if (visited[tempID1 - 1] != true && tempID1 != tempID2) {
        visited[tempID1 - 1] = true;
        break;
      }
    }
  }
}

// Helper function: help to check if the graph is connected
bool Map::is_connected() {
  // A bool vector with the same size as that of mapGraph
  int size = static_cast<int>(mapGraph.size());
  vector<bool> visited(size);
  dfs(0, visited);

  // It only returns true if all boolean values of "visited" are true
  for (int x = 0; x < visited.size(); x++) {
    if (!visited[x]) return false;
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
  // if each country of continentGraph is included in mapGraph, it will be
  // connected(checked by is_connected() function). Otherwise, it is not
  // connected.
  for (int x = 0; x < continentGraph.size(); x++) {
    Continent temp = *(continentGraph[x]);
    vector<Territory*> tempCountryInside = temp.getCountryInside();
    for (int y = 0; y < tempCountryInside.size(); y++) {
      is_subgraphs = is_subgraphs && countryMatched(tempCountryInside[y]);
    }
  }

  // If each continent is connected to at least one continent, boolean value
  // is_connected will be true.
  vector<bool> visited(static_cast<int>(continentGraph.size()));
  ccs(visited);
  // It only returns true if all boolean values of "visited" are true
  for (int x = 0; x < visited.size(); x++) {
    if (!visited[x]) {
      is_connected = false;
      break;
    }
  }

  return is_subgraphs && is_connected;
}

// Helper function: help to check whether each country belongs to one continent
// and only one continent
bool Map::belongTo_OneContinent() {
  bool belongTo = true;
  for (int x = 0; x < mapGraph.size(); x++) {
    Territory temp = *(mapGraph[x][0]);
    int tempID = temp.getBelongedContinentID();
    // A false value will fail the checking.
    belongTo = belongTo && continentMatched(tempID);
  }
  return belongTo;
}

// Helper function: help to check whether the given belongedContinentID mathces
// any of continents of continentGraph
bool Map::continentMatched(int continentID) {
  bool matched = false;
  if (continentID < 0) {
    return matched;
  }
  // if no ID mathced for the given one, it will return false(It means that the
  // country does not belong to any existing continent)
  for (int x = 0; x < continentGraph.size(); x++) {
    Continent temp = *(continentGraph[x]);
    int tempID = temp.getID();
    matched = matched || (continentID == tempID);
  }
  return matched;
}

// Helper function: help to find the index of the continent with the same ID as
// that of the given ID
int Map::continentMatched2(int continentID) {
  int index = -1;
  if (continentID < 0) {
    return index;
  }
  // if no ID mathced for the given one, it will return false(It means that the
  // country does not belong to any existing continent)
  for (int x = 0; x < continentGraph.size(); x++) {
    Continent temp = *(continentGraph[x]);
    int tempID = temp.getID();
    if (tempID == continentID) index = x;
  }
  return index;
}

// Accessors
vector<Continent*> Map::getContinentGraph() { return Map::continentGraph; }
vector<vector<Territory*>> Map::getMapGraph() { return Map::mapGraph; }

// Other class functions
void Map::displayLink(ostream& ost) const{
  int count = 1;
  // Display all countries and their adjacent countries
  for (vector<Territory*> v : mapGraph) {
    for (int x = 0; x < v.size(); x++) {
      Territory temp = *(v[x]);
      if (x == 0)
        ost << "Country " << count << ": [" << temp.getName()
             << "] has adjacent countries: " << endl;
      else if (x == v.size() - 1)
        ost << temp.getName() << endl;
      else
        ost << temp.getName() << ",  ";
    }
    count++;
    ost << endl;
  }
}

void Map::displayAllContinents(ostream& ost) const {
  for (Continent* c : continentGraph) {
    c->display();
  }
}



// Validate whether the map is a connected graph/continents are connected
// subgraphs/each country belongs to 1 and only 1 continent
bool Map::validate() {
  // First to check the entire graph is connected. Second to check if continents
  // are subgraphs(if they are subgraphs, they are connected as well). Third to
  // check if each country belongs to 1 continent
  bool stageCheck1 = is_connected();
  bool stageCheck2 = is_subgraphs();
  bool stageCheck3 = belongTo_OneContinent();

  // Dispay status info for each checkpoint
  std::cout << "--------------Validating-------------------------" << endl << endl;
  std::cout << "Map Graph is Connected: "
       << static_cast<string>(((stageCheck1) ? "[true]" : "[false]")) << endl;
  std::cout << "Continents are connected subrgaphs: "
       << static_cast<string>(((stageCheck2 && stageCheck1) ? "[true]" : "[false]")) << endl;
  std::cout << "Each country has oen continent: "
       << static_cast<string>(((stageCheck3) ? "[true]" : "[false]")) << endl;
  return stageCheck1 && stageCheck2 && stageCheck3;
}


bool Map::addCountry(Territory* t) {
  vector<Territory*> newAdjacencyList;
  int currentSize = static_cast<int>(mapGraph.size());

  // If country ID / continent ID is -1(uninitialized),no corresponding
  // continent ID, and duplicate country ID's, the country will not be added
  if ((t->getCountryID() == -1) || (getCountryIndex(t->getCountryID()) != -1) ||
      !(continentMatched(t->getBelongedContinentID()))) {
    return false;
  }

  // Add the argument territory into the vector<Territory>
  try {
    // Add a new vector<Territory> into mapGraph and its index is currentSize
    mapGraph.push_back(newAdjacencyList);
    mapGraph[currentSize].push_back(t);

    Territory temp = *(t);
    int tempID = temp.getBelongedContinentID();

    // To allocate the given country to the corresponding continent
    continentGraph[continentMatched2(tempID)]->getCountryInside().push_back(t);
    return true;
  } catch (exception& e) {
    std::cout << "Failed to add a country into the map! " << e.what() << endl;
    return false;
  }
}
bool Map::addContinent(Continent* conti1) {
  // If there are duplicate continent ID's, the continent wont be added
  if (continentMatched(conti1->getID())) {
    return false;
  }
  try {
    // Add a continent into continentGraph
    for (Continent* conti2 : continentGraph) {
      Continent temp1 = *(conti1);
      Continent temp2 = *(conti2);
      // The given continent already exists. It wont be added any more
      if (temp1.getID() == temp2.getID()) {
        return false;
      }
    }
    continentGraph.push_back(conti1);
    return true;
  } catch (exception& e) {
    std::cout << "Failed to add a continent into the map" << e.what() << endl;
    return false;
  }
}
// Add an edge for two ajacent countries
bool Map::addEdge(int id1, int id2) {
  int index1, index2;

  // If either of the ID arguments is invalid, it will return false.
  if ((index1 = getCountryIndex(id1)) == -1 ||
      (index2 = getCountryIndex(id2)) == -1) {
    return false;
  }
  mapGraph[index1].push_back(mapGraph[index2][0]);
  return true;
}

// Release memory of a map object
bool Map::releaseMap() {
  try {
    // Release the memory of the mapGraph
    for (int x = 0; x < mapGraph.size(); x++) {
      for (int y = 0; y < mapGraph[x].size(); y++) {
        delete mapGraph[x][y];
      }
    }

    // Release the memory of the continentGraph
    for (int x = 0; x < continentGraph.size(); x++) {
      Continent* tp = continentGraph[x];
      Continent t = *(tp);

      vector<Territory*> temp = t.getCountryInside();
      for (int y = 0; y < temp.size(); y++) {
        delete temp[y];
      }

      temp.clear();

      delete tp;
      tp = nullptr;
    }

    // Clear two vector variables
    mapGraph.clear();
    continentGraph.clear();

    return true;
  } catch (exception& e) {
    std::cout << e.what() << endl;
    return false;
  }
}

Map* Map::mapCreater(string mapPath) {
  bool edgeAdded = true;
  auto* mb_loader = new MapLoader();
  mb_loader->mapReader(mapPath);
  mb_loader->validate();

  // Create a map object whose only two private variables are mapGraph and
  // continentGraph
  Map* newMap = new Map();
  vector<vector<string>> temp1 = mb_loader->getCtiVec();
  vector<vector<string>> temp2 = mb_loader->getCtrVec();
  vector<vector<string>> temp3 = mb_loader->getBorderVec();
  vector<vector<string>> temp4 = mb_loader->getFileVec();

  // Fill the continentGraph (to build continents)
  for (int x = 0; x < temp1.size(); x++) {
    // check data type correct;
    if (MapLoader::isStrInt(temp1[x][0]) || !MapLoader::isStrInt(temp1[x][1])) {
      cerr << "Warning: some elements within Continent data has wrong type!"
           << endl;
      delete newMap;
      newMap = NULL;
      exit(0);
    }
    Continent* temp =
        new Continent((x + 1), temp1[x][0], stoi(temp1[x][1]), temp1[x][2]);
    newMap->addContinent(temp);
  }

  // Fill the mapGraph (to build countries and allocate them to the
  // corresponding continents)
  for (int x = 0; x < temp2.size(); x++) {
    // check data type correct;
    if (!MapLoader::isStrInt(temp2[x][0]) || MapLoader::isStrInt(temp2[x][1]) ||
        !MapLoader::isStrInt(temp2[x][2]) ||
        !MapLoader::isStrInt(temp2[x][3]) ||
        !MapLoader::isStrInt(temp2[x][4])) {
      cerr << "Warning: some elements within country data has wrong type!"
           << endl;
      delete newMap;
      newMap = NULL;
      exit(0);
    }
    Territory* temp =
        new Territory(temp2[x][1], stoi(temp2[x][0]), stoi(temp2[x][2]),
                      stoi(temp2[x][3]), stoi(temp2[x][4]));
    if (newMap->addCountry(temp))
      std::cout << temp2[x][1] << " added " << endl;
    else
      std::cout << temp2[x][1]
           << " NOT added, It might be because of duplicate territory ID's, "
              "non-exsiting continent ID, and missing territory ID's"
           << endl;
  }

  // Fill the adjacency list
  for (int x = 0; x < temp3.size(); x++) {
    // check data type correct;
    if (!MapLoader::isStrInt(temp3[x][0])) {
      cerr << "Warning: some elements within border data has wrong type!"
           << endl;
      delete newMap;
      newMap = NULL;
      exit(0);
    }
    // the ID of the target country
    int temp = stoi(temp3[x][0]);

    for (int y = 1; y < temp3[x].size(); y++) {
      // check data type correct;
      if (!MapLoader::isStrInt(temp3[x][y])) {
        cerr << "Warning: some elements within border data has wrong type!"
             << endl;
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
    std::cout << endl << "---Edge added---" << endl << endl;
  } else {
    cerr << endl << "Failed to add the edge" << endl << endl;
    delete newMap;
    newMap = NULL;
    exit(0);
  }
  return newMap;
}
#pragma endregion Map