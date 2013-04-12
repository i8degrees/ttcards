/******************************************************************************
    player.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "player.h"

Player::Player ( void )
{
  #ifdef DEBUG_PLAYER
    std::cout << "Hello, world! <From Player::Player>" << "\n" << std::endl;
  #endif

  //this->db.Load();
  //this->dealer.Init ( this->db.cards );

  //this->hand.Add ( this->dealer.selectCard ( 89 ) ); // Diablos
  //this->hand.Add ( this->dealer.selectCard ( 99 ) ); // Ward
  //this->hand.Add ( this->dealer.selectCard ( 0 ) ); // Geezard
  //this->hand.Add ( this->dealer.selectCard ( 109 ) ); // Squall
  //this->hand.Add ( this->dealer.selectCard ( 50 ) ); // Malboro

  //for ( int i = 0; i < MAX_HAND; i++ )
  //{
    //this->hand.Add ( this->dealer.Random() );
  //}


  //this->debug.ListCards ( this->hand.cards );
  //this->cdebug.ListCard ( this->hand.deck[0] );

  this->x = 0;
  this->y = 0;
  this->state = 0;
  this->turn = 0;
  this->score = 5;

  this->text_score.LoadTTF ( SCORE_FONTFACE, 32 );
  this->text_score.SetTextColor ( 255, 255, 255 );

}

Player::~Player ( void )
{
  #ifdef DEBUG_PLAYER
    std::cout << "Goodbye cruel world! <From Player::~Player>" << "\n" << std::endl;
  #endif
}

SDL_Rect Player::GetXY ( void )
{
  SDL_Rect coords;

  this->x = coords.x;
  this->y = coords.y;

  return coords;
}

void Player::SetXY ( unsigned int x, unsigned int y )
{
  this->x = x;
  this->y = y;
}

unsigned int Player::GetState ( void )
{
  return this->state;
}

void Player::SetState ( unsigned int state )
{
  this->state = state;
}

unsigned int Player::GetScore ( void )
{
  return this->score;
}

void Player::SetScore ( unsigned int score )
{
  this->score = score;
}

bool Player::Draw ( Gfx &engine, unsigned int x, unsigned int y ) // card id
{
  return true;
}

bool Player::DrawScore ( Gfx &engine, unsigned int x, unsigned int y )
{
  this->text_score.DrawText ( engine, std::to_string ( this->score ), x, y );

  return true;
}
