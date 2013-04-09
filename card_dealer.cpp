/******************************************************************************
    card_dealer.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_dealer.h"

Dealer::Dealer ( void )
{
  #ifdef DEBUG_CARD_DEALER
    std::cout << "Hello, world! <From Dealer::Dealer>" << "\n" << std::endl;
  #endif

  this->cards.clear();
}

Dealer::~Dealer ( void )
{
  #ifdef DEBUG_CARD_DEALER
    std::cout << "Goodbye cruel world! <From Dealer::~Dealer>" << "\n" << std::endl;
  #endif

  this->cards.clear();
}

bool Dealer::Init ( std::vector<Card> &card )
{
  for ( int i = 0; i < card.size(); i++ ) // size should be MAX_DECKSET
  {
    this->cards.push_back ( Card (  card[i].id, card[i].level, card[i].type, card[i].element,
                                    card[i].rank, card[i].name, card[i].face ) );
  }

  return true;
}

Card Dealer::Random ( void )
{
  int rid = 0;

  if ( this->cards.empty() == false )
  {
    for ( int i = 0; i < MAX_HAND; i++ )
    {
      rid = std::rand() % ( MAX_DECKSET );
      return this->cards[rid];
    }
  }

  return 0; // Should not ever execute unless err
}

Card Dealer::selectCard ( int id )
{
  if ( this->cards.empty() == false )
  {
    return this->cards[id];
  }

  return 0; // Should not ever execute unless err
}

Card Dealer::Deal ( void )
{
  // TODO
  return 0;
}

