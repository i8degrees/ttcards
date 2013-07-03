/******************************************************************************

    TTcards class game debug methods

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "Game.hpp"

using namespace nom;

// These cards should be discarded from player's hand ( MAX_HAND = 5 )
void Game::debugCardsDiscard ( void )
{
  this->hand[0].addCard ( this->collection.cards[88] ); // Carbuncle
  this->hand[0].addCard ( this->collection.cards[24] ); // TriFace

  this->hand[1].addCard ( this->collection.cards[88] ); // Carbuncle
}

// Debug player hand set for no and combo rulesets
void Game::debugCardsNoRuleset ( void )
{
/*
  this->hand[0].addCard ( this->collection.cards[89] ); // Diablos
  this->hand[0].addCard ( this->collection.cards[109] ); // Squall
  this->hand[0].addCard ( this->collection.cards[99] ); // Ward
  this->hand[0].addCard ( this->collection.cards[84] ); // Ifrit [pos 3]
  this->hand[0].addCard ( this->collection.cards[16] ); // Thrustaevis
*/
  this->hand[1].addCard ( this->collection.cards[20] ); // Jelleye
  this->hand[1].addCard ( this->collection.cards[88] ); // Carbuncle
  this->hand[1].addCard ( this->collection.cards[24] ); // TriFace
  this->hand[1].addCard ( this->collection.cards[66] ); // Propagator
  this->hand[1].addCard ( this->collection.cards[50] ); // Malboro
}

// Debug player hand set for same rulesets
void Game::debugCardsSameRuleset ( void )
{
  this->hand[0].addCard ( this->collection.cards[89] ); // Diablos
  this->hand[0].addCard ( this->collection.cards[109] ); // Squall
  this->hand[0].addCard ( this->collection.cards[99] ); // Ward
  this->hand[0].addCard ( this->collection.cards[84] ); // Ifrit [pos 3]
  //this->hand[0].addCard ( this->collection.cards[16] ); // Thrustaevis
  this->hand[0].addCard ( this->collection.cards[60] ); // Iguion

  this->hand[1].addCard ( this->collection.cards[20] ); // Jelleye
  this->hand[1].addCard ( this->collection.cards[2] ); // Bite Bug
  //this->hand[1].addCard ( this->collection.cards[88] ); // Carbuncle
  this->hand[1].addCard ( this->collection.cards[5] ); // Gayla
  this->hand[1].addCard ( this->collection.cards[63] ); // Oilboyle
  this->hand[1].addCard ( this->collection.cards[77] ); // Chubby Chocobo
  //this->hand[1].addCard ( this->collection.cards[50] ); // Malboro
}

// debug helper method
void Game::removePlayerCard ( void )
{
  unsigned int player_turn = get_turn();

  hand[player_turn].removeCard ( hand[player_turn].getSelectedCard() );
  hand[player_turn].clearSelectedCard();
  hand[player_turn].selectCard ( hand[player_turn].cards.front() );

  this->cursor.setPosition ( this->player_cursor_coords[player_turn].x, this->player_cursor_coords[player_turn].y );
}

// Debug -- input events helper method
void Game::debugBox ( void )
{
  if ( this->debug_box.isEnabled() == true )
    this->debug_box.disable ();
  else
    this->debug_box.enable ();
}

// Debug -- input events helper method
void Game::debugListCards ( int32_t mod )
{
  if ( mod == KMOD_LMETA )
    this->debug.ListCards ( this->hand[1].cards );
  else
    this->debug.ListCards ( this->hand[0].cards );
}

// Debug -- input events helper method
void Game::debugListCollection ( int32_t mod )
{
  if ( mod == KMOD_LMETA )
    this->debug.ListCards ( this->collection.cards );
  else
    this->board.List();
}
