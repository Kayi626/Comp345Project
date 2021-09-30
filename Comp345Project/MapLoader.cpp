//
// Created by Sulyvahn on 2021/9/28.
//
#include "MapLoader.h"

void MapLoader::mapReader(const string& mapName) {
  cout << "---Loading Map---" << endl;
  mapPath = mapName;
  int lineFlag;
  string line;
  ifstream mapFile;
  mapFile.open(mapName.c_str());

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

  cout << "---Map Loaded---" << endl;
}

void MapLoader::toString() {
  // test result
  cout << endl << "borders" << endl;
  for (const string& element : borders) {
    cout << element << endl;
  }
  cout << endl << "file" << endl;
  for (const string& element : file) {
    cout << element << endl;
  }
  cout << endl << "continents" << endl;
  for (const string& element : continents) {
    cout << element << endl;
  }
  cout << endl << "countries" << endl;
  for (const string& element : countries) {
    cout << element << endl;
  }
}

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

bool MapLoader::checkUnique(vector<string> vec) {
  sort(vec.begin(), vec.end());
  auto it = unique(vec.begin(), vec.end());
  bool wasUnique = (it == vec.end());
  return wasUnique;
}

MapLoader::MapLoader() = default;
