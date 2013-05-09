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
    Card (  unsigned int id = 0, unsigned int level = 0, unsigned int type = 0,
            unsigned int element = 0, std::array<int, 4> rank = { { 0, 0, 0, 0 } },
            std::string name = "\0", unsigned int player_id = 0 );

    ~Card ( void );

    unsigned int id;
    unsigned int level;
    unsigned int type;
    unsigned int element; // NONE is no element
    std::array<int, 4> rank; // NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3
    std::string name;

    // FIXME
    unsigned int player_id;

    // Card ( id, level, type, element, power[], name );
    // getID();
    // getLevel();
    // getType();
    // getElement();
    // getRank();
    // getName();
    // isFaceUp();
    // Flip();
    // Color();

  private:
    // ...
};

#endif // CARD_HEADERS defined
