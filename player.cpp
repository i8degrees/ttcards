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
  this->id = 0;
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

unsigned int Player::GetID ( void )
{
  return this->id;
}

void Player::SetID ( unsigned int id )
{
  this->id = id;
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
        this->SetID ( 1 ); // player2
        break;
      case SDLK_i:
        if ( this->GetID() == 0 ) // player1
        {
          this->SetID ( 1 ); // player2
        }
        break;
      case SDLK_LEFT:
        if ( this->GetID() == 0 ) // player1
        {
          this->mixer1.PlaySoundTrack ( CURSOR_MOVE, 1, 0);
        }
        break;
      case SDLK_RIGHT:
        if ( this->GetID() == 0 ) // player1
        {
          this->mixer2.PlaySoundTrack ( CURSOR_CANCEL, 2, 0 );
        }
        break;
      case SDLK_UP:
        if ( this->GetID() == 0 ) // player1
        {
          if ( this->cursor.GetY() > 16 && this->cursor.GetY() )
          {
            this->cursor.SetY ( this->cursor.GetY() - 32 );
          }
        }
        break;
      case SDLK_DOWN:
        if ( this->GetID() == 0 ) // player1
        {
          if ( this->cursor.GetY() >= 16 && this->cursor.GetY() <= 128 )
          {
            this->cursor.SetY ( this->cursor.GetY() + 32 );
          }
        }
        break;
      case SDLK_LEFTBRACKET: // DEBUG
        if ( this->GetID() == 0 ) // player1
        {
          this->debug.ListCards ( this->cards );
          this->board.Draw();
        }
        break;
      case SDLK_RIGHTBRACKET: // DEBUG
        if ( this->GetID() == 0 )
        {
          std::cout << this->GetID() << "\n" << std::endl;
        }
        break;
      case SDLK_1:
        // move selected card to grid[0][0] if possible
        if ( this->GetID() == 0 ) // player1
        {
          if ( this->board.GetStatus ( 0, 0 ) == false )
          {
            this->board.UpdateBoard ( 0, 0, this->cards[0].id );
            //this->RemoveCard ( this->cards[3] ); // Jelleye
          }
          this->board.Draw();
        }
        break;
      case SDLK_2:
        // move selected card to grid[0][1] if possible
        if ( this->GetID() == 0 ) // player1
        {
          if ( this->board.GetStatus ( 0, 1 ) == false )
          {
            this->board.UpdateBoard ( 0, 1, this->cards[1].id );
          }
          this->board.Draw();
        }
        break;
      case SDLK_3:
        // move selected card to grid[0][2] if possible
        if ( this->GetID() == 0 ) // player1
        {
          if ( this->board.GetStatus ( 0, 2 ) == false )
          {
            this->board.UpdateBoard ( 0, 2, this->cards[2].id );
          }
          this->board.Draw();
        }
        break;
      case SDLK_4:
        // move selected card to grid[1][0] if possible
        if ( this->GetID() == 0 ) // player1
        {
          if ( this->board.GetStatus ( 1, 0 ) == false )
          {
            this->board.UpdateBoard ( 1, 0, this->cards[3].id );
          }
          this->board.Draw();
        }
        break;
      case SDLK_5:
        // move selected card to grid[1][1] if possible
        if ( this->GetID() == 0 ) // player1
        {
          if ( this->board.GetStatus ( 1, 1 ) == false )
          {
            this->board.UpdateBoard ( 1, 1, this->cards[4].id );
          }
          this->board.Draw();
        }
        break;
      case SDLK_6:
        // move selected card to grid[1][2] if possible
        if ( this->GetID() == 0 ) // player1
        {
          if ( this->board.GetStatus ( 1, 2 ) == false )
          {
            /* Sanity check */
            this->board.UpdateBoard ( 1, 2, this->cards[5].id );
          }
          this->board.Draw();
        }
        break;
      case SDLK_7:
        // move selected card to grid[2][0] if possible
        break;
      case SDLK_8:
        // move selected card to grid[2][1] if possible
        break;
      case SDLK_9:
        // move selected card to grid[2][2] if possible
        break;
    }
  }
}

void Player::Draw ( Gfx &engine )
{
  unsigned int hand_index = 0;

  if ( this->GetID() == 0 ) // player1
  {
    for ( hand_index = 0; hand_index < this->cards.size(); hand_index++ )
    {
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( this->board.GetStatus ( 0, 0 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH * hand_index ), BOARD_ORIGIN_Y + ( CARD_HEIGHT * hand_index ), 0 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER1_ORIGIN_X + ( CARD_WIDTH / 2 ) * hand_index, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index, 0 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( this->board.GetStatus ( 0, 1 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH * hand_index ), BOARD_ORIGIN_Y, 0 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index , 0 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( this->board.GetStatus ( 0, 2 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH * hand_index ), BOARD_ORIGIN_Y, 0 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index, 0 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( this->board.GetStatus ( 1, 0 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X, BOARD_ORIGIN_Y + ( CARD_HEIGHT ), 0 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index, 0 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( this->board.GetStatus ( 1, 1 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH ), BOARD_ORIGIN_Y + ( CARD_HEIGHT), 0 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index, 0 );
      }
    }
    this->cursor.Draw ( engine );
  }
  else if ( this->GetID() == 1 ) // player2
  {
    for ( hand_index = 0; hand_index < this->cards.size(); hand_index++ )
    {
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[hand_index], 304, 16, 1 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[hand_index], 304, 48, 1 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[hand_index], 304, 80, 1 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[hand_index], 304, 112, 1 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        this->card.DrawCard ( engine, this->cards[hand_index], 304, 144, 1 );
      }
      hand_index+=1;
    }
  }
}

bool Player::DrawScore ( Gfx &engine, unsigned int x, unsigned int y )
{
  this->text_score.SetTextBuffer ( std::to_string ( this->score ) );
  this->text_score.DrawText ( engine, x, y );

  return true;
}
