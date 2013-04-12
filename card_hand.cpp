/******************************************************************************
    card_hand.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_hand.h"

CardHand::CardHand ( void )
{
  #ifdef DEBUG_CARD_HAND
    std::cout << "Hello, world! <From CardHand::CardHand>" << "\n" << std::endl;
  #endif

  this->cards.clear();
}

CardHand::~CardHand ( void )
{
  #ifdef DEBUG_CARD_HAND
    std::cout << "Goodbye cruel world! <From CardHand::~CardHand>" << "\n" << std::endl;
  #endif

  this->cards.clear();
}

bool CardHand::AddCard ( Card &card )
{
  this->cards.push_back ( card );

  return true;
}

bool SelectedCard ( Card &card )
{
  //return card;
}
