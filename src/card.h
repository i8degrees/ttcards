/******************************************************************************
    card.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_HEADERS
#define GAMEAPP_CARD_HEADERS

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>

#include "SDL/SDL.h"

#include "cfg.h"

//#include "card_debug.h"

//#define DEBUG_CARD
//#define DEBUG_CARD_OBJ

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
    unsigned int element;
    std::array<int, 4> rank;
    std::string name;

    // FIXME
    unsigned int player_id;

    // Player's Card Elemental type
    enum CardElement {
      NONE=0, // type 0; reserved / debug
      EARTH=1, // type 1 element
      FIRE=2, // type 2 element
      HOLY=3, // type 3 element
      ICE=4, // type 4 element
      POISON=5, // type 5 element
      THUNDER=6, // type 6 element
      WATER=7, // type 7 element
      WIND=8 // type 8 element
    };

    // Player's  Level Category Bracket
    enum CardType {
      MONSTER=1,
      BOSS=2,
      GF=3,
      PLAYER=4
    };

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
