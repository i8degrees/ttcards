/******************************************************************************
    player.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "player.h"

Player::Player ( void )
{
  //state = 0;
  Player::score = 5;
}

Player::~Player ( void )
{
}

unsigned int Player::GetScore ( void )
{
  return Player::score;
}

bool Player::SetScore ( unsigned int value )
{
  Player::score = value;
  return true;
}
