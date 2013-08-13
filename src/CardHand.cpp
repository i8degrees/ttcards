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
TTCARDS_LOG_CLASSINFO;

  this->clear();
  this->selectedCard = Card();
}

CardHand::~CardHand ( void )
{
TTCARDS_LOG_CLASSINFO;

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
    if ( card.getID() == this->cards[idx].getID() )
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
  std::ofstream fp; // output file handle
  json_spirit::Object node; // current JSON node we are writing
  json_spirit::Array game; // overall data to be written out
  json_spirit::Array ranks; // card rank attributes need to be separated

  for ( nom::uint32 idx = 0; idx < this->size(); idx++ )
  {
    // Initial card attributes (everything other than ranks)
    node.push_back ( json_spirit::Pair ( "ID", (int)this->cards[idx].getID() ) );
    node.push_back ( json_spirit::Pair ( "Name", this->cards[idx].getName() ) );
    node.push_back ( json_spirit::Pair ( "Level", (int)this->cards[idx].getLevel() ) );
    node.push_back ( json_spirit::Pair ( "Type", (int)this->cards[idx].getType() ) );
    node.push_back ( json_spirit::Pair ( "Element", (int)this->cards[idx].getElement() ) );

    // Card rank attributes
    ranks.push_back ( json_spirit::Value ( (int)this->cards[idx].getNorthRank() ) );
    ranks.push_back ( json_spirit::Value ( (int)this->cards[idx].getEastRank() ) );
    ranks.push_back ( json_spirit::Value ( (int)this->cards[idx].getSouthRank() ) );
    ranks.push_back ( json_spirit::Value ( (int)this->cards[idx].getWestRank() ) );

    // Push ranks values to our current node
    node.push_back ( json_spirit::Pair ( "Ranks", ranks ) );

    // Additional card attributes -- initialized in-game
    node.push_back ( json_spirit::Pair ( "PlayerID", (int)this->cards[idx].getPlayerID() ) );
    node.push_back ( json_spirit::Pair ( "Owner", (int)this->cards[idx].getPlayerOwner() ) );

    // Push current node to our overall game data to be written
    game.push_back ( node );

    // Get ready for the next inbound row
    node.clear();
    ranks.clear();
  }

  fp.open ( filename );

  if ( fp.is_open() && fp.good() )
  {
TTCARDS_LOG_INFO( "Saving CardHand to " + filename );
    json_spirit::write_stream ( json_spirit::Value ( game ), fp, json_spirit::single_line_arrays );
  }

  fp.close();
  return true;
}

bool CardHand::load ( const std::string& filename )
{
  // Card attributes to load in
  nom::uint32 id = 0;
  nom::uint32 level = 0;
  nom::uint32 type = 0;
  nom::uint32 element = 0;
  std::array<nom::int32, MAX_RANKS> rank = { { 0 } };
  std::string name = "\0";
  nom::uint32 player_id = 0;
  nom::uint32 player_owner = 0;

  std::ifstream fp; // input file handle
  json_spirit::Object node;
  json_spirit::Value value;
  json_spirit::Array game;

  // Iterators
  json_spirit::Array::size_type i;
  json_spirit::Object::size_type o;

  this->clear(); // otherwise we exceed our limit of cards

  fp.open ( filename );

  if ( fp.is_open() && fp.good() )
  {
TTCARDS_LOG_INFO( "Loading CardHand from " + filename );
    json_spirit::read_stream ( fp, value );
    fp.close();
  }
  else
  {
    fp.close();
    return false;
  }

  assert ( value.type() == json_spirit::array_type );
  game = value.get_array();

  for ( i = 0; i != game.size(); i++ )
  {
    assert ( game[i].type() == json_spirit::obj_type );
    node = game[i].get_obj();

    for ( o = 0; o != node.size(); o++ )
    {
      const json_spirit::Pair& pair = node[o];
      const std::string& path = pair.name_;
      const json_spirit::Value& value = pair.value_;

      if ( path == "ID" )
      {
        assert ( value.type() == json_spirit::int_type );
        id = value.get_int();
      }
      else if ( path == "Name" )
      {
        assert ( value.type() == json_spirit::str_type );
        name = value.get_str();
      }
      else if ( path == "Level" )
      {
        assert ( value.type() == json_spirit::int_type );
        level = value.get_int();
      }
      else if ( path == "Type" )
      {
        assert ( value.type() == json_spirit::int_type );
        type = value.get_int();
      }
      else if ( path == "Element" )
      {
        assert ( value.type() == json_spirit::int_type );
        element = value.get_int();
      }
      else if ( path == "Ranks" )
      {
        assert ( value.type() == json_spirit::array_type );
        const json_spirit::Array &ranks = value.get_array();

        assert ( ranks.size() == 4 );
        for ( nom::uint32 rdx = 0; rdx < ranks.size(); rdx++ )
        {
          rank[NORTH] = ranks[rdx].get_int();
          rdx++;
          rank[EAST] = ranks[rdx].get_int();
          rdx++;
          rank[SOUTH] = ranks[rdx].get_int();
          rdx++;
          rank[WEST] = ranks[rdx].get_int();
          rdx++;
        }
      }
      else if ( path == "PlayerID" )
      {
        assert ( value.type() == json_spirit::int_type );
        player_id = value.get_int();
      }
      else if ( path == "Owner" )
      {
        assert ( value.type() == json_spirit::int_type );
        player_owner = value.get_int();

        this->cards.push_back ( Card ( id, level, type, element, { { rank[NORTH], rank[EAST], rank[SOUTH], rank[WEST] } }, name, player_id, player_owner ) );
      }
    } // end current node loop
  } // end current array node

  return true;
}
