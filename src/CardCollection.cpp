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
#include "CardCollection.hpp"

CardCollection::CardCollection ( void )
{
NOM_LOG_TRACE ( TTCARDS );
  this->clear();
}

CardCollection::~CardCollection ( void )
{
NOM_LOG_TRACE ( TTCARDS );
}

void CardCollection::clear ( void )
{
  this->cards.clear();
}

nom::int32 CardCollection::size ( void ) const
{
  nom::int32 count = 0;

  count = this->cards.size();

  return count;
}

Card& CardCollection::getCards ( unsigned int idx )
{
  return this->cards[idx];
}

Cards CardCollection::getCards ( void )
{
  unsigned int idx = 0;
  Cards temp_cards; // temp var for return passing
  temp_cards.clear();

  for ( idx = 0; idx < this->cards.size(); idx++ )
    temp_cards.push_back ( this->cards[idx] );

  return temp_cards;
}

bool CardCollection::save( const std::string& filename )
{
  nom::JSON::FileWriter fp; // json_spirit wrapper for file output
  json_spirit::Array game; // Overall container; this is the parent
  json_spirit::Object node; // JSON object record; the child

  if ( this->cards.empty() ) return false;

  for ( nom::int32 idx = 0; idx < this->cards.size(); idx++ )
  {
    // A card object is one JSON object
    game.push_back ( this->cards[idx].serialize() );

    // Get ready for the next row
    node.clear();
  }

  if ( fp.save ( filename, game, nom::JSON::CompactArrays ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to save JSON file: " + filename );
    return false;
  }

  return true;
}

bool CardCollection::load( const std::string& filename )
{
  nom::JSON::FileReader fp; // json_spirit wrapper for file input
  json_spirit::Object obj;
  json_spirit::Value value;
  json_spirit::Array values;
  json_spirit::Array::size_type i; // iterator for object values looping
  json_spirit::Object::size_type o; // iterator for object values looping

  // The card attributes we are loading in will be stored in here, and once a
  // card has filled its buffer, we push it into its final resting place ...
  // CardCollection's Card vector.
  Card card;
  Cards input_cards;

  if ( fp.load ( filename, value ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to parse JSON input file: " + filename );
    return false;
  }

NOM_ASSERT ( value.type() == json_spirit::array_type );
  values = value.get_array();

  for ( i = 0; i != values.size(); i++ )
  {
NOM_ASSERT ( values[i].type() == json_spirit::obj_type );
    obj = values[i].get_obj();

    for ( o = 0; o != obj.size(); o++ )
    {
      const json_spirit::Pair &pair = obj[o];
      const std::string &path = pair.name_;
      const json_spirit::Value &value = pair.value_;

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

        // The next three lines *MUST* be at the end of the current
        // node object being read in -- or else epic failure will result!
        card.setPlayerID ( Card::NOPLAYER ); // placeholder
        card.setPlayerOwner ( Card::NOPLAYER ); // placeholder
        input_cards.push_back ( card );
      }
    }
  }

// Do a sanity check if we are not a debug version
#ifndef DEBUG
  if ( input_cards.size() < 10 ) // Sanity check
  {
NOM_LOG_ERR ( TTCARDS, "Cards collection data is invalid at file: " + filename );
    return false;
  }
#endif

  // All is well, let us make our freshly loaded data permanent
  this->cards = input_cards;

#ifdef DEBUG_CARD_COLLECTION
  debug.ListCards ( this->cards );
#endif

  return true;
}
