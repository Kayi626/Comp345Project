#include "Cards.h"

Card::Card() {}

Card::~Card() {}

Card::Card(const Card& card) {
  this->vec_types_card = *new vector<string>(card.vec_types_card);
  this->card_type = new string(*(card.card_type));
}

Card& Card::operator=(const Card& card) {
  this->vec_types_card = *new vector<string>(card.vec_types_card);
  this->card_type = new string(*(card.card_type));
  return *this;
}

// assign types to array
void Card::original_vec_type_card() {}

vector<string>* Card::get_pointer_type_arr() { return &vec_types_card; }

string* Card::get_card_type() { return card_type; }

// for printing the card type
void Card::print_vec_card() {
  cout << "the array types of card are: " << endl;
  for (int i = 0; i < vec_types_card.size(); i++) {
    cout << vec_types_card.at(i) << endl;
  }
}

void Card::set_card_type_id(int id) { card_type = &vec_types_card.at(id); }

Deck::Deck() {}

Deck::~Deck() {
  // delete the pointer to the card

  delete (pointer_card);
  delete (temp_card);
}

Deck::Deck(const Deck& deck) {
  this->vec_deck = *new vector<Card*>(deck.vec_deck);
  this->pointer_card = new Card(*(deck.pointer_card));
  this->temp_card = new Card(*(deck.temp_card));
}

Deck& Deck::operator=(const Deck& deck) {
  this->vec_deck = *new vector<Card*>(deck.vec_deck);
  this->pointer_card = new Card(*(deck.pointer_card));
  this->temp_card = new Card(*(deck.temp_card));
  return *this;
}

void Deck::original_vec_deck() {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      // create the new card
      pointer_card = new Card;

      switch (j) {
        case 0:
          pointer_card->set_card_type_id(0);
          vec_deck.push_back(pointer_card);
          break;

        case 1:
          pointer_card->set_card_type_id(1);
          vec_deck.push_back(pointer_card);
          break;

        case 2:
          pointer_card->set_card_type_id(2);
          vec_deck.push_back(pointer_card);
          break;

        case 3:
          pointer_card->set_card_type_id(3);
          vec_deck.push_back(pointer_card);
          break;

        case 4:
          pointer_card->set_card_type_id(4);
          vec_deck.push_back(pointer_card);
          break;
      }
    }
  }
  cout << ("*** Deck cards have been initilized ***") << endl;
}

void Deck::print_vec_size_of_deck() {
  cout << endl << ("*** The current vec_deck contains ") << vec_deck.size() <<(" cards ***") << endl;
}

void Deck::print_vec_deck() {
   cout << endl << ("*** The current vec_deck contains ") << vec_deck.size() <<(" cards ***") << endl;
  for (int i = 0; i < vec_deck.size(); i++) {
    cout << ("#") << i << (" card is ") << *vec_deck.at(i)->get_card_type() << endl;
  }
}

Card* Deck::draw() {
  if (vec_deck.size() > 0) {
    // shuffle the deck
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(vec_deck.begin(), vec_deck.end(), default_random_engine(seed));

    // draw from the top
    Card* card = vec_deck.at(0);
    // delete thisthe first card from the deck
    vec_deck.erase(vec_deck.begin());
    // then return the card
    return card;
  }
  // when there are no cards, retuen null pointer
  return nullptr;
}

void Deck::return_card_into_deck_vec(Card* card) { vec_deck.push_back(card); }

Hand::Hand() {}

Hand::~Hand() {
  // destructor
}

Hand::Hand(const Hand& hand) {
  this->vec_hand_cards = *new vector<Card*>(hand.vec_hand_cards);
  this->vec_play_cards = *new vector<Card*>(hand.vec_play_cards);
}

Hand& Hand::operator=(const Hand& hand) {
  this->vec_hand_cards = *new vector<Card*>(hand.vec_hand_cards);
  this->vec_play_cards = *new vector<Card*>(hand.vec_play_cards);
  return *this;
}

void Hand::set_vec_hand_cards(Card* h_card) {
  // add the card into the handcards,we can recall the deck->draw() to get a
  // card
  vec_hand_cards.push_back(h_card);
}

void Hand::print_play_hand_card() {
  if (vec_play_cards.size() > 0) {
    cout << ("Play cards order is: ") << endl;
    for (int i = 0; i < vec_play_cards.size(); i++) {
      cout << ("# ") << i << " is " << *vec_play_cards.at(i)->get_card_type()
           << endl;
    }
  }

  else {
    cout << "** The play cards are empty **" << endl;
  }
}

void Hand::print_vec_hand_cards() {
  if (vec_hand_cards.size() > 0) {
    cout << ("The Hand cards have: ") << vec_hand_cards.size() << endl;
    for (int i = 0; i < vec_hand_cards.size(); i++) {
      cout << ("# ") << i << (" is ") << *vec_hand_cards.at(i)->get_card_type()
           << endl;
    }
  }
}

void Hand::play(Card* h_card, Deck* h_deck) {
  // set this card to play order
  vec_play_cards.push_back(h_card);

  // put back into the deck
  put_played_card_back(h_deck);

  // remove from hand
  remove_played_card_of_hand_cards(h_card);

  // remove from play cards
  vec_play_cards.pop_back();
}

void Hand::put_played_card_back(Deck* a_deck) {
  for (int i = 0; i < vec_play_cards.size(); i++) {
    a_deck->return_card_into_deck_vec(vec_play_cards.at(i));
  }
}

vector<Card*>* Hand::get_vec_hand_cards() { return &vec_hand_cards; }

vector<Card*>* Hand::get_vec_play_cards() { return &vec_play_cards; }

void Hand::remove_played_card_of_hand_cards(Card* r_card) {
  // find the same cards and delete
  for (int i = 0; i < vec_hand_cards.size(); i++) {
    if (*vec_hand_cards.at(i)->get_card_type() == *r_card->get_card_type()) {
      vec_hand_cards.erase(vec_hand_cards.begin() + i);
      cout << ("Deleting the card: ") << *r_card->get_card_type() << endl;
      return;
    }
  }
}

void Hand::clear_play_cards() {
  vec_play_cards.clear();
  cout << "Played cards are cleared." << endl;
}
