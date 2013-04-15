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
  this->index = 0;

  this->text_score.LoadTTF ( SCORE_FONTFACE, 32 );
  this->text_score.SetTextColor ( 255, 255, 255 );

  this->left_cursor = Sprite ( 26, 16 );
  this->left_cursor.SetX ( PLAYER1_CURSOR_ORIGIN_X );
  this->left_cursor.SetY ( PLAYER1_CURSOR_ORIGIN_Y );
  this->left_cursor.LoadImage ( LEFT_CURSOR );

  this->right_cursor = Sprite ( 26, 16 );
  this->right_cursor.SetX ( PLAYER2_CURSOR_ORIGIN_X );
  this->right_cursor.SetY ( PLAYER2_CURSOR_ORIGIN_Y );
  this->right_cursor.LoadImage ( RIGHT_CURSOR );
}

Player::~Player ( void )
{
  #ifdef DEBUG_PLAYER_OBJ
    std::cout << "Player::~Player (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif
}

void Player::Init ( void )
{
  // Stub
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

void Player::Input ( unsigned int type, SDLKey key, SDLMod mod, Board &board )
{
  if ( type == SDL_KEYDOWN )
  {
    if ( key == SDLK_ESCAPE ) // skip turn
    {
      // Stub
    }

    else if ( key == SDLK_u ) // DEBUG
    {
      if ( this->GetState() == 0 ) // player1
        this->SetState ( 1 ); // player2
      else
        this->SetState ( 0 ); // player1
    }

    else if ( key == SDLK_LEFT )
    {
      if ( this->GetID() == 0 && this->GetState() == 0 ) // player1
      {
        if ( this->left_cursor.GetX() > 96 )
        {
          this->left_cursor.SetX ( this->left_cursor.GetX () - 96 );
        }
      }
      else if ( this->GetState() == 1 ) // player2
      {
        if ( this->right_cursor.GetX() > 96 )
        {
          this->right_cursor.SetX ( this->right_cursor.GetX () - 96 );
        }
      }
    }

    else if ( key == SDLK_RIGHT )
    {
      if ( this->GetID() == 0 && this->GetState() == 0 ) // player1
      {
        if ( this->left_cursor.GetX() < 224 )
        {
          this->left_cursor.SetX ( this->left_cursor.GetX () + 96 );
        }
      }
      else if ( this->GetState() == 1 ) // player2
      {
        if ( this->right_cursor.GetX() < 224 )
        {
          this->right_cursor.SetX ( this->right_cursor.GetX () + 96 );
        }
      }
    }

    else if ( key == SDLK_UP )
    {
      if ( this->GetID() == 0 && this->GetState() == 0 ) // player1
      {
        if ( this->left_cursor.GetY() > 16 && this->left_cursor.GetX() == PLAYER1_CURSOR_ORIGIN_X )
        {
          this->left_cursor.SetY ( this->left_cursor.GetY() - 32 );
          if ( index > 0 && index <= this->cards.size() )
          {
            index = index - 1;
            //this->SelectCard ( this->cards[index] );
          }
        }
        else if ( this->left_cursor.GetX() > 96 && this->left_cursor.GetY() > 16 )
        {
          this->left_cursor.SetY ( this->left_cursor.GetY() - 64 );
        }
      }
      else if ( this->GetState() == 1 ) // player2
      {
        if ( this->right_cursor.GetY() > 16 && this->right_cursor.GetX() == PLAYER2_CURSOR_ORIGIN_X )
        {
          this->right_cursor.SetY ( this->right_cursor.GetY() - 32 );
          if ( index > 0 && index <= this->cards.size() )
          {
            index = index - 1;
            //this->SelectCard ( this->cards[index] );
          }
        }
        else if ( this->right_cursor.GetX() > 96 && this->right_cursor.GetY() > 16 )
        {
          this->right_cursor.SetY ( this->right_cursor.GetY() - 64 );
        }
      }
    }

    else if ( key == SDLK_DOWN )
    {
      if ( this->GetID() == 0 && this->GetState() == 0 ) // player1
      {
        if ( this->left_cursor.GetY() < 128 && this->left_cursor.GetX() == PLAYER1_CURSOR_ORIGIN_X )
        {
          this->left_cursor.SetY ( this->left_cursor.GetY() + 32 );
        }
        else if ( this->left_cursor.GetX() > 96 && this->left_cursor.GetY() < 128 )
        {
          this->left_cursor.SetY ( this->left_cursor.GetY() + 64 );
        }
      }
      else if ( this->GetState() == 1 ) // player2
      {
        if ( this->right_cursor.GetY() < 128 && this->right_cursor.GetX() == PLAYER2_CURSOR_ORIGIN_X )
        {
          this->right_cursor.SetY ( this->right_cursor.GetY() + 32 );
        }
        else if ( this->right_cursor.GetX() > 96 && this->right_cursor.GetY() < 128 )
        {
          this->right_cursor.SetY ( this->right_cursor.GetY() + 64 );
        }
      }
    }

    else if ( key == SDLK_0 ) // DEBUG
    {
      if ( this->GetID() == 0 ) // player1
      {
        std::cout << this->GetID() << "\n" << std::endl;
      }
    }

    else if ( key == SDLK_LEFTBRACKET ) // DEBUG
    {
      if ( this->GetID() == 0 ) // player1
      {
        this->debug.ListCards ( this->cards );
        board.Draw();
      }
    }

    else if ( key == SDLK_RIGHTBRACKET ) // DEBUG
    else if ( key == SDLK_SPACE )
    {
      this->debug.ListCards ( this->cards );
      board.Draw();
    }

    else if ( key == SDLK_1 && mod == KMOD_LMETA )
    {
      // ...
      if ( this->GetID() == 0 && this->GetState() == 0 ) // player1
        this->SelectCard ( this->cards[index] );
      else if ( this->GetID() == 1 && this->GetState() == 1 )
        this->SelectCard ( this->cards[index] );
    }

    else if ( key == SDLK_1 ) // move selected card to grid[0][0] if possible
    {
      if ( this->GetState() == 0 && this->GetID() == 0 ) // player1
      {
        if ( board.GetStatus ( 0, 0 ) == false )
        {
          board.UpdateBoard ( 0, 0, this->cards[0].id );
          //this->RemoveCard ( this->cards[0] ); // Jelleye
        }

      }
      else if ( this->GetState() == 1 && this->GetID() == 1 ) // player2
      {
        if ( board.GetStatus ( 0, 0 ) == false )
        {
          board.UpdateBoard ( 0, 0, this->cards[0].id );
          //this->RemoveCard ( this->cards[0] ); // Jelleye
        }
      }
      board.Draw();
    }

    else if ( key == SDLK_2 ) // move selected card to grid[0][1] if possible
    {
      if ( this->GetID() == 0 && this->GetState() == 0 ) // player1
      {
        if ( board.GetStatus ( 0, 1 ) == false )
        {
          board.UpdateBoard ( 0, 1, this->cards[1].id );
        }
      }
      else if ( this->GetState() == 1 && this->GetID() == 1 ) // player2
      {
        if ( board.GetStatus ( 0, 1 ) == false )
        {
          board.UpdateBoard ( 0, 1, this->cards[1].id );
        }
      }
      board.Draw();
    }

    else if ( key == SDLK_3 ) // move selected card to grid[0][2] if possible
    {
      if ( this->GetID() == 0 && this->GetState() == 0 ) // player1
      {
        if ( board.GetStatus ( 0, 2 ) == false )
        {
          board.UpdateBoard ( 0, 2, this->cards[2].id );
        }
      }
      else if ( this->GetID() == 1 && this->GetState() == 1 ) // player2
      {
        if ( board.GetStatus ( 0, 2 ) == false )
        {
          board.UpdateBoard ( 0, 2, this->cards[2].id );
        }
      }
      board.Draw();
    }

    else if ( key == SDLK_4 ) // move selected card to grid[1][0] if possible
    {
      if ( this->GetID() == 0 && this->GetState() == 0 ) // player1
      {
        if ( board.GetStatus ( 1, 0 ) == false )
        {
          board.UpdateBoard ( 1, 0, this->cards[3].id );
        }
      }
      else if ( this->GetID() == 1 && this->GetState() == 1 ) // player2
      {
        if ( board.GetStatus ( 1, 0 ) == false )
        {
          board.UpdateBoard ( 1, 0, this->cards[3].id );
        }
      }
      board.Draw();
    }

    else if ( key == SDLK_5 ) // move selected card to grid[1][1] if possible
    {
      if ( this->GetID() == 0 && this->GetState() == 0 ) // player1
      {
        if ( board.GetStatus ( 1, 1 ) == false )
        {
          board.UpdateBoard ( 1, 1, this->cards[4].id );
        }
      }
      else if ( this->GetID() == 1 && this->GetState() == 1 ) // player2
      {
        if ( board.GetStatus ( 1, 1 ) == false )
        {
          board.UpdateBoard ( 1, 1, this->cards[4].id );
        }
      }
      board.Draw();
    }

    else if ( key == SDLK_6 ) // move selected card to grid[1][2] if possible
    {
      if ( this->GetID() == 0 && this->GetState() == 0 ) // player1
      {
        if ( board.GetStatus ( 1, 2 ) == false )
        {
          /* Sanity check */
          board.UpdateBoard ( 1, 2, this->cards[5].id );
        }
      }
      else if ( this->GetID() == 1 && this->GetState() == 1 ) // player2
      {
        if ( board.GetStatus ( 1, 2 ) == false )
        {
          /* Sanity check */
          board.UpdateBoard ( 1, 2, this->cards[5].id );
        }
      }
      board.Draw();
    }

    else if ( key == SDLK_7 ) // move selected card to grid[2][0] if possible
    {
      // Stub
    }

    else if ( key == SDLK_8 ) // move selected card to grid[2][1] if possible
    {
      // Stub
    }

    else if ( key == SDLK_9 ) // move selected card to grid[2][2] if possible
    {
      // Stub
    }
  }
}

void Player::Draw ( Gfx &engine, Board &board )
{
  unsigned int hand_index = 0;

  if ( this->GetID() == 0 ) // player1
  {
    for ( hand_index = 0; hand_index < this->cards.size(); hand_index++ )
    {
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( board.GetStatus ( 0, 0 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH * hand_index ), BOARD_ORIGIN_Y + ( CARD_HEIGHT * hand_index ), 0 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER1_ORIGIN_X + ( CARD_WIDTH / 2 ) * hand_index, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index, 0 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( board.GetStatus ( 0, 1 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH * hand_index ), BOARD_ORIGIN_Y, 0 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index , 0 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( board.GetStatus ( 0, 2 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH * hand_index ), BOARD_ORIGIN_Y, 0 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index, 0 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( board.GetStatus ( 1, 0 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X, BOARD_ORIGIN_Y + ( CARD_HEIGHT ), 0 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index, 0 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( board.GetStatus ( 1, 1 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH ), BOARD_ORIGIN_Y + ( CARD_HEIGHT), 0 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index, 0 );
      }
    }
  }
  else if ( this->GetID() == 1 ) // player2
  {
    for ( hand_index = 0; hand_index < this->cards.size(); hand_index++ )
    {
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( board.GetStatus ( 0, 0 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH * hand_index ), BOARD_ORIGIN_Y + ( CARD_HEIGHT * hand_index ), 0 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y, 1 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( board.GetStatus ( 0, 1 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH * hand_index ), BOARD_ORIGIN_Y, 1 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index, 1 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( board.GetStatus ( 0, 2 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH * hand_index ), BOARD_ORIGIN_Y, 1 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index, 1 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( board.GetStatus ( 1, 0 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X, BOARD_ORIGIN_Y + ( CARD_HEIGHT ), 1 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index, 1 );
      }
      hand_index+=1;
      if ( this->isValid ( this->cards[hand_index] ) == true )
      {
        if ( board.GetStatus ( 1, 1 ) == this->cards[hand_index].id )
          this->card.DrawCard ( engine, this->cards[hand_index], BOARD_ORIGIN_X + ( CARD_WIDTH ), BOARD_ORIGIN_Y + ( CARD_HEIGHT), 1 );
        else
          this->card.DrawCard ( engine, this->cards[hand_index], PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index, 1 );
      }
    }
  }
  if ( this->GetState() == 0 && this->GetID() == 0 )
  {
    this->left_cursor.Draw ( engine );
  }
  else if ( this->GetState() == 1 && this->GetID() == 1 )
  {
    this->right_cursor.Draw ( engine );
  }
}

void Player::DrawScore ( Gfx &engine, unsigned int x, unsigned int y )
{
  this->text_score.SetTextBuffer ( std::to_string ( this->score ) );
  this->text_score.DrawText ( engine, x, y );
}
