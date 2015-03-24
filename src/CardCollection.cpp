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
  this->cards.clear();
}

nom::size_type CardCollection::size() const
{
  return this->cards.size();
}

const Card& CardCollection::front() const
{
  if( this->cards.size() > 0 ) {
    return this->cards.front();
  } else {
    return Card::null;
  }
}

bool CardCollection::save(const std::string& filename)
{
  // Our JSON output will be a JSON object enclosing an array keyed "cards",
  // of which holds each of our individual, unnamed JSON objects.
  //
  // NOTE: I wished for "cards" to be an object as well, but then sorting gets
  // all messed up -- the "off by six-ish" bug from before -- so the compromise
  // until we figure these things out is going to have to be this!
  nom::Value obj( nom::Value::ObjectValues );
  nom::Value arr( nom::Value::ArrayValues );

  auto fp = nom::make_unique_json_serializer();
  if( fp == nullptr ) {
    NOM_LOG_ERR(  TTCARDS,
                  "Could not load input file: failure to allocate memory!" );
    return false;
  }

  if ( this->cards.size() > MAX_COLLECTION ) // Sanity check
  {
    NOM_LOG_ERR ( TTCARDS, "Failed MAX_COLLECTION sanity check before saving: " + filename );
    return false;
  }

  for ( nom::uint32 idx = 0; idx != this->cards.size(); ++idx )
  {
    // Serialize each card's attributes; said card attributes become JSON
    // objects, enclosed within our overall container ("cards" array).
    arr.push_back( this->cards[idx].serialize() );

    // Top-level array node
    obj["cards"] = arr;
  }

  if ( fp->save( obj, filename ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to save JSON file: " + filename );
    return false;
  }

  Card::CARDS_COLLECTION = this->cards.size();

  return true;
}

bool CardCollection::load(const std::string& filename)
{
  nom::Value value;

  // The card attributes we are loading in will be stored in here, and once a
  // card has filled its buffer, we push it into its final resting place ...
  // CardCollection's Card vector.
  Card card;
  Cards cards_buffer;

  auto fp = nom::make_unique_json_deserializer();
  if( fp == nullptr ) {
    NOM_LOG_ERR(  TTCARDS,
                  "Could not load input file: failure to allocate memory!" );
    return false;
  }

  if ( fp->load( filename, value ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to parse JSON input file: " + filename );
    return false;
  }

  if ( value.size() > MAX_COLLECTION ) // Sanity check
  {
    NOM_LOG_ERR ( TTCARDS, "Failed MAX_COLLECTION sanity check before loading: " + filename );
    return false;
  }

  for ( auto itr = value["cards"].begin(); itr != value["cards"].end(); ++itr )
  {
    nom::Value val = itr->ref();

    card.unserialize( val );

    // Additional attributes
    card.setPlayerID(Card::NO_PLAYER);
    card.setPlayerOwner(Card::NO_PLAYER);

    cards_buffer.push_back( card );
  }

  if ( cards_buffer.size() > MAX_COLLECTION ) // Sanity check
  {
    NOM_LOG_ERR ( TTCARDS, "Failed MAX_COLLECTION sanity check after loading: " + filename );
    return false;
  }

  // All is well, let us make our freshly loaded data permanent
  this->cards = cards_buffer;
  Card::CARDS_COLLECTION = this->cards.size();

#ifdef DEBUG_CARD_COLLECTION
  debug.ListCards ( this->cards );
#endif

  return true;
}

const Card& CardCollection::find(const std::string& card_name) const
{
  for( auto itr = this->cards.begin(); itr != this->cards.end(); ++itr ) {

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
  for( auto itr = this->cards.begin(); itr != this->cards.end(); ++itr ) {

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
  return this->cards.begin();
}

ConstCardsIterator CardCollection::end() const
{
  return this->cards.end();
}

CardsIterator CardCollection::begin()
{
  return this->cards.begin();
}

CardsIterator CardCollection::end()
{
  return this->cards.end();
}
