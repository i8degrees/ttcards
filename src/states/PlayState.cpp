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
#include "PlayState.hpp"

// Forward declarations
#include "Game.hpp"

using namespace nom;

PlayState::PlayState ( const nom::SDLApp::shared_ptr& object ) :
  nom::IState( Game::State::Play ),
  game( NOM_DYN_SHARED_PTR_CAST( Game, object) )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->turn = 0;
  this->cursor_locked = false;
  this->skip_turn = false;
  this->gameover_state = GameOverType::NotOver;
}

PlayState::~PlayState()
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

void PlayState::on_exit( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->game->info_box_.close();
  this->game->debug_box_.close();
  Rocket::Core::Factory::ClearStyleSheetCache();
  Rocket::Core::Factory::ClearTemplateCache();
}

void PlayState::on_pause( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

void PlayState::on_resume( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->game->input_mapper.activate_only( "PlayState" );
  this->game->input_mapper.activate( "Game" );
}

void PlayState::on_init( nom::void_ptr data )
{
  while( this->game->hand[0].size() < MAX_PLAYER_HAND )
  {
    // this->game->hand[0].shuffle(8, 10, this->game->collection);
    this->game->hand[0].shuffle(1, 1, this->game->collection);
  }

  // Clear CPUPlayer's hand (we will initialize it here soon enough)
  this->game->hand[1].clear();

  while( this->game->hand[1].size() < MAX_PLAYER_HAND )
  {
    // this->game->hand[1].shuffle(1, 10, this->game->collection);
    this->game->hand[1].shuffle(1, 1, this->game->collection);
  }

  this->game->cursor.set_position ( Point2i(PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y) );
  this->game->cursor.set_frame ( INTERFACE_CURSOR_NONE ); // default cursor image
  this->game->cursor.set_state ( 0 ); // default state; player hand select

  // this->game->rules.setRules(1);
  this->game->rules.setRules(0);
  this->game->board = Board ( this->game->rules, &this->game->card );

  this->player[0] = Player ( &this->game->hand[0], &this->game->card, this->game->rules );
  this->player[0].set_position( Point2i( PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y ) );

  this->player[1] = Player ( &this->game->hand[1], &this->game->card, this->game->rules );
  this->player[1].set_position( Point2i( PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y ) );

  // player1, player2 cursor X, Y coords
  this->player_cursor_coords[0] = nom::Point2i( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y );
  this->player_cursor_coords[1] = nom::Point2i( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y );

  // Compute the player's scoreboard X, Y coordinate positions
  this->player_scoreboard[0] = Point2i( PLAYER1_SCORE_ORIGIN_X, PLAYER1_SCORE_ORIGIN_Y );
  this->player_scoreboard[1] = Point2i( PLAYER2_SCORE_ORIGIN_X, PLAYER2_SCORE_ORIGIN_Y );

  for ( nom::int32 idx = 0; idx < MAX_PLAYER_HAND; idx++ )
  {
    this->cursor_coords_map[idx] = nom::Point2i( idx, this->player_cursor_coords[0].y + ( CARD_HEIGHT / 2 * idx ) );
  }

  // Northern message box

  if( this->game->debug_box_.set_context(&this->game->gui_window_) == false )
  {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could set GUI desktop." );
    // return false;
  }

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->game->debug_box_.load_document_file( this->game->config.getString("GUI_DEBUG") ) == false )
    {
      NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could not load file:",
                    this->game->config.getString("GUI_DEBUG") );
      // return false;
    }
  #else
    if( this->game->debug_box_.load_document_file( this->game->config.getString("GUI_DEBUG_SCALE2X") ) == false )
    {
      NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could not load file:",
                    this->game->config.getString("GUI_DEBUG_SCALE2X") );
      // return false;
    }
  #endif

  this->game->debug_box_.show();

  #if ! defined ( NOM_DEBUG )
    this->game->debug_box_.disable();
  #endif

  // Southern message box

  if( this->game->info_box_.set_context(&this->game->gui_window_) == false )
  {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could set GUI desktop." );
    // return false;
  }

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->game->info_box_.load_document_file( this->game->config.getString("GUI_MBOX") ) == false )
    {
      NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could not load file:",
                    this->game->config.getString("GUI_MBOX") );
      // return false;
    }
  #else
    if( this->game->info_box_.load_document_file( this->game->config.getString("GUI_MBOX_SCALE2X") ) == false )
    {
      NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could not load file:",
                    this->game->config.getString("GUI_MBOX_SCALE2X") );
      // return false;
    }
  #endif

  this->game->info_box_.show();

  nom::StringList ruleset = this->game->config.string_array("REGION_RULESET");
  for( auto itr = ruleset.begin(); itr != ruleset.end(); ++itr ) {
    if( (*itr) != "Open" ) {
      this->game->hand[1].set_face_down(true);
    }
  }

  // Initialize player cards to their respective defaults; this lets us know not
  // only whose cards they are originally but also presently -- critical in card
  // flipping, scoreboard keeping and end of game tallying logic.
  this->player[0].setID ( Card::PLAYER1 );
  this->player[1].setID ( Card::PLAYER2 );

  // Update both player scores now that we have the player scoreboard X, Y
  // origins calculated for rendering.
  //
  // Assuming a player hand of five, this should always assign a starting score
  // value of 5 for each player initially.
  this->updateScore();

  // Set whose turn it is initially using a random number generator with equal
  // odds -- 50/50 chance that you will have the first move!
  this->player_turn( nom::uniform_int_rand<nom::uint>(0, TOTAL_PLAYERS - 1) );

  // Initialize our animation state timers
  this->player_timer[1].setFrameRate ( 500 );
  this->cursor_blink.start();
  this->blink_cursor = false;

  CPU_Player::action_callback
    cpu_player_action_callback( [&] (BoardTile& tile) {
      this->moveTo( tile.bounds().x, tile.bounds().y );
    });

  this->cpu_player_.initialize( CPU_Player::Difficulty::Easy,
                                &this->game->board,
                                &this->game->hand[1],
                                cpu_player_action_callback );

  // this->game->input_mapper.clear();
  nom::InputActionMapper state;

  // Create input action mappings for player 1 key bindings
  state.insert( "moveto_1", nom::KeyboardAction( SDL_KEYDOWN, SDLK_1 ), nom::EventCallback( [&] ( const nom::Event& evt ) { /*if( this->get_turn() != PLAYER2 )*/ this->moveTo( 0, 0 ); } ) );
  state.insert( "moveto_2", nom::KeyboardAction( SDL_KEYDOWN, SDLK_2 ), nom::EventCallback( [&] ( const nom::Event& evt ) { /*if( this->get_turn() != PLAYER2 )*/ this->moveTo( 1, 0 ); } ) );
  state.insert( "moveto_3", nom::KeyboardAction( SDL_KEYDOWN, SDLK_3 ), nom::EventCallback( [&] ( const nom::Event& evt ) { /*if( this->get_turn() != PLAYER2 )*/ this->moveTo( 2, 0 ); } ) );
  state.insert( "moveto_4", nom::KeyboardAction( SDL_KEYDOWN, SDLK_4 ), nom::EventCallback( [&] ( const nom::Event& evt ) { /*if( this->get_turn() != PLAYER2 )*/ this->moveTo( 0, 1 ); } ) );
  state.insert( "moveto_5", nom::KeyboardAction( SDL_KEYDOWN, SDLK_5 ), nom::EventCallback( [&] ( const nom::Event& evt ) { /*if( this->get_turn() != PLAYER2 )*/ this->moveTo( 1, 1 ); } ) );
  state.insert( "moveto_6", nom::KeyboardAction( SDL_KEYDOWN, SDLK_6 ), nom::EventCallback( [&] ( const nom::Event& evt ) { /*if( this->get_turn() != PLAYER2 )*/ this->moveTo( 2, 1 ); } ) );
  state.insert( "moveto_7", nom::KeyboardAction( SDL_KEYDOWN, SDLK_7 ), nom::EventCallback( [&] ( const nom::Event& evt ) { /*if( this->get_turn() != PLAYER2 )*/ this->moveTo( 0, 2 ); } ) );
  state.insert( "moveto_8", nom::KeyboardAction( SDL_KEYDOWN, SDLK_8 ), nom::EventCallback( [&] ( const nom::Event& evt ) { /*if( this->get_turn() != PLAYER2 )*/ this->moveTo( 1, 2 ); } ) );
  state.insert( "moveto_9", nom::KeyboardAction( SDL_KEYDOWN, SDLK_9 ), nom::EventCallback( [&] ( const nom::Event& evt ) { /*if( this->get_turn() != PLAYER2 )*/ this->moveTo( 2, 2 ); } ) );

  // Register event callbacks for our input actions
  nom::EventCallback unlock_selected_card( [&] ( const nom::Event& evt ) { this->unlockSelectedCard(); } );
  nom::EventCallback lock_selected_card( [&] ( const nom::Event& evt ) { this->lockSelectedCard(); } );

  nom::EventCallback select_card( [&] ( const nom::Event& evt )
    {
      this->on_mouse_button_down( evt );
    }
  );

  nom::EventCallback move_cursor_up( [&] ( const nom::Event& evt ) { this->moveCursorUp(); } );
  nom::EventCallback move_cursor_down( [&] ( const nom::Event& evt ) { this->moveCursorDown(); } );
  nom::EventCallback move_cursor_left( [&] ( const nom::Event& evt ) { this->moveCursorLeft(); } );
  nom::EventCallback move_cursor_right( [&] ( const nom::Event& evt ) { this->moveCursorRight(); } );
  nom::EventCallback pause_game( [&] ( const nom::Event& evt ) { this->game->set_state( Game::State::Pause ); } );

  nom::EventCallback load_game( [&] ( const nom::Event& evt ) { this->load_game( 0 ); } );
  nom::EventCallback save_game( [&] ( const nom::Event& evt ) { this->save_game( 0 ); } );

  state.insert( "unlock_selected_card", nom::KeyboardAction( SDL_KEYDOWN, SDLK_x ), unlock_selected_card );
  state.insert( "lock_selected_card", nom::KeyboardAction( SDL_KEYDOWN, SDLK_SPACE ), lock_selected_card );

  state.insert( "move_cursor_up", nom::KeyboardAction( SDL_KEYDOWN, SDLK_UP ), move_cursor_up );
  state.insert( "move_cursor_up", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_Y, nom::MouseWheelAction::UP ), move_cursor_up );

  state.insert( "move_cursor_down", nom::KeyboardAction( SDL_KEYDOWN, SDLK_DOWN ), move_cursor_down );
  state.insert( "move_cursor_down", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_Y, nom::MouseWheelAction::DOWN ), move_cursor_down );

  state.insert( "move_cursor_left", nom::KeyboardAction( SDL_KEYDOWN, SDLK_LEFT ), move_cursor_left );
  state.insert( "move_cursor_right", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RIGHT ), move_cursor_right );

  // TODO: Declare a debug_state nom::InputMapper var so we can insert said
  // state only when the applicable debug flags are toggled on -- for sake of
  // cleanliness:
  //
  // debug_state.insert( "action", action, action_callback );
  // // ...and so on
  //
  // #if defined( TTCARDS_DEBUG_PLAY_STATE )
  //  this->game->input_mapper.insert( "DebugPlayState", debug_state, true );
  // #endif
  //
  #if ! defined( NDEBUG ) // Debug build
    nom::EventCallback control_turn( [&] ( const nom::Event& evt )
      {
        // FIXME: Why are these inversed???
        this->skip_turn = true;
        this->player[1].set_state( PlayerState::Debug );
        this->endTurn();
      }
    );

    nom::EventCallback skip_turn( [&] ( const nom::Event& evt )
      {
        // FIXME: Why are these inversed???
        this->skip_turn = false;
        this->player[1].set_state( PlayerState::Reserved );
        this->endTurn();
      }
    );

    nom::EventCallback delete_card( [&] ( const nom::Event& evt )
      {
        uint player_turn = this->get_turn();

        this->game->hand[player_turn].erase( this->game->hand[player_turn].getSelectedCard() );
        this->game->cursor.set_position( Point2i(this->player_cursor_coords[player_turn].x, this->player_cursor_coords[player_turn].y) );
      }
    );

    nom::EventCallback toggle_debug_box( [&] ( const nom::Event& evt )
      {
        if( this->game->debug_box_.enabled() == true )
        {
          this->game->debug_box_.disable();
        }
        else
        {
          this->game->debug_box_.enable();
        }
      }
    );

    state.insert( "control_turn", nom::KeyboardAction( SDL_KEYDOWN, SDLK_e, KMOD_LGUI ), control_turn );
    state.insert( "skip_turn", nom::KeyboardAction( SDL_KEYDOWN, SDLK_e ), skip_turn );
    state.insert( "delete_card", nom::KeyboardAction( SDL_KEYDOWN, SDLK_d ), delete_card );
    state.insert( "toggle_debug_box", nom::KeyboardAction( SDL_KEYDOWN, SDLK_i ), toggle_debug_box );

  #endif // NOT defined NDEBUG

  #if defined( TTCARDS_DEBUG_PLAY_STATE ) // Debug build
    nom::EventCallback increase_north_rank( [&] ( const nom::Event& evt ) { this->game->hand[ this->get_turn() ].modifyCardRank( true, NORTH ); } );
    nom::EventCallback decrease_north_rank( [&] ( const nom::Event& evt ) { this->game->hand[ this->get_turn() ].modifyCardRank( false, NORTH ); } );
    // nom::EventCallback increase_north_rank( [&] ( const nom::Event& evt ) { this->game->hand[ this->get_turn() ].modifyCardRank( true, NORTH ); this->moveCursorDown(); } );
    // nom::EventCallback decrease_north_rank( [&] ( const nom::Event& evt ) { this->game->hand[ this->get_turn() ].modifyCardRank( false, NORTH ); this->moveCursorDown(); } );

    nom::EventCallback increase_south_rank( [&] ( const nom::Event& evt ) { this->game->hand[ this->get_turn() ].modifyCardRank( true, SOUTH ); } );
    nom::EventCallback decrease_south_rank( [&] ( const nom::Event& evt ) { this->game->hand[ this->get_turn() ].modifyCardRank( false, SOUTH ); } );
    // nom::EventCallback increase_south_rank( [&] ( const nom::Event& evt ) { this->game->hand[ this->get_turn() ].modifyCardRank( true, SOUTH ); this->moveCursorUp(); } );
    // nom::EventCallback decrease_south_rank( [&] ( const nom::Event& evt ) { this->game->hand[ this->get_turn() ].modifyCardRank( false, SOUTH ); this->moveCursorUp(); } );

    nom::EventCallback increase_west_rank( [&] ( const nom::Event& evt ) { this->game->hand[ this->get_turn() ].modifyCardRank( true, WEST ); } );
    nom::EventCallback decrease_west_rank( [&] ( const nom::Event& evt ) { this->game->hand[ this->get_turn() ].modifyCardRank( false, WEST ); } );

    nom::EventCallback increase_east_rank( [&] ( const nom::Event& evt ) { this->game->hand[ this->get_turn() ].modifyCardRank( true, EAST ); } );
    nom::EventCallback decrease_east_rank( [&] ( const nom::Event& evt ) { this->game->hand[ this->get_turn() ].modifyCardRank( false, EAST ); } );

    state.insert( "increase_north_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_UP, KMOD_LSHIFT ), increase_north_rank );
    state.insert( "decrease_north_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_UP, KMOD_LCTRL ), decrease_north_rank );

    state.insert( "increase_south_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_DOWN, KMOD_LSHIFT ), increase_south_rank );
    state.insert( "decrease_south_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_DOWN, KMOD_LCTRL ), decrease_south_rank );

    state.insert( "increase_west_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_LEFT, KMOD_LSHIFT ), increase_west_rank );
    state.insert( "decrease_west_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_LEFT, KMOD_LCTRL ), decrease_west_rank );

    state.insert( "increase_east_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RIGHT, KMOD_LSHIFT ), increase_east_rank );
    state.insert( "decrease_east_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RIGHT, KMOD_LCTRL ), decrease_east_rank );
  #endif // defined TTCARDS_DEBUG_PLAY_STATE

  state.insert( "select_card", nom::MouseButtonAction( SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT ), select_card );
  state.insert( "select_card", nom::MouseButtonAction( SDL_MOUSEBUTTONDOWN, SDL_BUTTON_RIGHT ), select_card );

  // Joystick input action mappings
  state.insert( "unlock_selected_card", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::CIRCLE ), unlock_selected_card );
  state.insert( "lock_selected_card", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::CROSS ), lock_selected_card );

  state.insert( "move_cursor_up", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::UP ), move_cursor_up );
  state.insert( "move_cursor_down", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::DOWN ), move_cursor_down );
  state.insert( "move_cursor_left", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::LEFT ), move_cursor_left );
  state.insert( "move_cursor_right", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::RIGHT ), move_cursor_right );

  state.insert( "pause_game", nom::KeyboardAction( SDL_KEYDOWN, SDLK_p ), pause_game );
  state.insert( "pause_game", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::START ), pause_game );

  state.insert( "load_game", nom::KeyboardAction( SDL_KEYDOWN, SDLK_l ), load_game );
  state.insert( "save_game", nom::KeyboardAction( SDL_KEYDOWN, SDLK_s ), save_game );

  this->game->input_mapper.erase( "PlayState" );
  this->game->input_mapper.insert( "PlayState", state, true );
  this->game->input_mapper.activate_only( "PlayState" );
  this->game->input_mapper.activate( "Game" );
}

// Private scope

bool PlayState::on_event(const nom::Event& ev)
{
  this->game->gui_window_.process_event(ev);

  return true;
}

void PlayState::on_mouse_button_down( const nom::Event& ev )
{
  if( ev.type != SDL_MOUSEBUTTONDOWN ) return;

  uint32 player_turn = this->get_turn(); // Ignore player2 mouse input

  // Player cursor positioning
  Point2i player_pos = this->player[player_turn].position();

  // mouse input coordinates
  Point2i mouse_input( ev.mouse.x, ev.mouse.y );

  // Rectangle bounds of player's card
  IntRect card_bounds;

  // Disable mouse input if we are not controlling the other player
  if ( this->skip_turn == false )
  {
    if ( this->get_turn() != 0 ) return;
  }

  // Player hand selection checks; we must calculate the valid bounds of any
  // given card in the player's hand -- the current player position combined
  // with the known card size yields rectangular coordinates we can compare.
  for ( nom::int32 idx = 0; idx < this->game->hand[ player_turn ].size(); idx++ )
  {
    card_bounds = IntRect ( player_pos.x,
                            player_pos.y,
                            CARD_WIDTH,
                            ( CARD_HEIGHT / 2  ) * ( idx + 1 )
                          );

    // If the mouse coordinates match within the card bounds, we have a match!
    if( card_bounds.contains( mouse_input ) )
    {
      // 1. Update player's selected card
      // 2. Update cursor position
      // 3. Play sound event
      this->game->hand[ player_turn ].selectCard ( this->game->hand[ player_turn ].cards[ idx ] );

      this->game->cursor.set_position ( Point2i(this->player_cursor_coords[ player_turn ].x, this->player_cursor_coords[ player_turn ].y + ( CARD_HEIGHT / 2 ) * idx) );

      this->game->cursor_move->Play();

      // We must break the loop here upon the end of a matching coords check
      // in order to prevent a nasty "last card stays permanently selected"
      // bug from cropping back up!
      break;
    }
  }

  // Board grid coords check; player is attempting to place a card on the board
  // when the player hand coords check above comes back false
  IntRect mouse_map = this->game->board.getGlobalBounds( ev.mouse.x, ev.mouse.y );

  // Attempts to move card onto board; validity checking is performed within
  // the following method call
  if ( mouse_map != nom::IntRect::null ) // undefined if -1, -1
  {
    this->moveTo ( mouse_map.x, mouse_map.y );
  }
}

unsigned int PlayState::get_turn ( void )
{
  return this->turn;
}

void PlayState::player_turn ( unsigned int player )
{
  this->turn = player;

  this->resetCursor();
}

// Helper method for incrementing to next player's turn
void PlayState::endTurn ( void )
{
  this->updateScore();

  this->unlockSelectedCard();

  this->game->hand[PLAYER1].clearSelectedCard();
  this->game->hand[PLAYER2].clearSelectedCard();

  if ( this->get_turn() == PLAYER1 )
  {
    this->player_turn ( PLAYER2 );
  }
  else if ( this->get_turn() == PLAYER2 )
  {
    this->player_turn ( PLAYER1 );
  }
}

void PlayState::on_update_info_dialogs( void )
{
  nom::uint32 player_turn = get_turn();
  Card selected_card; // temp container var to hold our card info (ID, name)
  nom::IntRect coords; // temp container var to hold cursor pos mapping coords

  // Board selection state
  if ( this->isCursorLocked() == true )
  {
    coords = this->game->board.getGlobalBounds ( this->game->cursor.position().x, this->game->cursor.position().y );
    if ( coords != nom::IntRect::null )
    {
      selected_card = this->game->board.get ( coords.x, coords.y );
    }
  }
  else // Player hand selection state
  {
    // Allow watching both players make their card selections for DEBUG
    // (think: development) builds.
    #if defined ( NOM_DEBUG )
        selected_card = this->game->hand[player_turn].getSelectedCard();
    #else // Do not show the actions of other players
        selected_card = this->game->hand[PLAYER1].getSelectedCard();
    #endif
  }

  if ( selected_card.getID() != BAD_CARD_ID )
  {
    std::string card_id = selected_card.get_id_string();
    std::string card_name = selected_card.getName();

    // (Northern) debug info box
    this->game->debug_box_.set_message_text(card_id);

    // (Southern) info card box
    this->game->info_box_.set_message_text(card_name);
  }
}

bool PlayState::isCursorLocked ( void )
{
  if ( this->cursor_locked == true )
  {
    return true;
  }
  else
  {
    return false;
  }
}

void PlayState::lockCursor ( bool lock )
{
  this->cursor_locked = lock;
}

// Helper method for resetting cursor related input
void PlayState::resetCursor ( void )
{
  unsigned int player_turn = get_turn();

  this->game->hand[player_turn].selectCard ( this->game->hand[player_turn].cards.front() );

  this->game->cursor.set_state ( 0 );
  this->game->cursor.set_position ( Point2i(this->player_cursor_coords[0].x, this->player_cursor_coords[0].y) );

  // Only set the position of the game interface cursor for player2 when we are
  // controlling him
  if ( this->skip_turn == true )
  {
    this->game->cursor.set_position ( Point2i(this->player_cursor_coords[player_turn].x, this->player_cursor_coords[player_turn].y) );
  }
}

// helper method for cursor input selection
void PlayState::unlockSelectedCard ( void )
{
  this->game->cursor.set_state ( 0 ); // player card select

  this->resetCursor();

  this->lockCursor ( false );

  this->game->cursor_cancel->Play();
}

// helper method for cursor input selection
void PlayState::lockSelectedCard ( void )
{
  nom::IntRect coords; // temp container var to hold cursor pos mapping coords

  this->game->cursor.set_state ( 1 ); // board select

  if ( this->isCursorLocked() == false )
  {
    if ( get_turn() == 0 )
    {
      this->game->cursor.set_position ( Point2i(CURSOR_ORIGIN_X-16, CURSOR_ORIGIN_Y) ); // FIXME
    }
    else if ( get_turn() == 1 )
    {
      this->game->cursor.set_position ( Point2i(CURSOR_ORIGIN_X+16, CURSOR_ORIGIN_Y) ); // FIXME
    }

    this->lockCursor ( true );
  }
  else
  {
    coords = this->game->board.getGlobalBounds ( this->game->cursor.position().x, this->game->cursor.position().y );

    if ( coords != nom::IntRect::null )
    {
      this->moveTo ( coords.x, coords.y );
    }

    this->unlockSelectedCard();
  }
}

// Helper method for updating board with player's selected card
void PlayState::moveTo ( unsigned int x, unsigned int y )
{
  Card selected;
  nom::uint32 player_turn = this->get_turn();

  selected = this->game->hand[ player_turn ].getSelectedCard();

  if ( player_turn == PLAYER1 )
  {
    std::vector<BoardTile> adj = this->game->board.find_adjacent ( x, y );

    // Dump returned list of cards
    nom::uint32 line_number = 1;
    for ( auto idx = 0; idx < adj.size(); idx++ )
    {
      Card tile = adj[idx].tile();

      if ( tile.getPlayerID() != Card::PLAYER1 )
      {
        nom::IntRect pos = adj[idx].bounds();
        nom::uint32 element = adj[idx].element();

        std::cout << line_number
                  << ". "
                  << tile.getName()
                  << " is at pos "
                  << pos.x
                  << ", "
                  << pos.y
                  << " ("
                  << pos.w
                  << "x"
                  << pos.h
                  << ") "
                  << " with an element ID of "
                  << element
                  << "."
                  << "\n";

        line_number++;
      }
    }

    /*
        for ( nom::int32 cols = y; cols < BOARD_GRID_HEIGHT; cols++ )
        {
          for ( nom::int32 rows = x; rows < BOARD_GRID_WIDTH; rows++ )
          {
            if ( rows != 0 )
            {

            }
          }
        }
        */
  }

  //std::cout << "\n";

  if ( selected.getID() != BAD_CARD_ID )
  {
    if ( player_turn == PLAYER1 && this->game->board ( x, y ) != BAD_CARD_ID )
    {
      this->game->cursor_wrong->Play();
    }

    if ( this->game->board ( x, y ) == BAD_CARD_ID )
    {
      this->game->board.updateStatus ( x, y, this->game->hand[ player_turn ].getSelectedCard() );
      this->game->hand[ player_turn ].erase ( this->game->hand[ player_turn ].getSelectedCard() );

      this->game->card_place->Play();

      std::vector<std::pair<int, int>> grid = this->game->board.checkBoard ( x, y );

      if ( grid.empty() == false )
      {
        if ( this->game->rules.getRules() == 0 )
        {
          this->game->board.flipCard ( grid[0].first, grid[0].second, player_turn + 1 );
          this->game->card_flip->Play();
        }
      }

      // Combo rule is in effect
      if ( this->game->rules.getRules() != 0 )
      {
        for ( nom::int32 g = 0; g < grid.size(); g++ )
        {
          this->game->board.flipCard ( grid[g].first, grid[g].second, player_turn + 1 );
          this->game->card_flip->Play();

          std::vector<std::pair<int, int>> tgrid = this->game->board.checkBoard ( grid[g].first, grid[g].second );

          // temporary workaround until a more proper solution is found
          if ( this->game->rules.getRules() == 2 || this->game->rules.getRules() == 4 )
            continue;
          else
          {
            for ( nom::int32 tg = 0; tg < tgrid.size(); tg++ )
            {
              this->game->board.flipCard( tgrid[tg].first, tgrid[tg].second, player_turn + 1 );
              this->game->card_flip->Play();
            }
          }
        }
      }
      this->endTurn();
    }
  }
}

// Helper method for obtaining card hand index position based off given origin
// coords definitions, creating us an ID map, initialized early on within the
// encapsulating class
//
// Works with both player1 and player2 coords mapping
//
// cursor_coords_map
//   [ index, y coordinate value ]
//
unsigned int PlayState::getCursorPos ( void )
{
  unsigned int pos = 0;
  nom::int32 idx = 0;

  for ( idx = 0; idx < MAX_PLAYER_HAND; idx++ )
  {
    if ( this->game->cursor.position().y <= this->cursor_coords_map[idx].y )
      return this->cursor_coords_map[idx].x;
    else // catch all safety switch
      // assume we are at the last position in the index when all else fails
      pos = MAX_PLAYER_HAND;
  }

  return pos;
}

void PlayState::moveCursorLeft ( void )
{
  if ( this->game->cursor.state() == 1 ) // locked cursor to board select mode
  {
    if ( this->game->cursor.position().x > BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) )
      this->game->cursor.move ( -( CARD_WIDTH ), 0 );
  }
  this->game->cursor_move->Play();
}

void PlayState::moveCursorRight ( void )
{
  if ( this->game->cursor.state() == 1 ) // locked cursor to board select mode
  {
    if ( this->game->cursor.position().x < BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) )
      this->game->cursor.move ( ( CARD_WIDTH ), 0 );
  }
  this->game->cursor_move->Play();
}

void PlayState::moveCursorUp ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->game->cursor.state() == 0 )
  {
    if ( this->game->cursor.position().y > PLAYER1_CURSOR_ORIGIN_Y )
    {
      this->game->cursor.move ( 0, -( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->game->hand[player_turn].selectCard ( this->game->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->game->cursor.state() == 1 ) // locked cursor to board select mode
  {
    if ( this->game->cursor.position().y > BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) )
      this->game->cursor.move ( 0, -( CARD_HEIGHT ) );
  }
  this->game->cursor_move->Play();
}

void PlayState::moveCursorDown ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->game->cursor.state() == 0 )
  {
    if ( this->game->cursor.position().y < ( CARD_HEIGHT / 2 ) * ( this->game->hand[player_turn].size() ) )
    {
      this->game->cursor.move ( 0, ( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->game->hand[player_turn].selectCard ( this->game->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->game->cursor.state() == 1 ) // locked cursor to board select mode
  {
    if ( this->game->cursor.position().y < BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) )
      this->game->cursor.move ( 0, ( CARD_HEIGHT ) );
  }
  this->game->cursor_move->Play();
}

void PlayState::updateCursor ( void )
{
  if ( this->game->cursor.state() == 1 )
  {
    if ( this->cursor_blink.ticks() > 192 ) // Blinky blink!
    {
      this->cursor_blink.stop();
      this->game->cursor.set_frame ( INTERFACE_CURSOR_NONE );
      this->blink_cursor = true;
    }
  }

  if ( this->get_turn() == PLAYER1 && this->blink_cursor == false ) // player1
  {
    this->game->cursor.set_frame ( INTERFACE_CURSOR_RIGHT );
  }
  // Only show interface cursor for player2 when we are controlling him
  else if ( this->skip_turn == true && this->get_turn() == PLAYER2 && this->blink_cursor == false )
  {
    this->game->cursor.set_frame ( INTERFACE_CURSOR_LEFT );
  }

  this->game->cursor.update();
}

void PlayState::drawCursor ( nom::IDrawable::RenderTarget& target )
{
  this->game->cursor.draw ( target );

  if ( this->blink_cursor )
  {
    this->game->cursor.set_frame ( INTERFACE_CURSOR_RIGHT );
    this->cursor_blink.start();
    this->blink_cursor = false;
  }
}

void PlayState::updateScore ( void )
{
  for ( nom::uint32 players = 0; players < TOTAL_PLAYERS; players++ )
  {
    // Number of cards player owns on the board
    nom::uint32 board_count = this->game->board.getPlayerCount ( players + 1 );

    // Number of cards player has remaining
    nom::uint32 hand_count = this->game->hand[players].size();

    this->player[players].setScore ( board_count + hand_count );

    // Update the font responsible for rendering the score
    this->game->scoreboard_text[players].set_text ( this->player[players].getScoreAsString() );
    this->game->scoreboard_text[players].set_position ( nom::Point2i (this->player_scoreboard[players].x, this->player_scoreboard[players].y) );
  }
}

void PlayState::on_update( float delta_time )
{
  this->game->board.update();

  this->updateCursor();

  this->on_update_info_dialogs();
  this->game->gui_window_.update();

  this->player[0].update();
  this->player[1].update();

  // Player two animation effect
  if ( ! ( this->player_timer[1].ticks() + this->player_timer[1].framerate() >= delta_time ) )
  {
    this->player_timer[1].start();

    // Only show player2 animation when we are not controlling him
    if ( this->skip_turn == false )
    {
      // Fixes a out of bounds issue that occurs occasionally upon state phasing
      if( this->game->hand[PLAYER2].size() > 0 ) {
        nom::uint32 rand_pick = nom::uniform_int_rand<uint32>(0, this->game->hand[PLAYER2].size() - 1);
        this->game->hand[PLAYER2].selectCard ( this->game->hand[PLAYER2].cards[ rand_pick ] );
      }
    }
  }

  if ( this->get_turn() == 0 ) {// player1
    this->game->triad_.set_position(PLAYER1_TRIAD_ORIGIN);
  }
  else { // player2
    this->game->triad_.set_position(PLAYER2_TRIAD_ORIGIN);

    // Skipping a turn like this is only available in debug versions
    if ( this->skip_turn == false ) {
      this->cpu_player_.update(delta_time);
    } // player2

  } // end player1

  this->game->window.update();
}

void PlayState::on_draw( nom::RenderWindow& target )
{
  this->game->background.draw ( target );

  this->game->board.draw ( target );

  this->player[0].draw ( target );
  this->player[1].draw ( target );

  this->game->triad_.draw(target);

  this->drawCursor( target );

  this->game->gui_window_.draw();

  // Draw each player's scoreboard
  this->game->scoreboard_text[0].draw(target);
  this->game->scoreboard_text[1].draw(target);

  // FIXME: We keep game over check logic here in order to allow for the last
  // card placed to be shown to the player
  //
  // Game Over States
  // game / round is over when board card count >= 9
  if ( this->game->board.getCount () >= 9 || this->game->hand[ PLAYER1 ].size() == 0 || this->game->hand[ PLAYER2 ].size() == 0 )
  {
    if ( this->player[ PLAYER1 ].getScore() > this->player[ PLAYER2 ].getScore() )
    {
      this->gameover_state = GameOverType::Won;
      this->game->gameover_text.set_color ( nom::Color4i::White );
      this->game->gameover_text.set_text ( "You win!" );
    }
    else if ( this->player[ PLAYER1 ].getScore() < this->player[ PLAYER2 ].getScore() )
    {
      this->gameover_state = GameOverType::Lost;
      this->game->gameover_text.set_color ( nom::Color4i::White );
      this->game->gameover_text.set_text ( "You lose..." );
    }
    else // Assume a draw
    {
      this->gameover_state = GameOverType::Tie;
      this->game->gameover_text.set_color ( nom::Color4i::White );
      this->game->gameover_text.set_text ( "Tie!" );
    }

    nom::Point2i pos = nom::Point2i ( 0, 0 );
    nom::Size2i size = nom::Size2i ( GAME_RESOLUTION.w, GAME_RESOLUTION.h );

    this->game->gameover_text.set_position ( pos );
    this->game->gameover_text.set_size ( size );
    this->game->gameover_text.set_alignment ( nom::Anchor::MiddleCenter );
    this->game->gameover_text.draw ( target );
    this->game->window.update();

    // Chill for a second
    // nom::sleep ( 1000 );
    if ( this->gameover_state == GameOverType::Tie && this->game->rules.getRules() != CardRules::SuddenDeath )
    {
      this->game->set_state( Game::State::CardsMenu );
    }
    else
    {
      nom::uint32_ptr data = new nom::uint32(this->gameover_state);
      this->game->set_state( Game::State::GameOver, data );
    }
  }
}

bool PlayState::load_game( nom::uint32 flags )
{
  // if ( ev.key.mod == KMOD_LGUI ) // Special game load (player1 always wins!)
  if( flags != 0 )
  {
    if ( this->game->hand[0].load ( "Debug" + path.native() + "player1_unbeatable.json" ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Unable to load game data from: " + std::string("player1_unbeatable.json") );
      this->game->cursor_wrong->Play();
      return false;
    }
    NOM_LOG_INFO ( TTCARDS, "Loaded player 1 data from: " + std::string("player1_unbeatable.json") );
  }
  else // Normal game load for player1
  {
    if ( this->game->hand[0].load( USER_PLAYER1_FILENAME ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Unable to load game data from: " + USER_PLAYER1_FILENAME );
      this->game->cursor_wrong->Play();
      return false;
    }
    NOM_LOG_INFO ( TTCARDS, "Loaded player 1 data from: " + std::string(USER_PLAYER1_FILENAME) );
  }
  if ( this->game->hand[1].load( USER_PLAYER2_FILENAME ) == false )
  {
    NOM_LOG_ERR ( TTCARDS, "Unable to load game data from: " + USER_PLAYER2_FILENAME );
    this->game->cursor_wrong->Play();
    return false;
  }
  NOM_LOG_INFO ( TTCARDS, "Loaded player 2 data from: " + std::string(USER_PLAYER2_FILENAME) );

  if ( this->game->board.load( USER_BOARD_FILENAME ) == false )
  {
    NOM_LOG_ERR ( TTCARDS, "Unable to load game data from: " + USER_BOARD_FILENAME );
    this->game->cursor_wrong->Play();
    return false;
  }

  NOM_LOG_INFO ( TTCARDS, "Loaded board data from: " + std::string(USER_BOARD_FILENAME) );

  // Successful load of saved game!
  this->updateScore();
  this->resetCursor();

  this->game->load_game->Play();

  return true;
}

bool PlayState::save_game( nom::uint32 flags )
{
  if ( this->game->hand[0].save( USER_PLAYER1_FILENAME ) == false )
  {
    NOM_LOG_ERR ( TTCARDS, "Unable to save game data at: " + USER_PLAYER1_FILENAME );
    this->game->cursor_wrong->Play();
    return false;
  }

  NOM_LOG_INFO ( TTCARDS, "Saved player 1 hand data at: " + std::string(USER_PLAYER1_FILENAME) );

  if ( this->game->hand[1].save( USER_PLAYER2_FILENAME ) == false )
  {
    NOM_LOG_ERR ( TTCARDS, "Unable to save game data at: " + USER_PLAYER2_FILENAME );
    this->game->cursor_wrong->Play();
    return false;
  }

  NOM_LOG_INFO ( TTCARDS, "Saved player 2 hand data at: " + std::string(USER_PLAYER2_FILENAME) );

  if ( this->game->board.save( USER_BOARD_FILENAME ) == false )
  {
    NOM_LOG_ERR ( TTCARDS, "Unable to save game data at: " + USER_BOARD_FILENAME );
    this->game->cursor_wrong->Play();
    return false;
  }

  NOM_LOG_INFO ( TTCARDS, "Saved board data at: " + std::string(USER_BOARD_FILENAME) );

  this->game->save_game->Play(); // Successful saved game!

  return true;
}
