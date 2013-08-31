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

GameConfig::GameConfig( void )
{
TTCARDS_LOG_CLASSINFO;
}

GameConfig::~GameConfig( void )
{
TTCARDS_LOG_CLASSINFO;
}

const std::string GameConfig::getString ( const std::string& node ) const
{
  auto itr = config.find ( node );

  if ( itr == config.end() )
  {
    return "\0";
  }
  else
  {
    return itr->second.get_str();
  }
}

const int GameConfig::getInteger ( const std::string& node ) const
{
  auto itr = config.find ( node );

  if ( itr == config.end() )
  {
    return -1;
  }
  else
  {
    return itr->second.get_int();
  }
}

const json_spirit::Value GameConfig::setProperty ( const std::string& node, const json_spirit::Value& value )
{
  auto res = config.insert ( std::pair<std::string, json_spirit::Value> ( node, value ) ).first;

TTCARDS_LOG_INFO ( "GameConfig: " + node + " has been added to the cache." );

  return res->second;
}

bool GameConfig::save( const std::string& filename )
{
  std::ofstream fp; // output file handle
  json_spirit::Object node; // current JSON node we are writing
  json_spirit::Array game; // overall data to be written out

  // Order in which we save node paths does not matter
  node.push_back ( json_spirit::Pair ( "SCALE_ALGORITHM", this->getString("SCALE_ALGORITHM") ) );

  // Fonts
  node.push_back ( json_spirit::Pair ( "SCORE_FONTFACE", this->getString("SCORE_FONTFACE") ) );
  node.push_back ( json_spirit::Pair ( "GAMEOVER_FONTFACE", this->getString("GAMEOVER_FONTFACE") ) );
  node.push_back ( json_spirit::Pair ( "CARD_FONTFACE", this->getString("CARD_FONTFACE") ) );
  node.push_back ( json_spirit::Pair ( "INFO_FONTFACE", this->getString("INFO_FONTFACE") ) );
  node.push_back ( json_spirit::Pair ( "INFO_SMALL_FONTFACE", this->getString("INFO_SMALL_FONTFACE") ) );

  // Sprites & static backgrounds
  node.push_back ( json_spirit::Pair ( "BOARD_BACKGROUND", this->getString("BOARD_BACKGROUND") ) );
  node.push_back ( json_spirit::Pair ( "GAMEOVER_BACKGROUND", this->getString("GAMEOVER_BACKGROUND") ) );
  node.push_back ( json_spirit::Pair ( "CARD_ELEMENTS", this->getString("CARD_ELEMENTS") ) );
  node.push_back ( json_spirit::Pair ( "CARD_FACES", this->getString("CARD_FACES") ) );
  node.push_back ( json_spirit::Pair ( "CARD_BACKGROUNDS", this->getString("CARD_BACKGROUNDS" ) ) );
  node.push_back ( json_spirit::Pair ( "INTERFACE_CURSOR", this->getString("INTERFACE_CURSOR" ) ) );
  node.push_back ( json_spirit::Pair ( "MENU_ELEMENTS", this->getString("MENU_ELEMENTS" ) ) );

  // Audio effects
  node.push_back ( json_spirit::Pair ( "CURSOR_MOVE", this->getString("CURSOR_MOVE") ) );
  node.push_back ( json_spirit::Pair ( "CURSOR_CANCEL", this->getString("CURSOR_CANCEL") ) );
  node.push_back ( json_spirit::Pair ( "CURSOR_WRONG", this->getString("CURSOR_WRONG") ) );
  node.push_back ( json_spirit::Pair ( "CARD_FLIP", this->getString("CARD_FLIP") ) );
  node.push_back ( json_spirit::Pair ( "CARD_PLACE", this->getString("CARD_PLACE") ) );
  node.push_back ( json_spirit::Pair ( "SFX_LOAD_GAME", this->getString("SFX_LOAD_GAME") ) );
  node.push_back ( json_spirit::Pair ( "SFX_SAVE_GAME", this->getString("SFX_SAVE_GAME") ) );
  node.push_back ( json_spirit::Pair ( "MUSIC_TRACK", this->getString("MUSIC_TRACK") ) );
  node.push_back ( json_spirit::Pair ( "MUSIC_WIN_TRACK", this->getString("MUSIC_WIN_TRACK") ) );

  // Miscellaneous
  node.push_back ( json_spirit::Pair ( "APP_ICON", this->getString("APP_ICON") ) );
  node.push_back ( json_spirit::Pair ( "CARDS_DB", this->getString("CARDS_DB") ) );

/*
  node.push_back ( json_spirit::Pair ( "USER_BOARD_FILENAME", this->getString("USER_BOARD_FILENAME") ) );
  node.push_back ( json_spirit::Pair ( "USER_PLAYER1_FILENAME", this->getString("USER_PLAYER1_FILENAME") ) );
  node.push_back ( json_spirit::Pair ( "USER_PLAYER2_FILENAME", this->getString("USER_PLAYER2_FILENAME") ) );
*/

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

  if ( ! value.type() == json_spirit::array_type )
  {
TTCARDS_LOG_ERR( "Unable to parse JSON input file: " + filename );
    return false;
  }

  game = value.get_array();

  for ( i = 0; i != game.size(); i++ )
  {
    if ( ! game[i].type() == json_spirit::obj_type )
    {
TTCARDS_LOG_ERR( "Unable to parse JSON input file: " + filename );
      return false;
    }

    node = game[i].get_obj();

    for ( o = 0; o != node.size(); o++ )
    {
      const json_spirit::Pair& pair = node[o];
      const std::string& path = pair.name_;
      const json_spirit::Value& value = pair.value_;

      // Order in which we save node paths does not matter
      if ( path == "APP_ICON" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "BOARD_BACKGROUND" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "CARDS_DB" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "CARD_BACKGROUNDS" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "CARD_ELEMENTS" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "CARD_FACES" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "CARD_FLIP" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "CARD_FONTFACE" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "CARD_PLACE" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "CURSOR_CANCEL" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "CURSOR_MOVE" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "CURSOR_WRONG" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "GAMEOVER_BACKGROUND" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "GAMEOVER_FONTFACE" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "INFO_FONTFACE" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "INFO_SMALL_FONTFACE" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "INTERFACE_CURSOR" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "MENU_ELEMENTS" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "MUSIC_TRACK" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "MUSIC_WIN_TRACK" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "SCALE_ALGORITHM" )
      {
        if ( value.get_str() == "scale2x" || value.get_str() == "hqx" )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "SCORE_FONTFACE" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "SFX_LOAD_GAME" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "SFX_SAVE_GAME" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "USER_BOARD_FILENAME" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "USER_PLAYER1_FILENAME" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
      else if ( path == "USER_PLAYER2_FILENAME" )
      {
        if ( value.type() == json_spirit::str_type )
        {
          this->setProperty ( path, value.get_str() );
        }
      }
    } // end current node loop
  } // end current array node

  return true;
}
