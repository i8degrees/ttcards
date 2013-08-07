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
#include "GameOverState.h"

GameOver::GameOver ( std::shared_ptr<GameObject> object )
{
  #ifdef DEBUG_GAMEOVER_OBJ
    std::cout << "GameOver::GameOver (): Hello, world!" << "\n" << std::endl;
  #endif

  this->state = object;
}

GameOver::~GameOver ( void )
{
  #ifdef DEBUG_GAMEOVER_OBJ
    std::cout << "GameOver::~GameOver (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif
}

void GameOver::onInit ( void )
{
  // TODO
}

void GameOver::onExit ( void )
{
  std::cout << "\n" << "GameOver state onExit" << "\n";
}

void GameOver::Pause ( void )
{
  std::cout << "\n" << "GameOver state Paused" << "\n";
}

void GameOver::Resume ( void )
{
  std::cout << "\n" << "GameOver state Resumed" << "\n";
}

void GameOver::onKeyDown ( int32_t key, int32_t mod )
{
  switch ( key )
  {
    // Reset / New Game State
    case SDLK_r: nom::GameStates::ChangeState ( std::unique_ptr<CardsMenu>( new CardsMenu ( this->state ) ) ); break;
     // Pause State
    //case SDLK_p: this->engine->PopState (); break;

    default: break;
  }
}

void GameOver::Update ( nom::uint32 delta_time )
{
  this->state->context.Update();
}

void GameOver::Draw ( void* video_buffer )
{
  this->state->gameover_background.Draw ( video_buffer ); // draw static board background

  //this->gameOver_text.setTextBuffer ( "Game Over" );
  //signed int width = this->gameOver_text.getTextWidth ();
  //this->gameOver_text.Draw ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT - 128 ) / 2 );

  // Active player's card selection(s)
/*
  for ( nom::ulong cards_index = 0; cards_index < this->cards.size(); cards_index++ ) // TODO: std::get<1>(player_coords)
  {
    if ( this->cards.at ( cards_index ).getPlayerOwner() == Card::PLAYER1 )
      this->card.DrawCard ( video_buffer, this->cards.at ( cards_index ), PLAYER1_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * cards_index, PLAYER1_GAMEOVER_ORIGIN_Y );
    else if ( this->cards.at ( cards_index ).getPlayerOwner() == Card::PLAYER2 )
      this->card.DrawCard ( video_buffer, this->cards.at ( cards_index ), PLAYER2_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * cards_index, PLAYER2_GAMEOVER_ORIGIN_Y );
  }

  if ( this->state == 1 )
  {
    //this->gameOver_text.setTextBuffer ( "Player 1 wins!" );
    //signed int width = this->gameOver_text.getTextWidth ();
    //this->gameOver_text.Draw ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
  else if ( this->state == 2 )
  {
    //this->gameOver_text.setTextBuffer ( "Player 2 wins!" );
    //signed int width = this->gameOver_text.getTextWidth ();
    //this->gameOver_text.Draw ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
  else if ( this->state == 3 )
  {
    //this->gameOver_text.setTextBuffer ( "Tie!" );
    //signed int width = this->gameOver_text.getTextWidth ();
    //this->gameOver_text.Draw ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
*/
}
