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
#ifndef GAMEAPP_CARD_HEADERS
#define GAMEAPP_CARD_HEADERS

#include <string>
#include <array>
#include <algorithm>

#include <nomlib/types.hpp>

#include "config.hpp"

/// Maximum level a card can contain -- starting at 1
const nom::int32 MAX_LEVEL = 10;

/// Maximum type a card can contain -- starting at 1
const nom::int32 MAX_TYPE = 4;

/// Maximum element a card can contain -- starting at 1
const nom::int32 MAX_ELEMENT = 8;

/// Maximum rank a card can contain -- starting at 1
const nom::int32 MAX_RANK = 10;

/// Maximum name length a card can contain
const nom::int32 MAX_NAME = 14;

class Card
{
  public:
    Card ( void );

    Card  (   unsigned int id_, unsigned int level_, unsigned int type_,
              unsigned int element_, std::array<int, 4> rank_,
              std::string name_, unsigned int player_id_,
              unsigned int player_owner_ = Card::NOPLAYER
          );

    ~Card ( void );

    unsigned int getID ( void ) const;
    unsigned int getLevel ( void );
    unsigned int getType ( void );
    unsigned int getElement ( void );
    std::array<int, 4> getRanks ( void );
    unsigned int getNorthRank ( void );
    unsigned int getEastRank ( void );
    unsigned int getSouthRank ( void );
    unsigned int getWestRank ( void );
    std::string getName ( void );

    unsigned int getPlayerID ( void );
    unsigned int getPlayerOwner ( void );

    /// Clamps value to MAX_COLLECTION
    void setID ( unsigned int id_ );

    /// Clamps value to MAX_LEVEL
    void setLevel ( unsigned int level_ );

    /// Clamps value to MAX_TYPE
    void setType ( unsigned int type_ );

    /// Clamps value to MAX_ELEMENT
    void setElement ( unsigned int element_ );

    /// Clamps array values to MAX_RANK
    void setRanks ( std::array<nom::int32, 4> ranks );

    /// Clamps value to MAX_RANK
    void setNorthRank ( unsigned int rank );

    /// Clamps value to MAX_RANK
    void setEastRank ( unsigned int rank );

    /// Clamps value to MAX_RANK
    void setSouthRank ( unsigned int rank );

    /// Clamps value to MAX_RANK
    void setWestRank ( unsigned int rank );

    /// Clamps value to MAX_NAME
    void setName ( std::string name_ );

    /// Clamps value to TOTAL_PLAYERS
    void setPlayerID ( unsigned int player_id_ );

    /// Clamps value to TOTAL_PLAYERS
    void setPlayerOwner ( unsigned int player_owner_ );

    /// card.player_id AKA owner tag
    enum
    {
      NOPLAYER=0,
      PLAYER1=1,
      PLAYER2=2
    };

  private:
    unsigned int id;
    unsigned int level;
    unsigned int type;
    /// NONE is no element
    unsigned int element;
    /// NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3
    std::array<int, 4> rank;
    std::string name;

    /// Additional field; used to distinguish card background and also used to
    // track player in board in order to do card flipping, among other things
    // like score tallying
    unsigned int player_id;

    /// Additional field; original owner of the card
    unsigned int player_owner;
};

#endif // GAMEAPP_CARD_HEADERS defined
