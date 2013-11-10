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

GameConfig::GameConfig ( void ) {}

GameConfig::GameConfig ( const std::string& filename )
{
  if ( this->load ( filename ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not parse input file: " + filename );
  }
}

GameConfig::~GameConfig ( void ) {}

const std::string GameConfig::getString ( const std::string& node ) const
{
  auto itr = config.find ( node );

  if ( itr == config.end() )
  {
    return "\0";
  }
  else
  {
    return itr->second.asString();
  }
}

const nom::int32 GameConfig::getInteger ( const std::string& node ) const
{
  auto itr = config.find ( node );

  if ( itr == config.end() )
  {
    return -1;
  }
  else
  {
    return itr->second.asInt();
  }
}
/*
void GameConfig::insert ( const std::string& node, nom::uint32 flags )
{
  this->nodes.push_back ( std::make_pair ( node, flags ) );
}
*/
const nom::JSON::ValueType& GameConfig::setProperty ( const std::string& node, const nom::JSON::ValueType& value )
{
  auto res = config.insert ( std::pair<std::string, nom::JSON::ValueType> ( node, value ) ).first;

  if ( value.type() == nom::JSON::String )
  {
NOM_LOG_INFO ( TTCARDS, "GameConfig: " + node + ": " + "\"" + value.asString() + "\"" + " has been added to the cache." );
  }
  else if ( value.type() == nom::JSON::Integer )
  {
NOM_LOG_INFO ( TTCARDS, "GameConfig: " + node + ": " + std::to_string ( value.asInt() ) + " has been added to the cache." );
  }
  else
  {
NOM_LOG_INFO ( TTCARDS, "GameConfig: " + node + " has been added to the cache." );
  }

  return res->second;
}

bool GameConfig::save( const std::string& filename )
{
/* TODO
  std::ofstream fp; // output file handle
  json_spirit::Object node; // current JSON node we are writing
  json_spirit::Array game; // overall data to be written out

  for ( auto it = this->config.begin(); it != this->config.end(); ++it )
  {
    std::cout << it->first << ": ";
    if ( it->second.type() == json_spirit::str_type )
    {
      std::cout << it->second.get_str();
    }
    else if ( it->second.type() == json_spirit::int_type )
    {
      std::cout << it->second.get_int();
    }
    else
    {
      std::cout << "null";
    }
    std::cout << std::endl;
  }

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


  //node.push_back ( json_spirit::Pair ( "USER_BOARD_FILENAME", this->getString("USER_BOARD_FILENAME") ) );
  //node.push_back ( json_spirit::Pair ( "USER_PLAYER1_FILENAME", this->getString("USER_PLAYER1_FILENAME") ) );
  //node.push_back ( json_spirit::Pair ( "USER_PLAYER2_FILENAME", this->getString("USER_PLAYER2_FILENAME") ) );

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
NOM_LOG_ERR ( TTCARDS, "Unable to save JSON file: " + filename );
    fp.close();
    return false;
  }
TODO */
  return false;
}

bool GameConfig::load( const std::string& filename )
{
  nom::JSON::FileReader fp; // JSON parser
  nom::JSON::Value root; // JSON container

  // Storage buffer for our configuration we are loading in; if everything is
  // successful, we will overwrite the existing configuration map with this one.
  GameConfig cfg;

  if ( fp.load ( filename, root ) == false )
  {
NOM_LOG_ERR ( NOM, "Unable to open JSON file at: " + filename );
    return false;
  }

  nom::JSON::JSONMemberType members = root.members ( 0 );

  for ( auto idx = 0; idx != members.size(); ++idx )
  {
    std::string key = members[idx];

    if ( root.type ( key, 0 ) == nom::JSON::String )
    {
      cfg.setProperty ( key, root.get_string ( key, 0 ) );
    }
    else if ( root.type ( key, 0 ) == nom::JSON::Integer )
    {
      cfg.setProperty ( key, root.get_int ( key, 0 ) );
    }
  }

  // If we have made it this far, go ahead and overwrite our new configuration
  // onto the existing configuration map store!
  this->config = cfg.config;

  return true;
}
/*
    for ( o = 0; o != node.size(); o++ )
    {
      const json_spirit::Pair& pair = node[o];
      const std::string& path = pair.name_;
      const json_spirit::Value& value = pair.value_;

      for ( nom::int32 idx = 0; idx != this->nodes.size(); idx++ )
      {
        if ( this->nodes[idx].first == path )
        {
          if ( value.type() == json_spirit::str_type )
          {
            cfg.setProperty ( path, value.get_str() );
          }
          else if ( value.type() == json_spirit::int_type )
          {
            if ( this->nodes[idx].second == 1 )
            {
              cfg.setProperty ( path, value.get_int() * 2 );
            }
          }
        }
      }
    } // end current node loop
*/
