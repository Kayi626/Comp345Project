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

using namespace std;

string extractLineArgumentCommand(string str);

int main(int argc, char* argv[]) {
    std::ofstream clearText("gamelog.txt", std::ofstream::trunc);
    clearText.close();
    FileLineReader* flr = new FileLineReader("commands.txt");
    FileCommandProcessorAdapter* fcomP = new FileCommandProcessorAdapter(flr);
    GameEngine* ge = new GameEngine(fcomP, 6, true);
    //gameEngine的构造器参数：  FileLineReader【推荐FileCommandProcessorAdapter，可以直接用文本文档】

    //Game Loop (Important: If any invalid commands are received by the loop, it will prompt users to enter inputs by console)
    ge->startup();

    //De-allocation
    delete ge;
    delete fcomP;
    delete flr;
    ge = nullptr;
    fcomP = nullptr;
    flr = nullptr;
    return 0;
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


