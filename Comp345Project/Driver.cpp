#include "Continent.h"
#include "Map.h"
#include "Territory.h"
#include "GameEngine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <cstring>
#include "Map.h"
#include "Cards.h"
#include "Orders.h"
#include "Territory.h"
#include "Player.h"
#include "Continent.h"

using namespace std;


int main() {
//note: pragma region should only be used in driver for Demo!
//Many programmers believe this will only make your code worse.

#pragma region Map
  //Map* newMap = Map::mapCreater("LOTR2.map"); // bigeurope.map, LOTR2.map
  //// mb_loader->toString();
  //
  ////Display the data
  //newMap->displayLink();

  ////Validate the map
  //if (newMap->validate())
  //    cout << endl << "-------------Map is valid-------------------" << endl;
  //else
  //    cout << endl << "-------------Map is invalid-------------------" << endl;
  ////newMap->displayAllContinents();

  ////Handle memory leaks
  //delete newMap;
  //newMap = nullptr;
#pragma endregion Map
#pragma region Player
    //Map* newMap = Map::mapCreater("LOTR2.map");  // bigeurope.map, LOTR2.map
    //if (!newMap->validate()) {
    //  cout << "ERROR: the map we are loading didn't pass the validate process."
    //       << "\n";
    //  return 0;
    //}
    //newMap->displayLink();

    //cout << "DEBUG: Create a player Object."
    //     << "\n";
    //vector<vector<Territory*>> currentMapGraph = (newMap->getMapGraph());
    //Player player1(2, "Comp345", &currentMapGraph);

    //cout << "DEBUG: Create some orders and add them to player's orderlist."
    //     << "\n";
    //Territory t2("Canada", 1, 2, 3, 4);
    //Territory t3("US", 5, 6, 7, 8);
    //Territory t4("England", 9, 10, 11, 12);
    //Orders* o1 = new DeployOrder(101, 5, &t2);
    //Orders* o2 = new DeployOrder(101, 30, &t3);
    //Orders* o3 = new AdvanceOrder(101, 10, &t3, &t2, true);
    //(*player1.getOrderList()).put(o3);
    //(*player1.getOrderList()).put(o1);
    //(*player1.getOrderList()).put(o2);
    //(*player1.getOrderList()).displayAll();

    //cout << "DEBUG: Adding terrtories ownership player"
    //     << "\n";
    //player1.addTerrtories(currentMapGraph[1][0]);
    //player1.printPlayerTerrtories();
    //cout << "DEBUG: adding 1 more terrtories ownership player"
    //     << "\n";
    //player1.addTerrtories(currentMapGraph[3][0]);
    //player1.printPlayerTerrtories();
    //cout << "DEBUG: adding 3 more terrtories ownership player"
    //     << "\n";
    //player1.addTerrtories(currentMapGraph[2][0]);
    //player1.addTerrtories(currentMapGraph[5][0]);
    //player1.addTerrtories(currentMapGraph[10][0]);
    //player1.printPlayerTerrtories();

    //delete newMap;
    //newMap = nullptr;

    //cout << "Done!"
    //     << "\n";
#pragma endregion Player
#pragma region OrderList
    //OrderList oList1(105);
    //Territory t2("Canada", 1, 2, 3, 4);
    //Territory t3("US", 5, 6, 7, 8);
    //Territory t4("England", 9, 10, 11, 12);

    //Orders* o1 = new DeployOrder(101, 5, &t2);
    //Orders* o2 = new DeployOrder(101, 30, &t3);
    //Orders* o3 = new AdvanceOrder(101, 10, &t3, &t2, true);
    //Orders* o4 = new BombOrder(101, &t4, true);
    //Orders* o5 = new BlockadeOrder(101, &t2);
    //Orders* o6 = new AirliftOrder(101, 7, &t3, &t2);
    //Orders* o7 = new NegotiateOrder(101, &t2);

    //oList1.put(o1);
    //oList1.put(o2);
    //oList1.put(o3);
    //cout << "DEBUG: Create 7 orders, move first three inside the list."
    //     << "\n";
    //oList1.displayAll();
    //oList1.put(o4);
    //oList1.put(o5);
    //oList1.remove(*(o2->getOrderID()));
    //oList1.put(o6);
    //oList1.put(o7);
    //cout << "DEBUG: move 4 new order inside the list. remove the order with id:2 "
    //     << "\n";

    //oList1.displayAll();

    //oList1.move(3, 3);
    //cout << "DEBUG: move order 3 , 3 place down "
    //     << "\n";
    //oList1.displayAll();

    //oList1.move(5, 4);
    //cout << "DEBUG: move order 5 , 4 place down, which should result in nothing, "
    //        "since it's out of range "
    //     << "\n";
    //oList1.displayAll();

    //oList1.move(3, -3);
    //cout << "DEBUG: move order 3 , 3 place up"
    //     << "\n";
    //oList1.displayAll();
    //oList1.move(7, -4);
    //cout << "DEBUG: move order 7 , 4 place up, which should result in nothing, "
    //        "since it's out of range "
    //     << "\n";
    //oList1.displayAll();

    //oList1.remove(*(o1->getOrderID()));
    //oList1.remove(*(o4->getOrderID()));
    //oList1.remove(*(o3->getOrderID()));

    //cout << "DEBUG: remove order with id: 1,3,4 from order list "
    //     << "\n";
    //oList1.displayAll();

    //cout << "DEBUG: execute first order "
    //     << "\n";
    //cout << oList1.getFirst()->execute() << "\n";

    //cout << "DEBUG: execute last order "
    //     << "\n";
    //cout << oList1.getLast()->execute() << "\n";

    //oList1.displayAll();

    //cout << "DEBUG: pop the first order and execute it"
    //     << "\n";
    //cout << oList1.popFirst()->execute() << "\n";
    //oList1.displayAll();
    //cout << "DEBUG: pop the last order and execute it"
    //     << "\n";
    //cout << oList1.popLast()->execute() << "\n";
    //oList1.displayAll();

    //cout << "Done!"
    //     << "\n";
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
  //cout << "** Assign 5 random cards into hand card **" << endl;

  //for (int i = 0; i < 5; i++) {
  //  newHand->set_vec_hand_cards(newDeck->draw());
  //}

  //// print current cards
  //newHand->print_vec_hand_cards();

  //// print current size of deck
  //newDeck->print_vec_size_of_deck();

  //// test play cards order
  //// erase method of the vector will auto remove the cards
  //cout << ("* Testing to play the cards *") << endl;
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
#pragma endregion Card
#pragma region GameEngine
    GameEngine* ge = new GameEngine();
    ge->gameFlow();
#pragma endregion GameEngine
return 0;
}


