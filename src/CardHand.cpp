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
#include "CardHand.hpp"

CardHand::CardHand ( void )
{
NOM_LOG_TRACE ( TTCARDS );

  this->set_position ( 0 );
  this->clear();
  this->selectedCard = Card();
}

CardHand::~CardHand ( void )
{
NOM_LOG_TRACE ( TTCARDS );

  this->clear();
}

bool CardHand::push_back ( Card& card )
{
  // No go; we already have this card somewhere in our hand
  if ( this->exists( card ) == true ) return false;

  // No go -- we are out of space!
  else if ( this->size() > ( MAX_PLAYER_HAND - 1 ) ) return false;

  this->cards.push_back ( card );
  return true;
}

bool CardHand::erase ( Card& card )
{
  signed int position = 0;
  unsigned int previous_id = 0;
  std::string previous_name;

  position = this->at ( card );

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

  this->front();

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

nom::uint32 CardHand::size ( void ) const
{
  return this->cards.size();
}

nom::int32 CardHand::at ( Card& card )
{
  nom::int32 pos = -1;

  if ( this->size() > 0 )
  {
    for ( nom::uint32 idx = 0; idx < this->size() && pos == -1; idx++ )
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

void CardHand::front ( void )
{
  this->set_position ( 0 );
  this->selectCard ( this->cards.front() );
}

nom::int32 CardHand::position ( void )
{
  return this->card_position;
}

void CardHand::next ( void )
{
  nom::int32 pos = this->position();
  CardsIterator itr;

  if ( itr == this->cards.end() )
  {
    this->set_position ( 0 );
  }
  else
  {
    pos = pos + 1;
    this->set_position ( pos );
  }

  itr = this->cards.begin()+pos;
  this->selectedCard = *itr;
}

void CardHand::previous ( void )
{
  nom::int32 pos = this->position();
  CardsIterator itr;

  if ( itr == this->cards.end() || itr == this->cards.begin() )
  {
    this->set_position ( 0 );
  }
  else
  {
    pos = pos - 1;
    this->set_position ( pos );
  }

  itr = this->cards.begin()+pos;
  this->selectedCard = *itr;
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

  for ( nom::uint32 idx = 0; idx < this->size(); idx++ )
  {
    if ( card == this->cards[idx] ) return true;
  }

  return false;
}

void CardHand::randomize ( nom::int32 level_min, nom::int32 level_max, CardCollection& db, nom::int32 seedling )
{
  // Cards are picked out using our random number equal distribution generator; this needs to
  // be a value between 1..MAX_COLLECTION in order to yield an ID in the cards
  // database.
  nom::uint32 card_id = 1;
  nom::uint32 num_cards = 0; // iterator

  // Set the seed to the same as the previous game in order to produce the same
  // results for player2 hand
  nom::int32 seed = seedling;

  if ( seed == 0 )
  {
    seed = std::chrono::system_clock::now().time_since_epoch().count();
  }

  std::default_random_engine rand_generator ( seed );

  std::uniform_int_distribution<nom::int32> distribution ( 0, MAX_COLLECTION - 1 );

#if defined (NOM_DEBUG)
  std::cout << "Random Generator Seed: " << seed << std::endl << std::endl;
#endif

  this->clear();

  while ( num_cards < MAX_PLAYER_HAND )
  {
    card_id = distribution ( rand_generator );

    if ( db.cards[card_id].getLevel() <= LEVEL_MAX && db.cards[card_id].getLevel() >= LEVEL_MIN )
    {
      if ( this->push_back ( db.cards[card_id] ) )
      {
        num_cards++;
      }
    }
  }
}

bool CardHand::save ( const std::string& filename )
{
  nom::JSON::FileWriter fp;
  nom::JSON::Value node;

  for ( nom::uint32 idx = 0; idx < this->size(); idx++ )
  {
    // Serialize each card's attributes
    node.insert ( "id", this->cards[idx].getID(), idx );
    node.insert ( "name", this->cards[idx].getName(), idx );
    node.insert ( "level", this->cards[idx].getLevel(), idx );
    node.insert ( "type", this->cards[idx].getType(), idx );
    node.insert ( "element", this->cards[idx].getElement(), idx );
    node.insert ( "ranks", this->cards[idx].ranks_as_vector(), idx );

    node.insert ( "player_id", this->cards[idx].getPlayerID(), idx );
    node.insert ( "owner", this->cards[idx].getPlayerOwner(), idx );
  }

  if ( fp.save ( filename, node ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to save JSON file: " + filename );
    return false;
  }

  return true;
}

bool CardHand::load ( const std::string& filename )
{
  nom::JSON::FileReader fp;
  nom::JSON::Value node;

  // The card attributes we are loading in will be stored in here temporarily.
  // This will become the data to load onto the board if all goes well..!
  Card card;
  Cards cards_buffer;

  if ( fp.load ( filename, node ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to parse JSON input file: " + filename );
    return false;
  }

  for ( nom::uint32 idx = 0; idx != node.size(); ++idx )
  {
    card.setID ( node.get_int( "id", idx ) );
    card.setName ( node.get_string( "name", idx ) );
    card.setLevel ( node.get_int( "level", idx ) );
    card.setType ( node.get_int( "type", idx ) );
    card.setElement ( node.get_int( "element", idx ) );
    card.setType ( node.get_int( "type", idx ) );

    std::vector<int> ranks = node.get_ints ( "ranks", idx );
    card.set_ranks ( ranks );

    card.setPlayerID ( node.get_int( "player_id", idx ) );
    card.setPlayerOwner ( node.get_int( "owner", idx ) );

    // Commit contents to our buffer if all goes well
    cards_buffer.push_back ( card );
  } // end for loop

#if ! defined (NOM_DEBUG)
  if ( cards_buffer.size() < 1 ) // Sanity check
  {
NOM_LOG_ERR ( TTCARDS, "Player hand data is invalid from file: " + filename );
    return false;
  }
#endif

  this->clear(); // otherwise we may exceed our limit of cards in hand

  // All is well, let us make it permanent
  this->cards = cards_buffer;

  return true;
}

void CardHand::modifyCardRank ( bool modifier, nom::uint32 direction )
{
  Card selected = this->getSelectedCard();
  std::array<nom::int32, MAX_RANKS> ranks = {{ 0 }}; // card ranks container
  CardsIterator pos = this->cards.begin();

  // First, obtain current rank attributes of the selected card; validation is
  // done for us by the Card class.
  ranks = selected.getRanks();

  if ( modifier ) // increase
  {
    ranks [ direction ] = ranks [ direction ] + 1;
    selected.setRanks ( ranks );
  }
  else // assume a decrease
  {
    // This clamps the decreased attribute to not falling below one (1).
    ranks [ direction ] = std::max ( ranks [ direction ] - 1, 1 );
    selected.setRanks ( ranks );
  }

  // Get the position of the selected card before we erase it so we can
  // reposition the new card at the same index.
  pos = pos + this->at ( selected );

  this->erase ( selected );

  // Update the player hand with our modified card attributes
  this->cards.insert ( pos, selected );

  // Update the player's selected card
  this->selectCard ( selected );
}

const Card CardHand::strongest ( void )
{
  Cards strongest_cards ( this->cards );

  std::sort ( strongest_cards.begin(), strongest_cards.end(), std::greater<Card>() );

  return strongest_cards.front();
}

const Card CardHand::weakest ( void )
{
  Cards weakest_cards ( this->cards );

  std::sort ( weakest_cards.begin(), weakest_cards.end(), std::less<Card>() );

  return weakest_cards.front();
}

void CardHand::set_position ( nom::int32 pos )
{
  this->card_position = pos;
}

std::ostream& operator << ( std::ostream& os, const CardHand& rhs )
{
  for ( nom::uint32 idx = 0; idx < rhs.cards.size(); idx++ )
  {
    os  << rhs.cards[idx].getName()
        << card_delimiter
        << rhs.cards[idx].getID()
        << card_delimiter
        << rhs.cards[idx].getLevel()
        << card_delimiter
        << rhs.cards[idx].getType()
        << card_delimiter
        << rhs.cards[idx].getElement()
        << card_delimiter
        << rhs.cards[idx].getNorthRank()
        << card_delimiter
        << rhs.cards[idx].getEastRank()
        << card_delimiter
        << rhs.cards[idx].getSouthRank()
        << card_delimiter
        << rhs.cards[idx].getWestRank()
        << card_delimiter
        << rhs.cards[idx].getPlayerID()
        << card_delimiter
        << rhs.cards[idx].getPlayerOwner();
  }

  return os;
}

void Free_CardHand ( CardHand* ptr )
{
  // Do nothing custom (smart pointer) deleter
}
