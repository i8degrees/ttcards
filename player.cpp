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

  this->x = 0;
  this->y = 0;
  this->type = 0;
  this->state = 0;
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

unsigned int Player::GetType ( void )
{
  return this->type;
}

void Player::SetType ( unsigned int player_type )
{
  this->type = player_type;
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

void Player::Input ( SDL_Event &input )
{
  if ( input.type == SDL_KEYDOWN )
  {
    switch ( input.key.keysym.sym )
    {
      default:
        break;
      case SDLK_ESCAPE:
        // skip turn
        break;
      case SDLK_LEFT:
        this->mixer1.PlaySoundTrack ( CURSOR_MOVE, 1, 0);
        break;
      case SDLK_RIGHT:
        this->mixer2.PlaySoundTrack ( CURSOR_CANCEL, 2, 0 );
        break;
      case SDLK_UP:
        // cursor select
        break;
      case SDLK_DOWN:
        // cursor select
        break;
      case SDLK_1:
        // move selected card to grid[0][0] if possible
        break;
      case SDLK_2:
        // move selected card to grid[1][0] if possible
        break;
      case SDLK_3:
        // move selected card to grid[2][0] if possible
        break;
      case SDLK_4:
        // move selected card to grid[0][0] if possible
        break;
      case SDLK_5:
        // move selected card to grid[1][1] if possible
        break;
      case SDLK_6:
        // move selected card to grid[1][2] if possible
        break;
      case SDLK_7:
        // move selected card to grid[2][0] if possible
        break;
      case SDLK_8:
        // move selected card to grid[2][1] if possible
        break;
      case SDLK_9:
        // move selected card to grid[3][2] if possible
        break;
    }
  }
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
