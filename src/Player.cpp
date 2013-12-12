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
#include "Player.hpp"

Player::Player ( void ) : card ( nullptr ), hand ( nullptr ),
                          id ( 0 ), score ( 0 ),
                          player_state ( PlayerState::Reserved )
{
NOM_LOG_TRACE ( TTCARDS );
}

Player::Player ( CardHand* player_cards, CardView* view, const CardRules& ruleset )
{
NOM_LOG_TRACE ( TTCARDS );

  this->hand.reset ( player_cards, Free_CardHand );
  this->ruleset = ruleset;
  this->card = view;

  this->coords = nom::Coords ( 0, 0, 0, 0 ); // initialize X, Y origin coords
  this->id = 0;
  this->set_state ( PlayerState::Reserved );
  this->score = 0;
}

Player::~Player ( void )
{
NOM_LOG_TRACE ( TTCARDS );
}

nom::int32 Player::getX ( void )
{
  return this->coords.x;
}

nom::int32 Player::getY ( void )
{
  return this->coords.y;
}

const nom::Coords Player::getPosition ( void ) const
{
  return this->coords;
}

void Player::setPosition ( nom::int32 x, nom::int32 y )
{
  this->coords.setPosition ( x, y );
}

unsigned int Player::getID ( void )
{
  return this->id;
}

// Maps the player's (card) hand with their respective ID; this keeps track of
// who's card is which and is used in CardView -- the rendering the card
// background -- and most importantly, in the Board class where we compare cards
// placed to determine whom's card to flip over to the respective player.
//
// card.player_owner is the *original* player owner of a card and should never be
// altered once set initially here.
//
void Player::setID ( unsigned int id_ )
{
  this->id = id_;

  for ( nom::uint32 pid = 0; pid < this->hand->cards.size(); pid++ )
  {
    this->hand->cards[pid].setPlayerID ( id_ );
    this->hand->cards[pid].setPlayerOwner ( id_ );
  }
}

enum PlayerState Player::state ( void )
{
  return this->player_state;
}

void Player::set_state ( enum PlayerState state )
{
#if ! defined (NOM_DEBUG)
  if ( state == PlayerState::Debug ) return;
#endif

  this->player_state = state;
}

nom::uint32 Player::getScore ( void ) const
{
  return this->score;
}

const std::string Player::getScoreAsString ( void ) const
{
  return std::to_string ( this->score );
}

void Player::setScore ( unsigned int score )
{
  this->score = score;
}

void Player::draw ( nom::IDrawable::RenderTarget target )
{
  bool face_down = false;
/*
  if ( this->ruleset.getRules() != CardRules::Open )
  {
    face_down = true;
#if defined (NOM_DEBUG)
    if ( this->state() == PlayerState::Debug )
    {
      face_down = false;
    }
#endif
  }
*/

  for ( nom::int32 idx = 0; idx < this->hand->size(); idx++ )
  {
    this->player_pos = nom::Coords  (
                                      this->getX(),
                                      this->getY() + ( CARD_HEIGHT / 2 ) * idx
                                    );

    nom::int32 player_id = this->hand->cards.at( idx ).getPlayerID();
    Card selected_card = this->hand->getSelectedCard();
    nom::int32 hand_pos = this->hand->at ( selected_card );

    if ( player_id == Card::PLAYER2 && hand_pos == idx )
    {
      this->player_pos.x += 16;
      this->card->reposition ( this->player_pos );
      this->card->setViewCard ( this->hand->cards.at ( idx ) );
      this->card->face ( face_down );
      this->card->draw ( target );
    }
    else if ( player_id == Card::PLAYER1 && hand_pos == idx )
    {
      this->player_pos.x -= 16;

      this->card->reposition ( this->player_pos );
      this->card->setViewCard ( this->hand->cards.at ( idx ) );
      this->card->face ( false );
      this->card->draw ( target );
    }
    else
    {
      this->card->reposition ( this->player_pos );
      this->card->setViewCard ( this->hand->cards[idx] );

      if ( player_id == Card::PLAYER1 )
      {
        this->card->face ( false );
      }
      else if ( player_id == Card::PLAYER2 )
      {
        this->card->face ( face_down );
      }

      this->card->draw ( target );
    }
  } // end for this->hand loop

  // This is a lazy patch until I get around to fixing this :-)
  this->card->face ( false ); // Turns drawing card faces down off
}

void Player::update ( void )
{
  this->card->update();
}

