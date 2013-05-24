/******************************************************************************
    card_rules.h

    Card Logic Rules

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_RULES_HEADERS
#define GAMEAPP_CARD_RULES_HEADERS

#include <iostream>
#include <string>

#include "card.h"

// combo: plus, same, wall
// elemental

class CardRules
{
  public:
    CardRules ( void );
    ~CardRules ( void );

    unsigned int getRules ( void );
    void setRules ( unsigned int type );

    bool compareCards ( unsigned int r1, unsigned int r2 );

/*
    enum Rules {
      NONE = 0x00,
      COMBO = 0x01,
      SAME = 0x02,
      WALL = 0x04,
      PLUS = 0x08,
      ELEMENTAL = 0x10,
      LOSER_WINNER = 0x12
    };
*/

  private:
    unsigned int rules; // stores our card game rules in use
};

#endif // GAMEAPP_CARD_RULES_HEADERS defined
