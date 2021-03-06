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
#ifndef GAMEAPP_GAME_CONFIG_HEADERS
#define GAMEAPP_GAME_CONFIG_HEADERS

#include <iostream>
#include <string>
#include <fstream>
#include <map>

#include "nomlib/config.hpp"
#include <nomlib/serializers.hpp>

#include "config.hpp"

/// \brief Simple, convenient file-based settings interface
///
/// \remarks This interface does not allow duplicate key (node) members to be
/// added.
class GameConfig
{
  public:
    GameConfig();
    GameConfig( const std::string& filename);
    ~GameConfig();

    const std::string getString ( const std::string& node ) const;
    const nom::int32 getInteger ( const std::string& node ) const;
    bool get_bool(const std::string& node) const;

    /// \returns std::vector<std::string>
    nom::StringList string_array(const std::string& node) const;

    // const nom::Value& array(const std::string& node) const;

    const nom::Value& setProperty ( const std::string& node, const nom::Value& value );

    /// Load saved board grid data from a file encoded as RFC 4627 compliant
    /// JSON objects.
    ///
    /// \todo Restore the current configuration if we fail to parse config and
    /// have cleared the
    bool load ( const std::string& filename );

  private:
    std::map<std::string, nom::Value> config;
};


#endif // include guard defined
