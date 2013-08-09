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
#include "Game.hpp"

// These cards should be discarded from player's hand ( MAX_HAND = 5 )
void PlayState::debugCardsDiscard ( void )
{
  this->state->hand[0].addCard ( this->state->collection.cards[88] ); // Carbuncle
  this->state->hand[0].addCard ( this->state->collection.cards[24] ); // TriFace

  this->state->hand[1].addCard ( this->state->collection.cards[88] ); // Carbuncle
}

// Debug player hand set for no and combo rulesets
void PlayState::debugCardsNoRuleset ( void )
{
/*
  this->state->hand[0].addCard ( this->state->collection.cards[89] ); // Diablos
  this->state->hand[0].addCard ( this->state->collection.cards[109] ); // Squall
  this->state->hand[0].addCard ( this->state->collection.cards[99] ); // Ward
  this->state->hand[0].addCard ( this->state->collection.cards[84] ); // Ifrit [pos 3]
  this->state->hand[0].addCard ( this->state->collection.cards[16] ); // Thrustaevis
*/
  this->state->hand[1].addCard ( this->state->collection.cards[20] ); // Jelleye
  this->state->hand[1].addCard ( this->state->collection.cards[88] ); // Carbuncle
  this->state->hand[1].addCard ( this->state->collection.cards[24] ); // TriFace
  this->state->hand[1].addCard ( this->state->collection.cards[66] ); // Propagator
  this->state->hand[1].addCard ( this->state->collection.cards[50] ); // Malboro
}

// Debug player hand set for same rulesets
void PlayState::debugCardsSameRuleset ( void )
{
  this->state->hand[0].addCard ( this->state->collection.cards[89] ); // Diablos
  this->state->hand[0].addCard ( this->state->collection.cards[109] ); // Squall
  this->state->hand[0].addCard ( this->state->collection.cards[99] ); // Ward
  this->state->hand[0].addCard ( this->state->collection.cards[84] ); // Ifrit [pos 3]
  //this->state->hand[0].addCard ( this->state->collection.cards[16] ); // Thrustaevis
  this->state->hand[0].addCard ( this->state->collection.cards[60] ); // Iguion

  this->state->hand[1].addCard ( this->state->collection.cards[20] ); // Jelleye
  this->state->hand[1].addCard ( this->state->collection.cards[2] ); // Bite Bug
  //this->state->hand[1].addCard ( this->state->collection.cards[88] ); // Carbuncle
  this->state->hand[1].addCard ( this->state->collection.cards[5] ); // Gayla
  this->state->hand[1].addCard ( this->state->collection.cards[63] ); // Oilboyle
  this->state->hand[1].addCard ( this->state->collection.cards[77] ); // Chubby Chocobo
  //this->state->hand[1].addCard ( this->state->collection.cards[50] ); // Malboro
}

// debug helper method
void PlayState::removePlayerCard ( void )
{
  unsigned int player_turn = get_turn();

  this->state->hand[player_turn].removeCard ( this->state->hand[player_turn].getSelectedCard() );
  this->state->hand[player_turn].clearSelectedCard();
  this->state->hand[player_turn].selectCard ( this->state->hand[player_turn].cards.front() );

  this->state->cursor.setPosition ( this->player_cursor_coords[player_turn].x, this->player_cursor_coords[player_turn].y );
}

// Debug -- input events helper method
void PlayState::debugBox ( void )
{
  if ( this->debug_box.isEnabled() == true )
    this->debug_box.disable ();
  else
    this->debug_box.enable ();
}

// Debug -- input events helper method
void PlayState::debugListCards ( int32_t mod )
{
  if ( mod == KMOD_LMETA )
    this->debug.ListCards ( this->state->hand[1].cards );
  else
    this->debug.ListCards ( this->state->hand[0].cards );
}

// Debug -- input events helper method
void PlayState::debugListCollection ( int32_t mod )
{
  if ( mod == KMOD_LMETA )
    this->debug.ListCards ( this->state->collection.cards );
  else
    this->state->board.List();
}

void PlayState::debugModifyCardRank ( bool modifier, nom::uint32 direction )
{
  Card selected = Card(); // temporary placeholder card for the update
  std::array<nom::int32, 4> ranks = { 0, 0, 0, 0 }; // card ranks container
  nom::int32 pos = 0; // position in player hand
  nom::uint32 player_turn = this->get_turn(); // current player turn

  // First, obtain current rank attributes of the selected card; validation is
  // done for us by the Card class.
  selected = this->state->hand[ player_turn ].getSelectedCard();
  ranks = selected.getRanks();

  if ( modifier ) // increase
  {
    ranks [ direction ] = ranks [ direction ] + 1;
    selected.setRanks ( ranks );
  }
  else // assume a decrease
  {
    // This clamps the decreased attribute to not falling below one (1).
    ranks [ direction ] = std::max ( ranks [ direction ] - 1, 1 );
    selected.setRanks ( ranks );
  }

  // Update the player hand with our modified card attributes.
  this->state->hand[ player_turn].removeCard ( selected );
  this->state->hand[ player_turn].addCard ( selected );

  // Update the player's selected card (player hand is a LILO type).
  this->state->hand[ player_turn].selectCard ( this->state->hand[ player_turn].cards.back() );

  // Lastly, calculate the (new) position of our selected card and use
  // this information to update the cursor position to match the selected
  // card.
  selected = this->state->hand[ player_turn ].getSelectedCard();
  pos = this->state->hand[ player_turn ].pos(selected);

  this->state->cursor.setPosition ( this->player_cursor_coords[ player_turn ].x, this->player_cursor_coords[ player_turn ].y + ( CARD_HEIGHT / 2 ) * pos );
}
