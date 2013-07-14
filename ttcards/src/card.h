/******************************************************************************

    Card container class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_HEADERS
#define GAMEAPP_CARD_HEADERS

#include <iostream>
#include <fstream>
#include <vector>
#include <array>

#include "cfg.h"

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

    void setID ( unsigned int id_ );
    void setLevel ( unsigned int level_ );
    void setType ( unsigned int type_ );
    void setElement ( unsigned int element_ );
    void setNorthRank ( unsigned int rank );
    void setEastRank ( unsigned int rank );
    void setSouthRank ( unsigned int rank );
    void setWestRank ( unsigned int rank );
    void setName ( std::string name_ );

    /// card.player_id AKA owner tag
    enum
    {
      NOPLAYER=0,
      PLAYER1=1,
      PLAYER2=2
    };

    unsigned int getPlayerID ( void );
    void setPlayerID ( unsigned int player_id_ );

    unsigned int getPlayerOwner ( void );
    void setPlayerOwner ( unsigned int player_owner_ );

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
