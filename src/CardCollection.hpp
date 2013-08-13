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
#ifndef GAMEAPP_CARD_COLLECTION_HEADERS
#define GAMEAPP_CARD_COLLECTION_HEADERS

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include <json_spirit_reader_template.h>
#include <json_spirit_writer_template.h>

#ifndef JSON_SPIRIT_VALUE_ENABLED
  #define JSON_SPIRIT_VALUE_ENABLED
#endif

#include "card.hpp"
#include "card_debug.hpp"
#include "config.hpp"

class CardCollection
{
  public:
    CardCollection ( void );
    ~CardCollection ( void );

    bool LoadJSON ( std::string filename );
    bool LoadASCII ( std::string filename );

    bool ExportASCII ( std::string filename ); // NOT IMPLEMENTED
    bool ExportJSON ( std::string filename );

    // TODO: a) ERR handling; b) reconsider how we pass
    Card& getCards ( unsigned int idx );
    std::vector<Card> getCards ( void );

    void clear ( void );
    nom::int32 size ( void ) const;

    std::vector<Card> cards; // TODO: redeclare private scope
  private:
    /// debug support for card attributes
    CardDebug debug;
};

#endif // GAMEAPP_CARD_COLLECTION_HEADERS defined
