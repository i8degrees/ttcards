/******************************************************************************
    player.cpp

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

  this->coords.setCoords ( 0, 0, 0, 0 ); // initialize X, Y origin coords
  id = 0;
  state = 0;
  score = 5;
}

Player::~Player ( void )
{
  #ifdef DEBUG_PLAYER_OBJ
    std::cout << "Player::~Player (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->card != NULL )
  {
    this->card = NULL;
  }

  if ( this->hand != NULL )
  {
    this->hand = NULL;
  }
}

void Player::Init ( CardHand *player_cards, CardView *card_gfx )
{
  this->hand = player_cards;
  this->card = card_gfx;
}

unsigned int Player::getX ( void )
{
  return this->coords.getX();
}

unsigned int Player::getY ( void )
{
  return this->coords.getY();
}

std::pair <int, int> Player::getXY ( void )
{
  return this->coords.getXY();
}

void Player::setXY ( unsigned int x, unsigned int y )
{
  this->coords.setXY ( x, y );
}

unsigned int Player::getID ( void )
{
  return this->id;
}

// Maps the player's (card) hand with their respective ID; this keeps track of
// who's card is which and is used in CardView -- the rendering the card
// background -- and most importantly, in the Board class where we compare cards
// placed to determine whom's card to flip over to the respective player.
void Player::setID ( unsigned int id )
{
  this->id = id;

  for ( int pid = 0; pid < this->hand->cards.size(); pid++ )
    this->hand->cards[pid].setPlayerID ( id );
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

void Player::Draw ( Gfx *engine )
{
  unsigned int hand_index = 0;

  for ( hand_index = 0; hand_index < this->hand->getCount(); hand_index++ )
  {
    if ( this->hand->isValid ( this->hand->cards.at ( hand_index) ) == true )
    {
      if ( this->getID() == 1 )
      {
        if ( this->hand->isValid ( this->hand->getSelectedCard() ) && this->hand->cardPosition ( this->hand->getSelectedCard() ) == hand_index )
          this->card->DrawCard ( engine, this->hand->cards.at ( hand_index ), this->getX() - 16, this->getY() + ( CARD_HEIGHT / 2 ) * hand_index );
        else
          this->card->DrawCard ( engine, this->hand->cards.at ( hand_index ), this->getX(), this->getY() + ( CARD_HEIGHT / 2 ) * hand_index );
      }
      else if ( this->getID() == 2 )
      {
        if ( this->hand->isValid ( this->hand->getSelectedCard() ) && this->hand->cardPosition ( this->hand->getSelectedCard() ) == hand_index )
          this->card->DrawCard ( engine, this->hand->cards.at ( hand_index ), this->getX() + 16, this->getY() + ( CARD_HEIGHT / 2 ) * hand_index );
        else
          this->card->DrawCard ( engine, this->hand->cards.at ( hand_index ), this->getX(), this->getY() + ( CARD_HEIGHT / 2 ) * hand_index );
      }
    }
  }
}

void Player::Update ( Gfx *engine )
{
  // TODO
}

