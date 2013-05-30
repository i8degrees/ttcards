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
  this->element = NONE;
  this->rank = { { 0, 0, 0, 0 } };
  this->name = "\0";
  this->player_id = NOPLAYER;
}

unsigned int Card::getID ( void )
{
  return this->id;
}

unsigned int Card::getLevel ( void )
{
  return this->level;
}

unsigned int Card::getType ( void )
{
  return this->type;
}

unsigned int Card::getElement ( void )
{
  return this->element;
}

std::array<int, 4> Card::getRanks ( void )
{
  return this->rank;
}

unsigned int Card::getNorthRank ( void )
{
  return this->rank[NORTH];
}

unsigned int Card::getEastRank ( void )
{
  return this->rank[EAST];
}

unsigned int Card::getSouthRank ( void )
{
  return this->rank[SOUTH];
}

unsigned int Card::getWestRank ( void )
{
  return this->rank[WEST];
}

std::string Card::getName ( void )
{
  return this->name;
}

unsigned int Card::getPlayerID ( void )
{
  return this->player_id;
}

// Presently, this is the only card attribute that is ever modified in game
void Card::setPlayerID ( unsigned int player )
{
  this->player_id = player;
}

void Card::setID ( unsigned int id_ )
{
  this->id = id_;
}

void Card::setLevel ( unsigned int level_ )
{
  this->level = level_;
}

void Card::setType ( unsigned int type_ )
{
  this->type = type_;
}

void Card::setElement ( unsigned int element_ )
{
  this->element = element_;
}

void Card::setNorthRank ( unsigned int rank )
{
  this->rank[NORTH] = rank;
}

void Card::setEastRank ( unsigned int rank )
{
  this->rank[EAST] = rank;
}

void Card::setSouthRank ( unsigned int rank )
{
  this->rank[SOUTH] = rank;
}

void Card::setWestRank ( unsigned int rank )
{
  this->rank[WEST] = rank;
}

void Card::setName ( std::string name_ )
{
  this->name = name_;
}
