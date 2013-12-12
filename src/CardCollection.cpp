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
#include "CardCollection.hpp"

CardCollection::CardCollection ( void )
{
NOM_LOG_TRACE ( TTCARDS );
  this->clear();
}

CardCollection::~CardCollection ( void )
{
NOM_LOG_TRACE ( TTCARDS );
}

void CardCollection::clear ( void )
{
  this->cards.clear();
}

nom::int32 CardCollection::size ( void ) const
{
  nom::int32 count = 0;

  count = this->cards.size();

  return count;
}

Card& CardCollection::getCards ( unsigned int idx )
{
  return this->cards[idx];
}

Cards CardCollection::getCards ( void )
{
  unsigned int idx = 0;
  Cards temp_cards; // temp var for return passing
  temp_cards.clear();

  for ( idx = 0; idx < this->cards.size(); idx++ )
    temp_cards.push_back ( this->cards[idx] );

  return temp_cards;
}

bool CardCollection::save( const std::string& filename )
{
  nom::JSON::FileWriter fp;
  nom::JSON::Value node;
  nom::JSON::Value card;

  if ( this->cards.empty() ) return false;

  for ( nom::uint32 idx = 0; idx < this->cards.size(); ++idx )
  {
    // Serialize each card's attributes
    node.insert ( "id", this->cards[idx].getID(), idx );
    node.insert ( "name", this->cards[idx].getName(), idx );
    node.insert ( "level", this->cards[idx].getLevel(), idx );
    node.insert ( "type", this->cards[idx].getType(), idx );
    node.insert ( "element", this->cards[idx].getElement(), idx );
    node.insert ( "ranks", this->cards[idx].ranks_as_vector(), idx );
  }

  if ( fp.save ( filename, node ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to save JSON file: " + filename );
    return false;
  }

  return true;
}

bool CardCollection::load( const std::string& filename )
{
  nom::JSON::FileReader fp;
  nom::JSON::Value node;

  // The card attributes we are loading in will be stored in here, and once a
  // card has filled its buffer, we push it into its final resting place ...
  // CardCollection's Card vector.
  Card card;
  Cards cards_buffer;

  if ( fp.load ( filename, node, false ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Unable to parse JSON input file: " + filename );
    return false;
  }

  for ( nom::uint32 idx = 0; idx != node.size(); ++idx )
  {
    card.setID ( node.get_int ( "id", idx ) );
    card.setName ( node.get_string ( "name", idx ) );
    card.setLevel ( node.get_int ( "level", idx ) );
    card.setType ( node.get_int ( "type", idx ) );
    card.setElement ( node.get_int ( "element", idx ) );

    std::vector<int> ranks = node.get_ints ( "ranks", idx );
    card.set_ranks ( ranks );

    card.setPlayerID ( Card::NOPLAYER ); // placeholder
    card.setPlayerOwner ( Card::NOPLAYER ); // placeholder

    cards_buffer.push_back ( card );
  }

// Do a sanity check if we are not a debug version
#if ! defined (NOM_DEBUG)
  if ( cards_buffer.size() < 10 ) // Sanity check
  {
NOM_LOG_ERR ( TTCARDS, "Cards collection data is invalid at file: " + filename );
    return false;
  }
#endif

  this->clear(); // otherwise we may exceed our limit of cards

  // All is well, let us make our freshly loaded data permanent
  this->cards = cards_buffer;

#ifdef DEBUG_CARD_COLLECTION
  debug.ListCards ( this->cards );
#endif

  return true;
}
