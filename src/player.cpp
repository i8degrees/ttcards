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
#include "player.h"

void Free_CardHand ( CardHand* player_cards )
{
  // Do nothing custom (smart pointer) deleter
  //
  // We produce a segmentation fault without this custom deleter right now due
  // to the fact that CardHand is an array (AKA smart pointers call delete, not
  // delete[].
}

Player::Player ( void ) : coords ( 0, 0, 0, 0 ), id ( 0 ), //state ( 0 ),
                          score ( 5 ), hand ( nullptr ), card ( nullptr )
{
#ifdef DEBUG_PLAYER_OBJ
  std::cout << "Player::Player (): " << "Hello, world!" << "\n" << std::endl;
#endif
}

Player::Player ( CardHand* player_cards, CardView* view )
{
#ifdef DEBUG_PLAYER_OBJ
  std::cout << "Player::Player (): " << "Hello, world!" << "\n" << std::endl;
#endif

  this->hand.reset ( player_cards, Free_CardHand );
  this->card = view;

  this->coords = nom::Coords ( 0, 0, 0, 0 ); // initialize X, Y origin coords
  this->id = 0;
  //this->state = 0;
  this->score = 5;
}

Player::~Player ( void )
{
  #ifdef DEBUG_PLAYER_OBJ
    std::cout << "Player::~Player (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif
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
  nom::ulong pid = 0; // iterator

  this->id = id_;

  for ( pid = 0; pid < this->hand->cards.size(); pid++ )
  {
    this->hand->cards[pid].setPlayerID ( id_ );
    this->hand->cards[pid].setPlayerOwner ( id_ );
  }
}

unsigned int Player::getState ( void )
{
  return 0; //return this->state;
}

void Player::setState ( unsigned int state )
{
  return; //this->state = state;
}

unsigned int Player::getScore ( void )
{
  return this->score;
}

void Player::setScore ( unsigned int score )
{
  this->score = score;
}

void Player::Draw ( void* video_buffer )
{
  nom::int32 hand_index = 0; // iterator

  for ( hand_index = 0; hand_index < this->hand->size(); hand_index++ )
  {
    if ( this->hand->cards.at( hand_index ).getPlayerID() == Card::PLAYER1 )
    {
      if ( this->hand->pos ( this->hand->getSelectedCard() ) == hand_index )
        this->card->DrawCard ( video_buffer, this->hand->cards.at ( hand_index ), this->getX() - 16, this->getY() + ( CARD_HEIGHT / 2 ) * hand_index );
      else
        this->card->DrawCard ( video_buffer, this->hand->cards.at ( hand_index ), this->getX(), this->getY() + ( CARD_HEIGHT / 2 ) * hand_index );
    }
    else if ( this->hand->cards.at( hand_index ).getPlayerID() == Card::PLAYER2 )
    {
      if ( this->hand->pos ( this->hand->getSelectedCard() ) == hand_index )
        this->card->DrawCard ( video_buffer, this->hand->cards.at ( hand_index ), this->getX() + 16, this->getY() + ( CARD_HEIGHT / 2 ) * hand_index );
      else
        this->card->DrawCard ( video_buffer, this->hand->cards.at ( hand_index ), this->getX(), this->getY() + ( CARD_HEIGHT / 2 ) * hand_index );
    }
  }
}

void Player::Update ( void )
{
  // TODO
}

