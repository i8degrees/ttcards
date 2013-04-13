/******************************************************************************
    card_hand.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_hand.h"

CardHand::CardHand ( void )
{
  #ifdef DEBUG_CARD_HAND_OBJ
    std::cout << "CardHand::CardHand (): Hello, world!" << "\n" << std::endl;
  #endif

  this->cards.clear();
}

CardHand::~CardHand ( void )
{
  #ifdef DEBUG_CARD_HAND_OBJ
    std::cout << "CardHand::~CardHand (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  this->cards.clear();
}

bool CardHand::AddCard ( Card &card )
{
    #ifdef DEBUG_CARD_HAND
      std::cout << "CardHand::AddCard (): " << "Discarding card " << card.id << ' ' << card.name << std::endl;
    #endif
  this->cards.push_back ( card );

  return true;
}

bool SelectedCard ( Card &card )
{
  //return card;
}
