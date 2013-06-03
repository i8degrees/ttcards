/******************************************************************************
    card.h

  Card container class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_HEADERS
#define GAMEAPP_CARD_HEADERS

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>

#include "cfg.h"

class Card
{
  public:
    Card  (   unsigned int id_ = 0, unsigned int level_ = 0, unsigned int type_ = 0,
              unsigned int element_ = NONE, std::array<int, 4> rank_ = { { 0, 0, 0, 0 } },
              std::string name_ = "\0", unsigned int player_id_ = Card::NOPLAYER,
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

    // card.player_id AKA owner tag
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
    unsigned int element; // NONE is no element
    std::array<int, 4> rank; // NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3
    std::string name;

    // used to distinguish card background and also used to track player in board
    // in order to do card flipping, among other things like score tallying
    unsigned int player_id;

    // Original owner of the card
    unsigned int player_owner;
};

#endif // CARD_HEADERS defined
