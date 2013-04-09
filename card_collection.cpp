/******************************************************************************
    card_collection.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_collection.h"

Deck::Deck ( void )
{
  #ifdef DEBUG_DECK
    std::cout << "Hello, world! <From Deck::Deck>" << "\n" << std::endl;
  #endif
}

Deck::~Deck ( void )
{
  #ifdef DEBUG_DECK
    std::cout << "Goodbye cruel world! <From Deck::~Deck>" << "\n" << std::endl;
  #endif

  cards.clear();

}

bool Deck::Load ( std::string filename )
{
  unsigned int id, level, type, element;
  std::array<int, 4> power;
  std::string name;

  std::ifstream load ( filename );

  if ( ! load )
  {
    std::cout << "ERR: Input file " << filename << std::endl;
    exit ( EXIT_FAILURE );
  }

  for ( int i = 0; i < MAX_DECKSET; i++ )
  {
    load >> id;
    load >> level;
    load >> type;
    load >> element;
    load >> power[0];
    load >> power[1];
    load >> power[2];
    load >> power[3];
    load >> name;

    //std::cout << id << " " << level << " " << type << " " << element << " " << name << std::endl;
    cards.push_back ( Card (id, level, type, element, power[0], power[1], power[2], power[3], name) );

  }

  load.close();

  return true;
}

bool Deck::List ( void )
{
  if ( cards.empty() == true )
  {
    #ifdef DEBUG_DECK
      std::cout << "Deck is empty. <From Deck::List>" << "\n" << std::endl;
    #endif
  }

  for ( int i = 0; i < cards.size(); i++ )
  {
    std::cout << cards[i].id << " ";
    std::cout << cards[i].level << " ";
    std::cout << cards[i].type << " ";
    std::cout << cards[i].element << " ";

    for ( int p = 0; p < 4; p++ )
    {
      std::cout << cards[i].power[p] << " ";
    }

    std::cout << cards[i].name;
    std::cout << std::endl;
  }

  return true;
}

Dealer::Dealer ( void )
{
  #ifdef DEBUG_DEALER
    std::cout << "Hello, world! <From Dealer::Dealer>" << "\n" << std::endl;
  #endif

  // We need to reconsider placement of our seed
  this->seed = std::chrono::system_clock::now().time_since_epoch().count();
}

Dealer::~Dealer ( void )
{
  #ifdef DEBUG_DEALER
    std::cout << "Goodbye cruel world! <From Dealer::~Dealer>" << "\n" << std::endl;
  #endif
}

bool Dealer::Init ( std::vector<Card> &card )
{
  for ( int i = 0; i < card.size(); i++ ) // size should be MAX_DECKSET
  {
    cards.push_back ( Card ( card[i].id, card[i].level, card[i].type, card[i].element,
                    card[i].power[0], card[i].power[1], card[i].power[2], card[i].power[3],
                    card[i].name ) );
  }

  return true;
}

bool Dealer::Random ( void )
{
  shuffle ( cards.begin(), cards.end(), std::default_random_engine ( this->seed) );

  return true;
}

bool Dealer::List ( void )
{
  if ( cards.empty() == true )
  {
    #ifdef DEBUG_DECK
      std::cout << "Dealer is empty. <From Dealer::List>" << "\n" << std::endl;
    #endif
  }

  for ( int i = 0; i < cards.size(); i++ )
  {
    std::cout << cards[i].id << " ";
    std::cout << cards[i].level << " ";
    std::cout << cards[i].type << " ";
    std::cout << cards[i].element << " ";

    for ( int p = 0; p < 4; p++ )
    {
      std::cout << cards[i].power[p] << " ";
    }

    std::cout << cards[i].name;
    std::cout << std::endl;
  }

  return true;
}

Card Dealer::selectCard ( int id )
{
  std::vector<Card> card; // temp buffer

  //card.push_back ( Card ( cards[id].id, cards[id].level, cards[id].type, cards[id].element,
                    //cards[id].power[0], cards[id].power[1], cards[id].power[2], cards[id].power[3],
                    //cards[id].name ) );
  //card.push_back ( cards.at ( id ) );
  return cards[id];
  //return cards[id];
}

/*
std::vector<Card> Dealer::Deal ( void )
{
  for ( int i = 0; i < 5; i++ )
  {
    return pick;
    // std::cout << cards[i].id << ' ' << cards[i].name << std::endl;
  }
}
*/
