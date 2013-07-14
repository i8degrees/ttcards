/******************************************************************************

    Card container class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_HEADERS
#define GAMEAPP_CARD_HEADERS

#include <string>
#include <array>
#include <algorithm>

#include <nomlib/types.hpp>
#include "cfg.h"

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

    unsigned int getID ( void );
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
