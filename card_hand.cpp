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
  if ( this->CardCount() > MAX_PLAYER_HAND - 1) // Remember, we are counting from zero
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

  position = this->CardPosition ( card );

  if ( position == -1 )
  {
    #ifdef DEBUG_CARD_HAND
      std::cout << "CardHand::RemoveCard (): " << "Not removing card at pos: " << position << std::endl;
    #endif
    return false;
  }

  if ( this->isEmpty() == false )
  {
    previous_id = cards[position].id;
    previous_name = cards[position].name;
    cards.erase ( cards.begin() + position );
    #ifdef DEBUG_CARD_HAND
      std::cout << "CardHand::RemoveCard (): " << "Removed card at pos: " << position << ' ' << "(" << previous_id << ' ' << previous_name << ")" << std::endl;
    #endif
    return true;
  }
  return false;
}

Card & CardHand::GetSelectedCard ( void )
{
  return this->selectedCard;
}

void CardHand::SelectCard ( Card &card )
{
  if ( this->isValid ( card ) )
  {
    this->selectedCard = card;
  }

  #ifdef DEBUG_CARD_HAND
    std::cout << "CardHand::SelectCard (): " << this->selectedCard.id << std::endl;
  #endif
}

bool CardHand::isValid ( Card &card )
{
  unsigned int idx = 0;

  for ( idx = 0; idx < this->CardCount(); idx++ )
  {
    if ( ( cards[idx].id == card.id ) && ( cards[idx].name == card.name ) )
      return true;
  }

  return false;
}

bool CardHand::isEmpty ( void )
{
  if ( cards.empty() )
    return true;

  return false;
}

unsigned int CardHand::CardCount ( void )
{
  unsigned int count = 0;

  count = cards.size();

  return count;
}

signed int CardHand::CardPosition ( Card &card )
{
  unsigned int idx;
  signed int pos = -1;

  if ( this->isEmpty() == false )
  {
    for ( idx = 0; idx < this->CardCount() && pos == -1; idx++ )
    {
      if ( cards[idx].id == card.id && cards[idx].name == card.name )
      {
        pos = idx;
        #ifdef DEBUG_CARD_HAND
          std::cout << "CardHand::CardPosition (): " << "Position at: " << pos << ' ' << "of card: " << ' ' << cards[idx].id << ' ' << cards[idx].name << std::endl;
        #endif
      }
    }
  }
  return pos;
}
