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

GameOver::GameOver ( std::shared_ptr<GameObject> object, nom::uint32 gameover_state )
{
#ifdef DEBUG_GAMEOVER_OBJ
  std::cout << "GameOver::GameOver (): Hello, world!" << "\n" << std::endl;
#endif

  this->state = object;
  this->show_results = false;
  this->gameover_state = gameover_state;
}

GameOver::~GameOver ( void )
{
#ifdef DEBUG_GAMEOVER_OBJ
  std::cout << "GameOver::~GameOver (): " << "Goodbye cruel world!" << "\n" << std::endl;
#endif
}

void GameOver::onInit ( void )
{
  // Fully reconstruct the player's hand by adding the cards placed on the board
  // back into their respective hand.
  //
  // 1. Make a a copy of each player's hand as it existed (on the board).
  // 2. Reset each player card found back to their original color.
  // 3. Add the cards found on the board back onto the respective player's hand.
  //
  // * Now we have two distinct copies made -- the original hand and the results
  // of the end game -- so we can deduce and display who wins and loses what.
  for ( nom::int32 y = 0; y < BOARD_GRID_HEIGHT; y++ )
  {
    for ( nom::int32 x = 0; x < BOARD_GRID_WIDTH; x++ )
    {
      Card card = this->state->board.getCard ( x, y );

      if ( card.getPlayerOwner() == Card::PLAYER1 )
      {
        this->player_cards[0].push_back ( card );
        card.setPlayerID( Card::PLAYER1 );
        this->state->hand[0].addCard ( card );
      }
      else if ( card.getPlayerOwner() == Card::PLAYER2 )
      {
        this->player_cards[1].push_back ( card );
        card.setPlayerID( Card::PLAYER2 );
        this->state->hand[1].addCard ( card );
      }
    }
  }

  // Commence the countdown on the showing of results!
  this->update.Start();
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

void GameOver::onKeyDown ( nom::int32 key, nom::int32 mod )
{
  switch ( key )
  {
    default: break;

    case SDLK_p: nom::GameStates::PushState ( std::unique_ptr<PauseState>( new PauseState ( this->state ) ) ); break;
    // Reset (New Game)
    case SDLK_r: nom::GameStates::ChangeState ( std::unique_ptr<CardsMenu>( new CardsMenu ( this->state ) ) ); break;
  }
}

void GameOver::Update ( nom::uint32 delta_time )
{
  if ( this->update.getTicks() > 2500 )
  {
    this->update.Stop();
    this->show_results = true;
  }

  this->state->context.Update();
}

void GameOver::Draw ( void* video_buffer )
{
  this->state->gameover_background.Draw ( video_buffer );

  // Show Player 1 starting hand
  for ( nom::ulong cards_index = 0; cards_index < this->state->hand[0].size(); cards_index++ )
    this->state->card.DrawCard ( video_buffer, this->state->hand[0].cards.at( cards_index ), PLAYER1_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * cards_index, PLAYER1_GAMEOVER_ORIGIN_Y );

  // Show Player 2 starting hand
  for ( nom::ulong cards_index = 0; cards_index < this->state->hand[1].size(); cards_index++ )
    this->state->card.DrawCard ( video_buffer, this->state->hand[1].cards.at( cards_index ), PLAYER2_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * cards_index, PLAYER2_GAMEOVER_ORIGIN_Y );

  // Update both player's hand with the ending game round results when the time
  // is right
  if ( this->show_results == true )
  {
    switch ( this->gameover_state )
    {
      case 0:
      default: /* Do nothing, it's a tie */ break;

      case 2: // Player 1 lost; show what cards they lost
      {
        for ( nom::ulong cards_index = 0; cards_index < this->player_cards[0].size(); cards_index++ )
          this->state->card.DrawCard ( video_buffer, this->player_cards[0].at( cards_index ), PLAYER1_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * cards_index, PLAYER1_GAMEOVER_ORIGIN_Y );
      }
      break;

      case 1: // Player 1 won; show what cards they won
      {
        for ( nom::ulong cards_index = 0; cards_index < this->player_cards[1].size(); cards_index++ )
          this->state->card.DrawCard ( video_buffer, this->player_cards[1].at( cards_index ), PLAYER2_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * cards_index, PLAYER2_GAMEOVER_ORIGIN_Y );
      }
      break;
    } // this->gameover_state
  } // this->show_results
}
