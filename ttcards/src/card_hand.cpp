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

  this->clear();

  this->selectedCard = Card();
}

CardHand::~CardHand ( void )
{
  #ifdef DEBUG_CARD_HAND_OBJ
    std::cout << "CardHand::~CardHand (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  this->clear();
}

bool CardHand::addCard ( Card& card )
{
  if ( this->size() > MAX_PLAYER_HAND - 1) // minus one padding because we are counting from zero, not one
  {
    #ifdef DEBUG_CARD_HAND
      std::cout << "CardHand::addCard (): " << "Discarding card " << this->cards.back().getID() << ' ' << this->cards.back().getName() << std::endl;
    #endif

    this->cards.pop_back();
  }

  this->cards.push_back ( card );

  return true;
}

bool CardHand::removeCard ( Card& card )
{
  signed int position = 0;
  unsigned int previous_id = 0;
  std::string previous_name;

  position = this->pos ( card );

  if ( position == -1 )
  {
    #ifdef DEBUG_CARD_HAND
      std::cout << "CardHand::removeCard (): " << "Not removing card at pos: " << position << std::endl;
    #endif
    return false;
  }

  if ( this->empty() == false )
  {
    previous_id = this->cards[position].getID();
    previous_name = this->cards[position].getName();
    this->cards.erase ( this->cards.begin() + position );
    #ifdef DEBUG_CARD_HAND
      std::cout << "CardHand::removeCard (): " << "Removed card at pos: " << position << ' ' << "(" << previous_id << ' ' << previous_name << ")" << std::endl;
    #endif
    return true;
  }
  return false;
}

void CardHand::clearSelectedCard ( void )
{
  this->selectedCard = Card();
}

Card & CardHand::getSelectedCard ( void )
{
  return this->selectedCard;
}

bool CardHand::selectCard ( Card& card )
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
bool CardHand::isValid ( Card& card )
{
  unsigned int idx = 0;

  for ( idx = 0; idx < this->size(); idx++ )
  {
    if ( ( this->cards[idx].getID() == card.getID() ) && ( this->cards[idx].getName() == card.getName() ) )
      return true;
  }

  return false;
}

bool CardHand::empty ( void )
{
  if ( this->cards.empty() )
    return true;

  return false;
}

unsigned int CardHand::size ( void )
{
  unsigned int count = 0;

  count = this->cards.size();

  return count;
}

signed int CardHand::pos ( Card& card )
{
  unsigned int idx;
  signed int pos = -1;

  if ( this->empty() == false )
  {
    for ( idx = 0; idx < this->size() && pos == -1; idx++ )
    {
      if ( this->cards[idx].getID() == card.getID() && this->cards[idx].getName() == card.getName() )
      {
        pos = idx;
        #ifdef DEBUG_CARD_HAND
          std::cout << "CardHand::pos (): " << "Position at: " << pos << ' ' << "of card: " << ' ' << this->cards[idx].getID() << ' ' << this->cards[idx].getName() << std::endl;
        #endif
      }
    }
  }
  return pos;
}

void CardHand::clear ( void )
{
  this->cards.clear();
}
