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

#include "PauseState.hpp"
#include "CardsMenuState.hpp"
#include "ContinueMenuState.hpp"

using namespace nom;

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
  nom::Gradient linear;

  // Initialize interface cursor
  this->cursor = GameOverStateCursor ( "images/cursors.json" );

  if ( this->cursor.load ( this->game->config.getString("INTERFACE_CURSOR"), nom::Color4u::Black, true ) == true )
  {
    if ( this->game->config.getString("SCALE_ALGORITHM") == "scale2x" )
    {
      this->cursor.resize ( nom::ResizeAlgorithm::scale2x );
    }
    else if ( this->game->config.getString("SCALE_ALGORITHM") == "hqx" )
    {
      this->cursor.resize ( nom::ResizeAlgorithm::hq2x );
    }

    this->cursor.set_position_map ( &this->game->hand[1] );
    this->cursor.setSize ( CURSOR_WIDTH, CURSOR_HEIGHT );
    this->cursor.setPosition  (
                                PLAYER2_GAMEOVER_CURSOR_ORIGIN_X,
                                PLAYER2_GAMEOVER_CURSOR_ORIGIN_Y
                              );
    this->cursor.setSheetID ( INTERFACE_CURSOR_RIGHT );
  }
  else // EPIC FAIL
  {
#if defined ( DEBUG )
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("INTERFACE_CURSOR") );
#else
    nom::DialogMessageBox ( "Error", "Could not load resource file: " + this->game->config.getString("INTERFACE_CURSOR") );
#endif
    exit ( EXIT_FAILURE );
  }

/*
  if ( this->gameover_state == 0 ) // Draw
  {
  }
  else if ( this->gameover_state == 1 ) // Player 1 won
  {
*/


  //} // end gameover_state == 1
/*
  else if ( this->gameover_state == 2 ) // Player 2 won
  {
    this->cursor.setPosition  (
                                PLAYER1_GAMEOVER_ORIGIN_X,
                                PLAYER1_GAMEOVER_ORIGIN_Y * 2
                              );

    this->cursor.setSheetID ( INTERFACE_CURSOR_LEFT );
  }
*/
// REMOVE ME
this->game->hand[0].randomize ( 8, 10, this->game->collection );
this->game->hand[1].randomize ( 1, 7, this->game->collection );
// REMOVE ME
for ( auto idx = 0; idx < MAX_PLAYER_HAND; idx++ )
{
this->game->hand[0].cards[idx].setPlayerID(Card::PLAYER1);
this->game->hand[1].cards[idx].setPlayerID(Card::PLAYER2);
}
  // Reset the player's hand back to the front, so our cursor tracking is
  // always accurate.
  //
  // POSSIBLE BUG; if we try to do this before we initialize the game cursor,
  // we crash for some strange reason! I think it may have something to do
  // with the order of initialization?
  this->game->hand[1].front();

  // Initialize our message box background containers (linear gradient)
  linear.setStartColor ( nom::Color4u ( 67, 67, 67, 255 ) );
  linear.setEndColor ( nom::Color4u ( 99, 99, 99, 255 ) );
  linear.setFillDirection ( nom::Gradient::FillDirection::Left );

  // Top display message box; "description" info box
  this->info_box = nom::ui::MessageBox  (
                                          INFO_BOX_ORIGIN_X, DEBUG_BOX_ORIGIN_Y,
                                          INFO_BOX_WIDTH, INFO_BOX_HEIGHT,
                                          nom::ui::FrameStyle::Gray, linear
                                        );

  // Bottom display message box; card info (card name)
  this->card_info_box = nom::ui::MessageBox (
                                              INFO_BOX_ORIGIN_X, INFO_BOX_ORIGIN_Y,
                                              INFO_BOX_WIDTH, INFO_BOX_HEIGHT,
                                              nom::ui::FrameStyle::Gray, linear
                                            );

  this->info_box.setWindowTitleFont ( &this->game->info_small_text );
  this->info_box.setLabelFont ( &this->game->info_text );
  this->info_box.setLabel ( "Select 1 card(s) you want" );
  this->info_box.setLabelTextAlignment ( nom::IFont::TextAlignment::MiddleCenter );

  this->card_info_box.setWindowTitleFont ( &this->game->info_small_text );
  this->card_info_box.setLabelFont ( &this->game->info_text );
  this->card_info_box.setLabel ( this->game->hand[1].getSelectedCard().getName() );
  this->card_info_box.setLabelTextAlignment ( nom::IFont::TextAlignment::MiddleCenter );

  // Fully reconstruct the player's hand by adding the cards placed on the board
  // back into each player's respective hand.
  //
  // 1. Make a a copy of each player's hand as it existed (on the board).
  // 2. Reset each player card found back to their original color.
  //
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
/*
  if ( this->gameover_state == 0 ) // Draw
  {
    // Do stuff related to game tie
  }
  else if ( this->gameover_state == 1 ) // You won!
  {
*/
    // Play "You won!" music track
    this->game->music_track.Stop();
#if ! defined ( DEBUG )
    this->game->winning_track.Play();
#endif

  //}
/*
  else if ( this->gameover_state == 2 ) // You lost
  {
    //nom::int32 rand_pick = nom::randomInteger ( 0, this->game->hand[0].size() );
    this->game->hand[0].front();
  }
*/
  //this->selected_card = this->game->hand[1].getSelectedCard();

  // Commence the countdown on the showing of results!
  this->transistion.start();
}

void GameOverState::onExit ( void )
{
NOM_LOG_TRACE ( TTCARDS );

  this->game->winning_track.Stop();
}

void GameOverState::Resume ( nom::int32 response )
{
  if ( response == 0 )
  {
    event.dispatch ( nom::EventDispatcher::UserEvent::Animation );
  }
}

void GameOverState::onKeyDown ( int32 key, int32 mod, uint32 window_id )
{
  switch ( key )
  {
    default: break;

    // Start a new game
    case SDLK_RETURN: nom::GameStates::ChangeState ( CardsMenuStatePtr( new CardsMenuState ( this->game ) ) ); break;

    // Pause game
    case SDLK_p: nom::GameStates::PushState ( PauseStatePtr( new PauseState ( this->game ) ) ); break;

    case SDLK_LEFT: this->cursor.move_left(); break;
    case SDLK_RIGHT: this->cursor.move_right(); break;
    case SDLK_SPACE:
    {
      this->selected_card = this->game->hand[1].getSelectedCard();

      nom::GameStates::PushState ( ContinueMenuStatePtr ( new ContinueMenuState ( this->game ) ) );
    }
    break;
  }
}

void GameOverState::onMouseLeftButtonDown ( nom::int32 x, nom::int32 y, nom::uint32 window_id )
{
  nom::Coords coords ( x, y ); // mouse input coordinates

  // Player hand selection checks
  for ( nom::int32 idx = 0; idx < this->game->hand[1].size(); idx++ )
  {
    this->player2_pos = nom::Coords (
                                      PLAYER2_GAMEOVER_ORIGIN_X,
                                      PLAYER2_GAMEOVER_ORIGIN_Y,
                                      ( CARD_WIDTH ) * ( idx + 1 ), CARD_WIDTH
                                    );

    if ( player2_pos.intersects ( coords ) )
    {
      // 1. Update cursor position
      // 2. Update player's selected card
      // 3. Update the card info message box
      // 4. Play sound event

      this->game->hand[1].set_position ( idx );
      this->cursor.setPosition ( PLAYER2_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * idx, this->cursor.y() );

      this->game->hand[1].selectCard ( this->game->hand[1].cards[ idx ] );
      Card selected_card = this->game->hand[1].getSelectedCard();

      this->card_info_box.setLabel ( selected_card.getName() );

      this->game->cursor_move.Play();
      // We must break the loop here upon the end of a matching coords check
      // in order to prevent a nasty "last card stays permanently selected"
      // bug from cropping back up!
      break;
    }
  }
}

void GameOverState::onMouseWheel ( nom::int32 x, nom::int32 y, nom::uint32 window_id )
{
  if ( y > 0 )
  {
    this->cursor.move_left();
  }
  else if ( y < 0 )
  {
    this->cursor.move_right();
  }
}

void GameOverState::onJoyButtonDown ( nom::int32 which, nom::int32 button )
{
  switch ( button )
  {
    default: NOM_LOG_INFO ( TTCARDS, "FIXME: GameOverState needs joystick implementation!" ); break;
  } // switch
}

void GameOverState::onUserEvent ( nom::uint32 type, nom::int32 code, void* data1, void* data2 )
{
  if ( type != SDL_USEREVENT ) return;

  if ( code == static_cast<nom::int32> ( nom::EventDispatcher::UserEvent::UI ) )
  {
    Card selected_card = this->game->hand[1].getSelectedCard();
    this->card_info_box.setLabel ( selected_card.getName() );

    this->game->cursor_move.Play();
  }
  else if ( code == static_cast<nom::int32> ( nom::EventDispatcher::UserEvent::Animation ) )
  {
    this->card_info_box.disable();

    nom::int32 card_pos = this->game->hand[1].at ( selected_card );
    if ( this->game->hand[1].cards[card_pos].getPlayerID() != Card::PLAYER1 )
    {
      // FIXME; should have no spacing on card printout
      this->info_box.setLabel ( this->selected_card.getName() + " card acquired" );
      this->game->hand[1].cards[card_pos].setPlayerID(Card::PLAYER1);

      this->game->card_flip.Play();
    }
    else
    {
      //this->info_box.diable();
      this->game->cursor_wrong.Play();
    }
  }
}

void GameOverState::update ( float delta_time )
{
  this->game->card.update();

  this->info_box.update();
  this->card_info_box.update();
  this->cursor.update();

  if ( this->transistion.ticks() > 1500 )
  {
    this->transistion.stop();
    this->show_results = true;
  }

  this->game->context.update();
}

void GameOverState::draw ( nom::IDrawable::RenderTarget target )
{
  this->game->gameover_background.draw ( target );

  // Show Player 2 hand on the top
  for ( nom::int32 idx = 0; idx < this->game->hand[1].size(); idx++ )
  {
    this->player2_pos = nom::Coords (
                                      PLAYER2_GAMEOVER_ORIGIN_X +
                                      ( CARD_WIDTH * idx ),
                                      PLAYER2_GAMEOVER_ORIGIN_Y
                                    );

    this->game->card.reposition ( this->player2_pos );
    this->game->card.setViewCard ( this->game->hand[1].cards.at( idx ) );
    this->game->card.draw ( target );
  }

  // Show Player 1 hand on the bottom
  for ( nom::int32 idx = 0; idx < this->game->hand[0].size(); idx++ )
  {
    this->player1_pos = nom::Coords (
                                      PLAYER1_GAMEOVER_ORIGIN_X +
                                      ( CARD_WIDTH * idx ),
                                      PLAYER1_GAMEOVER_ORIGIN_Y
                                    );

    this->game->card.reposition ( this->player1_pos );
    this->game->card.setViewCard ( this->game->hand[0].cards.at( idx ) );
    this->game->card.draw ( target );
  }

  this->info_box.draw( target );
  this->card_info_box.draw( target );

  this->cursor.draw ( target );
  if ( this->show_results == true )
  {
    //Card lost_card = this->game->hand[0].getSelectedCard();
//NOM_LOG_INFO ( TTCARDS, lost_card.getName() + "Card lost" );

    //nom::sleep ( 1000 );

    // Restart game
    //nom::GameStates::ChangeState( CardsMenuStatePtr ( new CardsMenuState ( this->game ) ) );
    this->transistion.stop();
    this->show_results = true;
  } // end if update
}
