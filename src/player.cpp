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

  std::pair <int, int> coords ( 0, 0 ); // initialize x, y values
  this->id = 0; // no player
  this->state = 0; // not used
  this->score = 5; // initialize scoreboard

  //this->text_score.LoadTTF ( SCORE_FONTFACE, 32 );
  //this->text_score.SetTextColor ( 255, 255, 255 ); // white
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

std::pair <int, int> Player::getXY ( void )
{
  return coords;
}

void Player::setXY ( unsigned int x, unsigned int y )
{
  coords = std::make_pair ( x, y );
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
  {
    this->hand->cards[pid].player_id = id;
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

void Player::Draw ( Gfx *engine )
{
  unsigned int hand_index = 0;
  coords = getXY();

  for ( hand_index = 0; hand_index < this->hand->cards.size(); hand_index++ )
  {
    if ( this->hand->isValid ( this->hand->cards.at ( hand_index) ) == true )
    {
      if ( this->getID() == 1 )
      {
        if ( this->hand->isValid ( this->hand->GetSelectedCard() ) && this->hand->getCardIndex() == hand_index )
          this->card->DrawCard ( engine, this->hand->cards.at ( hand_index ), std::get<0>(coords) + 16, std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * hand_index );
        else
          this->card->DrawCard ( engine, this->hand->cards.at ( hand_index ), std::get<0>(coords), std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * hand_index );
      }
      else if ( this->getID() == 2 )
      {
        if ( this->hand->isValid ( this->hand->GetSelectedCard() ) && this->hand->getCardIndex() == hand_index )
          this->card->DrawCard ( engine, this->hand->cards.at ( hand_index ), std::get<0>(coords) - 16, std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * hand_index );
        else
          this->card->DrawCard ( engine, this->hand->cards.at ( hand_index ), std::get<0>(coords), std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * hand_index );
      }
    }
  }
}

void Player::Update ( Gfx *engine )
{
}

