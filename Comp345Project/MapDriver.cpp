#include "Continent.h"
#include "Map.h"
#include "Territory.h"
#include "GameEngine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "Map.h"
using namespace std;


int main() {
 /*Part1 Map Class
  Map* newMap = Map::mapCreater("LOTR2.map"); // bigeurope.map, LOTR2.map
  // mb_loader->toString();
  
  //Display the data
  newMap->displayLink();

  //Validate the map
  if (newMap->validate())
      cout << endl << "-------------Map is valid-------------------" << endl;
  else
      cout << endl << "-------------Map is invalid-------------------" << endl;
  //newMap->displayAllContinents();

  //Handle memory leaks
  delete newMap;
  newMap = nullptr;*/
    GameEngine* ge = new GameEngine();
    ge->gameFlow();
 
  return 0;
}


