#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <cstring>
#include "Cards.h"
#include "Orders.h"
#include "Player.h"
#include "Map.h"
#include "GameEngine.h"
#include "CommandProcessing.h"
#include "PlayerStrategies.h"

using namespace std;

string extractLineArgumentCommand(string str);

int main(int argc, char* argv[]) {
    

    //Driver: for part 2
    //swtich between -console and commands file. When input -file filename commnad, it should be "-file <filename>" 
    //Validate the tournament codes
    std::ofstream clearText("gamelog.txt", std::ofstream::trunc);
    clearText.close();    
    std::regex reg1("-console");
    std::regex reg2("-file <(.*)>");
    if (argc == 1) {
        cout << "No Argument Command Line Received! The program exists" << endl;
        return 0;
    } 
    else if (std::regex_match(argv[1], reg1)) {
            GameEngine::defualtTerritoriesAmount = 1;
            GameEngine::useFileCommandProcessor = false;
            GameEngine::isDebugMode = true;
            GameEngine::instance()->startup();
            return 0;
    }
    else if (std::regex_match(argv[1], reg2)) {
            GameEngine::defualtTerritoriesAmount = 2;
            GameEngine::useFileCommandProcessor = true;
            GameEngine::setFilePath(extractLineArgumentCommand(argv[1]));
            GameEngine::isDebugMode = true;
            GameEngine::instance()->startup();
            return 0;
    }
    else{
        cout << "Invalid Command Line Argument! The program exists! ";
        return 0;
    }
    
    
}

//Used to extract commands from command Line Argument(Get the filename from <>)
string extractLineArgumentCommand(string str) {
    //Two patterns(addplayer + loadmap). It will extract the substring from <>.
    const std::string s = str;
    std::regex rgx("(-file\\s+<)(.*)(>)");
    std::smatch match;
    if (std::regex_search(s.begin(), s.end(), match, rgx))
        return match[2];
    else
        return "";
}


