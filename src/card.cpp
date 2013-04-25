/******************************************************************************
    card.cpp

  Card container class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card.h"

Card::Card (  unsigned int id, unsigned int level, unsigned int type,
              unsigned int element, std::array<int, 4> rank,
              std::string name, unsigned int player_id )
{
  #ifdef DEBUG_CARD_OBJ
    std::cout << "Card::Card (): Hello, world!" << "\n" << std::endl;
  #endif

  this->id = id;
  this->level = level;
  this->type = type;
  this->element = element;
  this->rank = rank;
  this->name = name;
  this->player_id = player_id;
}

Card::~Card ( void )
{
  #ifdef DEBUG_CARD_OBJ
    std::cout << "Card::~Card(): Goodbye cruel world!" << "\n" << std::endl;
  #endif

  this->id = 0;
  this->level = 0;
  this->type = 0;
  this->element = 0;
  this->rank = { { 0, 0, 0, 0 } };
  this->name = "\0";
  this->player_id = 0;
}
