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
#ifndef GAMEAPP_BOARD_HEADERS
#define GAMEAPP_BOARD_HEADERS

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <fstream>

#include "nomlib/config.hpp"
#include <nomlib/math.hpp>
#include <nomlib/serializers.hpp>

#include "config.hpp"
#include "resources.hpp"
#include "BoardTile.hpp"
#include "Card.hpp"
#include "CardDebug.hpp"
#include "CardRules.hpp"
#include "CardView.hpp"

class Board
{
  public:
    Board ( void );
    Board ( CardRules& ruleset, CardView* view );
    ~Board ( void );

    /// Empties board vector
    void clear ( void );

    /// Translates local board coordinates into positioning coordinates used by
    /// display rendering (units are naturally in pixels) -- usefulness ranges
    /// from collision inputs detection to interface cursor navigation.
    ///
    /// Input x, y arguments is in local board grid coordinates:
    ///
    ///   0, 0    1, 0    2, 0
    ///   0, 1    1, 1    2, 1
    ///   0, 2    1, 2    2, 2
    ///
    /// Returns a nom::IntRect filled with the global coordinate values; the
    /// width and height member values are the bounding boxes of each local grid
    /// coordinate. The x & y member values are the input grid coordinates.
    ///
    /// Returns -1, -1 when undefined.
    const nom::IntRect getGlobalBounds ( nom::int32 x, nom::int32 y ) const;

    const std::vector<BoardTile> find_adjacent ( nom::int32 x, nom::int32 y ) const;

    std::vector<std::pair<nom::int32, nom::int32>> checkBoard ( nom::int32 x, nom::int32 y );

    /// Getter helper method for obtaining total count of placed cards on board
    nom::uint32 getCount ( void );

    /// get card count by player's cards on board
    nom::uint32 getPlayerCount ( nom::int32 player_id );

    /// Getter helper method for obtaining card ID at x, y coords
    const nom::int32 getStatus ( nom::int32 x, nom::int32 y ) const;

    /// Setter helper method for placing a card on the board at x, y coords
    void updateStatus ( nom::int32 x, nom::int32 y, const Card& card );

    /// Getter helper method for obtaining player owner tag / ID on a card at x, y
    /// coords
    const nom::int32 getPlayerID ( nom::int32 x, nom::int32 y ) const;

    /// Setter helper method for swapping player owner tags / IDs on a card at x, y
    /// coords
    void flipCard ( nom::int32 x, nom::int32 y, nom::int32 player_id );

    /// Getter helper method for obtaining card name at x, y coords
    const std::string getName ( nom::int32 x, nom::int32 y ) const;

    /// (Private) Getter helper method for obtaining card placed at x, y coords;
    /// Used within Board::Draw(), Game::showCardInfoBox() method calls
    const Card& get ( nom::int32 x, nom::int32 y ) const;

    const BoardTile& tile ( nom::int32 x, nom::int32 y ) const;

    void update ( void );

    /// Draws our active board grid based on their values (card IDs)
    void draw ( nom::IDrawable::RenderTarget& target );

    /// Pretty print the current board tiles
    void list ( void );

    /// Save the current board grid data to a file as a series of RFC 4627
    /// compliant JSON objects.
    bool save ( const std::string& filename );

    /// Load saved board grid data from a file encoded as RFC 4627 compliant
    /// JSON objects.
    bool load ( const std::string& filename );

    const nom::int32 operator() ( const nom::int32 x, const nom::int32 y );

    // std::vector<BoardTile> free_tiles() const;

  private:
    void initialize ( void );

    /// Card rule logic
    CardRules rules;

    /// Card rendering
    CardView* card;

    /// Debug support for card attributes
    CardDebug debug;

    /// 2D vector of BoardTiles
    std::vector<std::vector<BoardTile>> grid;

    /// X, Y translation coordinates for mapping local to global positions on
    /// the board
    nom::IntRect board_map[9];
};

#endif // GAMEAPP_BOARD_HEADERS defined
