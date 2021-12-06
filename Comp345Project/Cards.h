#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h> //for drawing the card randomly 
#include <iterator>
#include <chrono>
#include <random>

using namespace std;

class Card {
 public:
  Card();

  ~Card();

  // Copy constructor
  Card(const Card& car);
  // assignment operators
  Card& operator=(const Card& car);

  // Set the original type to vector
  void original_vec_type_card();

  // Get pointer and convert to type vector
  vector<string>* get_pointer_type_arr();
  // for test to print
  void print_vec_card();

  // Since we have five type of card, set card type id, from 0-4
  void set_card_type_id(int num);

  // get card type
  string get_card_type();

 private:
  // 0 for boomb, 1 for reinforcement, 2 for blockade, 3 for airlift, and 4 for
  // diplomacy
  vector<string> vec_types_card = {"bomb", "reinforcement", "blockade", "airlift", "diplomacy"};
  // set for card type
  string card_type;
};

// inherit the card class
class Deck : public Card {
 public:
  Deck();
  ~Deck();

  // copt constructor
  Deck(const Deck& dec);
  // assignment operators
  Deck& operator=(const Deck& dec);

  // original deck vec, assign 25 cards in it, each type have 5 cards
  void original_vec_deck();
  // for printing all card of original cards
  void print_vec_deck();
  // for printing the size of deck
  void print_vec_size_of_deck();

  // allow player to draw cards randomly from the deck
  Card* draw();

  // put the card back into deck
  void return_card_into_deck_vec(Card* one_card);

 private:
  // store deck cards
  vector<Card*> vec_deck;
  // pointer to card
  Card* pointer_card;
  // for draw and remove cards
  Card* temp_card;
};

class Hand : public Card {
 public:
  Hand();
  ~Hand();
  // copy constructor
  Hand(const Hand& h);
  // assignment operators
  Hand& operator=(const Hand& h);

  // set the hand card to vec
  void set_vec_hand_cards(Card* a_Card);
  // test hand card
  void print_vec_hand_cards();
  // test play hand cards
  void print_play_hand_card();

  // play method
  void play(Card* a_Card, Deck* a_Deck);

  // put played card back
  void put_played_card_back(Deck* h_deck);

  // get pointer of hand
  vector<Card*>* get_vec_hand_cards();

  // get pointer of play
  vector<Card*>* get_vec_play_cards();

  // remove the a played card from hand_cards, for play() method
  void remove_played_card_of_hand_cards(Card* r_card);

  // remove the a played card from hand_cards, for play() method
  bool removeCardWithTypeID(string type);

  // clear play_cards after player played cards
  void clear_play_cards();

 private:
  // for store hand cards
  vector<Card*> vec_hand_cards;

  // for store play cards
  vector<Card*> vec_play_cards;
};
