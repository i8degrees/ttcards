/******************************************************************************

    Card Logic Rules

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_rules.h"

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

