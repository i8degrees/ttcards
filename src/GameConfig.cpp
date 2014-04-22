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
#include "GameConfig.hpp"

GameConfig::GameConfig ( void )
{
  //
}

GameConfig::GameConfig ( const std::string& filename )
{
  if ( this->load ( filename ) == false )
  {
    NOM_LOG_ERR ( TTCARDS, "Could not parse input file: " + filename );
  }
}

GameConfig::~GameConfig ( void )
{
  //
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
    return itr->second.get_string();
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
    return itr->second.get_int();
  }
}

const nom::Value& GameConfig::setProperty ( const std::string& node, const nom::Value& value )
{
  auto res = config.insert ( std::pair<std::string, nom::Value> ( node, value ) ).first;

  if ( value.string_type() )
  {
    NOM_LOG_INFO ( TTCARDS, "GameConfig: " + node + ": " + "\"" + value.get_string() + "\"" + " has been added to the cache." );
  }
  else if ( value.int_type() )
  {
    NOM_LOG_INFO ( TTCARDS, "GameConfig: " + node + ": " + std::to_string ( value.get_int() ) + " has been added to the cache." );
  }
  else
  {
    NOM_LOG_INFO ( TTCARDS, "GameConfig: " + node + " has been added to the cache." );
  }

  return res->second;
}

bool GameConfig::save( const std::string& filename )
{
  // High-level JSON interface
  nom::IValueSerializer* fp;

  // Temporary buffer used to collect data to be
  // stored and processed.
  nom::Value object;

  fp = new nom::JsonCppSerializer();

  for ( auto it = this->config.begin(); it != this->config.end(); ++it )
  {
    #if defined( TTCARDS_DEBUG_GAME_CONFIG_SAVE )
      std::cout << it->first << ": ";
    #endif

    if ( it->second.string_type() )
    {
      #if defined( TTCARDS_DEBUG_GAME_CONFIG_SAVE )
        std::cout << it->second.get_string();
      #endif
    }
    else if ( it->second.int_type() )
    {
      #if defined( TTCARDS_DEBUG_GAME_CONFIG_SAVE )
        std::cout << it->second.get_int();
      #endif
    }
    else
    {
      #if defined( TTCARDS_DEBUG_GAME_CONFIG_SAVE )
        std::cout << "null";
      #endif
    }

    #if defined( TTCARDS_DEBUG_GAME_CONFIG_SAVE )
      std::cout << std::endl;
    #endif
  }

  // Order in which we save node paths does not matter
  object["root"]["SCALE_ALGORITHM"] = this->getString("SCALE_ALGORITHM");

  // Fonts
  object["root"]["SCORE_FONTFACE"] = this->getString("SCORE_FONTFACE");
  object["root"]["GAMEOVER_FONTFACE"] = this->getString("GAMEOVER_FONTFACE");
  object["root"]["CARD_FONTFACE"] = this->getString("CARD_FONTFACE");
  object["root"]["INFO_FONTFACE"] = this->getString("INFO_FONTFACE");
  object["root"]["INFO_SMALL_FONTFACE"] = this->getString("INFO_SMALL_FONTFACE");

  // Sprites & static backgrounds
  object["root"]["BOARD_BACKGROUND"] = this->getString("BOARD_BACKGROUND");
  object["root"]["GAMEOVER_BACKGROUND"] =  this->getString("GAMEOVER_BACKGROUND");
  object["root"]["CARD_ELEMENTS"] = this->getString("CARD_ELEMENTS");
  object["root"]["CARD_FACES"] = this->getString("CARD_FACES");
  object["root"]["CARD_BACKGROUNDS"] = this->getString("CARD_BACKGROUNDS");
  object["root"]["INTERFACE_CURSOR"] = this->getString("INTERFACE_CURSOR");
  object["root"]["MENU_ELEMENTS"] = this->getString("MENU_ELEMENTS");

  // Audio effects
  object["root"]["CURSOR_MOVE"] = this->getString("CURSOR_MOVE");
  object["root"]["CURSOR_CANCEL"] = this->getString("CURSOR_CANCEL");
  object["root"]["CURSOR_WRONG"] = this->getString("CURSOR_WRONG");
  object["root"]["CARD_FLIP"] = this->getString("CARD_FLIP");
  object["root"]["CARD_PLACE"] = this->getString("CARD_PLACE");
  object["root"]["SFX_LOAD_GAME"] = this->getString("SFX_LOAD_GAME");
  object["root"]["SFX_SAVE_GAME"] = this->getString("SFX_SAVE_GAME");
  object["root"]["MUSIC_TRACK"] = this->getString("MUSIC_TRACK");
  object["root"]["MUSIC_WIN_TRACK"] = this->getString("MUSIC_WIN_TRACK");

  // Miscellaneous
  object["root"]["APP_ICON"] = this->getString("APP_ICON");
  object["root"]["CARDS_DB"] = this->getString("CARDS_DB");

  object["root"]["USER_BOARD_FILENAME"] = this->getString("USER_BOARD_FILENAME");
  object["root"]["USER_PLAYER1_FILENAME"] = this->getString("USER_PLAYER1_FILENAME");
  object["root"]["USER_PLAYER2_FILENAME"] = this->getString("USER_PLAYER2_FILENAME");

  #if defined( TTCARDS_DEBUG_GAME_CONFIG_SAVE )
    NOM_DUMP( object );
  #endif

  // Commit data to our top-level node object; this creates a top-level JSON
  // object called "root" to store everything under.
  if( fp->save( object, filename ) == false )
  {
    NOM_LOG_ERR( TTCARDS, "Failed to serialize JSON in file: " + filename );
    return false;
  }

  return true;
}

bool GameConfig::load( const std::string& filename )
{
  // High-level file I/O interface
  nom::IValueDeserializer* fp = new nom::JsonCppDeserializer();
  std::string key;
  nom::Value objects;

  // Storage buffer for our configuration we are loading in; if everything is
  // successful, we will overwrite the existing configuration map with this one.
  GameConfig cfg;

  if ( fp->load( filename, objects ) == false )
  {
    NOM_LOG_ERR ( NOM, "Unable to open JSON file at: " + filename );
    return false;
  }

  #if defined( TTCARDS_DEBUG_GAME_CONFIG_LOAD )
    NOM_DUMP( objects );
    NOM_DUMP( objects.size() );
  #endif

  for( auto itr = objects.begin(); itr != objects.end(); ++itr )
  {
    if( itr->object_type() )
    {
      nom::Object obj = itr->object();

      for( auto it = obj.begin(); it != obj.end(); ++it )
      {
        nom::Value::ConstIterator members( it );
        std::string key = members.key();

        #if defined( TTCARDS_DEBUG_GAME_CONFIG_LOAD )
          NOM_DUMP( key );
        #endif

        if( members->string_type() )
        {
          cfg.setProperty( key, members->get_string() );
        }
        else if( members->int_type() )
        {
          cfg.setProperty( key, members->get_int() );
        }
      }
    }
  }

  // If we have made it this far, go ahead and overwrite our new configuration
  // onto the existing configuration map store!
  this->config = cfg.config;

  // Sanity check
  // NOM_ASSERT( this->config.size() == objects.size() );

  return true;
}
