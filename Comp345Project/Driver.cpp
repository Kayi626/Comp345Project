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

    //Driver: for part 1
    //COMP345:Commands for A3 demo, part1
    //
    //Dear professor, there's some command prepared for A3 demo. I will copy&paste theses command during the demo, testing out the corresponding grading requirements.
    //
    //1.2.1 Human player : requires user interactions to make decisions
    //1.2.2 Aggressive player : computer player that focuses on attack(deploys or advances armies on its strongest country, then always advances to enemy territories until it cannot do so anymore).
    //1.2.3 Benevolent player : computer player that focuses on protecting its weak countries(deploys or advances armies on its weakest countries, never advances to enemy territories).
    //1.2.11 Driver that demonstrates that different players can be assigned different strategies that lead to different behavior using the Strategy design pattern.
    //1.2.13Driver that demonstrates that the human player makes decisions according to user interaction, and computer players make decisions automatically, which are both implemented using the strategy pattern
    //
    //tournament - M 1 - P A B H - G 1 - D 10
    //
    //
    //1.2.4 Neutral player : computer player that never issues any order.
    //1.2.6 If a Neutral player is attacked, it becomes an Aggressive player.
    //1.2.12 Driver that demonstrates that the strategy adopted by a player can be changed dynamically during play.
    //
    //tournament - M 2 - P N H - G 1 - D 10
    //
    //
    //1.2.5 Cheater player : computer player that automatically conquers all territories that are adjacent to its own territories(only once per turn).
    //
    //tournament - M 2 - P C H - G 1 - D 10


    std::ofstream clearText("gamelog.txt", std::ofstream::trunc);
    clearText.close();
    GameEngine::fileLineReaderFilePath = "commands_p2_t1.txt";
    GameEngine::defualtTerritoriesAmount = 2;
    GameEngine::isDebugMode = true;
    GameEngine::instance()->startup();
    return 0;


    //Driver: for part 2
    //swtich between -console and commands file. When input -file filename command, it should be "-file <filename>" 
    //Validate the tournament codes
 /*   std::ofstream clearText("gamelog.txt", std::ofstream::trunc);
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
    */
    
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


