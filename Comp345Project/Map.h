#pragma once
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

class MapLoader {
 private:
  string mapPath;
  vector<string> file;
  vector<string> countries;
  vector<string> continents;
  vector<string> borders;
  vector<vector<string>> fileVec;
  vector<vector<string>> ctrVec;
  vector<vector<string>> ctiVec;
  vector<vector<string>> borderVec;

 public:
  MapLoader();

  void mapReader(const string& mapName);
  void toString();

  static vector<string>& split(const string& text, char delim,
                               vector<string>& element);
  static vector<string> split(const string& text, char delim);

  static bool checkUnique(vector<string> vec);
  void validate();

  // Accessors
  vector<vector<string>> getFileVec();
  vector<vector<string>> getCtrVec();
  vector<vector<string>> getCtiVec();
  vector<vector<string>> getBorderVec();

  // Mutators

  // Other class functions

  static bool isStrInt(string str);
};
class Territory {
 private:
  string ctrName;
  int ctrID;
  int armyNumber;
  int controlledPlayerID;
  // the ID of player who's controlling this.
  // default set to -1, which means it's a neutral territory.
  int contiBelong;
  int ctrAxisX;
  int ctrAxisY;

 public:
  // Constructors
  Territory(string countryName, int countryID, int belongToWhichContinent,
            int ctrAxisX, int ctrAxisY);
  Territory(const Territory& t);
  Territory();

  // Other functions
  // Accessors
  string getName();
  int getArmyNumber();
  int getCountryID();
  int getBelongedContinentID();
  int getcontrolledPlayerID();
  int getCtrAxisX();
  int getCtrAxisY();
  // Mutators
  void setName(string countryName);
  void setArmyNumber(int armyNumber);
  void setControlledPlayerID(int newPlayerID);
  // void setCountryID(int countryID);
  void setBelongedContinentID(int belongToWhichContinent);
  void setCtrAxisX(int x);
  void setCtrAxisY(int y);
};
class Continent {
 private:
  int continentID;
  string continentName;
  string continentColor;
  int continentBonus;
  vector<Territory*> countryInside;

 public:
  // Constructors
  Continent(int continentID, string continentName, int bonus, string color);
  Continent();
  Continent(const Continent& conti);

  // Other class functions
  void display();
  int getID();
  string getName();
  string getColor();
  int getBonus();
  vector<Territory*>& getCountryInside();
};
class Map {
 private:
  // A list of vectors of countries; each vector is an adjency list of the
  // vector's first element.
  vector<vector<Territory*>> mapGraph;

  // A list of vectors of countries,serving as subgraphs of the mapGrph,
  // represents all continents. Each element' reference links to the reference of
  // the corresponding element. For example, mapGrph[0][0] is Canada.
  // ContinentGraph[0] is North America. Its vector<Territory> is supposed to
  // contain tehe pointer to  mapGraph[0][0], which is Canada.
  vector<Continent*> continentGraph;

  // Helper function: help to iterate nodes of a graph in the depth-first
  // principle
  void dfs(int i, vector<bool>& visited);

  // Helper function: help to find the connectivity of continents
  void ccs(vector<bool>& visited);

  // Helper function: help to check if the graph is connected
  bool is_connected();

  // Helper function: help to obtain the index of a country given its ID
  int getCountryIndex(int ID);

  // Helper function: help to check whether each country of the continentGraph
  // is included in mapGraph. If so, they for sure are connected
  bool is_subgraphs();

  // Helper function: help to check whether a given territory has the same
  // address as any of territories of the mapGraph
  bool countryMatched(Territory* territory);

  // Helper function: help to check whether each country belongs to one
  // continent
  bool belongTo_OneContinent();

  // Helper function: help to check whether the given belongedContinentID
  // mathces any of continents of continentGraph
  bool continentMatched(int continentID);

  // Helper function: help to find the index of the continent with the same ID
  // as that of the given ID
  int continentMatched2(int continentID);

 public:
  // Constructors
  Map();
  Map(const Map& map);

  // Accessor
  vector<Continent*> getContinentGraph();
  vector<vector<Territory*>> getMapGraph();
  // Mutator

  // Other class functions
  void displayLink();
  void displayAllContinents();
  bool addCountry(Territory* t);
  bool addContinent(Continent* c);
  bool addEdge(int id1, int id2);
  bool releaseMap();
  bool validate();
  static Map* mapCreater(string s);
};