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
    //std::ofstream clearText("gamelog.txt", std::ofstream::trunc);
    //clearText.close();
    //FileLineReader* flr = new FileLineReader("commands.txt");
    //FileCommandProcessorAdapter* fcomP = new FileCommandProcessorAdapter(flr);
    //GameEngine* ge = new GameEngine(fcomP, 6, true);
    //ge->startup();
    //delete ge;
    //delete fcomP;
    //delete flr;
    //ge = nullptr;
    //fcomP = nullptr;
    //flr = nullptr;
    //return 0;


    //Driver: for part 1
    //swtich between -console and commands file. When input -file filename commnad, it should be "-file <filename>"  
    std::ofstream clearText("gamelog.txt", std::ofstream::trunc);
    clearText.close();    
    std::regex reg1("-console");
    std::regex reg2("-file <(.*)>");
    if (argc == 1) {
        cout << "No Argument Command Line Received! The program exists" << endl;
        return 0;
    } 
    else if (std::regex_match(argv[1], reg1)) {
            GameEngine::defualtTerritoriesAmount = 3;
            GameEngine::useFileCommandProcessor = false;
            GameEngine::isDebugMode = true;
            GameEngine::instance()->startup();
            return 0;
    }
    else if (std::regex_match(argv[1], reg2)) {
            GameEngine::defualtTerritoriesAmount = 3;
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
    
    /*Driver: for part 2
    * Dear professor, the "isDebugMode"can enable the debug made for testing functions and abilities.
    * when "isDebugMode" is set to true, 
    * -issuing order by player wont cost their card.
    * -the starting order of players are not random anymore, but starting from the first added player
    * "isDebugMode" is default set to be false.
    */


    /*>>>>>>>>>>>>>>>>>>>>>>testing:
    * 1.the loadmap command results in successfully loading a readable map, transitioning to the maploaded state
    * 2.the validatemap command is used to validate the map. If successful, the game transitions to the mapValidated state
    * 3.the addplayer command can be used to create new players and insert them in the game (2-6 players).
    * 4.the gamestart command results in fairly distributing the territories among all players
    * 6.the gamestart command results in giving 50 initial armies to each player
    * 7.the gamestart command results in each player to draw 2 cards each from the deck
    * 8.the gamestart command results transiting to the play phase 
    * 14-17. demonstrates the effect of commands
    */
   /* std::ofstream clearText("gamelog.txt", std::ofstream::trunc);
    clearText.close();
    GameEngine::fileLineReaderFilePath = "commands_p2_t1.txt";
    GameEngine::defualtTerritoriesAmount = 6;
    GameEngine::isDebugMode = true;
    GameEngine::instance()->startup();
    return 0;*/


    /*>>>>>>>>>>>>>>>>>>>>>>testing:
    * 5.the gamestart command results in randomly determine the order of play of the players in the game
    * 9.Invalid commands for the current state are rejected. 
    */
   /* std::ofstream clearText("gamelog.txt", std::ofstream::trunc);
    clearText.close();
    GameEngine::fileLineReaderFilePath = "commands_p2_t2.txt";
    GameEngine::defualtTerritoriesAmount = 2;
    GameEngine::isDebugMode = false;
    GameEngine::instance()->startup();
    return 0;*/




    /*Driver: for part 3
    /*>>>>>>>>>>>>>>>>>>>>>>testing:
    * 1.Players get the correct amount of reinforcement during te reinforcement phase during game play, after which the issue orders phase starts. 
    * 2.Each player's issueOrder() method is called in round-robin fachion during the issue orders phase.
    * 3.After all players have signified that they dont have any more orders to isse, the orders execution phases starts
    * 5.The game engine gets the top order from the list of each players orders list in a round-robin fashion and executes them one by one. 
    * 6.Once all orders have been executed, the game engine goes back to the reinforcement phase.
    * 12.Driver clearly demonstrates that a player receives the correct number of armies in the reinforcement phase (showing different cases) 
    * 14.Driver clearly demonstrates that a player can issue advance orders to either defend or attack, based on the toAttack() and toDefend() lists 
    * 
    * Since the 'debug mode' is on, it wont requries there to be a card to issue the order.
    * I will demostrate the function of cards system in the next example
    */
 /*  std::ofstream clearText("gamelog.txt", std::ofstream::trunc);
    clearText.close();
    GameEngine::fileLineReaderFilePath = "commands_p3_t1.txt";
    GameEngine::defualtTerritoriesAmount = 4;
    GameEngine::isDebugMode = true;
    GameEngine::instance()->startup();
    return 0;*/


    /*>>>>>>>>>>>>>>>>>>>>>>testing:
    * 7.if during order execution one player controls all territories, the game goes to the win state, after which the replay command would put the game back into the start state, or the quit command to stop the application.
    * 16.Driver clearly demonstrates that a player that does not control any territory is removed from the game
    * 17.Driver clearly demonstrates that the game ends when a single player controls all the territories
    */
    /*std::ofstream clearText("gamelog.txt", std::ofstream::trunc);
    clearText.close();
    GameEngine::fileLineReaderFilePath = "commands_p3_t2.txt";
    GameEngine::defualtTerritoriesAmount = 2;
    GameEngine::isDebugMode = true;
    GameEngine::instance()->startup();
    return 0;*/

    /*>>>>>>>>>>>>>>>>>>>>>>testing:
    * 4.A player can create any kind of order, inlcuding those that can only be created using cards
    * 13.Driver clearly demonstrates that a player will only issue deploy orders and no other kind of orders if they still have armies in their reinforcement pool
    * 15.Driver clearly demonstrates that a player can play cards to issue orders 
    * 
    * for this part, I will enter the command by console, in order to show you the card has been used after issue the order, 
    * and the order will failed to add to the order list if that player dont have the card.
    */
   /* std::ofstream clearText("gamelog.txt", std::ofstream::trunc);
    clearText.close();
    GameEngine::fileLineReaderFilePath = "commands_p3_t3.txt";
    GameEngine::defualtTerritoriesAmount = 3;
    GameEngine::isDebugMode = false;
    GameEngine::instance()->startup();
    return 0;*/



    /*Driver: for part 4
    * 1,2.All orders are implemented and properly validated
    * 3.All orders execution result in the correct effect
    * 8.Driver clearly demonstrates that each order is validated before being executed.
    * 9.Driver clearly demonstrates that ownership of a territory is transferred to the attacking player if a territory is conquered as a result of an advance order
    * 10.Driver clearly demonstrates that one card is given to a player if they conquer at least one territory in a turn (not more than one card per turn).
    * 11.Driver clearly demonstrates that the negotiate order prevents attacks between the two players involved
    * 12.Driver clearly demonstrates that the blockade order transfers ownership to the Neutral player
    * 13.Driver clearly demonstrates that all the orders described above can be issued by a player and executed by the game engine.
    */
     /*std::ofstream clearText("gamelog.txt", std::ofstream::trunc);
     clearText.close();*/
    //GameEngine::fileLineReaderFilePath = "commands_p4_t1.txt";
    //GameEngine::defualtTerritoriesAmount = 3;
    //GameEngine::isDebugMode = true;
    //GameEngine::instance()->startup();
    //return 0;

    /*Driver: for part 5*/
    /* 
        note that everything below is just for driver demo part
        !!No real game engine running!!
    */
    //std::ofstream clearText("gamelog.txt", std::ofstream::trunc);
    //clearText.close();
    //
    //Territory *tt = new Territory("oi", 11, 1, 11, 1);
    //Orders *ord = new DeployOrder(111, 1111, tt);
    //GameEngine _ge;
    //Command c;
    //CommandProcessor cp;
    //OrderList ol;

    //LogObserver logO;

    //// has attach and notify, therefore subclass of subject
    //_ge.attach(&logO);
    //c.attach(&logO);
    //cp.attach(&logO);
    //ord->attach(&logO);
    //ol.attach(&logO);

    //// has output information, therefore subclass of ILoggable
    //int newState = 1;
    //string effect = "effect for demo";
    //string command = "command for demo";
    //_ge.transition(newState);
    //c.saveEffect(effect);
    //cp.saveCommand(command);
    //ord->execute();
    //ol.addOrder(ord);
    //// check the gamelog.txt
   
    //delete tt;
    //tt = nullptr;

    //return 0;
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


