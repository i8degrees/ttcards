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
#include "card_rules.hpp"

CardRules::CardRules ( void )
{
  #ifdef DEBUG_CARD_RULES_OBJ
    std::cout << "CardRules::CardRules (): Hello, world!" << "\n" << std::endl;
  #endif

  this->rules = 0;
}

CardRules::~CardRules ( void )
{
  #ifdef DEBUG_CARD_RULES_OBJ
    std::cout << "CardRules::~CardRules (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  this->rules = 0;
}

unsigned int CardRules::getRules ( void )
{
  return this->rules;
}

void CardRules::setRules ( unsigned int type )
{
  this->rules = type;
}

bool CardRules::compareCards ( unsigned int r1, unsigned int r2 )
{
  if ( this->getRules() == 0 ) // NONE
  {
    if ( r1 > r2 )
    {
      return true;
    }
  }
  else if ( this->getRules() == 1 ) // COMBO
  {
    if ( r1 > r2 )
    {
      return true;
    }
  }
  else if ( this->getRules() == 2 ) // SAME
  {
    if ( r1 > r2 )
    {
      return true;
    }
  }
  else if ( this->getRules() == 3 ) // WALL
  {
    if ( r1 > r2 )
    {
      return true;
    }
  }
  else if ( this->getRules() == 4 ) // PLUS
  {
    if ( r1 > r2 )
    {
      return true;
    }
  }
  else if ( this->getRules() == 5 ) // ELEMENTAL
  {
    if ( r1 > r2 )
    {
      return true;
    }
  }
  else if ( this->getRules() == 6 ) // LOSER_WINNER
  {
    if ( r1 < r2 )
    {
      return true;
    }
  }

  return false;
}

