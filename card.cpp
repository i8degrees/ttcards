/******************************************************************************
    card.cpp

  Card container class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card.h"

Card::Card (  unsigned int id, unsigned int level, unsigned int type,
              unsigned int element, std::array<int, 4> rank,
              std::string name, std::string face )

{
  #ifdef DEBUG_CARD
    std::cout << "Hello, world! <From Card::Card>\n" << std::endl;
  #endif

  this->id = id;
  this->level = level;
  this->type = type;
  this->element = element;
  this->rank = rank;
  this->name = name;
  this->face = face;
}

Card::~Card ( void )
{
  #ifdef DEBUG_CARD
    std::cout << "Good cruel world! <From Card::~Card>\n" << std::endl;
  #endif

  this->id = 0;
  this->level = 0;
  this->type = 0;
  this->element = 0;
  this->rank = { { 0, 0, 0, 0 } };
  this->name = "\0";
  this->face = "\0";
}
