/******************************************************************************

    Human Player Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "player.h"

Player::Player ( void )
{
  #ifdef DEBUG_PLAYER_OBJ
    std::cout << "Player::Player (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->hand = NULL;

  this->coords = nom::Coords ( 0, 0, 0, 0 ); // initialize X, Y origin coords
  id = 0;
  state = 0;
  score = 5;
}

Player::~Player ( void )
{
  #ifdef DEBUG_PLAYER_OBJ
    std::cout << "Player::~Player (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->hand != NULL )
  {
    this->hand = NULL;
  }
}

void Player::Init ( CardHand *player_cards )
{
  this->hand = player_cards;
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
  return this->state;
}

void Player::setState ( unsigned int state )
{
  this->state = state;
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
    if ( this->hand->isValid ( this->hand->cards.at ( hand_index) ) == true )
    {
      if ( this->getID() == 1 )
      {
        if ( this->hand->isValid ( this->hand->getSelectedCard() ) && this->hand->pos ( this->hand->getSelectedCard() ) == hand_index )
          this->card.DrawCard ( video_buffer, this->hand->cards.at ( hand_index ), this->getX() - 16, this->getY() + ( CARD_HEIGHT / 2 ) * hand_index );
        else
          this->card.DrawCard ( video_buffer, this->hand->cards.at ( hand_index ), this->getX(), this->getY() + ( CARD_HEIGHT / 2 ) * hand_index );
      }
      else if ( this->getID() == 2 )
      {
        if ( this->hand->isValid ( this->hand->getSelectedCard() ) && this->hand->pos ( this->hand->getSelectedCard() ) == hand_index )
          this->card.DrawCard ( video_buffer, this->hand->cards.at ( hand_index ), this->getX() + 16, this->getY() + ( CARD_HEIGHT / 2 ) * hand_index );
        else
          this->card.DrawCard ( video_buffer, this->hand->cards.at ( hand_index ), this->getX(), this->getY() + ( CARD_HEIGHT / 2 ) * hand_index );
      }
    }
  }
}

void Player::Update ( void )
{
  // TODO
}

