/******************************************************************************
    card_hand.cpp

  Helper Class for player defined cards

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_hand.h"

CardHand::CardHand ( void )
{
  #ifdef DEBUG_CARD_HAND_OBJ
    std::cout << "CardHand::CardHand (): Hello, world!" << "\n" << std::endl;
  #endif

  this->cards.clear();

  this->selectedCard = 0;
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
  if ( this->getCount() > MAX_PLAYER_HAND - 1) // Remember, we are counting from zero
  {
    #ifdef DEBUG_CARD_HAND
      std::cout << "CardHand::AddCard (): " << "Discarding card " << card.id << ' ' << card.name << std::endl;
    #endif
    return false;
  }

  this->cards.push_back ( card );

  return true;
}

bool CardHand::RemoveCard ( Card &card )
{
  signed int position = 0;
  unsigned int previous_id = 0;
  std::string previous_name;

  position = this->cardPosition ( card );

  if ( position == -1 )
  {
    #ifdef DEBUG_CARD_HAND
      std::cout << "CardHand::RemoveCard (): " << "Not removing card at pos: " << position << std::endl;
    #endif
    return false;
  }

  if ( this->isEmpty() == false )
  {
    previous_id = this->cards[position].getID();
    previous_name = this->cards[position].getName();
    this->cards.erase ( this->cards.begin() + position );
    #ifdef DEBUG_CARD_HAND
      std::cout << "CardHand::RemoveCard (): " << "Removed card at pos: " << position << ' ' << "(" << previous_id << ' ' << previous_name << ")" << std::endl;
    #endif
    return true;
  }
  return false;
}

void CardHand::clearSelectedCard ( void )
{
  this->selectedCard = 0;
}

Card & CardHand::getSelectedCard ( void )
{
  return this->selectedCard;
}

bool CardHand::selectCard ( Card &card )
{
  if ( this->isValid ( card ) )
  {
    this->selectedCard = card;
    return true;
  }

  return false;

  #ifdef DEBUG_CARD_HAND
    std::cout << "CardHand::selectCard (): " << this->selectedCard.getID() << std::endl;
  #endif
}
bool CardHand::isValid ( Card &card )
{
  unsigned int idx = 0;

  for ( idx = 0; idx < this->getCount(); idx++ )
  {
    if ( ( this->cards[idx].getID() == card.getID() ) && ( this->cards[idx].getName() == card.getName() ) )
      return true;
  }

  return false;
}

bool CardHand::isEmpty ( void )
{
  if ( this->cards.empty() )
    return true;

  return false;
}

unsigned int CardHand::getCount ( void )
{
  unsigned int count = 0;

  count = this->cards.size();

  return count;
}

signed int CardHand::cardPosition ( Card &card )
{
  unsigned int idx;
  signed int pos = -1;

  if ( this->isEmpty() == false )
  {
    for ( idx = 0; idx < this->getCount() && pos == -1; idx++ )
    {
      if ( this->cards[idx].getID() == card.getID() && this->cards[idx].getName() == card.getName() )
      {
        pos = idx;
        #ifdef DEBUG_CARD_HAND
          std::cout << "CardHand::CardPosition (): " << "Position at: " << pos << ' ' << "of card: " << ' ' << this->cards[idx].getID() << ' ' << this->cards[idx].getName() << std::endl;
        #endif
      }
    }
  }
  return pos;
}
