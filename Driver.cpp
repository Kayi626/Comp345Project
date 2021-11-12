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
    //note: pragma region should only be used in driver for Demo!
    //Many programmers believe this will only make your code worse.

#pragma region Map

  //Map* newMap = Map::mapCreater("LOTR2.map"); // bigeurope.map, LOTR2.map

  //std::cout << *newMap << endl;

  ////Validate the map
  //if (newMap->validate())
  //    std::cout << endl << "-------------Map is valid-------------------" << endl;
  //else
  //    std::cout << endl << "-------------Map is invalid-------------------" << endl;

  ////Handle memory leaks
  //delete newMap;
  //newMap = nullptr;


#pragma endregion Map
#pragma region Player

    // Map* newMap = Map::mapCreater("../Comp345Project/LOTR2.map"); // bigeurope.map, LOTR2.map
    // if (!newMap->validate()) {
    //     std::cout << "ERROR: the map we are loading didn't pass the validate process." << "\n";
    //     return 0;
    // }

    //std::cout << *newMap << endl;

    // std::cout << "DEBUG: Create a player Object." << "\n";
    // vector<vector<Territory*>> currentMapGraph = (newMap->getMapGraph());
    // Player player1(2, "Comp345", &currentMapGraph);
    // Hand* newHand = player1.getHandsOfCard();
    //
    // // create Deck
    // Deck* newDeck = new Deck;

    // newDeck->original_vec_deck();

    // // print 25 cards and contains 5 types
    // newDeck->print_vec_deck();

    //  // apply draw() method to get random card from deck, then add into hand
    //  std::cout << "** Assign 5 random cards into hand card **" << endl;

    //  for (int i = 0; i < 5; i++) {
    //    newHand->set_vec_hand_cards(newDeck->draw());
    //  }

    //  // print current cards
    //  newHand->print_vec_hand_cards();

    //  // print current size of deck
    //  newDeck->print_vec_size_of_deck();

    //  // test play cards order
    //  // erase method of the vector will auto remove the cards
    //  std::cout << ("* Testing to play the cards *") << endl;
    //  int size_hand = newHand->get_vec_hand_cards()->size();

    //  for (int i = 0; i < size_hand; i++) {
    //    newHand->play(newHand->get_vec_hand_cards()->at(0), newDeck);
    //  }

    //  // print the play value
    //  newHand->print_play_hand_card();
    //  // print the hand value
    //  newHand->print_vec_hand_cards();


    // std::cout << "DEBUG: Adding terrtories ownership player" << "\n";
    // player1.addTerrtories(currentMapGraph[1][0]);
    // player1.printPlayerTerrtories();
    // std::cout << "DEBUG: adding 1 more terrtories ownership player" << "\n";
    // player1.addTerrtories(currentMapGraph[3][0]);
    // player1.printPlayerTerrtories();
    // std::cout << "DEBUG: adding 3 more terrtories ownership player" << "\n";
    // player1.addTerrtories(currentMapGraph[2][0]);
    // player1.addTerrtories(currentMapGraph[5][0]);
    // player1.addTerrtories(currentMapGraph[10][0]);
    // player1.printPlayerTerrtories();


    // std::cout << "DEBUG: Create some orders and add them to player's orderlist." << "\n";
    // player1.issueOrder(0, currentMapGraph[5][0], 10, currentMapGraph[3][0]);
    // player1.issueOrder(0, currentMapGraph[6][0], 10, currentMapGraph[3][0]);
    // player1.issueOrder(1, currentMapGraph[2][0], 5, currentMapGraph[3][0]);
    // (*player1.getOrderList()).displayAll();

    // std::cout << "DEBUG: Create some more orders and add them to player's orderlist." << "\n";
    // player1.issueOrder(5, currentMapGraph[17][0], 12345, currentMapGraph[17][0]);
    // player1.issueOrder(3, currentMapGraph[11][0], 12345, currentMapGraph[17][0]);
    // //this will be an invaild order, since terrtories on currentMapGraph[11][0] is not owned by current player. leave for testing of validate() method
    // player1.issueOrder(2, currentMapGraph[17][0], 12345, currentMapGraph[17][0]);
    // player1.issueOrder(1, currentMapGraph[1][0], 5, currentMapGraph[2][0]);
    // player1.issueOrder(1, currentMapGraph[2][0], 15, currentMapGraph[3][0]);
    // (*player1.getOrderList()).displayAll();

    // std::cout << "DEBUG: Testing the 'validate'method: " << "\n";
    // bool valid = player1.getOrderList()->popFirst()->validate();
    // std::cout << "POP the first order and  -- is it validate? :" << valid <<"\n";
    // std::cout << "The first order is poped. checking the order list: " << "\n";
    // (*player1.getOrderList()).displayAll();
    // valid = player1.getOrderList()->popFirst()->validate();
    // std::cout << "POP the 2nd order(currently the first) and  -- is it validate? :" << valid << "\n";
    // std::cout << "the result of the order above should be 0(false), since currentMapGraph[6][0] is not controled by the current player." << valid << "\n";

    // std::cout << "POP and exeute the last order checking the execute() method :" <<  "\n";
    // std::cout << "result is(should be error ):" << player1.getOrderList()->popLast()->execute() << "\n";


    // std::cout << "get and exeute the 2nd last order(current the last order) checking the execute() method :" << "\n";
    // currentMapGraph[2][0]->setArmyNumber(30);
    // std::cout << "result is(should be successful since it meet 3 requirement ):" << player1.getOrderList()->getLast()->execute() << "\n";


    // delete newMap;
    // newMap = nullptr;

    // std::cout << "Done!" << "\n";
#pragma endregion Player
#pragma region OrderList
   //OrderList oList1(105);
   //  Territory t2("Canada", 1, 2, 3, 4);
   //  Territory t3("US", 5, 6, 7, 8);
   //  Territory t4("England", 9,10,11,12);

   //  Orders* o1 = new DeployOrder(101,5, &t2);
   //  Orders* o2 = new DeployOrder(101, 30, &t3);
   //  Orders* o3 = new AdvanceOrder(101, 10, &t3, &t2,true);
   //  Orders* o4 = new BombOrder(101, &t4,true);
   //  Orders* o5 = new BlockadeOrder(101, &t2);
   //  Orders* o6 = new AirliftOrder(101, 7, &t3, &t2);
   //  Orders* o7 = new NegotiateOrder(101,&t2);

   //  std::cout << "TESTING Stream operator" << endl;
   //  std::std::cout << "testing subclass: " << *(new DeployOrder(101, 5, &t2)) << endl;
   //  std::cout << "TESTING Stream operator , testing#2" << endl;
   //  std::std::cout << "o1 (but Orders object):" << *o1 << endl;
   //  std::std::cout << "o3 (Orders object):" << *o3 << endl;
   //  std::std::cout << "o4 (Orders object):" << *o4 << endl;

   //  oList1.put(o1);
   //  oList1.put(o2);
   //  oList1.put(o3);
   //  std::cout << "DEBUG: Create 7 orders, move first three inside the list." << "\n";
   //  oList1.displayAll();
   //  oList1.put(o4);
   //  oList1.put(o5);
   //  oList1.remove(*(o2->getOrderID()));
   //  oList1.put(o6);
   //  oList1.put(o7);
   //  std::cout << "DEBUG: move 4 new order inside the list. remove the order with id:2 " << "\n";

   //  oList1.displayAll();

   //  oList1.move(3, 3);
   //  std::cout << "DEBUG: move order 3 , 3 place down " << "\n";
   //  oList1.displayAll();

   //  oList1.move(5, 4);
   //  std::cout << "DEBUG: move order 5 , 4 place down, which should result in nothing, since it's out of range " << "\n";
   //  oList1.displayAll();

   //  oList1.move(3, -3);
   //  std::cout << "DEBUG: move order 3 , 3 place up" << "\n";
   //  oList1.displayAll();
   //  oList1.move(7, -4);
   //  std::cout << "DEBUG: move order 7 , 4 place up, which should result in nothing, since it's out of range " << "\n";
   //  oList1.displayAll();

   //  oList1.remove(*(o1->getOrderID()));
   //  oList1.remove(*(o4->getOrderID()));
   //  oList1.remove(*(o3->getOrderID()));

   //  std::cout << "DEBUG: remove order with id: 1,3,4 from order list " << "\n";
   //  oList1.displayAll();

   //  std::cout << "DEBUG: execute first order " << "\n";
   //  std::cout << oList1.getFirst()->execute() << "\n";

   //  std::cout << "DEBUG: execute last order " << "\n";
   //  std::cout << oList1.getLast()->execute() << "\n";

   //  oList1.displayAll();

   //  std::cout << "DEBUG: pop the first order and execute it" << "\n";
   //  std::cout << oList1.popFirst()->execute() << "\n";
   //  oList1.displayAll();
   //  std::cout << "DEBUG: pop the last order and execute it" << "\n";
   //  std::cout << oList1.popLast()->execute() << "\n";
   //  oList1.displayAll();


   //  std::cout << "====I also checked the 'validate()'method and 'execute()'method in PlayerDriver. ====" << "\n";
   //  std::cout << "====we can't get a valid result in this part of driver, since there's no real map implemented. ====" << "\n";
   //  std::cout << "====for more details of 'validate()'method and 'execute()'method for Orders class, check the 'PlayerDriver.cpp'. ====" << "\n";


   //  std::cout << "Done!" << "\n";
#pragma endregion OrderList
#pragma region Card

  //// create Deck
  //Deck* newDeck = new Deck;

  //newDeck->original_vec_deck();

  //// print 25 cards and contains 5 types
  //newDeck->print_vec_deck();

  //Hand* newHand = new Hand;

  //// print the size of deck
  //newDeck->print_vec_size_of_deck();

  //// apply draw() method to get random card from deck, then add into hand
  //std::cout << "** Assign 5 random cards into hand card **" << endl;

  //for (int i = 0; i < 5; i++) {
  //  newHand->set_vec_hand_cards(newDeck->draw());
  //}

  //// print current cards
  //newHand->print_vec_hand_cards();

  //// print current size of deck
  //newDeck->print_vec_size_of_deck();

  //// test play cards order
  //// erase method of the vector will auto remove the cards
  //std::cout << ("* Testing to play the cards *") << endl;
  //int size_hand = newHand->get_vec_hand_cards()->size();

  //for (int i = 0; i < size_hand; i++) {
  //  newHand->play(newHand->get_vec_hand_cards()->at(0), newDeck);
  //}

  //// print the play value
  //newHand->print_play_hand_card();
  //// print the hand value
  //newHand->print_vec_hand_cards();

  //// test the size of the current deck
  //newDeck->print_vec_size_of_deck();

  //// delete the pointer and data
  //delete newHand;
  //delete newDeck;
  //newHand = nullptr;
  //newDeck = nullptr;
  //
#pragma endregion Card
#pragma region GameEngine

    FileLineReader* flr = new FileLineReader("../Comp345Project/commands.txt");
    FileCommandProcessorAdapter* fcomP = new FileCommandProcessorAdapter(flr);
    GameEngine* ge = new GameEngine(fcomP,6,true);


    //Map* newMap = Map::mapCreater("../Comp345Project/bigeurope.map"); // bigeurope.map, LOTR2.map
    //Map* newMap = Map::mapCreater("C:/Users/InterfaceGu/source/repos/Comp345Project_DN10/Comp345Project/bigeurope.map"); // bigeurope.map, LOTR2.map

    //Game Loop (Important: If any invalid commands are received by the loop, it will prompt users to enter inputs by console)
    ge->startup();

    //De-allocation
    delete ge;
    delete fcomP;
    delete flr;
    ge = nullptr;
    fcomP = nullptr;
    flr = nullptr;








    //if (argc == 1) {
    //    cout << "Error: No command line argument has been entered! The program exits." << endl << endl;
    //    exit(0);
    //}
    //cout <<"Command Line Argument: "<<argv[1] << endl;
    ////Console inputs
    //if(std::regex_match(argv[1], std::regex("-console\\s*"))){
    //    GameEngine* ge = new GameEngine();
    //    CommandProcessor* comP = new CommandProcessor();

    //    //GameLoop 
    //    ge->gameFlow(*comP);

    //    //De-allocation
    //    delete ge;
    //    delete comP;
    //    ge = nullptr;
    //    comP = nullptr;
    //}
    ////File inputs(Important: In command line argument, double quotes are needed in "-file <filename>".
    //else if(std::regex_match(argv[1], std::regex("-file\\s+<(.*)>"))) {
    //    
    //    GameEngine* ge = new GameEngine();
    //    FileLineReader* flr = new FileLineReader(extractLineArgumentCommand(argv[1]));
    //    FileCommandProcessorAdapter * fcomP = new FileCommandProcessorAdapter(flr);
 
    //    
    //    //Game Loop (Important: If any invalid commands are received by the loop, it will prompt users to enter inputs by console)
    //    ge->gameFlow(*fcomP);

    //    //De-allocation
    //    delete ge;
    //    delete fcomP;
    //    delete flr;
    //    ge = nullptr;
    //    fcomP = nullptr;
    //    flr = nullptr;
    //    
    //    
    //}

    //else {
    //    cout << "Invalid Command Line Arguments. Please try again. The program exits" << endl;
    //    exit(0);
    //}
    
   
#pragma endregion GameEngine
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


