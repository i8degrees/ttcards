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

CardCollection::CardCollection ( void )
{
NOM_LOG_TRACE ( TTCARDS );
}

CardCollection::~CardCollection ( void )
{
NOM_LOG_TRACE ( TTCARDS );
}

void CardCollection::clear ( void )
{
  this->cards.clear();
}

nom::uint32 CardCollection::size ( void ) const
{
  return this->cards.size();
}

Card& CardCollection::getCards ( unsigned int idx )
{
  return this->cards[idx];
}

Cards CardCollection::getCards ( void )
{
  Cards temp_cards; // temp var for return passing

  for ( nom::uint32 idx = 0; idx < this->cards.size(); idx++ )
  {
    temp_cards.push_back ( this->cards[idx] );
  }

  return temp_cards;
}

bool CardCollection::save( const std::string& filename )
{
  nom::ISerializer* fp; // High-level file I/O interface
  nom::Object obj;

  fp = new nom::JsonCppSerializer();

  if ( this->cards.size() < MIN_COLLECTION ) // Sanity check
  {
    NOM_LOG_ERR ( TTCARDS, "Failed MIN_COLLECTION sanity check before saving: " + filename );
    return false;
  }

  if ( this->cards.size() > MAX_COLLECTION ) // Sanity check
  {
    NOM_LOG_ERR ( TTCARDS, "Failed MAX_COLLECTION sanity check before saving: " + filename );
    return false;
  }

  for ( nom::uint32 idx = 0; idx != this->cards.size(); ++idx )
  {
    // Serialize each card's attributes
    obj = this->cards[idx].serialize();
  }

  if ( fp->serialize( obj, filename ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to save JSON file: " + filename );
    return false;
  }

  Card::CARDS_COLLECTION = this->cards.size();

  return true;
}

bool CardCollection::load( const std::string& filename )
{
  nom::ISerializer* fp; // High-level file I/O interface
  nom::Value value;
  nom::Array arr;

  // The card attributes we are loading in will be stored in here, and once a
  // card has filled its buffer, we push it into its final resting place ...
  // CardCollection's Card vector.
  Card card;
  Cards cards_buffer;

  fp = new nom::JsonCppSerializer();

  if ( fp->unserialize( filename, value ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to parse JSON input file: " + filename );
    return false;
  }

  if ( value.size() > MAX_COLLECTION ) // Sanity check
  {
    NOM_LOG_ERR ( TTCARDS, "Failed MAX_COLLECTION sanity check before loading: " + filename );
    return false;
  }

  for ( auto itr = value.begin(); itr != value.end(); ++itr )
  {
    nom::Object obj = itr->object();

    card.unserialize( obj );

    // Additional attributes
    card.setPlayerID( Card::NOPLAYER );     // placeholder
    card.setPlayerOwner( Card::NOPLAYER );  // placeholder

    cards_buffer.push_back( card );
  }

  if ( cards_buffer.size() < MIN_COLLECTION ) // Sanity check
  {
    NOM_LOG_ERR ( TTCARDS, "Failed MIN_COLLECTION sanity check after loading: " + filename );
    return false;
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
