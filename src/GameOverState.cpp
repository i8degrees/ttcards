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
#include "GameOverState.hpp"

GameOverState::GameOverState  ( std::shared_ptr<GameObject> object,
                                enum GameOverType gameover_state
                              )
{
NOM_LOG_TRACE ( TTCARDS );

  this->game = object;
  this->show_results = false;
  this->gameover_state = gameover_state;
}

GameOverState::~GameOverState ( void )
{
NOM_LOG_TRACE ( TTCARDS );
}

void GameOverState::onInit ( void )
{
  // Fully reconstruct the player's hand by adding the cards placed on the board
  // back into their respective hand.
  //
  // 1. Make a a copy of each player's hand as it existed (on the board).
  // 2. Reset each player card found back to their original color.
  //
  // * Now we have two distinct copies made -- the original hand and the results
  // of the end game -- so we can deduce and display who wins and loses what.
  for ( nom::int32 y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( nom::int32 x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      Card card = this->game->board.get ( x, y );

      if ( card.getPlayerOwner() == Card::PLAYER1 )
      {
        card.setPlayerID( Card::PLAYER1 );
        this->game->hand[0].push_back ( card );
      }
      else if ( card.getPlayerOwner() == Card::PLAYER2 )
      {
        card.setPlayerID( Card::PLAYER2 );
        this->game->hand[1].push_back ( card );
      }
    }
  }

  if ( this->gameover_state == 1 ) // You won!
  {
    this->game->music_track.Stop();
    this->game->winning_track.Play();
  }

  // Commence the countdown on the showing of results!
  this->update.Start();
}

void GameOverState::onExit ( void )
{
NOM_LOG_TRACE ( TTCARDS );

  this->game->winning_track.Stop();
}

void GameOverState::onKeyDown ( nom::int32 key, nom::int32 mod )
{
  switch ( key )
  {
    default: break;

    // Start a new game
    case SDLK_RETURN: nom::GameStates::ChangeState ( CardsMenuStatePtr( new CardsMenuState ( this->game ) ) ); break;

    // Pause game
    case SDLK_p: nom::GameStates::PushState ( PauseStatePtr( new PauseState ( this->game ) ) ); break;
  }
}

void GameOverState::Update ( float delta_time )
{
  if ( this->update.getTicks() > 1500 )
  {
    this->update.Stop();
    this->show_results = true;
  }

  this->game->context.Update();
}

void GameOverState::Draw ( void* video_buffer )
{
  this->game->gameover_background.Draw ( video_buffer );

  // Show Player 2 hand on the top
  for ( nom::ulong cards_index = 0; cards_index < this->game->hand[1].size(); cards_index++ )
  {
    this->game->card.DrawCard ( video_buffer, this->game->hand[1].cards.at( cards_index ), PLAYER2_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * cards_index, PLAYER2_GAMEOVER_ORIGIN_Y );
  }

  // Show Player 1 hand on the bottom
  for ( nom::ulong cards_index = 0; cards_index < this->game->hand[0].size(); cards_index++ )
  {
    this->game->card.DrawCard ( video_buffer, this->game->hand[0].cards.at( cards_index ), PLAYER1_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * cards_index, PLAYER1_GAMEOVER_ORIGIN_Y );
  }

  if ( this->update.getTicks() > 1500 )
  {
    //nom::int32 rand_pick = nom::randomInteger ( 0, this->game->hand[0].size() );

    //Card lost_card = this->game->hand[0].getSelectedCard();
//NOM_LOG_INFO ( TTCARDS, lost_card.getName() + "Card lost" );

    //nom::sleep ( 1000 );

    // Restart game
    //nom::GameStates::ChangeState( CardsMenuStatePtr ( new CardsMenuState ( this->game ) ) );
    this->update.Stop();
    this->show_results = true;
  } // end if update
}
