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
#include "CardCollection.hpp"

// Private headers
#include <nomlib/serializers.hpp>

using namespace nom;

namespace tt {

CardCollection::CardCollection()
{
  NOM_LOG_TRACE ( TTCARDS_LOG_CATEGORY_TRACE );
}

CardCollection::~CardCollection()
{
  NOM_LOG_TRACE ( TTCARDS_LOG_CATEGORY_TRACE );
}

void CardCollection::clear()
{
  this->cards_.clear();
}

nom::size_type CardCollection::size() const
{
  return this->cards_.size();
}

const Card& CardCollection::front() const
{
  if( this->cards_.size() > 0 ) {
    return this->cards_.front();
  } else {
    return Card::null;
  }
}

bool CardCollection::save(const std::string& filename)
{
  // NOTE: I wished for "cards" to be an object as well, but then sorting gets
  // all messed up -- the "off by six-ish" bug from before -- so the compromise
  // until we figure these things out is going to have to be this!
  nom::Value obj(nom::Value::ObjectValues);
  nom::Value arr(nom::Value::ArrayValues);

  auto fp = nom::make_unique_json_serializer();
  if( fp == nullptr ) {
    NOM_LOG_ERR(  TTCARDS,
                  "Could not load input file: failure to allocate memory!" );
    return false;
  }

  for( auto itr = this->cards_.begin(); itr != this->cards_.end(); ++itr ) {

    // Serialize each card's attributes; said card attributes become JSON
    // objects, enclosed within our overall container ("cards" array).
    arr.push_back( (*itr).serialize() );

    // Top-level array node
    obj["cards"] = arr;
  }

  if( fp->save(obj, filename) == false ) {
    NOM_LOG_ERR(TTCARDS, "Unable to save JSON file: " + filename);
    return false;
  }

  return true;
}

bool CardCollection::load(const std::string& filename)
{
  Card card;
  Cards cards_buffer;
  nom::Value value;

  auto fp = nom::make_unique_json_deserializer();
  if( fp == nullptr ) {
    NOM_LOG_ERR(  TTCARDS,
                  "Could not load input file: failure to allocate memory!" );
    return false;
  }

  if( fp->load(filename, value) == false ) {
    NOM_LOG_ERR(TTCARDS, "Unable to parse JSON input file: " + filename);
    return false;
  }

  nom::Value deck = value["cards"];
  for( auto itr = deck.begin(); itr != deck.end(); ++itr ) {

    nom::Value attr = itr->ref();
    card.unserialize(attr);

    // Additional attributes
    card.setPlayerID(Card::NO_PLAYER);
    card.setPlayerOwner(Card::NO_PLAYER);

    cards_buffer.push_back(card);
  }

  // All is well, let us make our freshly loaded data permanent
  this->cards_ = cards_buffer;

  return true;
}

const Card& CardCollection::find(const std::string& card_name) const
{
  for( auto itr = this->cards_.begin(); itr != this->cards_.end(); ++itr ) {

    if( (*itr).getName() == card_name ) {
      // Successful match
      return *itr;
    }
  }

  // No match
  return Card::null;
}

const Card& CardCollection::find(int32 card_id) const
{
  for( auto itr = this->cards_.begin(); itr != this->cards_.end(); ++itr ) {

    if( (*itr).getID() == card_id ) {
      // Successful match
      return *itr;
    }
  }

  // No match
  return Card::null;
}

ConstCardsIterator CardCollection::begin() const
{
  return this->cards_.begin();
}

ConstCardsIterator CardCollection::end() const
{
  return this->cards_.end();
}

CardsIterator CardCollection::begin()
{
  return this->cards_.begin();
}

CardsIterator CardCollection::end()
{
  return this->cards_.end();
}

} // namespace tt
