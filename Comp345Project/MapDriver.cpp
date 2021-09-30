#include "Continent.h"
#include "Map.h"
#include "Territory.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include <vector>
#include "MapLoader.h"
#include <regex>

//is string a int?
//save it for later;;
bool isStrInt(string str) { 
	regex reg("^[0-9]+$"); 
    return regex_match(str, reg);
}

int main() {
  auto* mb_loader = new MapLoader();
  // mb_loader->mapReader("bigeurope.map");
  mb_loader->mapReader("LOTR2.map");
  // mb_loader->toString();
  mb_loader->validate();
  return 0;
}