/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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
  NOM_LOG_TRACE ( TTCARDS_LOG_CATEGORY_TRACE );

  this->set_position ( 0 );
  this->clear();
  this->selectedCard = Card();
}

CardHand::~CardHand ( void )
{
  NOM_LOG_TRACE ( TTCARDS_LOG_CATEGORY_TRACE );

  this->clear();
}

bool CardHand::push_back ( const Card& card )
{
  // FIXME: Duplicate cards are allowed. The player's selected card X offset is
  // broken without this check; see Player.cpp for details.

  // No go; we already have this card somewhere in our hand
  // if ( this->exists( card ) == true ) return false;

  // No go -- we are out of space!
  if ( this->size() > ( MAX_PLAYER_HAND - 1 ) ) return false;

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

bool CardHand::empty() const
{
  return this->cards.empty();
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

void CardHand::front()
{
  this->set_position(0);
}

nom::size_type CardHand::position()
{
  return this->position_;
}

void CardHand::next()
{
  nom::size_type pos = this->position();

  if( pos >= this->cards.size() ) {
    this->set_position(0);
  }
  else {
    pos = pos + 1;
    this->set_position(pos);
  }
}

void CardHand::previous()
{
  nom::size_type pos = this->position();

  if( pos >= this->cards.size() || pos == 0) {
    this->set_position(0);
  }
  else {
    pos = pos - 1;
    this->set_position(pos);
  }
}

void CardHand::clear ( void )
{
  this->cards.clear();
  this->clearSelectedCard();
}

bool CardHand::exists ( const Card& card ) const
{
  if ( card.getID() < 0 || card.getID() > Card::CARDS_COLLECTION )
  {
    return false;
  }

  for ( nom::uint32 idx = 0; idx < this->size(); idx++ )
  {
    if ( card == this->cards[idx] ) return true;
  }

  return false;
}

void CardHand::shuffle( nom::int32 level_min, nom::int32 level_max, const CardCollection& db)
{
  // Cards are picked out using our random number equal distribution generator;
  // this needs to be a value between 0..Card::CARDS_COLLECTION in order to yield a
  // ID in the cards database.
  nom::uint32 card_id = 0;
  nom::uint32 num_cards = 0; // iterator

  NOM_ASSERT( level_min >= LEVEL_MIN );
  NOM_ASSERT( level_max <= LEVEL_MAX );

  // The last ID is reserved for the no face sprite frame
  card_id = nom::uniform_int_rand<nom::uint32>(0, Card::CARDS_COLLECTION - 1);

  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_CARD_HAND, "card_id: ", card_id );
  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_CARD_HAND, "CARDS_COLLECTION: ", Card::CARDS_COLLECTION );

  if( db.cards[card_id].getLevel() <= level_max &&
      db.cards[card_id].getLevel() >= level_min ) {

    if( this->push_back(db.cards[card_id]) ) num_cards++;
  }
}

bool CardHand::save ( const std::string& filename )
{
   // High-level file I/O interface
  nom::IValueSerializer* fp = new nom::JsonCppSerializer();

  nom::Value value(nom::Value::ArrayValues);
  nom::Value card(nom::Value::ObjectValues);

  // Sanity check
  if ( this->size() <= MIN_PLAYER_HAND || this->size() > MAX_PLAYER_HAND )
  {
    NOM_LOG_ERR ( TTCARDS, "Player hand data is invalid in file: " + filename );
    return false;
  }

  for ( nom::uint32 idx = 0; idx < this->size(); idx++ )
  {
    // Serialize each card's attributes
    card = this->cards[idx].serialize();

    // Additional card attributes
    card["player_id"] = this->cards[idx].getPlayerID();
    card["owner"] = this->cards[idx].getPlayerOwner();

    value.push_back( card );
  }

  if ( fp->save( value, filename ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to save JSON file: " + filename );
    return false;
  }

  return true;
}

bool CardHand::load ( const std::string& filename )
{
   // High-level file I/O interface
  nom::IValueDeserializer* fp = new nom::JsonCppDeserializer();
  nom::Value values;

  // The card attributes we are loading in will be stored in here temporarily.
  // This will become the data to load onto the board if all goes well..!
  Card card;
  Cards cards_buffer;

  if ( fp->load( filename, values ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to parse JSON input file: " + filename );
    return false;
  }

  for ( auto itr = values.begin(); itr != values.end(); ++itr )
  {
    nom::Value obj = itr->ref();
    card.unserialize( obj );

    // Additional attributes
    card.setPlayerID( obj["player_id"].get_int() );
    card.setPlayerOwner( obj["owner"].get_int() );

    // Commit contents to our buffer if all goes well
    cards_buffer.push_back ( card );

  } // end for loop

  // Sanity check
  if ( cards_buffer.size() <= MIN_PLAYER_HAND || cards_buffer.size() > MAX_PLAYER_HAND )
  {
    NOM_LOG_ERR ( TTCARDS, "Player hand data is invalid in file: " + filename );
    return false;
  }

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
  // this->set_position( this->at(selected) );
  this->selectCard(selected);
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

void CardHand::set_position(nom::size_type pos)
{
  NOM_ASSERT( pos <= this->size() );

  this->position_ = pos;

  if( this->size() > 0 && pos < this->size() ) {
    this->selectedCard = this->cards[pos];
  }
}

void CardHand::set_face_down(bool state)
{
  for(auto itr = this->cards.begin(); itr != this->cards.end(); ++itr ) {
    (*itr).set_face_down(state);
  }
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
