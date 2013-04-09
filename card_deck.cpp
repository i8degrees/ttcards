/******************************************************************************
    card_deck.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_deck.h"

Deck::Deck ( void )
{
  #ifdef DEBUG_CARD_DECK
    std::cout << "Hello, world! <From Deck::Deck>" << "\n" << std::endl;
  #endif

  this->cards.clear();
}

Deck::~Deck ( void )
{
  #ifdef DEBUG_CARD_DECK
    std::cout << "Goodbye cruel world! <From Deck::~Deck>" << "\n" << std::endl;
  #endif

  this->cards.clear();
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

  #ifdef DEBUG_CARD_DECK
    std::cout << filename << " " << "<From Deck::Load>" << "\n" << std::endl;
  #endif

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
    load >> face;

    #ifdef DEBUG_CARD_DECK
      std::cout << id;
      std::cout << " ";
      std::cout << level;
      std::cout << " ";
      std::cout << type;
      std::cout << " ";
      std::cout << element;
      std::cout << " ";
      for ( int rdx = 0; rdx < 4; rdx++ )
      {
        std::cout << rank[rdx];
        std::cout << " ";
      }
      std::cout << name;
      std::cout << " ";
      std::cout << face;

      std::cout << std::endl;
    #endif // defined DEBUG_CARD_DECK

    this->cards.push_back ( Card (id, level, type, element, { { rank[0], rank[1], rank[2], rank[3] } }, name, face ) );
  }

  #ifdef DEBUG_CARD_DECK
    std::cout << "EOF: " << filename << " " << "<From Deck::Load>" << "\n" << std::endl;
  #endif

  load.close();

  return true;
}
