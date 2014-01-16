/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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

using namespace nom;

GameOverState::GameOverState  ( const nom::SDLApp::SharedPtr& object,
                                nom::void_ptr state
                              ) :
  nom::IState { Game::State::GameOver },
  game { std::dynamic_pointer_cast<Game> (object) },
  show_results ( false )/*,
  gameover_state { static_cast<nom::uint32_ptr>(state) }*/
{
  NOM_LOG_TRACE( TTCARDS );
}

GameOverState::~GameOverState ( void )
{
  NOM_LOG_TRACE( TTCARDS );
}

void GameOverState::on_init ( nom::void_ptr data )
{
  nom::Gradient linear;

  // Initialize interface cursor
  this->cursor = GameOverStateCursor ( "images/cursors.json" );

  if ( this->cursor.load ( this->game->config.getString("INTERFACE_CURSOR"), 0 ) == true )
  {
    if ( this->game->config.getString("SCALE_ALGORITHM") == "scale2x" )
    {
      this->cursor.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    }
    else if ( this->game->config.getString("SCALE_ALGORITHM") == "hqx" )
    {
      this->cursor.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    }

    this->cursor.set_position_map ( &this->game->hand[1] );
    this->cursor.set_size ( Size2i ( CURSOR_WIDTH, CURSOR_HEIGHT ) );
    this->cursor.set_position ( Point2i (
                                PLAYER2_GAMEOVER_CURSOR_ORIGIN_X,
                                PLAYER2_GAMEOVER_CURSOR_ORIGIN_Y
                                        )
                              );
    this->cursor.set_frame ( INTERFACE_CURSOR_RIGHT );
  }
  else // EPIC FAIL
  {
#if defined (NOM_DEBUG)
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("INTERFACE_CURSOR") );
#else
    nom::DialogMessageBox ( "Error", "Could not load resource file: " + this->game->config.getString("INTERFACE_CURSOR") );
#endif
    exit ( NOM_EXIT_FAILURE );
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
  while ( this->game->hand[0].size() < MAX_PLAYER_HAND )
  {
    this->game->hand[0].shuffle ( 8, 10, this->game->collection );
  }

  while ( this->game->hand[1].size() < MAX_PLAYER_HAND )
  {
    this->game->hand[1].shuffle ( 1, 7, this->game->collection );
  }

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
  linear.set_start_color ( nom::Color4i::Gray );
  linear.set_end_color ( nom::Color4i::LightGray );
  linear.set_fill_direction ( nom::Gradient::FillDirection::Left );

  this->info_text = nom::Text("Select 1 card(s) you want", this->game->info_text, 12, nom::Text::Alignment::MiddleCenter);

  this->card_text = nom::Text(this->game->hand[1].getSelectedCard().getName(), this->game->info_text, 12, nom::Text::MiddleCenter);

  // Top display message box; "description" info box
  this->info_box = nom::MessageBox  (
                                          INFO_BOX_ORIGIN_X, DEBUG_BOX_ORIGIN_Y,
                                          INFO_BOX_WIDTH, INFO_BOX_HEIGHT,
                                          nom::MessageBox::Style::Gray, linear
                                        );

  this->info_box.set_title ( nom::Text("INFO.", this->game->info_small_text, 9, nom::Text::Alignment::TopLeft) );
  this->info_box.set_text ( this->info_text );

  // Bottom display message box; card info (card name)
  this->card_info_box = nom::MessageBox (
                                              INFO_BOX_ORIGIN_X, INFO_BOX_ORIGIN_Y,
                                              INFO_BOX_WIDTH, INFO_BOX_HEIGHT,
                                              nom::MessageBox::Style::Gray, linear
                                            );

  this->card_info_box.set_title ( nom::Text("INFO.", this->game->info_small_text, 9, nom::Text::Alignment::TopLeft) );
  this->card_info_box.set_text ( this->card_text );

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
#if ! defined (NOM_DEBUG)
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

void GameOverState::on_exit ( nom::void_ptr data )
{
NOM_LOG_TRACE ( TTCARDS );

  this->game->winning_track.Stop();
}

void GameOverState::on_pause ( nom::void_ptr data )
{
  // Hide the cursor so that it doesn't show up during undesirable states such
  // as during the ContinueMenu or Pause states.
  this->cursor.set_frame ( INTERFACE_CURSOR_NONE );
  this->cursor.update();
}

void GameOverState::on_resume ( nom::void_ptr data )
{
  // Restore the rendering of the player's cursor
  this->cursor.set_frame ( INTERFACE_CURSOR_RIGHT );

  nom::int32_ptr response = static_cast<nom::int32_ptr> (data);

  // "Yes" to verification of choice; we expect this response to be coming from
  // ContinueMenuState.
  if ( response != nullptr && this->game->previous_state() == Game::State::GameOver )
  {
    this->event.dispatch ( nom::EventDispatcher::UserEvent::Animation );
  }
}

void GameOverState::onKeyDown ( nom::int32 key, nom::int32 mod, nom::uint32 window_id )
{
  switch ( key )
  {
    default: /* Ignore non-mapped keys */ break;

    // Pause game
    case SDLK_p:
    {
      this->game->set_state( Game::State::Pause );
      break;
    }

    // Start a new game
    case SDLK_RETURN: this->game->set_state( Game::State::CardsMenu ); break;

    case SDLK_LEFT: this->cursor.move_left(); break;
    case SDLK_RIGHT: this->cursor.move_right(); break;
    case SDLK_SPACE:
    {
      this->selected_card = this->game->hand[1].getSelectedCard();

      this->game->set_state( Game::State::ContinueMenu );
      break;
    }
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
      this->cursor.set_position ( Point2i(PLAYER2_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * idx, this->cursor.y() ) );

      this->game->hand[1].selectCard ( this->game->hand[1].cards[ idx ] );
      Card selected_card = this->game->hand[1].getSelectedCard();

      this->card_text.set_text ( selected_card.getName() );
      //this->card_info_box.set_text ( nom::Text(, this->game->info_text, 12, nom::Text::Alignment::MiddleCenter) );

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
  if ( x > 0 || y > 0 )
  {
    this->cursor.move_left();
  }
  else if ( x < 0 || y < 0 )
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
    this->card_text.set_text ( selected_card.getName() );

    this->game->cursor_move.Play();
  }
  else if ( code == static_cast<nom::int32> ( nom::EventDispatcher::UserEvent::Animation ) )
  {
    this->card_info_box.disable();

    nom::int32 card_pos = this->game->hand[1].at ( selected_card );
    if ( this->game->hand[1].cards[card_pos].getPlayerID() != Card::PLAYER1 )
    {
      // FIXME; should have no spacing on card printout
      this->info_text.set_text ( this->selected_card.getName() + " card acquired" );
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

void GameOverState::on_update ( float delta_time )
{
  this->game->card.update();

  this->info_box.set_text ( this->info_text );
  this->card_info_box.set_text ( this->card_text );

  this->cursor.update();

  if ( this->transistion.ticks() > 1500 )
  {
    this->transistion.stop();
    this->show_results = true;
  }

  this->game->window.update();
}

void GameOverState::on_draw ( nom::IDrawable::RenderTarget target )
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
    //this->game->set_state ( Game::State::CardsMenu );
    this->transistion.stop();
    this->show_results = true;
  } // end if update
}
