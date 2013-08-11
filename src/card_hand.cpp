/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include "card_hand.hpp"

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
  if ( this->exists( card ) == true )
    return false;
  else if ( this->size() > MAX_PLAYER_HAND - 1 ) // minus one padding because we are counting from zero, not one
    return false;

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

  previous_id = this->cards[position].getID();
  previous_name = this->cards[position].getName();
  this->cards.erase ( this->cards.begin() + position );
#ifdef DEBUG_CARD_HAND
  std::cout << "CardHand::removeCard (): " << "Removed card at pos: " << position << ' ' << "(" << previous_id << ' ' << previous_name << ")" << std::endl;
#endif

  return true;
}

void CardHand::clearSelectedCard ( void )
{
  this->selectedCard = Card();
}

Card & CardHand::getSelectedCard ( void )
{
  return this->selectedCard;
}

void CardHand::selectCard ( Card& card )
{
  if ( this->exists ( card ) )
  {
    this->selectedCard = card;
  }

#ifdef DEBUG_CARD_HAND
  std::cout << "CardHand::selectCard (): " << this->selectedCard.getID() << std::endl;
#endif
}

nom::int32 CardHand::size ( void ) const
{
  nom::int32 count = 0;

  count = this->cards.size();

  return count;
}

nom::int32 CardHand::pos ( Card& card )
{
  nom::int32 idx;
  nom::int32 pos = -1;

  if ( this->size() > 0 )
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
  this->clearSelectedCard();
}

bool CardHand::exists ( const Card& card ) const
{
  if ( card.getID() < 1 || card.getID() > MAX_COLLECTION )
    return false;

  for ( auto idx = 0; idx < this->size(); idx++ )
  {
    if ( card.getID() == this->cards[idx].getID() )
      return true;
  }

  return false;
}

void CardHand::randomize ( nom::uint32 level_min, nom::uint32 level_max, Collection& db, nom::uint64 seedling )
{
  // Cards are picked out using our random number equal distribution generator; this needs to
  // be a value between 1..MAX_COLLECTION in order to yield an ID in the cards
  // database.
  nom::uint32 card_id = 1;
  nom::int32 num_cards = 0; // iterator

  // Set the seed to the same as the previous game in order to produce the same
  // results for player2 hand
  nom::uint64 seed = seedling;

  if ( seed == 0 )
    seed = std::chrono::system_clock::now().time_since_epoch().count();

  std::default_random_engine rand_generator ( seed );

  std::uniform_int_distribution<nom::int32> distribution ( 1, MAX_COLLECTION );

#ifdef DEBUG_GAME
  std::cout << "Random Generator Seed: " << seed << std::endl << std::endl;
#endif

  this->clear();

  while ( num_cards < MAX_PLAYER_HAND )
  {
    card_id = distribution ( rand_generator );

    if ( db.cards[card_id].getLevel() <= level_max && db.cards[card_id].getLevel() >= level_min )
    {
      if ( this->addCard ( db.cards[card_id] ) )
      {
        num_cards++;
      }
    }
  }
}
