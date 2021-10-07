#include "Continent.h"
#include "Map.h"
#include "Territory.h"
#include "GameEngine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "Map.h"
#include "Cards.h"
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
#pragma endregion Player
#pragma region ObjectList
#pragma endregion ObjectList
#pragma region Card
  // create Deck
  Deck* newDeck = new Deck;

  newDeck->original_vec_deck();

  // print 25 cards and contains 5 types
  newDeck->print_vec_deck();

  Hand* newHand = new Hand;

  // print the size of deck
  newDeck->print_vec_size_of_deck();

  // apply draw() method to get random card from deck, then add into hand
  cout << "** Assign 5 random cards into hand card **" << endl;

  for (int i = 0; i < 5; i++) {
    newHand->set_vec_hand_cards(newDeck->draw());
  }

  // print current cards
  newHand->print_vec_hand_cards();

  // print current size of deck
  newDeck->print_vec_size_of_deck();

  // test play cards order
  // erase method of the vector will auto remove the cards
  cout << ("* Testing to play the cards *") << endl;
  int size_hand = newHand->get_vec_hand_cards()->size();

  for (int i = 0; i < size_hand; i++) {
    newHand->play(newHand->get_vec_hand_cards()->at(0), newDeck);
  }

  // print the play value
  newHand->print_play_hand_card();
  // print the hand value
  newHand->print_vec_hand_cards();

  // test the size of the current deck
  newDeck->print_vec_size_of_deck();

  // delete the pointer and data
  delete newHand;
  delete newDeck;
#pragma endregion Card
#pragma region GameEngine
    //GameEngine* ge = new GameEngine();
    //ge->gameFlow();
#pragma endregion GameEngine
  return 0;
}


