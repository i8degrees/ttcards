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
#ifndef GAMEAPP_BOARD_HEADERS
#define GAMEAPP_BOARD_HEADERS

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <fstream>
#include <cassert>

#include "json_spirit_writer_template.h"
#include "json_spirit_reader_template.h"

#include <nomlib/types.hpp>

#include "Card.hpp"
#include "CardDebug.hpp"
#include "CardRules.hpp"
#include "CardView.hpp"
#include "config.hpp"

class Board
{
  public:
    Board ( void );
    Board ( CardRules& ruleset, CardView* view );
    ~Board ( void );

    /// Empties board vector
    void clear ( void );

    std::vector<std::pair<nom::int32, nom::int32>> checkBoard ( nom::int32 x, nom::int32 y );

    // TODO: Consider branching this into Score class
    unsigned int getCount ( void );
    unsigned int getPlayerCount ( unsigned int player_id );

    nom::int32 getStatus ( nom::int32 x, nom::int32 y );
    void updateStatus ( unsigned int x, unsigned int y, Card &card );
    unsigned int getPlayerID ( unsigned int x, unsigned int y );
    void flipCard ( unsigned int x, unsigned int y, unsigned int player_id );
    std::string getName ( unsigned int x, unsigned int y );
    Card &getCard ( unsigned int x, unsigned int y );

    void Update ( unsigned int x, unsigned int y ); // TODO
    void Draw ( void* video_buffer );
    void List ( void );

    /// Save the current board grid data to a file as a series of RFC 4627
    /// compliant JSON objects.
    ///
    /// Order in which data is saved *does* matter and must match to the order
    /// in which it is loaded.
    bool save ( const std::string& filename );

    /// Load saved board grid data from a file encoded as RFC 4627 compliant
    /// JSON objects.
    ///
    /// Loading order *does* matter and expects the order in which it was saved
    /// to match precisely.
    bool load ( const std::string& filename );

    const nom::int32 operator() ( const nom::int32 x, const nom::int32 y );

  private:
    void initialize ( void );
    /// Card rule logic
    CardRules rules;
    /// Card rendering
    CardView* card;
    /// Debug support for card attributes
    CardDebug debug;
    /// 2D vector of Card data containers
    std::vector<std::vector<Card>> grid;
};

#endif // GAMEAPP_BOARD_HEADERS defined
