#include "Continent.h"
#include "Map.h"
#include "Territory.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include <vector>
#include "MapLoader.h"

int main() {
  auto* mb_loader = new MapLoader();
  // mb_loader->mapReader("bigeurope.map");
  mb_loader->mapReader("LOTR2.map");
  mb_loader->toString();
  mb_loader->validate();
  return 0;
}