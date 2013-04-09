/******************************************************************************
    card.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card.h"

Card::Card ( void )
{
  #ifdef DEBUG_CARDS
    std::cout << "Hello, world! <From Card::Card\n" << std::endl;
  #endif
}

Card::Card (  unsigned int id, unsigned int level, unsigned int type,
              unsigned int element, int p0, int p1, int p2, int p3, std::string name)

{
  this->id = id;
  this->level = level;
  this->type = type;
  this->element = element;
  this->power[0] = p0;
  this->power[1] = p1;
  this->power[2] = p2;
  this->power[3] = p3;
  this->name = name;
}

Card::~Card ( void )
{
  #ifdef DEBUG_CARDS
    std::cout << "Good cruel world! <From Card::~Card\n" << std::endl;
  #endif
}
