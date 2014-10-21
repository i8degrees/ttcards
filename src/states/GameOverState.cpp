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

// Forward declarations
#include "Game.hpp"

using namespace nom;

GameOverState::GameOverState  ( const nom::SDLApp::shared_ptr& object,
                                nom::void_ptr state
                              ) :
  nom::IState(Game::State::GameOver),
  game( NOM_DYN_SHARED_PTR_CAST( Game, object) ),
  show_results( false ),
  gameover_state( *static_cast<nom::uint32_ptr>(state) )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

GameOverState::~GameOverState( void )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

void GameOverState::on_init( nom::void_ptr data )
{
  nom::SpriteSheet frames;

  // Initialize interface cursor
  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( frames.load_file( this->game->config.getString("INTERFACE_CURSOR_ATLAS") ) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not load sprite sheet:",
                    this->game->config.getString("INTERFACE_CURSOR_ATLAS") );
      // return false;
    }
  #else
    if( frames.load_file( this->game->config.getString("INTERFACE_CURSOR_ATLAS_SCALE2X") ) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not load sprite sheet:",
                    this->game->config.getString("INTERFACE_CURSOR_ATLAS_SCALE2X") );
      // return false;
    }
  #endif

  this->cursor_.set_texture(this->game->cursor_tex_);
  this->cursor_.set_sprite_sheet(frames);

  this->cursor_.set_position_map(&this->game->hand[1]);
  this->cursor_.set_size( Size2i ( CURSOR_WIDTH, CURSOR_HEIGHT ) );
  this->cursor_.set_position(  Point2i (
                                        PLAYER2_GAMEOVER_CURSOR_ORIGIN_X,
                                        PLAYER2_GAMEOVER_CURSOR_ORIGIN_Y
                                      ));
  this->cursor_.set_frame(INTERFACE_CURSOR_RIGHT);

  if( this->gameover_state == 0 ) // NotOver
  {
    // ???
  }
  else if( this->gameover_state == 1 ) // Draw
  {
    NOM_DUMP("Draw...");
  }
  else if( this->gameover_state == 2 ) // Player 1 won
  {
    NOM_DUMP("You won!");
    // ...
  } // end gameover_state == 1

  else if( this->gameover_state == 3 ) // Player 2 won
  {
    NOM_DUMP("You lost!");
    // Point2i offset( PLAYER1_GAMEOVER_ORIGIN_X,
    //                 // PLAYER1_GAMEOVER_ORIGIN_Y * 2 );
    //                 PLAYER1_GAMEOVER_ORIGIN_Y );

    // this->cursor_.set_position(offset);
    // this->cursor_.set_frame(INTERFACE_CURSOR_LEFT);
  }

  // REMOVE ME
  // while ( this->game->hand[0].size() < MAX_PLAYER_HAND )
  // {
  //   this->game->hand[0].shuffle ( 8, 10, this->game->collection );
  // }

  // while ( this->game->hand[1].size() < MAX_PLAYER_HAND )
  // {
  //   this->game->hand[1].shuffle ( 1, 7, this->game->collection );
  // }

  for( auto idx = 0; idx < MAX_PLAYER_HAND; idx++ )
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

  // Northern message box

  if( this->game->info_box_.set_context(&this->game->gui_window_) == false )
  {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could set GUI desktop." );
    // return false;
  }

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->game->info_box_.load_document_file( this->game->config.getString("GUI_GAMEOVER_RULES_INFO") ) == false )
    {
      NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could not load file:",
                    this->game->config.getString("GUI_GAMEOVER_RULES_INFO") );
      // return false;
    }
  #else
    if( this->game->info_box_.load_document_file( this->game->config.getString("GUI_GAMEOVER_RULES_INFO_SCALE2X") ) == false )
    {
      NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could not load file:",
                    this->game->config.getString("GUI_GAMEOVER_RULES_INFO_SCALE2X") );
      // return false;
    }
  #endif

  this->game->info_box_.set_message_text("Select 1 card(s) you want");
  this->game->info_box_.show();

  // Southern message box (card name)

  if( this->game->card_info_box_.set_context(&this->game->gui_window_) == false )
  {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could set GUI desktop." );
    // return false;
  }

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->game->card_info_box_.load_document_file( this->game->config.getString("GUI_GAMEOVER_CARD_INFO") ) == false )
    {
      NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could not load file:",
                    this->game->config.getString("GUI_GAMEOVER_CARD_INFO") );
      // return false;
    }
  #else
    if( this->game->card_info_box_.load_document_file( this->game->config.getString("GUI_GAMEOVER_CARD_INFO_SCALE2X") ) == false )
    {
      NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could not load file:",
                    this->game->config.getString("GUI_GAMEOVER_CARD_INFO_SCALE2X") );
      // return false;
    }
  #endif

  this->game->card_info_box_.set_message_text( this->game->hand[1].getSelectedCard().getName() );
  this->game->card_info_box_.show();

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

  if ( this->gameover_state == 0 ) // Draw
  {
    // Do stuff related to game tie
  }
  else if ( this->gameover_state == 1 ) // You won!
  {
    // Play "You won!" music track
    this->game->music_track->Stop();

    #if ! defined (NOM_DEBUG)
      this->game->winning_track->Play();
    #endif

  }

  else if( this->gameover_state == 3 ) // You lost
  {
    //nom::int32 rand_pick = nom::randomInteger ( 0, this->game->hand[0].size() );
    this->game->hand[0].front();
  }

  //this->selected_card = this->game->hand[1].getSelectedCard();

  // Both player hands should **always** be shown with the card face rendered
  this->game->hand[0].set_face_down(false);
  this->game->hand[1].set_face_down(false);

  // Commence the countdown on the showing of results!
  this->transistion.start();

  nom::InputActionMapper state;

  nom::EventCallback pause_game( [&] ( const nom::Event& evt ) { this->game->set_state( Game::State::Pause ); } );

  nom::EventCallback move_cursor_left( [&] ( const nom::Event& evt ) { this->cursor_.move_left(); } );
  nom::EventCallback move_cursor_right( [&] ( const nom::Event& evt ) { this->cursor_.move_right(); } );

  nom::EventCallback select_card( [&] ( const nom::Event& evt )
    {
      this->on_mouse_button_down( evt );
    }
  );

  nom::EventCallback select_cards( [&] ( const nom::Event& evt )
    {
      this->selected_card = this->game->hand[1].getSelectedCard();
      this->game->set_state( Game::State::ConfirmationDialog );
    }
  );

  // Debugging aids
  #if ! defined( NDEBUG )
    nom::EventCallback restart_game( [&] ( const nom::Event& evt )
      {
        this->selected_card = this->game->hand[1].getSelectedCard();
        this->game->set_state( Game::State::CardsMenu );
      }
    );

    state.insert( "restart_game", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RETURN ), restart_game );
    state.insert( "restart_game", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::SELECT ), restart_game );
  #endif // NOT defined NDEBUG

  // Keyboard mappings
  state.insert( "pause_game", nom::KeyboardAction( SDL_KEYDOWN, SDLK_p ), pause_game );
  state.insert( "move_cursor_left", nom::KeyboardAction( SDL_KEYDOWN, SDLK_LEFT ), move_cursor_left );
  state.insert( "move_cursor_right", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RIGHT ), move_cursor_right );
  state.insert( "select_cards", nom::KeyboardAction( SDL_KEYDOWN, SDLK_SPACE ), select_cards );

  // Mouse button & mouse wheel mappings
  state.insert( "move_cursor_left", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_Y, nom::MouseWheelAction::DOWN ), move_cursor_left );
  state.insert( "move_cursor_right", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_Y, nom::MouseWheelAction::UP ), move_cursor_right );
  state.insert( "select_card", nom::MouseButtonAction( SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT ), select_card );
  state.insert( "select_cards", nom::MouseButtonAction( SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT, 2 ), select_cards );

  // Joystick button mappings
  state.insert( "pause_game", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::START ), pause_game );
  state.insert( "move_cursor_left", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::LEFT ), move_cursor_left );
  state.insert( "move_cursor_right", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::RIGHT ), move_cursor_right );
  state.insert( "select_cards", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::CROSS ), select_cards );

  this->game->input_mapper.erase( "GameOverState" );
  this->game->input_mapper.insert( "GameOverState", state, true );
  this->game->input_mapper.activate_only( "GameOverState" );
  this->game->input_mapper.activate( "Game" );
}

void GameOverState::on_exit( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->game->winning_track->Stop();

  this->game->info_box_.close();
  this->game->card_info_box_.close();
  Rocket::Core::Factory::ClearStyleSheetCache();
  Rocket::Core::Factory::ClearTemplateCache();
}

void GameOverState::on_pause( nom::void_ptr data )
{
  // Hide the cursor so that it doesn't show up during undesirable states such
  // as during the ConfirmationDialogState or Pause states.
  this->cursor_.set_frame(INTERFACE_CURSOR_NONE);
}

void GameOverState::on_resume( nom::void_ptr data )
{
  // Restore the rendering of the player's cursor
  this->cursor_.set_frame ( INTERFACE_CURSOR_RIGHT );

  nom::int32_ptr response = static_cast<nom::int32_ptr> (data);

  // "Yes" to verification of choice; we expect this response to be coming from
  // ConfirmationDialogState.
  if( response != nullptr && *response == 1 && this->game->state()->current_state_id() == Game::State::GameOver )
  {
    nom::Event ev;
    ev.user.code = GameEvent::AnimationEvent;
    ev.user.data1 = nullptr;
    ev.user.data2 = nullptr;
    ev.user.window_id = 0;
    this->event.dispatch( ev );
  }

  this->game->input_mapper.activate_only( "GameOverState" );
  this->game->input_mapper.activate( "Game" );
}

bool GameOverState::on_event( const nom::Event& ev )
{
  // this->game->gui_window_.process_event(ev);

  // Do **not** override the normal event loop (otherwise this state breaks)
  return false;
}

void GameOverState::on_mouse_button_down( const nom::Event& ev )
{
  // Sanity check
  if( ev.type != SDL_MOUSEBUTTONDOWN ) return;

  nom::Point2i coords ( ev.mouse.x, ev.mouse.y ); // mouse input coordinates

  // Player hand selection checks
  for ( nom::int32 idx = 0; idx < this->game->hand[1].size(); idx++ )
  {
    IntRect player2_card_pos  ( PLAYER2_GAMEOVER_ORIGIN_X,
                                PLAYER2_GAMEOVER_ORIGIN_Y,
                                ( CARD_WIDTH ) * ( idx + 1 ),
                                CARD_WIDTH
                              );

    if ( player2_card_pos.contains ( coords ) )
    {
      // 1. Update cursor position
      // 2. Update player's selected card
      // 3. Update the card info message box
      // 4. Play sound event

      this->game->hand[1].set_position(idx);
      this->cursor_.set_position( Point2i(PLAYER2_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * idx, this->cursor_.position().y ) );

      Card selected_card = this->game->hand[1].cards[idx];
      this->game->card_info_box_.set_message_text( selected_card.getName() );

      this->game->cursor_move->Play();
      // We must break the loop here upon the end of a matching coords check
      // in order to prevent a nasty "last card stays permanently selected"
      // bug from cropping back up!
      break;
    }
  }
}

void GameOverState::on_user_event( const nom::Event& ev )
{
  // Nothing to do; not the right event type for us!
  if( ev.type != SDL_USEREVENT ) return;

  if( ev.user.code == GameEvent::GUIEvent )
  {
    NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_EVENTS, "GameEvent::GUIEvent" );

    Card selected_card = this->game->hand[1].getSelectedCard();
    this->game->card_info_box_.set_message_text( selected_card.getName() );

    this->game->cursor_move->Play();
  }
  else if( ev.user.code == GameEvent::AnimationEvent )
  {
    NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_EVENTS, "GameEvent::AnimationEvent" );

    // NOTE: This should not be disabled until the total number of won cards
    // has been reached.
    this->game->card_info_box_.disable();

    nom::int32 card_pos = this->game->hand[1].at ( selected_card );
    if ( this->game->hand[1].cards[card_pos].getPlayerID() != Card::PLAYER1 )
    {
      // FIXME; should have no spacing on card printout
      this->game->info_box_.set_message_text( this->selected_card.getName() + " card acquired" );

      this->game->hand[1].cards[card_pos].setPlayerID(Card::PLAYER1);

      this->game->card_flip->Play();
    }
    else
    {
      //this->game->info_box_.diable();
      this->game->cursor_wrong->Play();
    }
  }
}

void GameOverState::on_update( float delta_time )
{
  this->game->card.update();

  this->game->gui_window_.update();

  if ( this->transistion.ticks() > 1500 )
  {
    this->transistion.stop();
    this->show_results = true;
  }

  this->game->window.update();
}

void GameOverState::on_draw( nom::RenderWindow& target )
{
  this->game->gameover_background.draw(target);

  // Show Player 2 hand on the top
  for ( nom::int32 idx = 0; idx < this->game->hand[1].size(); idx++ )
  {
    this->player2_pos = nom::Point2i (  PLAYER2_GAMEOVER_ORIGIN_X
                                        +
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
    this->player1_pos = nom::Point2i  ( PLAYER1_GAMEOVER_ORIGIN_X
                                        +
                                        ( CARD_WIDTH * idx ),
                                        PLAYER1_GAMEOVER_ORIGIN_Y
                                      );

    this->game->card.reposition ( this->player1_pos );
    this->game->card.setViewCard ( this->game->hand[0].cards.at( idx ) );
    this->game->card.draw ( target );
  }

  this->game->gui_window_.draw();

  this->cursor_.draw(target);
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
