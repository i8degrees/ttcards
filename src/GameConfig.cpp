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

GameConfig::GameConfig()
{
  //
}

GameConfig::GameConfig(const std::string& filename)
{
  if( this->load(filename) == false )
  {
    NOM_LOG_ERR ( TTCARDS_LOG_CATEGORY_CFG, "Could not parse input file: " + filename );
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

bool GameConfig::get_bool(const std::string& node) const
{
  auto itr = config.find(node);

  if( itr == config.end() ) {
    return false;
  }
  else {
    return itr->second.get_bool();
  }
}

nom::StringList GameConfig::string_array(const std::string& node) const
{
  nom::StringList out;

  auto itr = this->config.find(node);

  if( itr != this->config.end() ) {

    NOM_ASSERT( itr->second.array_type() == false );

    nom::Value arr = itr->second;

    for( auto it = arr.begin(); it != arr.end(); ++it ) {
      out.push_back( it->get_string() );
    } // end for loop
  } // end if found

  return out;
}

// const nom::Value& GameConfig::array(const std::string& node) const
// {
//   auto itr = this->config.find(node);

//   if( itr != this->config.end() ) {

//     NOM_ASSERT( itr->second.array_type() == false );
//     return itr->second;

//   } // end if found
// }

const nom::Value& GameConfig::setProperty(  const std::string& node,
                                            const nom::Value& value )
{
  auto res = config.insert ( std::pair<std::string, nom::Value> ( node, value ) ).first;

  if ( value.string_type() )
  {
    NOM_LOG_INFO ( TTCARDS_LOG_CATEGORY_CFG, "GameConfig: " + node + ": " + "\"" + value.get_string() + "\"" + " has been added to the cache." );
  }
  else if( value.int_type() )
  {
    NOM_LOG_INFO ( TTCARDS_LOG_CATEGORY_CFG, "GameConfig: " + node + ": " + std::to_string ( value.get_int() ) + " has been added to the cache." );
  }
  else if( value.bool_type() )
  {
    NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_CFG,
                  "GameConfig: ", node, ":", std::to_string( value.get_bool() ),
                  " has been added to the cache." );
  }
  else
  {
    NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_CFG,
                  "GameConfig: " + node + ": " + " has been added to the cache." );
  }

  return res->second;
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

  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_CFG, objects );
  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_CFG, objects.size() );

  for( auto itr = objects.begin(); itr != objects.end(); ++itr )
  {
    if( itr->object_type() )
    {
      nom::Object obj = itr->object();

      for( auto it = obj.begin(); it != obj.end(); ++it )
      {
        nom::Value::ConstIterator members( it );
        std::string key = members.key();

        NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_CFG, key );

        if( members->string_type() )
        {
          cfg.setProperty( key, members->get_string() );
        }
        else if( members->int_type() )
        {
          cfg.setProperty( key, members->get_int() );
        }
        else if( members->bool_type() )
        {
          cfg.setProperty(key, members->get_bool() );
        }
        else if( members->array_type() )
        {
          cfg.setProperty(key, members->array() );
        }
        else if( members->object_type() )
        {
          cfg.setProperty(key, members->object() );
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
