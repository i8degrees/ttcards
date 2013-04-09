/******************************************************************************
    card_hand.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_hand.h"

CardHand::CardHand ( void )
{
  #ifdef DEBUG_CARDHAND
    std::cout << "Hello, world! <From CardHand::CardHand>" << "\n" << std::endl;
  #endif

  deck.clear();
}

CardHand::~CardHand ( void )
{
  #ifdef DEBUG_CARDHAND
    std::cout << "Goodbye cruel world! <From CardHand::~CardHand>" << "\n" << std::endl;
  #endif

  deck.clear();
}

bool CardHand::Add ( Card card )
{

  deck.push_back ( card );

  return true;

}
