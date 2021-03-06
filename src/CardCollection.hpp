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
#ifndef GAMEAPP_CARD_COLLECTION_HEADERS
#define GAMEAPP_CARD_COLLECTION_HEADERS

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <nomlib/serializers.hpp>

#include "Card.hpp"
#include "CardDebug.hpp"
#include "config.hpp"

class CardCollection
{
  public:
    CardCollection ( void );
    ~CardCollection ( void );

    Card& getCards ( unsigned int idx );
    Cards getCards ( void );

    void clear ( void );
    nom::uint32 size ( void ) const;

    /// Save the current board grid data to a file as a series of RFC 4627
    /// compliant JSON objects.
    bool save( const std::string& filename );

    /// Load saved board grid data from a file encoded as RFC 4627 compliant
    /// JSON objects.
    bool load( const std::string& filename );

    const Card& lookup_by_name( const std::string& name ) const;
    const Card& lookup_by_id( int id ) const;

    /// \todo redeclare private scope
    Cards cards;

  private:
    /// debug support for card attributes
    CardDebug debug;
};

#endif // GAMEAPP_CARD_COLLECTION_HEADERS defined
