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
#ifndef GAMEAPP_BOARD_TILE_HPP
#define GAMEAPP_BOARD_TILE_HPP

#include <iostream>
#include <string>

#include <nomlib/math.hpp>
#include <nomlib/system.hpp>

#include "config.hpp"
#include "Card.hpp"
#include "CardDebug.hpp"

class BoardTile
{
  public:
    /// Default constructor
    BoardTile ( void );

    /// Construct a new board tile
    BoardTile ( const Card& card, const nom::Coords& pos, const nom::uint32 element );

    /// Destructor
    ~BoardTile ( void );

    /// Getter for tile_card instance var
    const Card& tile ( void ) const;

    /// Getter for tile_pos instance var
    const nom::Coords& bounds ( void ) const;

    /// Getter for tile_element instance var
    const nom::uint32 element ( void ) const;

    /// Update an existing tile
    void update ( const nom::Coords& pos, const Card& card );

  private:
    friend class Board;
    Card tile_card;
    nom::Coords tile_pos;
    nom::uint32 tile_element;
    enum BoardPosition board_tile;
};

#endif // include guard defined
