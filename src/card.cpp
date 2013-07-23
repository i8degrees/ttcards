/******************************************************************************

    Card container class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card.h"

Card::Card ( void ) : id ( 0 ), level ( 0 ), type ( 0 ), element ( NONE ),
                      rank( { { 0, 0, 0, 0 } } ), name ( "\0" ),
                      player_id ( Card::NOPLAYER ),
                      player_owner ( Card::NOPLAYER )
{
  #ifdef DEBUG_CARD_OBJ
    std::cout << "Card::Card (): Hello, world!" << "\n" << std::endl;
  #endif
}

Card::Card  ( unsigned int id_, unsigned int level_, unsigned int type_,
              unsigned int element_, std::array<int, 4> rank_,
              std::string name_, unsigned int player_id_,
              unsigned int player_owner_
            )
{
  #ifdef DEBUG_CARD_OBJ
    std::cout << "Card::Card (): Hello, world!" << "\n" << std::endl;
  #endif

  this->setID ( id_ );
  this->setLevel ( level_ );
  this->setType ( type_ );
  this->setElement ( element_ );
  this->setRanks ( rank_ );
  this->setName ( name_ );

  this->setPlayerID ( player_id_ );
  this->setPlayerOwner ( player_owner_ );
}

Card::~Card ( void )
{
  #ifdef DEBUG_CARD_OBJ
    std::cout << "Card::~Card(): Goodbye cruel world!" << "\n" << std::endl;
  #endif
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

unsigned int Card::getPlayerOwner ( void )
{
  return this->player_owner;
}

void Card::setID ( unsigned int id_ )
{
  this->id = std::min ( id_, static_cast<nom::uint32> ( MAX_COLLECTION ) );
}

void Card::setLevel ( unsigned int level_ )
{
  this->level = std::min ( level_, static_cast<nom::uint32> ( MAX_LEVEL ) );
}

void Card::setType ( unsigned int type_ )
{
  this->type = std::min ( type_, static_cast<nom::uint32> ( MAX_TYPE ) );
}

void Card::setElement ( unsigned int element_ )
{
  this->element = std::min ( element_, static_cast<nom::uint32> ( MAX_ELEMENT ) );
}

void Card::setRanks ( std::array<nom::int32, 4> ranks )
{
  this->setNorthRank ( ranks[NORTH] );
  this->setEastRank ( ranks[EAST] );
  this->setSouthRank ( ranks[SOUTH] );
  this->setWestRank ( ranks[WEST] );
}

void Card::setNorthRank ( unsigned int rank )
{
  this->rank[NORTH] = std::min ( rank, static_cast<nom::uint32> ( MAX_RANK ) );
}

void Card::setEastRank ( unsigned int rank )
{
  this->rank[EAST] = std::min ( rank, static_cast<nom::uint32> ( MAX_RANK ) );
}

void Card::setSouthRank ( unsigned int rank )
{
  this->rank[SOUTH] = std::min ( rank, static_cast<nom::uint32> ( MAX_RANK ) );
}

void Card::setWestRank ( unsigned int rank )
{
  this->rank[WEST] = std::min ( rank, static_cast<nom::uint32> ( MAX_RANK ) );
}

void Card::setName ( std::string name_ )
{
  if ( name_.length() > MAX_NAME )
    name_.resize ( MAX_NAME );

  this->name = name_;
}

void Card::setPlayerID ( unsigned int player_id_ )
{
  this->player_id = std::min ( player_id_, TOTAL_PLAYERS );
}

void Card::setPlayerOwner ( unsigned int player_owner_ )
{
  this->player_owner = std::min ( player_owner_, TOTAL_PLAYERS );
}
