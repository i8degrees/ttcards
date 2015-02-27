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

// Private headers
#include "CardRenderer.hpp"
#include "Board.hpp"
#include "helpers.hpp"
#include "resources.hpp"

// Forward declarations
#include "Game.hpp"
#include "HumanPlayer.hpp"
#include "CPU_Player.hpp"
#include "Card.hpp"

// Create keyboard bindings for a game board position
#define CREATE_MOVE_TO_KEY_BINDING(var_name, x, y) \
  auto var_name( [=](const nom::Event& evt) { \
    auto pturn = this->turn(); \
    if( this->game->debug_game_ == true ) { \
      this->move_to( nom::Point2i(x, y) ); \
    } else if( this->game->debug_game_ == false && pturn != PLAYER2 ) { \
      this->move_to( nom::Point2i(x, y) ); \
    } \
  });

using namespace nom;
using namespace ttcards;

PlayState::PlayState(nom::SDLApp* object) :
  nom::IState( Game::State::Play ),
  game( NOM_SCAST(Game*, object) ),
  turn_(PLAYER1),
  cursor_state_(CursorState::PLAYER),
  cursor_locked(false),
  skip_turn(false)
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

PlayState::~PlayState()
{
  NOM_LOG_TRACE(TTCARDS_LOG_CATEGORY_TRACE_STATES);

  NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_TRACE_STATES, "num_actions:",
                  this->game->actions_.num_actions() );
}

void PlayState::on_exit( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->game->info_box_.close();
  this->game->debug_box_.close();
  Rocket::Core::Factory::ClearStyleSheetCache();
  Rocket::Core::Factory::ClearTemplateCache();

  this->game->actions_.cancel_actions();
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
  NOM_ASSERT(this->game != nullptr);

  this->game->board_.reset( new Board() );
  NOM_ASSERT(this->game->board_ != nullptr);

  this->gameover_state_ = GameOverType::NotOver;
  this->cursor_state_ = CursorState::PLAYER;
  this->turn_ = PLAYER1;
  this->cursor_locked = false;
  this->skip_turn = false;

  this->text_action_sprite_ =
    std::make_shared<Sprite>();
  NOM_ASSERT(text_action_sprite_ != nullptr);

  auto p1_db = this->game->cards_db_[PLAYER1].get();
  auto p2_db = this->game->cards_db_[PLAYER2].get();
  NOM_ASSERT(p1_db != nullptr);
  NOM_ASSERT(p2_db != nullptr);

  CPU_Player::
  action_callback cpu_player_action_callback( [=](BoardTile& tile) {
    this->move_to( tile.bounds().position() );
  });

  // TODO: Finish implementing CardRules interface -- bitwise flags!
  bool open_ruleset = false;
  bool elemental_ruleset = false;
  nom::StringList ruleset = this->game->config_->get_array("REGION_RULESET");
  for( auto itr = ruleset.begin(); itr != ruleset.end(); ++itr ) {

    if( (*itr) == "Open" ) {
      open_ruleset = true;
    }

    if( (*itr) == "Elemental" ) {
      elemental_ruleset = true;
    }
  }

  // The open region rule set is not enabled, so no peeking at the opponent's
  // cards! =P
  if( open_ruleset == false ) {
    set_face_down(&this->game->hand[PLAYER2], true);
  } else {
    set_face_down(&this->game->hand[PLAYER2], false);
  }

  while( this->game->hand[PLAYER1].size() < MAX_PLAYER_HAND ) {
    // this->game->hand[PLAYER1].shuffle(8, 10, this->game->collection);
    this->game->hand[PLAYER1].shuffle(1, 1, *p1_db);
  }

  this->game->board_->initialize(&this->game->rules, this->game->card_res_.get() );

  if( elemental_ruleset == true ) {
    this->game->board_->initialize_board_elements();
  }

  this->game->cursor_->set_position( Point2i(PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y) );
  this->game->cursor_->set_frame(INTERFACE_CURSOR_HIDDEN);

  this->players_[0].reset( new HumanPlayer(&this->game->hand[0]) );
  // this->players_[0].reset( new CPU_Player( CPU_Player::Difficulty::Easy,
  //                                   &this->game->board,
  //                                   &this->game->hand[0],
  //                                   cpu_player_action_callback ) );

  this->players_[0]->set_position( Point2i( PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y ) );

  this->players_[1].reset( new CPU_Player(  CPU_Player::Difficulty::Easy,
                                            this->game->board_.get(),
                                            &this->game->hand[1],
                                            cpu_player_action_callback ) );

  this->players_[1]->set_position( Point2i( PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y ) );

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

  const auto GUI_DEBUG =
    this->game->res_cfg_->get_string("GUI_DEBUG");
  if( this->game->debug_box_.load_document_file(GUI_DEBUG) == false ) {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load resource from file:", GUI_DEBUG );
    exit(NOM_EXIT_FAILURE);
    // return false;
  }

  this->game->debug_box_.show();

  if( this->game->debug_game_ == false ) {
    this->game->debug_box_.disable();
  }

  // Southern message box

  if( this->game->info_box_.set_context(&this->game->gui_window_) == false )
  {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could set GUI desktop." );
    // return false;
  }

  const auto GUI_MBOX =
    this->game->res_cfg_->get_string("GUI_MBOX");
  if( this->game->info_box_.load_document_file(GUI_MBOX) == false ) {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load resource from file:", GUI_MBOX );
    exit(NOM_EXIT_FAILURE);
    // return false;
  }

  this->game->info_box_.show();

  // Update both player scores now that we have the player scoreboard X, Y
  // origins calculated for rendering.
  //
  // Assuming a player hand of five, this should always assign a starting score
  // value of 5 for each player initially.
  this->updateScore();

  // Set whose turn it is initially using a random number generator with equal
  // odds -- 50/50 chance that you will have the first move!
  uint32 random_choice =
    nom::uniform_int_rand<nom::uint32>(0, TOTAL_PLAYERS - 1);
  this->set_player_turn(random_choice);

  if( this->turn() == PLAYER2 ) {
    this->initialize_cpu_player_turn();
  }

  this->cpu_hand_delay_seconds_ =
    this->game->config_->get_real32("CPU_HAND_DELAY_SECONDS");
  NOM_ASSERT(this->cpu_hand_delay_seconds_ >= 0.0f);

  // Convert to milliseconds
  this->cpu_hand_delay_seconds_ = this->cpu_hand_delay_seconds_ * 1000;

  // this->game->input_mapper.clear();
  nom::InputActionMapper state;

  CREATE_MOVE_TO_KEY_BINDING(move_to_1, 0, 0);
  CREATE_MOVE_TO_KEY_BINDING(move_to_2, 1, 0);
  CREATE_MOVE_TO_KEY_BINDING(move_to_3, 2, 0);
  CREATE_MOVE_TO_KEY_BINDING(move_to_4, 0, 1);
  CREATE_MOVE_TO_KEY_BINDING(move_to_5, 1, 1);
  CREATE_MOVE_TO_KEY_BINDING(move_to_6, 2, 1);
  CREATE_MOVE_TO_KEY_BINDING(move_to_7, 0, 2);
  CREATE_MOVE_TO_KEY_BINDING(move_to_8, 1, 2);
  CREATE_MOVE_TO_KEY_BINDING(move_to_9, 2, 2);

  state.insert( "moveto_1", nom::KeyboardAction(SDL_KEYDOWN, SDLK_1),
                move_to_1 );
  state.insert( "moveto_2", nom::KeyboardAction(SDL_KEYDOWN, SDLK_2),
                move_to_2 );
  state.insert( "moveto_3", nom::KeyboardAction(SDL_KEYDOWN, SDLK_3),
                move_to_3 );
  state.insert( "moveto_4", nom::KeyboardAction(SDL_KEYDOWN, SDLK_4),
                move_to_4 );
  state.insert( "moveto_5", nom::KeyboardAction(SDL_KEYDOWN, SDLK_5),
                move_to_5 );
  state.insert( "moveto_6", nom::KeyboardAction(SDL_KEYDOWN, SDLK_6),
                move_to_6 );
  state.insert( "moveto_7", nom::KeyboardAction(SDL_KEYDOWN, SDLK_7),
                move_to_7 );
  state.insert( "moveto_8", nom::KeyboardAction(SDL_KEYDOWN, SDLK_8),
                move_to_8 );
  state.insert( "moveto_9", nom::KeyboardAction(SDL_KEYDOWN, SDLK_9),
                move_to_9 );

  // Register event callbacks for our input actions
  auto unlock_selected_card( [=](const nom::Event& evt) {
    this->unlockSelectedCard();
  });
  auto lock_selected_card( [=](const nom::Event& evt) {
    this->lockSelectedCard();
  });

  auto select_card( [=](const nom::Event& evt) {
    this->on_mouse_button_down(evt);
  });

  auto move_cursor_up( [=](const nom::Event& evt) {
    this->moveCursorUp();
  });

  auto move_cursor_down( [=](const nom::Event& evt) {
    this->moveCursorDown();
  });

  auto move_cursor_left( [=](const nom::Event& evt) {
    this->moveCursorLeft();
  });

  auto move_cursor_right( [=](const nom::Event& evt) {
    this->moveCursorRight();
  });

  auto pause_game( [=](const nom::Event& evt) {
    this->game->set_state( Game::State::Pause );
  });

  auto load_game( [=](const nom::Event& evt) {
    this->load_game();
  });

  auto save_game( [=](const nom::Event& evt) {
    this->save_game();
  });

  state.insert( "unlock_selected_card", nom::KeyboardAction( SDL_KEYDOWN, SDLK_x ), unlock_selected_card );
  state.insert( "lock_selected_card", nom::KeyboardAction( SDL_KEYDOWN, SDLK_SPACE ), lock_selected_card );

  state.insert( "move_cursor_up", nom::KeyboardAction( SDL_KEYDOWN, SDLK_UP ), move_cursor_up );
  state.insert( "move_cursor_up", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_Y, nom::MouseWheelAction::UP ), move_cursor_up );

  state.insert( "move_cursor_down", nom::KeyboardAction( SDL_KEYDOWN, SDLK_DOWN ), move_cursor_down );
  state.insert( "move_cursor_down", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_Y, nom::MouseWheelAction::DOWN ), move_cursor_down );

  state.insert( "move_cursor_left", nom::KeyboardAction( SDL_KEYDOWN, SDLK_LEFT ), move_cursor_left );
  state.insert( "move_cursor_right", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RIGHT ), move_cursor_right );

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
  if( this->game->debug_game_ == true ) {

    auto control_turn( [=](const nom::Event& evt) {
      // FIXME: Why are these inversed???
      this->skip_turn = true;
      this->end_turn();
    });

    auto skip_turn( [=](const nom::Event& evt) {
        // FIXME: Why are these inversed???
        this->skip_turn = false;
        this->end_turn();
    });

    auto delete_card( [=](const nom::Event& evt) {
      auto pturn = this->turn();
      Point2i cursor_pos;
      cursor_pos.x = this->player_cursor_coords[pturn].x;
      cursor_pos.y = this->player_cursor_coords[pturn].y;

      Card selected_card = this->game->hand[pturn].getSelectedCard();

      this->game->hand[pturn].erase(selected_card);
      this->game->cursor_->set_position(cursor_pos);
    });

    state.insert( "control_turn", nom::KeyboardAction( SDL_KEYDOWN, SDLK_e, KMOD_LGUI ), control_turn );
    state.insert( "skip_turn", nom::KeyboardAction( SDL_KEYDOWN, SDLK_e ), skip_turn );
    state.insert( "delete_card", nom::KeyboardAction( SDL_KEYDOWN, SDLK_d ), delete_card );

    auto increase_north_rank( [=](const nom::Event& evt) {
      auto pturn = this->turn();
      ttcards::modify_card_rank(  this->game->card_res_.get(),
                                  &this->game->hand[pturn],
                                  true, NORTH );
    });

    auto decrease_north_rank( [=](const nom::Event& evt) {
      auto pturn = this->turn();
      ttcards::modify_card_rank(  this->game->card_res_.get(),
                                  &this->game->hand[pturn],
                                  false, NORTH );
    });

    auto increase_south_rank( [=](const nom::Event& evt) {
      auto pturn = this->turn();
      ttcards::modify_card_rank(  this->game->card_res_.get(),
                                  &this->game->hand[pturn],
                                  true, SOUTH );
    });

    auto decrease_south_rank( [=](const nom::Event& evt) {
      auto pturn = this->turn();
      ttcards::modify_card_rank(  this->game->card_res_.get(),
                                  &this->game->hand[pturn],
                                  false, SOUTH );
    });

    auto increase_west_rank( [=](const nom::Event& evt) {
      auto pturn = this->turn();
      ttcards::modify_card_rank(  this->game->card_res_.get(),
                                  &this->game->hand[pturn],
                                  true, WEST );
    });

    auto decrease_west_rank( [=](const nom::Event& evt) {
      auto pturn = this->turn();
      ttcards::modify_card_rank(  this->game->card_res_.get(),
                                  &this->game->hand[pturn],
                                  false, WEST );
    });

    auto increase_east_rank( [=](const nom::Event& evt) {
      auto pturn = this->turn();
      ttcards::modify_card_rank(  this->game->card_res_.get(),
                                  &this->game->hand[pturn],
                                  true, EAST );
    });

    auto decrease_east_rank( [=](const nom::Event& evt) {
      auto pturn = this->turn();
      ttcards::modify_card_rank(  this->game->card_res_.get(),
                                  &this->game->hand[pturn],
                                  false, EAST );
    });

    state.insert( "increase_north_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_UP, KMOD_LSHIFT ), increase_north_rank );
    state.insert( "decrease_north_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_UP, KMOD_LCTRL ), decrease_north_rank );

    state.insert( "increase_south_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_DOWN, KMOD_LSHIFT ), increase_south_rank );
    state.insert( "decrease_south_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_DOWN, KMOD_LCTRL ), decrease_south_rank );

    state.insert( "increase_west_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_LEFT, KMOD_LSHIFT ), increase_west_rank );
    state.insert( "decrease_west_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_LEFT, KMOD_LCTRL ), decrease_west_rank );

    state.insert( "increase_east_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RIGHT, KMOD_LSHIFT ), increase_east_rank );
    state.insert( "decrease_east_rank", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RIGHT, KMOD_LCTRL ), decrease_east_rank );
  } // end if DEBUG_GAME

  this->game->input_mapper.erase( "PlayState" );
  this->game->input_mapper.insert( "PlayState", state, true );
  this->game->input_mapper.activate_only( "PlayState" );
  this->game->input_mapper.activate( "Game" );

  this->game->triad_->set_frame(0);
  this->game->actions_.run_action(this->game->triad_action_, "triad_action");
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

  if( this->game->actions_.action_running("move_card_up") == true ) {
    return;
  }

  uint32 player_turn = this->turn(); // Ignore player2 mouse input

  // Player cursor positioning
  Point2i player_pos = this->players_[player_turn]->position();

  // mouse input coordinates
  Point2i mouse_input( ev.mouse.x, ev.mouse.y );

  // Rectangle bounds of player's card
  IntRect card_bounds;

  // Disable mouse input if we are not controlling the other player
  if( this->skip_turn == false ) {
    if( this->turn() != 0 ) {
      return;
    }
  }

  // Player hand selection checks; we must calculate the valid bounds of any
  // given card in the player's hand -- the current player position combined
  // with the known card size yields rectangular coordinates we can compare.
  for ( nom::int32 idx = 0; idx < this->game->hand[player_turn].size(); idx++ )
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
      this->game->hand[player_turn].set_position(idx);

      this->game->cursor_->set_position( Point2i(this->player_cursor_coords[ player_turn ].x, this->player_cursor_coords[ player_turn ].y + ( CARD_HEIGHT / 2 ) * idx) );

      this->game->cursor_move->Play();

      // We must break the loop here upon the end of a matching coords check
      // in order to prevent a nasty "last card stays permanently selected"
      // bug from cropping back up!
      break;
    }
  }

  // Board grid coords check; player is attempting to place a card on the board
  // when the player hand coords check above comes back false
  IntRect mouse_map =
    this->game->board_->getGlobalBounds( ev.mouse.x, ev.mouse.y );

  // Attempts to move card onto board; validity checking is performed within
  // the following method call
  if( mouse_map != nom::IntRect::null ) {
    this->move_to( mouse_map.position() );
  }
}

nom::uint32 PlayState::turn()
{
  return this->turn_;
}

void PlayState::set_player_turn(nom::uint32 player_id)
{
  this->turn_ = player_id;

  this->resetCursor();
}

void PlayState::end_turn()
{
  this->unlockSelectedCard();

  this->game->hand[PLAYER1].clearSelectedCard();
  this->game->hand[PLAYER2].clearSelectedCard();

  if( this->turn() == PLAYER1 ) {
    this->set_player_turn(PLAYER2);

    this->initialize_cpu_player_turn();
  }
  else if( this->turn() == PLAYER2 ) {
    this->set_player_turn(PLAYER1);
  }
}

void PlayState::on_update_info_dialogs()
{
  nom::uint32 player_turn = this->turn();
  Card selected_card;
  nom::Point2i cursor_pos;
  nom::IntRect board_pos;
  std::stringstream os;

  cursor_pos = this->game->cursor_->position();
  board_pos = this->game->board_->getGlobalBounds(cursor_pos.x, cursor_pos.y);

  if( this->isCursorLocked() == true ) {

    // ...Board select state...
    selected_card = this->game->board_->get(board_pos.x, board_pos.y);
  } else {
    // ...player hand select state...

    if( this->game->debug_game_ == true ) {
      // Watch both player's card data stream in debug builds
      selected_card = this->game->hand[player_turn].getSelectedCard();
    }
  }

  if( this->game->debug_game_ == true ) {

    // Additional card info
    int32 card_id = selected_card.getID();
    int32 player_id = selected_card.getPlayerID();
    int32 player_owner = selected_card.getPlayerOwner();

    os << card_id << "/" << player_id << " " << "[" << player_owner << "]";
  } else {
    selected_card = this->game->hand[PLAYER1].getSelectedCard();
  }

  if( selected_card == Card::null ) {
    this->game->debug_box_.hide();
    this->game->info_box_.hide();
  } else {

    if( this->game->debug_game_ == true ) {
      this->game->debug_box_.show();
    }

    this->game->info_box_.show();

    std::string card_name = selected_card.getName();

    // Northern info window (debug info)
    this->game->debug_box_.set_message_text( os.str() );

    // Southern info window (always shown)
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
  Point2i cursor_pos(Point2i::zero);
  uint32 player_turn = this->turn();

  this->game->hand[player_turn].front();

  this->cursor_state_ = CursorState::PLAYER;

  // Only set the position of the game interface cursor for the opponent when
  // we are controlling him (debug builds)
  if( this->skip_turn == true ) {
    cursor_pos.x = this->player_cursor_coords[player_turn].x;
    cursor_pos.y = this->player_cursor_coords[player_turn].y;
  } else {
    cursor_pos.x = this->player_cursor_coords[0].x;
    cursor_pos.y = this->player_cursor_coords[0].y;
  }

  this->game->cursor_->set_position(cursor_pos);
}

// helper method for cursor input selection
void PlayState::unlockSelectedCard ( void )
{
  this->cursor_state_ = CursorState::PLAYER;

  this->resetCursor();

  this->lockCursor ( false );

  this->game->cursor_cancel->Play();
}

// helper method for cursor input selection
void PlayState::lockSelectedCard ( void )
{
  nom::IntRect coords; // temp container var to hold cursor pos mapping coords

  this->cursor_state_ = CursorState::BOARD;

  if ( this->isCursorLocked() == false )
  {
    if( this->turn() == 0 ) {
      Point2i cursor_pos;
      cursor_pos.x = CURSOR_ORIGIN_X-16;  // FIXME
      cursor_pos.y = CURSOR_ORIGIN_Y;
      this->game->cursor_->set_position(cursor_pos);
    }
    else if( this->turn() == 1 ) {
      Point2i cursor_pos;
      cursor_pos.x = CURSOR_ORIGIN_X+16;  // FIXME
      cursor_pos.y = CURSOR_ORIGIN_Y;
      this->game->cursor_->set_position(cursor_pos);
    }

    this->lockCursor ( true );
  }
  else
  {
    int cursor_x = this->game->cursor_->position().x;
    int cursor_y = this->game->cursor_->position().y;

    coords = this->game->board_->getGlobalBounds(cursor_x, cursor_y);
    if ( coords != nom::IntRect::null ) {
      this->move_to( coords.position() );
    }

    this->unlockSelectedCard();
  }
}

void PlayState::move_to(const nom::Point2i& rel_board_pos)
{
  nom::uint32 player_turn = this->turn();

  Card selected_card = this->game->hand[player_turn].getSelectedCard();
  if( selected_card.getID() == BAD_CARD_ID ) {
    NOM_LOG_ERR( TTCARDS, "Sanity check failed: the selected card is invalid!" );
    return; // Do not end turn
  }

  // Audible indicator that the move is **not** allowed
  if( this->game->board_->status(rel_board_pos) != BAD_CARD_ID ) {
    // Move is not allowed; card exists
    this->game->cursor_wrong->Play();
    return; // Do not end turn
  }

  if( this->game->actions_.action_running("move_card_up") == true ) {
    return;
  }

  if( this->game->board_->status(rel_board_pos) == BAD_CARD_ID ) {

    // ...Move is allowed...

    // Animation for the card leaving the player's hand and landing onto the
    // board
    this->move_card_up_action(rel_board_pos, [=](const Card& card) {

      Card selected_card = card;
      this->game->board_->update(rel_board_pos, selected_card);
      this->game->hand[player_turn].erase(selected_card);

      this->flip_cards(rel_board_pos, [=]() {
        this->end_turn();
      });
    }); // end move_card_up_action callback
  } // end if game board position is open
}

void
PlayState::flip_cards(  const nom::Point2i& rel_board_pos,
                        const std::function<void()>& on_completion_func )
{
  uint32 player_turn = this->turn();

  // Check for flippable cards
  board_tiles_result grid =
    this->game->board_->check_board(rel_board_pos);
  for( auto itr = grid.begin(); itr != grid.end(); ++itr ) {

    int gpos0 = itr->tile.position().x;
    int gpos1 = itr->tile.position().y;
    Point2i gpos(gpos0, gpos1);
    uint32 applied_rule = itr->applied_rule;

    if( applied_rule == CardRules::Same ) {
      this->text_action_sprite_ = this->game->same_text_sprite_;
    } else {
      this->text_action_sprite_ = nullptr;
    }

    auto flip_text_action = std::make_shared<MoveByAction>(
      this->text_action_sprite_, Point2i(-GAME_RESOLUTION.w, 0), 1.0f);
    NOM_ASSERT(flip_text_action != nullptr);

    // NOTE: This action has two separate action sprites given to it, and thus
    // the card flipping animation will occur in either condition
    if( applied_rule == CardRules::Same ) {
      // Render the "Same!" scrolling text animation
      flip_text_action->set_name("same_text_action");
    } else {
      // Render the card flipping animation
      flip_text_action->set_name("flip_card");
    }

    // Reset position for action to translate from
    if( this->text_action_sprite_ != nullptr ) {
      nom::set_alignment( this->text_action_sprite_.get(), Point2i::zero,
                          GAME_RESOLUTION, nom::Anchor::MiddleRight );
    }

    this->game->actions_.run_action(flip_text_action, [=]() {

      this->flip_card_action(gpos);

      this->game->board_->flip_card(gpos, player_turn + 1);

      this->updateScore();
      this->game->card_flip->Play();

      if( this->game->rules.getRules() != CardRules::NoRules ) {

        // Do a second round of flippable cards check for the COMBO rule-set
        board_tiles_result tgrid =
          this->game->board_->check_board(gpos);

        for( auto itr = tgrid.begin(); itr != tgrid.end(); ++itr ) {
          int tgpos0 = itr->tile.position().x;
          int tgpos1 = itr->tile.position().y;
          Point2i tgpos(tgpos0, tgpos1);
          // uint32 applied_rule = itr->applied_rule;

          this->text_action_sprite_ = this->game->combo_text_sprite_;

          auto combo_action = std::make_shared<MoveByAction>(
            this->text_action_sprite_, Point2i(-GAME_RESOLUTION.w, 0), 1.0f);
          NOM_ASSERT(combo_action != nullptr);
          combo_action->set_name("combo_text_action");

          // Reset position for action to translate from
          nom::set_alignment( this->text_action_sprite_.get(),
                              Point2i::zero, GAME_RESOLUTION,
                              nom::Anchor::MiddleRight );

          this->game->actions_.run_action(combo_action, [=]() {

            this->flip_card_action(tgpos);

            this->game->board_->flip_card(tgpos, player_turn + 1);

            this->updateScore();
            this->game->card_flip->Play();
          });
        } // end inner for loop (additional flips)
      } // end if combo rule is in effect
    }); // end outer callback
  } // end outer for loop (flippable cards)

  NOM_ASSERT(on_completion_func != nullptr);
  on_completion_func.operator()();
}

// TODO: In order to resolve the cursor / card selection synchronization issue,
// we need to ensure that we do **not** update the selection anytime this
// action is enqueued.
void
PlayState::move_card_up_action( const nom::Point2i& rel_board_pos,
                                const move_card_up_action_callback&
                                on_completion_func )
{
  uint32 pturn = this->turn();

  this->game->actions_.cancel_action("move_card_up");

  Card pcard =
    this->game->hand[pturn].getSelectedCard();

  std::shared_ptr<CardRenderer> card_renderer =
    pcard.card_renderer();

  this->move_card_up_sprite_ =
    card_renderer->rendered_card();
  NOM_ASSERT(this->move_card_up_sprite_ != nullptr);
  NOM_ASSERT(this->move_card_up_sprite_->valid() == true);

  Point2i player_pos = this->players_[pturn]->position();

  // We begin the animation by moving the selected card from the player's hand
  // upward, so we need to apply the offset that is rendered in IPlayer::draw

  // TODO: Perhaps we should just apply the rendering offset to the player's
  // position getter???
  player_pos.y += (CARD_HEIGHT / 2 ) * this->game->hand[pturn].position();
  if( pturn == 0 ) {
    player_pos.x -= 16;
  } else if( pturn == 1 ) {
    player_pos.x += 16;
  }

  this->move_card_up_sprite_->set_position(player_pos);

  auto move_card_up =
    std::make_shared<MoveByAction>( this->move_card_up_sprite_,
                                    Point2i(0, -GAME_RESOLUTION.h), 1.0f);
  NOM_ASSERT(move_card_up != nullptr);
  move_card_up->set_timing_mode(nom::Expo::ease_out);

  // Global (screen) coordinates
  Point2i board_pos;
  board_pos.x =
    this->game->board_->tile(rel_board_pos.x, rel_board_pos.y).bounds().size().w;
  board_pos.y =
    this->game->board_->tile(rel_board_pos.x, rel_board_pos.y).bounds().size().h;

  // We must reverse the board position coordinates for 2, y and 0, y
  if( pturn == PLAYER1 ) {
    if( rel_board_pos.x == 2 ) {
      board_pos.x = BOARD_ORIGIN_X;
    } else if( rel_board_pos.x == 0 ) {
      board_pos.x = BOARD_ORIGIN_X + (CARD_WIDTH * 2);
    }
  }

  // The off-screen animation delta for moving along the X-axis
  Point2i delta(board_pos.x, 0);

  if( pturn == PLAYER1 ) {
    // Subtract away the player's card rendering offset; this undoes the
    // player_pos calculation done above to get us the actual board position
    delta.x -= 48;
    // We are going right to left here, from the player's origin point
    delta.x = -delta.x;
    delta.y = 0;
  }

  // The opponent requires no special delta calculation because we do not need
  // to compute the board position from right to left as we do with the player

  auto move_card_along =
    std::make_shared<MoveByAction>( this->move_card_up_sprite_,
                                    delta, 0.1f );
  NOM_ASSERT(move_card_along != nullptr);

  auto move_card_down =
    std::make_shared<MoveByAction>( this->move_card_up_sprite_,
                                    Point2i(0, board_pos.y), 0.3f);
  NOM_ASSERT(move_card_down != nullptr);
  move_card_down->set_timing_mode(nom::Expo::ease_out);

  auto move_card_sequence =
    nom::create_action<SequenceAction>( { move_card_up, move_card_along,
                                          move_card_down }, "move_card_up" );
  NOM_ASSERT(move_card_sequence != nullptr);

  this->game->card_place->Play();

  this->game->actions_.run_action(move_card_sequence, [=]() {

    NOM_ASSERT(on_completion_func != nullptr);
    on_completion_func.operator()(pcard);
  });
}

// This works by alpha blending a sprite of the player's card on top of the
// flip (new owner) -- the sprite is rendered only until the action is
// completed.
void PlayState::flip_card_action(const nom::Point2i& rel_board_pos)
{
  Card pcard =
    this->game->board_->get(rel_board_pos.x, rel_board_pos.y);

  std::shared_ptr<CardRenderer> card_renderer =
    pcard.card_renderer();
  NOM_ASSERT(card_renderer != nullptr);

  this->flip_card_sprite_ =
    card_renderer->rendered_card();
  NOM_ASSERT(this->flip_card_sprite_ != nullptr);
  this->flip_card_sprite_->set_alpha(Color4i::ALPHA_OPAQUE);

  const real32 FLIP_CARD_FADE_DURATION =
    this->game->config_->get_real32("FLIP_CARD_FADE_DURATION");

  auto flip_card_action0 =
    nom::create_action<FadeOutAction>(  this->flip_card_sprite_,
                                        FLIP_CARD_FADE_DURATION );
  NOM_ASSERT(flip_card_action0 != nullptr);
  flip_card_action0->set_name("flip_card");

  // auto flip_card_action1 =
    // nom::create_action<FadeInAction>( this->flip_card_sprite_,
                                      // FLIP_CARD_FADE_DURATION );
  // NOM_ASSERT(flip_card_action1 != nullptr);
  // flip_card_action0->set_name("flip_card");

  this->game->actions_.run_action(flip_card_action0, [=]() {

    // this->game->actions_.run_action(flip_card_action1, [=]() {
    //   NOM_DUMP( this->flip_card_sprite_->position() );
    //   NOM_DUMP( (int)this->flip_card_sprite_->alpha() );
    // });
  });
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
    if( this->game->cursor_->position().y <= this->cursor_coords_map[idx].y )
      return this->cursor_coords_map[idx].x;
    else // catch all safety switch
      // assume we are at the last position in the index when all else fails
      pos = MAX_PLAYER_HAND;
  }

  return pos;
}

void PlayState::moveCursorLeft ( void )
{
  Point2i move_to_offset(Point2i::zero);

  // locked cursor to board select mode
  if( this->cursor_state_ == CursorState::BOARD ) {

    if( this->game->cursor_->position().x >
        BOARD_ORIGIN_X + (CARD_WIDTH * 1) )
    {
      move_to_offset.x = -(CARD_WIDTH);
      move_to_offset.y = 0;
      this->game->cursor_->translate(move_to_offset);
    }
  }

  this->game->cursor_move->Play();
}

void PlayState::moveCursorRight ( void )
{
  Point2i move_to_offset(Point2i::zero);

  // locked cursor to board select mode
  if( this->cursor_state_ == CursorState::BOARD ) {

    if( this->game->cursor_->position().x <
        BOARD_ORIGIN_X + (CARD_WIDTH * 2) )
    {
      move_to_offset.x = CARD_WIDTH;
      move_to_offset.y = 0;
      this->game->cursor_->translate(move_to_offset);
    }
  }

  this->game->cursor_move->Play();
}

void PlayState::moveCursorUp ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = this->turn();
  Point2i move_to_offset(Point2i::zero);

  if( this->game->actions_.action_running("move_card_up") ) {
    return;
  }

  if ( this->cursor_state_ == CursorState::PLAYER )
  {
    if( this->game->cursor_->position().y > PLAYER1_CURSOR_ORIGIN_Y ) {
      move_to_offset.x = 0;
      move_to_offset.y = -(CARD_HEIGHT / 2);
      this->game->cursor_->translate(move_to_offset);

      pos = this->getCursorPos();
      this->game->hand[player_turn].previous();
    }
  }
  else if ( this->cursor_state_ == CursorState::BOARD ) // locked cursor to board select mode
  {
    if( this->game->cursor_->position().y >
        BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) )
    {
      move_to_offset.x = 0;
      move_to_offset.y = -(CARD_HEIGHT);
      this->game->cursor_->translate(move_to_offset);
    }
  }
  this->game->cursor_move->Play();
}

void PlayState::moveCursorDown ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = this->turn();
  Point2i move_to_offset(Point2i::zero);

  if( this->game->actions_.action_running("move_card_up") ) {
    return;
  }

  if ( this->cursor_state_ == CursorState::PLAYER )
  {
    if( this->game->cursor_->position().y <
        (CARD_HEIGHT / 2) * ( this->game->hand[player_turn].size() ) )
    {
      move_to_offset.x = 0;
      move_to_offset.y = (CARD_HEIGHT / 2);
      this->game->cursor_->translate(move_to_offset);

      pos = this->getCursorPos();
      this->game->hand[player_turn].next();
    }
  }
  else if ( this->cursor_state_ == CursorState::BOARD ) // locked cursor to board select mode
  {
    if( this->game->cursor_->position().y <
        BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) )
    {
      move_to_offset.x = 0;
      move_to_offset.y = CARD_HEIGHT;
      this->game->cursor_->translate(move_to_offset);
    }
  }
  this->game->cursor_move->Play();
}

void PlayState::updateCursor()
{
  uint32 player_turn = this->turn();
  bool blinking_cursor_action =
    this->game->actions_.action_running("blinking_cursor_action");

  if( this->cursor_state_ == CursorState::BOARD ) {

    if( blinking_cursor_action == false ) {
      this->game->cursor_->set_frame(INTERFACE_CURSOR_HIDDEN);
      this->game->actions_.run_action(  this->game->blinking_cursor_action_,
                                        "blinking_cursor_action" );
    }
  } else {  // this->cursor_state_ != CursorState::BOARD

    this->game->actions_.cancel_action("blinking_cursor_action");

    if( player_turn == PLAYER1 ) {

      this->game->cursor_->set_sprite_sheet(this->game->right_cursor_frames_);
      this->game->cursor_->set_frame(INTERFACE_CURSOR_SHOWN);
    } else if( this->skip_turn == true && player_turn == PLAYER2 ) {

      this->game->cursor_->set_sprite_sheet(this->game->left_cursor_frames_);
      this->game->cursor_->set_frame(INTERFACE_CURSOR_SHOWN);
    }
  }
}

void PlayState::updateScore ( void )
{
  for ( nom::uint32 players = 0; players < TOTAL_PLAYERS; players++ )
  {
    // Number of cards player owns on the board
    nom::uint32 board_count = this->game->board_->getPlayerCount(players + 1);

    // Number of cards player has remaining
    nom::uint32 hand_count = this->game->hand[players].size();

    this->players_[players]->set_score(board_count + hand_count);

    // Update the font responsible for rendering the score
    this->game->scoreboard_text[players].set_text( this->players_[players]->score_string() );
    this->game->scoreboard_text[players].set_position( nom::Point2i (this->player_scoreboard[players].x, this->player_scoreboard[players].y) );
  }
}

void PlayState::on_update(nom::real32 delta_time)
{
  uint32 player_turn = this->turn();

  this->updateCursor();

  this->on_update_info_dialogs();
  this->game->gui_window_.update();

  this->players_[0]->update(delta_time);

  // CPU player's hand animation
  if( delta_time >= this->last_delta_ + this->cpu_hand_delay_seconds_ ) {
    last_delta_ = delta_time;

    // Only show the animation when we are not controlling him (debug builds)
    if( this->skip_turn == false ) {

      // Fixes a out of bounds issue that occurs occasionally upon state
      // phasing
      if( this->game->hand[PLAYER2].size() > 0 ) {
        nom::uint32 rand_pick =
          nom::uniform_int_rand<uint32>(0, this->game->hand[PLAYER2].size() - 1);
        this->game->hand[PLAYER2].set_position(rand_pick);
      }
    }
  }

  if( player_turn == PLAYER1 ) {

    this->game->triad_->set_position(PLAYER1_TRIAD_ORIGIN);
  } else if( player_turn == PLAYER2 ) {

    this->game->triad_->set_position(PLAYER2_TRIAD_ORIGIN);

    // Skipping a turn like this is only available in debug versions
    if( this->skip_turn == false ) {

      if( this->game->actions_.action_running("cpu_move_delay") == false &&
          this->game->actions_.action_running("move_card_up") == false )
      {
        this->players_[PLAYER2]->update(delta_time);
      }
    } // end if skip_turn == FALSE
  } else {
    NOM_ASSERT("PlayState::update(): player_turn > PLAYER2");
  }

  this->game->window.update();

  this->check_gameover_conditions();
}

void PlayState::on_draw( nom::RenderWindow& target )
{
  this->game->background.draw ( target );

  this->game->board_->draw(target);

  this->players_[PLAYER1]->draw(target);
  this->players_[PLAYER2]->draw(target);

  this->game->triad_->draw(target);

  if( this->game->cursor_ != nullptr && this->game->cursor_->valid() ) {
    this->game->cursor_->draw(target);
  }

  this->game->gui_window_.draw();

  // Draw each player's scoreboard
  this->game->scoreboard_text[0].draw(target);
  this->game->scoreboard_text[1].draw(target);

  if( this->game->actions_.action_running("same_text_action") == true ) {
    if( this->text_action_sprite_ != nullptr &&
        this->text_action_sprite_->valid() == true )
    {
      this->text_action_sprite_->draw(target);
    }
  }

  if( this->game->actions_.action_running("combo_text_action") == true ) {
    if( this->text_action_sprite_ != nullptr &&
        this->text_action_sprite_->valid() == true )
    {
      this->text_action_sprite_->draw(target);
    }
  }

  if( this->game->actions_.action_running("gameover_action") == true ) {
    if( this->gameover_text_action_sprite_ != nullptr &&
        this->gameover_text_action_sprite_->valid() == true )
    {
      this->gameover_text_action_sprite_->draw(target);
    }
  }

#if 1
  if( this->game->actions_.action_running("move_card_up") == true ) {
    if( this->move_card_up_sprite_ != nullptr &&
        this->move_card_up_sprite_->valid() == true )
    {
      this->move_card_up_sprite_->draw(target);
    }
  }
#endif

#if 1
  if( this->game->actions_.action_running("flip_card") == true ) {
    if( this->flip_card_sprite_ != nullptr &&
        this->flip_card_sprite_->valid() == true )
    {
      this->flip_card_sprite_->draw(target);
    }
  }
#endif
}

bool PlayState::save_game()
{
  if( ttcards::save_game(this->game->board_.get(), this->game->hand) == false ) {
    this->game->cursor_wrong->Play();
    return false;
  }

  // Success!
  this->game->save_game->Play();
  return true;
}

bool PlayState::load_game()
{
  if( ttcards::load_game(this->game->board_.get(), this->game->hand) == false ) {
    this->game->cursor_wrong->Play();
    return false;
  }

  // Success!
  this->updateScore();
  this->resetCursor();

  this->game->load_game->Play();
  return true;
}

void PlayState::check_gameover_conditions()
{
  auto board_count = this->game->board_->getCount();
  NOM_ASSERT(board_count <= BOARD_COUNT_MAX);

  auto player1_score = this->players_[PLAYER1]->score();
  auto player1_num_cards = this->game->hand[PLAYER1].size();

  auto player2_score = this->players_[PLAYER2]->score();
  auto player2_num_cards = this->game->hand[PLAYER2].size();

  auto rule_set = this->game->rules.getRules();

  if( this->game->actions_.action_running("move_card_up") == true ) {
    return;
  }

  if( this->game->actions_.action_running("flip_card") == true ) {
    return;
  }

  if( this->game->actions_.action_running("same_text_action") == true ) {
    return;
  }

  if( this->game->actions_.action_running("combo_text_action") == true ) {
    return;
  }

  if( board_count == BOARD_COUNT_MAX || player1_num_cards == 0 ||
      player2_num_cards == 0 )
  {
    if( this->gameover_state_ != GameOverType::NotOver ) {
      this->game->actions_.cancel_actions();

      // TODO: Implement CardRules::SuddenDeath
      if( this->gameover_state_ == GameOverType::Tie &&
          rule_set != CardRules::SuddenDeath )
      {
        this->game->set_state(Game::State::CardsMenu);
      } else {
        this->game->set_state(Game::State::GameOver, &this->gameover_state_);
      }
    }

    if( this->game->actions_.action_running("gameover_action") == true ) {
      return;
    }

    if( player1_score > player2_score ) {

      auto gameover_text_action =
        create_gameover_text_action(GameOverType::Won, "gameover_action");

      if( this->game->winning_track->getStatus() != Playing ) {
        this->game->theme_track_->Stop();
        this->game->winning_track->Play();
      }

      this->game->actions_.run_action(gameover_text_action, [=]() {
        this->gameover_state_ = GameOverType::Won;
      });
    } else if( player1_score < player2_score ) {

      auto gameover_text_action =
        create_gameover_text_action(GameOverType::Lost, "gameover_action");

      this->game->actions_.run_action(gameover_text_action, [=]() {
        this->gameover_state_ = GameOverType::Lost;
      });
    } else {

      auto gameover_text_action =
        create_gameover_text_action(GameOverType::Tie, "gameover_action");

      this->game->actions_.run_action(gameover_text_action, [=]() {
        this->gameover_state_ = GameOverType::Tie;
      });
    }
  } // end game over conditions check
}

std::shared_ptr<nom::IActionObject>
PlayState::create_gameover_text_action( GameOverType type,
                                        const std::string& action_name )
{
  const real32 GAMEOVER_TEXT_FADE_DURATION =
    this->game->config_->get_real32("GAMEOVER_TEXT_FADE_DURATION");
  NOM_ASSERT(GAMEOVER_TEXT_FADE_DURATION > 0.0f);

  auto transition_delay_action =
    std::make_shared<WaitForDurationAction>(GAMEOVER_TEXT_FADE_DURATION);
  NOM_ASSERT(transition_delay_action != nullptr);

  if( type == GameOverType::Won ) {
    this->gameover_text_action_sprite_ = this->game->won_text_sprite_;
  } else if( type == GameOverType::Lost ) {
    this->gameover_text_action_sprite_ = this->game->lost_text_sprite_;
  } else if( type == GameOverType::Tie ) {
    this->gameover_text_action_sprite_ = this->game->tied_text_sprite_;
  } else {
    this->gameover_text_action_sprite_ = nullptr;
  }

  // Reset the action's internal state
  this->gameover_text_action_sprite_->set_alpha(Color4i::ALPHA_TRANSPARENT);

  auto gameover_text_fade_in_action =
    std::make_shared<FadeInAction>( this->gameover_text_action_sprite_,
                                    GAMEOVER_TEXT_FADE_DURATION );
  NOM_ASSERT(gameover_text_fade_in_action != nullptr);

  nom::action_list sequence_list = {  gameover_text_fade_in_action,
                                      transition_delay_action };
  auto gameover_text_action =
    nom::create_action<SequenceAction>(sequence_list, action_name);
  NOM_ASSERT(gameover_text_action != nullptr);

  return gameover_text_action;
}

void PlayState::initialize_cpu_player_turn()
{
  real32 cpu_move_delay_seconds =
    this->game->config_->get_real32("CPU_MOVE_DELAY_SECONDS");
  if( cpu_move_delay_seconds < 0.0f ) {
    cpu_move_delay_seconds = 1.0f;
  }

  auto cpu_move_delay_timer =
    std::make_shared<WaitForDurationAction>(cpu_move_delay_seconds);
  NOM_ASSERT(cpu_move_delay_timer != nullptr);

  if( cpu_move_delay_timer != nullptr ) {
    this->game->actions_.run_action(cpu_move_delay_timer, "cpu_move_delay");
  }
}
