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

std::vector<Card> CardCollection::getCards ( void )
{
  unsigned int idx = 0;
  std::vector<Card> temp_cards; // temp var for return passing
  temp_cards.clear();

  for ( idx = 0; idx < this->cards.size(); idx++ )
    temp_cards.push_back ( this->cards[idx] );

  return temp_cards;
}

bool CardCollection::save( const std::string& filename )
{
  std::ofstream fp; // output file stream object
  unsigned int idx = 0; // iterator
  json_spirit::Object obj;
  json_spirit::Array values;
  json_spirit::Array ranks;

  if ( this->cards.empty() )
    return false;

  for ( idx = 0; idx < this->cards.size(); idx++ )
  {
    obj.push_back ( json_spirit::Pair ( "ID", (int)this->cards[idx].getID() ) );
    obj.push_back ( json_spirit::Pair ( "Name", this->cards[idx].getName() ) );
    obj.push_back ( json_spirit::Pair ( "Level", (int)this->cards[idx].getLevel() ) );
    obj.push_back ( json_spirit::Pair ( "Type", (int)this->cards[idx].getType() ) );
    obj.push_back ( json_spirit::Pair ( "Element", (int)this->cards[idx].getElement() ) );

    ranks.push_back ( json_spirit::Value ( (int)this->cards[idx].getNorthRank() ) );
    ranks.push_back ( json_spirit::Value ( (int)this->cards[idx].getEastRank() ) );
    ranks.push_back ( json_spirit::Value ( (int)this->cards[idx].getSouthRank() ) );
    ranks.push_back ( json_spirit::Value ( (int)this->cards[idx].getWestRank() ) );

    obj.push_back ( json_spirit::Pair ( "Ranks", ranks ) );

    values.push_back ( obj );

    obj.clear();
    ranks.clear();
  }

  fp.open ( filename );

  if ( fp.is_open() && fp.good() )
  {
    json_spirit::write_stream ( json_spirit::Value ( values ), fp, json_spirit::single_line_arrays );
    fp.close();
    return true;
  }
  else
  {
NOM_LOG_ERR ( TTCARDS, "Unable to save JSON file: " + filename );
    fp.close();
    return false;
  }
}

bool CardCollection::load( const std::string& filename )
{
  unsigned int rdx = 0; // iterator
  unsigned int id = 0;
  unsigned int level = 0;
  unsigned int type = 0;
  unsigned int element = 0;
  std::array<nom::int32, MAX_RANKS> rank = {{ 0 }};
  std::string name = "\0";
  std::ifstream fp; // input file stream object
  json_spirit::Object obj;
  json_spirit::Value value;
  json_spirit::Array values;
  json_spirit::Array::size_type i; // iterator for object values looping
  json_spirit::Object::size_type o; // iterator for object values looping

  fp.open ( filename );

  if ( fp.is_open() && fp.good() )
  {
    if ( json_spirit::read_stream ( fp, value ) == false )
    {
NOM_LOG_ERR ( TTCARDS, "Unable to parse JSON input file: " + filename );
      fp.close();
      return false;
    }
    fp.close();
  }
  else
  {
    fp.close();
    return false;
  }

  assert ( value.type() == json_spirit::array_type );
  values = value.get_array();

  for ( i = 0; i != values.size(); i++ )
  {
    assert ( values[i].type() == json_spirit::obj_type );
    obj = values[i].get_obj();

    for ( o = 0; o != obj.size(); o++ )
    {
      const json_spirit::Pair &pair = obj[o];
      const std::string &path = pair.name_;
      const json_spirit::Value &value = pair.value_;

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
        for ( rdx = 0; rdx < ranks.size(); rdx++ )
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
        this->cards.push_back ( Card ( id, level, type, element, rank, name, Card::NOPLAYER, Card::NOPLAYER ) );
      }
    }
  }

#ifdef DEBUG_CARD_COLLECTION
  debug.ListCards ( this->cards );
#endif

  return true;
}
