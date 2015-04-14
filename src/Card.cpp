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

// Private headers
#include <nomlib/ptree/Value.hpp>

// Forward declarations
#include "CardRenderer.hpp"

using namespace nom;

namespace tt {

// Static initialization
Card Card::null = Card();
nom::int32 Card::CARDS_COLLECTION = 0;

Card::Card() :
  id(BAD_CARD_ID),
  level(0),
  type(0),
  element(::NONE),
  rank( {{0}} ),
  name("INVALID"),
  player_id(Card::NO_PLAYER),
  player_owner(Card::NO_PLAYER),
  num_(0),
  face_down_(false),
  card_renderer_(nullptr)
{
  // NOM_LOG_TRACE_PRIO(TTCARDS_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE);
}

Card::~Card()
{
  // NOM_LOG_TRACE_PRIO(TTCARDS_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE);
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

int Card::num() const
{
  return this->num_;
}

bool Card::face_down() const
{
  return this->face_down_;
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

std::shared_ptr<CardRenderer>& Card::card_renderer()
{
  return this->card_renderer_;
}

void Card::setID ( nom::int32 id_ )
{
  this->id = id_;
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
  this->player_id = player_id_;
}

void Card::setPlayerOwner ( nom::int32 player_owner_ )
{
  this->player_owner = player_owner_;
}

void Card::set_num(int num_cards)
{
  this->num_ = std::min(num_cards, MAX_NUM);
}

void Card::set_face_down(bool state)
{
  this->face_down_ = state;
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

  obj["num"] = this->num_;

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

  this->set_num( obj["num"].get_int() );
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

void Card::set_card_renderer(CardRenderer* renderer)
{
  NOM_LOG_TRACE_PRIO(TTCARDS_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE);

  this->card_renderer_.reset(renderer);
}

std::ostream& operator <<(std::ostream& os, const Card& rhs)
{
  os  << "[name="
      << rhs.getName()
      << ", card_id="
      << rhs.getID()
      << ", element_id="
      << rhs.getElement()
      << ", player_id="
      << rhs.getPlayerID()
      << ", player_owner="
      << rhs.getPlayerOwner()
      << ", num="
      << rhs.num()
      << ", ranks="
      << rhs.getNorthRank()
      << ", "
      << rhs.getEastRank()
      << ", "
      << rhs.getSouthRank()
      << ", "
      << rhs.getWestRank()
      << "] ";

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
          ( lhs.getName() == rhs.getName() );                 //&&
          // ( lhs.num() == rhs.num() );
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

bool operator <(const Card& lhs, const Card& rhs)
{
  return( lhs.getID() < rhs.getID() );
}

bool operator >(const Card& lhs, const Card& rhs)
{
  return( rhs.getID() > lhs.getID() );
}

bool strongest_card(const Card& lhs, const Card& rhs)
{
  nom::int32 lhs_total_strengths = 0;
  nom::int32 rhs_total_strengths = 0;

  lhs_total_strengths += lhs.getNorthRank();
  lhs_total_strengths += lhs.getEastRank();
  lhs_total_strengths += lhs.getWestRank();
  lhs_total_strengths += lhs.getSouthRank();

  rhs_total_strengths += rhs.getNorthRank();
  rhs_total_strengths += rhs.getEastRank();
  rhs_total_strengths += rhs.getWestRank();
  rhs_total_strengths += rhs.getSouthRank();

  if( rhs_total_strengths == lhs_total_strengths ) {
    return( rhs.getID() < lhs.getID() );
  } else {
    return(rhs_total_strengths < lhs_total_strengths);
  }
}

bool weakest_card(const Card& lhs, const Card& rhs)
{
  nom::int32 lhs_total_strengths = 0;
  nom::int32 rhs_total_strengths = 0;

  lhs_total_strengths += lhs.getNorthRank();
  lhs_total_strengths += lhs.getEastRank();
  lhs_total_strengths += lhs.getWestRank();
  lhs_total_strengths += lhs.getSouthRank();

  rhs_total_strengths += rhs.getNorthRank();
  rhs_total_strengths += rhs.getEastRank();
  rhs_total_strengths += rhs.getWestRank();
  rhs_total_strengths += rhs.getSouthRank();

  if( lhs_total_strengths == rhs_total_strengths ) {
    return( lhs.getID() < rhs.getID() );
  } else {
    return(lhs_total_strengths < rhs_total_strengths);
  }
}

} // namespace tt
