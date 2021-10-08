#pragma once
#include <cstring>
#include <iostream>
#include <vector>

#include "Continent.h"
#include "Territory.h"

using namespace std;

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
