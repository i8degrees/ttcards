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
#ifndef GAMEAPP_CARD_RULES_HEADERS
#define GAMEAPP_CARD_RULES_HEADERS

#include <iostream>
#include <string>

#include "Card.hpp"

// combo: plus, same, wall
// elemental

class CardRules
{
  public:

    /// Default constructor (no game rule-set)
    CardRules ( void );

    /// Destructor
    ~CardRules ( void );

    CardRules ( nom::uint32 ruleset );

    nom::uint32 getRules ( void );
    void setRules ( nom::uint32 type );

    bool compareCards ( unsigned int r1, unsigned int r2 );

    /// Combo ruleset applies automatically when Same, SameWall or Plus or
    /// has been triggered.
    enum
    {
      NoRules = 0,  // Default (enables opponent's card face down)
      Open,         // Inverse of the default above; show the opponent's cards
      Elemental,
      Same,
      SameWall,
      Plus,
      Combo,
      SuddenDeath,
      LoserWinner,
      Random // Random choice from this list
    };

    enum TradeRules
    {

      One=1, // Winner takes any one of the opponent's cards!
      // Winner takes as many cards as there is a difference between yours
      // and theirs (of flipped cards).
      Diff,
      // Winner takes cards that are their color at the end of the game
      Direct,

      All // Winner takes all..!
    };

  private:
    nom::uint32 rules; // stores our card game rules in use
};

#endif // GAMEAPP_CARD_RULES_HEADERS defined
