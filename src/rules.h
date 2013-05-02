/******************************************************************************
    rules.h

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

    unsigned int GetRules ( void );
    void SetRules ( unsigned int type );

    bool CompareCards ( unsigned int r1, unsigned int r2 );

/*
    enum Rules {
      NONE = 0x00,
      SAME = 0x01,
      WALL = 0x02,
      PLUS = 0x04,
      ELEMENTAL = 0x08,
      LOSER_WINNER = 0x10
    };
*/

  private:
    unsigned int rules; // stores our card game rules in use
};

#endif // GAMEAPP_CARD_RULES_HEADERS defined
