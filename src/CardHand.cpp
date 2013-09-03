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
  if ( this->exists( card ) == true )
    return false;
  else if ( this->size() > MAX_PLAYER_HAND - 1 ) // minus one padding because we are counting from zero, not one
    return false;

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

nom::int32 CardHand::size ( void ) const
{
  nom::int32 count = 0;

  count = this->cards.size();

  return count;
}

nom::int32 CardHand::at ( Card& card )
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

void CardHand::front ( void )
{
  selectCard ( this->cards.front() );
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
    if ( card == this->cards[idx] )
      return true;
  }

  return false;
}

void CardHand::randomize ( nom::uint32 level_min, nom::uint32 level_max, CardCollection& db, nom::uint64 seedling )
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
      if ( this->push_back ( db.cards[card_id] ) )
      {
        num_cards++;
      }
    }
  }
}

bool CardHand::save ( const std::string& filename )
{
  nom::JSON::FileWriter fp; // json_spirit wrapper for file output
  json_spirit::Array game; // Overall container; this is the parent
  json_spirit::Object node; // JSON object record; the child

  for ( nom::int32 idx = 0; idx < this->size(); idx++ )
  {
    // Primary card attributes
    node = this->cards[idx].serialize();

    // Additional attributes to append onto
    node.push_back ( json_spirit::Pair ( "PlayerID", this->cards[idx].getPlayerID() ) );
    node.push_back ( json_spirit::Pair ( "Owner", this->cards[idx].getPlayerOwner() ) );

    // Push current node to our overall game data to be written
    game.push_back ( node );

    // Get ready for the next inbound row
    node.clear();
  }

  if ( fp.save ( filename, game, nom::JSON::CompactArrays ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to save JSON file: " + filename );
    return false;
  }

  return true;
}

bool CardHand::load ( const std::string& filename )
{
  nom::JSON::FileReader fp; // json_spirit wrapper for file input
  json_spirit::Object node;
  json_spirit::Value value;
  json_spirit::Array game;
  // Iterators
  json_spirit::Array::size_type i;
  json_spirit::Object::size_type o;

  // The card attributes we are loading in will be stored in here temporarily.
  // This will become the data to load onto the board if all goes well..!
  Card card;
  std::vector<Card> input_cards;

  if ( fp.load ( filename, value ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to parse JSON input file: " + filename );
    return false;
  }

  // FIXME
  this->clear(); // otherwise we exceed our limit of cards

NOM_ASSERT ( value.type() == json_spirit::array_type );
  game = value.get_array();

  for ( i = 0; i != game.size(); i++ )
  {
NOM_ASSERT ( game[i].type() == json_spirit::obj_type );
    node = game[i].get_obj();

    for ( o = 0; o != node.size(); o++ )
    {
      const json_spirit::Pair& pair = node[o];
      const std::string& path = pair.name_;
      const json_spirit::Value& value = pair.value_;

      if ( path == "ID" )
      {
NOM_ASSERT ( value.type() == json_spirit::int_type );
        card.setID ( value.get_int() );
      }
      else if ( path == "Name" )
      {
NOM_ASSERT ( value.type() == json_spirit::str_type );
        card.setName ( value.get_str() );
      }
      else if ( path == "Level" )
      {
NOM_ASSERT ( value.type() == json_spirit::int_type );
        card.setLevel ( value.get_int() );
      }
      else if ( path == "Type" )
      {
NOM_ASSERT ( value.type() == json_spirit::int_type );
        card.setType ( value.get_int() );
      }
      else if ( path == "Element" )
      {
NOM_ASSERT ( value.type() == json_spirit::int_type );
        card.setElement ( value.get_int() );
      }
      else if ( path == "Ranks" )
      {
NOM_ASSERT ( value.type() == json_spirit::array_type );
        const json_spirit::Array &ranks = value.get_array();

NOM_ASSERT ( ranks.size() == 4 );
        card.setNorthRank ( ranks[NORTH].get_int() );
        card.setEastRank ( ranks[EAST].get_int() );
        card.setSouthRank ( ranks[SOUTH].get_int() );
        card.setWestRank ( ranks[WEST].get_int() );
      }
      else if ( path == "PlayerID" )
      {
NOM_ASSERT ( value.type() == json_spirit::int_type );
        card.setPlayerID ( value.get_int() );
      }
      else if ( path == "Owner" )
      {
NOM_ASSERT ( value.type() == json_spirit::int_type );
        card.setPlayerOwner ( value.get_int() );

        // The next line *MUST* be at the end of the current node object being
        // read in -- or else epic failure will result!
        input_cards.push_back ( card );
      }
    } // end current node loop
  } // end current array node

#ifndef DEBUG
  if ( input_cards.size() < 1 ) // Sanity check
  {
NOM_LOG_ERR ( TTCARDS, "Player hand data is invalid at file: " + filename );
    return false;
  }
#endif

  // All is well, let us make our freshly loaded data permanent
  this->cards = input_cards;

  return true;
}

void CardHand::modifyCardRank ( bool modifier, nom::uint32 direction )
{
  Card selected = this->getSelectedCard();
  std::array<nom::int32, MAX_RANKS> ranks = {{ 0 }}; // card ranks container
  std::vector<Card>::iterator pos = this->cards.begin();

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
  std::vector<Card> strongest_cards ( this->cards );

  std::sort ( strongest_cards.begin(), strongest_cards.end(), std::greater<Card>() );

  return strongest_cards.front();
}

const Card CardHand::weakest ( void )
{
  std::vector<Card> weakest_cards ( this->cards );

  std::sort ( weakest_cards.begin(), weakest_cards.end(), std::less<Card>() );

  return weakest_cards.front();
}
