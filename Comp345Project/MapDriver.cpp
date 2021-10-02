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

//is string or int?
bool isStrInt(string str);
Map* mapCreater(string mapPath);

int main() {
  Map* newMap = mapCreater("LOTR2.map"); // bigeurope.map, LOTR2.map
  // mb_loader->toString();
  
  //Display the data
  newMap->displayLink();
  //newMap->displayAllContinents();
  return 0;
}


bool isStrInt(string str) { 
	regex reg("^[0-9]+$"); 
    return regex_match(str, reg);
}

Map* mapCreater(string mapPath){
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
        if(isStrInt(temp1[x][0])||!isStrInt(temp1[x][1])){
            cerr << "Warning: some elements within Continent data has wrong type!" << endl;
            newMap->releaseMap();
            exit (0);
        }
        Continent* temp = new Continent((x + 1), temp1[x][0],stoi(temp1[x][1]),temp1[x][2]);
        newMap->addContinent(temp);
    }
    
    //Fill the mapGraph (to build countries and allocate them to the corresponding continents)
    for (int x = 0; x < temp2.size(); x++) {
        // check data type correct;
        if(!isStrInt(temp2[x][0])||isStrInt(temp2[x][1])||!isStrInt(temp2[x][2])||!isStrInt(temp2[x][3])||!isStrInt(temp2[x][4])){
              cerr << "Warning: some elements within country data has wrong type!" << endl;
              newMap->releaseMap();
                       exit (0);
        }
        Territory* temp = new Territory(temp2[x][1],stoi(temp2[x][0]),stoi(temp2[x][2]),stoi(temp2[x][3]),stoi(temp2[x][4]));
        if (newMap->addCountry(temp))
            cout << temp2[x][1] << " added " << endl;
        else
            cout << temp2[x][1] << " NOT added, It might be because of duplicate territory ID's, non-exsiting continent ID, and missing territory ID's" << endl;
    }
   
    //Fill the adjacency list 
    for (int x = 0; x < temp3.size(); x++) {
        // check data type correct;
        if(!isStrInt(temp3[x][0])){
           cerr << "Warning: some elements within border data has wrong type!" << endl;
           newMap->releaseMap();
           exit (0);
        }
        //the ID of the target country
        int temp = stoi(temp3[x][0]);
   
        for (int y = 1; y < temp3[x].size(); y++) {
            // check data type correct;
            if(!isStrInt(temp3[x][y])){
              cerr << "Warning: some elements within border data has wrong type!" << endl;
              newMap->releaseMap();
                       exit (0);
            }
            if (!newMap->addEdge(temp,stoi(temp3[x][y]))){
                edgeAdded = false;
            }	  		  
        }
    }
     if(edgeAdded){
        cout << endl << "---Edge added---" << endl<< endl;
     }else{
        cerr << endl << "Failed to add the edge" << endl<< endl;	
        newMap->releaseMap();
        exit (0);
     }
    return newMap;
}
