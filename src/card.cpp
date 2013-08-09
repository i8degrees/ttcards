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
#include "card.hpp"

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

unsigned int Card::getID ( void ) const
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
