
#pragma once
#include <string>
#include <vector>
using namespace std;
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

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

  void mapReader(const string &mapName);
  void toString();

  static vector<string> &split(const string &text, char delim,
                               vector<string> &element);
  static vector<string> split(const string &text, char delim);

  static bool checkUnique(vector<string> vec);
  void validate();

  //Accessors
  vector<vector<string>> getFileVec();
  vector<vector<string>> getCtrVec();
  vector<vector<string>> getCtiVec();
  vector<vector<string>> getBorderVec();
  
  //Mutators
};
