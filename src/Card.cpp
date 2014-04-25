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
#include "Card.hpp"

nom::int32 Card::CARDS_COLLECTION = 0;

Card::Card ( void ) :
  id ( BAD_CARD_ID ),
  level ( 0 ),
  type ( 0 ),
  element ( NONE ),
  rank( { { 0 } } ),
  player_id ( Card::NOPLAYER ),
  player_owner ( Card::NOPLAYER )
{
  // ...
}

Card::Card  (
              nom::int32 id, nom::int32 level, nom::int32 type,
              nom::int32 element, std::array<nom::int32, MAX_RANKS> rank,
              std::string name, nom::int32 player_id, nom::int32 player_owner
            ) : id ( id ), level ( level ), type ( type ), element ( element ),
            rank { { rank[NORTH], rank[EAST], rank[SOUTH], rank[WEST] } },
            name ( name ), player_id ( player_id ), player_owner ( player_owner )
{
  // ...
}

Card::~Card ( void )
{
  // ...
}

const nom::int32 Card::getID ( void ) const
{
  return this->id;
}

const std::string Card::get_id_string( void ) const
{
  return std::to_string( this->id );
}

const nom::int32 Card::getLevel ( void ) const
{
  return this->level;
}

const nom::int32 Card::getType ( void ) const
{
  return this->type;
}

const nom::int32 Card::getElement ( void ) const
{
  return this->element;
}

const std::array<nom::int32, MAX_RANKS> Card::getRanks ( void ) const
{
  return this->rank;
}

const std::vector<int> Card::ranks_as_vector ( void ) const
{
  return std::vector<int> { this->rank[NORTH], this->rank[EAST],
                            this->rank[SOUTH], this->rank[WEST] };
}

const nom::int32 Card::getNorthRank ( void ) const
{
  return this->rank[NORTH];
}

const nom::int32 Card::getEastRank ( void ) const
{
  return this->rank[EAST];
}

const nom::int32 Card::getSouthRank ( void ) const
{
  return this->rank[SOUTH];
}

const nom::int32 Card::getWestRank ( void ) const
{
  return this->rank[WEST];
}

const std::string& Card::getName ( void ) const
{
  return this->name;
}

const nom::int32 Card::getPlayerID ( void ) const
{
  return this->player_id;
}

const nom::int32 Card::getPlayerOwner ( void ) const
{
  return this->player_owner;
}

void Card::setID ( nom::int32 id_ )
{
  this->id = id_;
  // FIXME:
  //this->id = std::min ( id_, Card::CARDS_COLLECTION );
}

void Card::setLevel ( nom::int32 level_ )
{
  this->level = std::min ( level_, LEVEL_MAX );
}

void Card::setType ( nom::int32 type_ )
{
  this->type = std::min ( type_, MAX_TYPE );
}

void Card::setElement ( nom::int32 element_ )
{
  this->element = std::min ( element_, MAX_ELEMENT );
}

void Card::setRanks ( std::array<nom::int32, MAX_RANKS> ranks )
{
  this->setNorthRank ( ranks[NORTH] );
  this->setEastRank ( ranks[EAST] );
  this->setSouthRank ( ranks[SOUTH] );
  this->setWestRank ( ranks[WEST] );
}

void Card::set_ranks ( std::vector<nom::int32> ranks )
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

void Card::setPlayerID ( nom::int32 player_id_ )
{
  this->player_id = std::min ( player_id_, TOTAL_PLAYERS );
}

void Card::setPlayerOwner ( nom::int32 player_owner_ )
{
  this->player_owner = std::min ( player_owner_, TOTAL_PLAYERS );
}

nom::Value Card::serialize( void ) const
{
  nom::Value obj;

  obj["id"] = this->id;
  obj["name"] = this->name;
  obj["level"] = this->level;
  obj["type"] = this->type;
  obj["element"] = this->element;

  for( auto it = this->rank.begin(); it != this->rank.end(); ++it )
  {
    // If we do not insert the array elements as an integer here, nom::Value
    // gets confused and assigns the values as boolean.
    int val = *it;

    obj["ranks"].push_back( val );
  }

  return obj;
}

void Card::unserialize( nom::Value& obj )
{
  this->setID( obj["id"].get_int() );
  this->setName( obj["name"].get_string() );
  this->setLevel( obj["level"].get_int() );
  this->setType( obj["type"].get_int() );
  this->setElement( obj["element"].get_int() );

  nom::Value arr = obj["ranks"].array();

  nom::uint idx = 0;
  for( auto it = arr.begin(); it != arr.end(); ++it )
  {
    this->rank[idx] = it->get_int();
    ++idx;
  }
}

void Card::increaseNorthRank ( void )
{
  this->setNorthRank ( this->getNorthRank() + 1 );
}

void Card::increaseEastRank ( void )
{
  this->setEastRank ( this->getEastRank() + 1 );
}

void Card::increaseSouthRank ( void )
{
  this->setSouthRank ( this->getSouthRank() + 1 );
}

void Card::increaseWestRank ( void )
{
  this->setWestRank ( this->getWestRank() + 1 );
}

void Card::decreaseNorthRank ( void )
{
  nom::int32 modified_rank = std::max ( this->getNorthRank() - 1, MIN_RANK );
  this->setNorthRank ( modified_rank );
}

void Card::decreaseEastRank ( void )
{
  nom::int32 modified_rank = std::max ( this->getEastRank() - 1, MIN_RANK );
  this->setEastRank ( modified_rank );
}

void Card::decreaseSouthRank ( void )
{
  nom::int32 modified_rank = std::max ( this->getSouthRank() - 1, MIN_RANK );
  this->setSouthRank ( modified_rank );
}

void Card::decreaseWestRank ( void )
{
  nom::int32 modified_rank = std::max ( this->getWestRank() - 1, MIN_RANK );
  this->setWestRank ( modified_rank );
}

nom::int32 Card::strength ( void )
{
  nom::int32 total_strength_value = 0;

  total_strength_value += this->getNorthRank();
  total_strength_value += this->getEastRank();
  total_strength_value += this->getWestRank();
  total_strength_value += this->getSouthRank();

  return total_strength_value;
}

std::ostream& operator << ( std::ostream& os, const Card& rhs )
{
  os  << rhs.getName()
      << card_delimiter
      << rhs.getID()
      << card_delimiter
      << rhs.getLevel()
      << card_delimiter
      << rhs.getType()
      << card_delimiter
      << rhs.getElement()
      << card_delimiter
      << rhs.getNorthRank()
      << card_delimiter
      << rhs.getEastRank()
      << card_delimiter
      << rhs.getSouthRank()
      << card_delimiter
      << rhs.getWestRank()
      << card_delimiter
      << rhs.getPlayerID()
      << card_delimiter
      << rhs.getPlayerOwner();

  return os;
}

bool operator == ( const Card& lhs, const Card& rhs )
{
  return  ( lhs.getID() == rhs.getID() )                      &&
          ( lhs.getLevel() == rhs.getLevel() )                &&
          ( lhs.getType() == rhs.getType() )                  &&
          ( lhs.getElement() == rhs.getElement() )            &&
          ( lhs.getNorthRank() == rhs.getNorthRank() )        &&
          ( lhs.getEastRank() == rhs.getEastRank() )          &&
          ( lhs.getSouthRank() == rhs.getSouthRank() )        &&
          ( lhs.getWestRank() == rhs.getWestRank() )          &&
          ( lhs.getName() == rhs.getName() );
}

bool operator != ( const Card& lhs, const Card& rhs )
{
  return ! ( lhs == rhs );
}

bool operator <= ( const Card& lhs, const Card& rhs )
{
  return ! (rhs < lhs );
}

bool operator >= ( const Card& lhs, const Card& rhs )
{
  return ! ( lhs < rhs );
}

bool operator < ( const Card& lhs, const Card& rhs )
{
  nom::uint32 lhs_total_strengths = 0;
  nom::uint32 rhs_total_strengths = 0;

  lhs_total_strengths += lhs.getNorthRank();
  lhs_total_strengths += lhs.getEastRank();
  lhs_total_strengths += lhs.getWestRank();
  lhs_total_strengths += lhs.getSouthRank();

  rhs_total_strengths += rhs.getNorthRank();
  rhs_total_strengths += rhs.getEastRank();
  rhs_total_strengths += rhs.getWestRank();
  rhs_total_strengths += rhs.getSouthRank();

  if ( lhs_total_strengths == rhs_total_strengths )
  {
    return lhs_total_strengths < rhs_total_strengths;
  }
  else
  {
    return lhs_total_strengths < rhs_total_strengths;
  }
}

bool operator > ( const Card& lhs, const Card& rhs )
{
  nom::uint32 lhs_total_strengths = 0;
  nom::uint32 rhs_total_strengths = 0;

  lhs_total_strengths += lhs.getNorthRank();
  lhs_total_strengths += lhs.getEastRank();
  lhs_total_strengths += lhs.getWestRank();
  lhs_total_strengths += lhs.getSouthRank();

  rhs_total_strengths += rhs.getNorthRank();
  rhs_total_strengths += rhs.getEastRank();
  rhs_total_strengths += rhs.getWestRank();
  rhs_total_strengths += rhs.getSouthRank();

  return rhs_total_strengths < lhs_total_strengths;
}
