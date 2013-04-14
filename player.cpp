/******************************************************************************
    player.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "player.h"

Player::Player ( void )
{
  #ifdef DEBUG_PLAYER_OBJ
    std::cout << "Player::Player (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->x = 0;
  this->y = 0;
  this->type = 0;
  this->state = 0;
  this->score = 5;

  this->text_score.LoadTTF ( SCORE_FONTFACE, 32 );
  this->text_score.SetTextColor ( 255, 255, 255 );

  this->cursor = Sprite ( 26, 16 );
  this->cursor.SetX ( 80 );
  this->cursor.SetY ( 16 );
  this->cursor.LoadImage ( LEFT_CURSOR );
}

Player::~Player ( void )
{
  #ifdef DEBUG_PLAYER_OBJ
    std::cout << "Player::~Player (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif
}

void Player::Init ( void )
{
  // ...
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
      case SDLK_u:
        // skip turn
        this->SetType ( 1 );
        break;
      case SDLK_i:
        if ( this->GetType() == 0 )
        {
          this->SetType ( 1 );
        }
        break;
      case SDLK_LEFT:
        if ( this->GetType() == 0 )
        {
          this->mixer1.PlaySoundTrack ( CURSOR_MOVE, 1, 0);
          std::cout << this->type << "\n";
        }
        break;
      case SDLK_RIGHT:
        if ( this->GetType() == 0 )
        {
          this->mixer2.PlaySoundTrack ( CURSOR_CANCEL, 2, 0 );
          std::cout << this->type << "\n";
        }
        break;
      case SDLK_UP:
        if ( this->GetType() == 0 )
        {
          if ( this->cursor.GetY() > 16 && this->cursor.GetY() )
          {
            this->cursor.SetY ( this->cursor.GetY() - 32 );
          }
        }
        break;
      case SDLK_DOWN:
        if ( this->GetType() == 0 )
        {
          if ( this->cursor.GetY() >= 16 && this->cursor.GetY() <= 128 )
          {
            this->cursor.SetY ( this->cursor.GetY() + 32 );
          }
        }
        break;
      case SDLK_1:
        // move selected card to grid[0][0] if possible
        if ( this->GetType() == 0 )
        {
          this->debug.ListCards ( this->cards );
        }
        else
        {
          this->debug.ListCards ( this->cards );
        }
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
        if ( this->GetType() == 0 )
        {
          this->RemoveCard ( this->cards[3] ); // Jelleye
        }
        break;
    }
  }
}

void Player::Draw ( Gfx &engine )
{
  if ( this->GetType() == 0 )
  {
    for ( int idx = 0; idx < this->cards.size(); idx++ )
    {
      if ( this->isValid( this->cards[idx] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[idx], 16, 16, 0 ); // x = ( SCREEN_WIDTH - 64 ) / 20; y = ( SCREEN_HEIGHT - 32 ) / 12
      }
      idx+=1;
      if ( this->isValid( this->cards[idx] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[idx], 16, 48, 0 ); // x = ( SCREEN_WIDTH - 64 ) / 20; y = ( SCREEN_HEIGHT - 32 ) / 4
      }
      idx+=1;
      if ( this->isValid( this->cards[idx] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[idx], 16, 80, 0 ); // x = (SCREEN_WIDTH - 64 ) / 20; y = ( SCREEN_HEIGHT - 32 ) / 2.40
      }
      idx+=1;
      if ( this->isValid( this->cards[idx] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[idx], 16, 112, 0 ); // x = (SCREEN_WIDTH - 64 ) / 20; y = ( SCREEN_HEIGHT - 32 ) / 1.71
      }
      idx+=1;
      if ( this->isValid( this->cards[idx] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[idx], 16, 144, 0 ); // x = (SCREEN_WIDTH - 64 ) / 20; y = ( SCREEN_HEIGHT - 32 ) / 1.325
      }
    }
    this->cursor.Draw ( engine );
  }
  else if ( this->GetType() == 1 )
  {
    for ( int idx = 0; idx < this->cards.size(); idx++ )
    {
      if ( this->isValid( this->cards[idx] ) )
      {
        this->card.DrawCard ( engine, this->cards[idx], 304, 16, 1 );
      }
      idx+=1;
      if ( this->isValid( this->cards[idx] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[idx], 304, 48, 1 );
      }
      idx+=1;
      if ( this->isValid( this->cards[idx] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[idx], 304, 80, 1 );
      }
      idx+=1;
      if ( this->isValid( this->cards[idx] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[idx], 304, 112, 1 );
      }
      idx+=1;
      if ( this->isValid( this->cards[idx] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[idx], 304, 144, 1 );
      }
      idx+=1;
    }
  }
}

bool Player::DrawScore ( Gfx &engine, unsigned int x, unsigned int y )
{
  this->text_score.SetTextBuffer ( std::to_string ( this->score ) );
  this->text_score.DrawText ( engine, x, y );

  return true;
}
