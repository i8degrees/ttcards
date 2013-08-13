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
#include "GameConfig.hpp"

GameConfig::GameConfig( void ) {}
GameConfig::~GameConfig( void ) {}

bool GameConfig::save( const std::string& filename )
{
  std::ofstream fp; // output file handle
  json_spirit::Object node; // current JSON node we are writing
  json_spirit::Array game; // overall data to be written out

  // Initial card attributes (everything other than ranks)
  //node.push_back ( json_spirit::Pair ( "CARD_WIDTH", (nom::int32) CARD_WIDTH ) );

  // Card rank attributes
  //ranks.push_back ( json_spirit::Value ( (int)this->grid[x][y].getNorthRank() ) );

  // Push ranks values to our current node
  //node.push_back ( json_spirit::Pair ( "Ranks", ranks ) );

  // Push current node to our overall game data to be written
  game.push_back ( node );

  fp.open ( filename );

  if ( fp.is_open() && fp.good() )
  {
    json_spirit::write_stream ( json_spirit::Value ( game ), fp, json_spirit::single_line_arrays );
    fp.close();
    return true;
  }
  else
  {
TTCARDS_LOG_ERR( "Unable to save JSON file: " + filename );
    fp.close();
    return false;
  }
}

bool GameConfig::load( const std::string& filename )
{
  std::ifstream fp; // input file handle
  json_spirit::Object node;
  json_spirit::Value value;
  json_spirit::Array game;

  // Iterators
  json_spirit::Array::size_type i;
  json_spirit::Object::size_type o;

  fp.open ( filename );

  if ( fp.is_open() && fp.good() )
  {
    if ( json_spirit::read_stream ( fp, value ) == false )
    {
TTCARDS_LOG_ERR( "Unable to parse JSON input file: " + filename );
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

      // ...

    } // end current node loop
  } // end current array node

  return true;
}
