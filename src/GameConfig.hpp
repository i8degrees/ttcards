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
#ifndef GAMEAPP_GAME_CONFIG_HEADERS
#define GAMEAPP_GAME_CONFIG_HEADERS

#include <iostream>
#include <string>
#include <fstream>
#include <map>

#include "json_spirit_writer_template.h"
#include "json_spirit_reader_template.h"

#include <nomlib/types.hpp>

#include "config.hpp"
/*
template <typename T>
class ConfigValue
{
  public:
    ConfigValue ( T value ) { this->set ( value ); }
    ~ConfigValue ( void ) {}

    T get ( void ) { return this->value; }

    void set ( T value ) { this->value = value; }

  private:
    T value;
};

typedef ConfigValue<nom::int32> ConfigIntegerValue;
typedef ConfigValue<std::string> ConfigStringValue;
*/
class GameConfig
{
  public:
    GameConfig( void );
    GameConfig ( const std::string& filename );
    ~GameConfig( void );

    const std::string getString ( const std::string& node ) const;
    const nom::int32 getInteger ( const std::string& node ) const;

    void init ( const std::string& node, const json_spirit::Value& value )
    {
      if ( value.type() == json_spirit::str_type )
      {
        this->config.at(node) = value.get_str();
      }
      else if ( value.type() == json_spirit::int_type )
      {
        this->config.at(node) = value.get_int();
      }
    }

    //void insert ( const std::string& node, nom::uint32 flags = 0 );

    //const T setProperty ( const std::string& node, const json_spirit::Value& value );
    const json_spirit::Value setProperty ( const std::string& node, const json_spirit::Value& value );

    /// Save the current board grid data to a file as a series of RFC 4627
    /// compliant JSON objects.
    bool save ( const std::string& filename );

    /// Load saved board grid data from a file encoded as RFC 4627 compliant
    /// JSON objects.
    ///
    /// \todo Restore the current configuration if we fail to parse config and
    /// have cleared the
    bool load ( const std::string& filename );

  private:
    std::map<std::string, json_spirit::Value> config;
    //std::vector<std::pair<std::string, nom::uint32>> nodes;
};


#endif // include guard defined
