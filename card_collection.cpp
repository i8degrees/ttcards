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

  cards.clear();
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
  unsigned int id, level, type, element = 0;
  std::array<int, 4> rank = { { 0 } };
  std::string name, face = "\0";

  std::ifstream load ( filename );

  if ( ! load )
  {
    std::cout << "ERR: Input file " << filename << std::endl;
    exit ( EXIT_FAILURE );
  }

  for ( int i = 0; i < ( MAX_DECKSET + 1); i++ )
  {
    load >> id;
    load >> level;
    load >> type;
    load >> element;
    load >> rank[0];
    load >> rank[1];
    load >> rank[2];
    load >> rank[3];
    load >> name;
    load >> face; // TODO: a temporary value until sprite sheet is ready

    //std::cout << id << " " << level << " " << type << " " << element << " " << name << std::endl;
    cards.push_back ( Card (id, level, type, element, { { rank[0], rank[1], rank[2], rank[3] } }, name, face ) );

  }

  load.close();

  return true;
}

Dealer::Dealer ( void )
{
  #ifdef DEBUG_DEALER
    std::cout << "Hello, world! <From Dealer::Dealer>" << "\n" << std::endl;
  #endif

  cards.clear();
}

Dealer::~Dealer ( void )
{
  #ifdef DEBUG_DEALER
    std::cout << "Goodbye cruel world! <From Dealer::~Dealer>" << "\n" << std::endl;
  #endif

  cards.clear();
}

bool Dealer::Init ( std::vector<Card> &card )
{
  for ( int i = 0; i < card.size(); i++ ) // size should be MAX_DECKSET
  {
    cards.push_back ( Card ( card[i].id, card[i].level, card[i].type, card[i].element,
                    card[i].rank,
                    //card[i].rank[0], card[i].rank[1], card[i].rank[2], card[i].rank[3],
                    card[i].name, card[i].face ) );
  }

  return true;
}

Card Dealer::Random ( void )
{
  int rid = 0;

  if ( cards.empty() == false )
  {
    for ( int i = 0; i < MAX_HAND; i++ )
    {
      rid = std::rand() % ( MAX_DECKSET );
      return cards[rid];
    }
  }

  return 0; // Should not ever execute unless err
}

Card Dealer::selectCard ( int id )
{
  if ( cards.empty() == false )
  {
    return cards[id];
  }

  return 0; // Should not ever execute unless err
}

Card Dealer::Deal ( void )
{
  // TODO
  return 0;
}

