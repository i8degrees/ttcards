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
#include "Card.hpp"

Card::Card ( void ) : id ( 0 ), level ( 0 ), type ( 0 ), element ( NONE ),
                      rank( { { 0 } } ), name ( "\0" ),
                      player_id ( Card::NOPLAYER ),
                      player_owner ( Card::NOPLAYER )
{}

Card::Card  (
              nom::uint32 id, nom::uint32 level, nom::uint32 type,
              nom::uint32 element, std::array<nom::int32, MAX_RANKS> rank,
              std::string name, nom::uint32 player_id, nom::uint32 player_owner
            )
            : id ( id ), level ( level ), type ( type ), element ( element ),
            rank { { rank[NORTH], rank[EAST], rank[SOUTH], rank[WEST] } },
            name ( name ), player_id ( player_id ), player_owner ( player_owner )
{}

Card::~Card ( void ) {}

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

std::array<nom::int32, MAX_RANKS> Card::getRanks ( void )
{
  return this->rank;
}

nom::int32 Card::getNorthRank ( void )
{
  return this->rank[NORTH];
}

nom::int32 Card::getEastRank ( void )
{
  return this->rank[EAST];
}

nom::int32 Card::getSouthRank ( void )
{
  return this->rank[SOUTH];
}

nom::int32 Card::getWestRank ( void )
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

void Card::setRanks ( std::array<nom::int32, MAX_RANKS> ranks )
{
  this->setNorthRank ( ranks[NORTH] );
  this->setEastRank ( ranks[EAST] );
  this->setSouthRank ( ranks[SOUTH] );
  this->setWestRank ( ranks[WEST] );
}

void Card::setNorthRank ( nom::int32 rank )
{
  this->rank[NORTH] = std::min ( rank, MAX_RANK );
}

void Card::setEastRank ( nom::int32 rank )
{
  this->rank[EAST] = std::min ( rank, MAX_RANK );
}

void Card::setSouthRank ( nom::int32 rank )
{
  this->rank[SOUTH] = std::min ( rank, MAX_RANK );
}

void Card::setWestRank ( nom::int32 rank )
{
  this->rank[WEST] = std::min ( rank, MAX_RANK );
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
