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

// Private headers
#include "CardRenderer.hpp"
#include "Board.hpp"
#include "CardRules.hpp"

// Forward declarations
#include "Game.hpp"

using namespace nom;

namespace tt {

GameOverState::GameOverState(nom::SDLApp* object, nom::void_ptr state) :
  nom::IState(Game::State::GameOver),
  game( NOM_SCAST(Game*, object) ),
  show_results( false ),
  gameover_state_( *static_cast<nom::uint32_ptr>(state) )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

GameOverState::~GameOverState( void )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  NOM_LOG_DEBUG(  TTCARDS_LOG_CATEGORY_TRACE_STATES, "num_actions:",
                  this->game->actions_.num_actions() );
}

void GameOverState::on_init(nom::void_ptr data)
{
  this->cursor_.set_event_handler(this->game->evt_handler_);
  this->cursor_.set_texture(this->game->cursor_tex_);
  this->cursor_.set_sprite_sheet(this->game->right_cursor_frames_);
  this->cursor_.set_frame(INTERFACE_CURSOR_SHOWN);

  this->cursor_.set_position_map(&this->game->hand[PlayerIndex::PLAYER_2]);
  this->cursor_.set_size( Size2i ( CURSOR_WIDTH, CURSOR_HEIGHT ) );
  this->cursor_.set_position(  Point2i (
                                        PLAYER2_GAMEOVER_CURSOR_ORIGIN_X,
                                        PLAYER2_GAMEOVER_CURSOR_ORIGIN_Y
                                      ));

  // Fully reconstruct the player's hand by adding the cards placed on the board
  // back into each player's respective hand.
  //
  // 1. Make a a copy of each player's hand as it existed (on the board).
  // 2. Reset each player card found back to their original color.
  //
  for( auto y = 0; y != BOARD_GRID_HEIGHT; ++y) {
    for( auto x = 0; x != BOARD_GRID_WIDTH; ++x) {

      // Err; invalid game board
      if( this->game->board_ == nullptr ) {
        break;
      }

      Card pcard = this->game->board_->get(x, y);
      pcard.face_down = false;

      if( pcard.player_owner == PlayerID::PLAYER_ID_1 ) {
        pcard.player_id = PlayerID::PLAYER_ID_1;

        // Render the new card background based on the new owner
        auto renderer = tt::create_card_renderer(this->game->card_res_.get(), pcard);
        pcard.card_renderer.reset(renderer);
        NOM_ASSERT(pcard.card_renderer != nullptr);
        NOM_ASSERT(pcard.card_renderer->valid() == true);

        this->game->hand[PlayerIndex::PLAYER_1].push_back(pcard);
      } else if( pcard.player_owner == PlayerID::PLAYER_ID_2 ) {
        pcard.player_id = PlayerID::PLAYER_ID_2;

        // Render the new card background based on the new owner
        auto renderer =
          tt::create_card_renderer(this->game->card_res_.get(), pcard);
        pcard.card_renderer.reset(renderer);
        NOM_ASSERT(pcard.card_renderer != nullptr);
        NOM_ASSERT(pcard.card_renderer->valid() == true);

        this->game->hand[PlayerIndex::PLAYER_2].push_back(pcard);
      }
    }
  }

  // ...Set the card rendering layout for the player and opponent...

  auto p1_hand = this->game->hand[PlayerIndex::PLAYER_1];
  auto p2_hand = this->game->hand[PlayerIndex::PLAYER_2];

  auto p2_hand_idx = 0;
  Point2i p2_pos(Point2i::zero);
  for( auto itr = p2_hand.begin(); itr != p2_hand.end(); ++itr ) {

   p2_pos.x = PLAYER2_GAMEOVER_ORIGIN_X + (CARD_WIDTH * p2_hand_idx);
   p2_pos.y = PLAYER2_GAMEOVER_ORIGIN_Y;
   ++p2_hand_idx;

    auto card_renderer =
      itr->card_renderer;
    if( card_renderer != nullptr && card_renderer->valid() == true ) {
      card_renderer->set_position(p2_pos);
    }
  }

  auto p1_hand_idx = 0;
  Point2i p1_pos(Point2i::zero);
  for( auto itr = p1_hand.begin(); itr != p1_hand.end(); ++itr ) {

   p1_pos.x = PLAYER1_GAMEOVER_ORIGIN_X + (CARD_WIDTH * p1_hand_idx);
   p1_pos.y = PLAYER1_GAMEOVER_ORIGIN_Y;
   ++p1_hand_idx;

    auto card_renderer =
      itr->card_renderer;
    if( card_renderer != nullptr && card_renderer->valid() == true ) {
      card_renderer->set_position(p1_pos);
    }
  }

  if( this->gameover_state_ == GameOverType::Tie ) {
    NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_GAME_OVER_STATE, "Draw...");
  } else if( this->gameover_state_ == GameOverType::Won ) {
    NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_GAME_OVER_STATE, "You win!");
  } else if( this->gameover_state_ == GameOverType::Lost ) {
    NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_GAME_OVER_STATE, "You lose...");

    // Stub logic
    NOM_ASSERT( this->game->hand[PlayerIndex::PLAYER_1].empty() == false );
    if( this->game->hand[PlayerIndex::PLAYER_1].size() > 0 ) {

      Card strongest_card = this->game->hand[PlayerIndex::PLAYER_1].strongest();
      this->game->hand[PlayerIndex::PLAYER_1].selectCard(strongest_card);
      NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_GAME_OVER_STATE,
                    "Lost", strongest_card.name );
    }
  } else {  // GameOverType::NotOver
    // We shouldn't ever see this
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Game was not over:",
                  "state should not have been brought into existence?!");
    // this->game->set_state(Game::State::CardsMenu);

    // So we know something is up
    exit(NOM_EXIT_FAILURE);
  }

  // Reset the player's hand back to the front, so our cursor tracking is
  // accurate.
  this->game->hand[PlayerIndex::PLAYER_2].front();

  // Northern message box

  if( this->game->info_box_.set_context(&this->game->gui_window_) == false )
  {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could set GUI desktop." );
    // return false;
  }

  const auto GUI_GAMEOVER_RULES_INFO =
    this->game->res_cfg_->get_string("GUI_GAMEOVER_RULES_INFO");
  if( this->game->info_box_.load_document_file(GUI_GAMEOVER_RULES_INFO) == false ) {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load resource file:", GUI_GAMEOVER_RULES_INFO );
    exit(NOM_EXIT_FAILURE);
    // return false;
  }

  this->game->info_box_.set_message_text("Select 1 card(s) you want");
  this->game->info_box_.show();

  // Southern message box (card name)

  if( this->game->card_info_box_.set_context(&this->game->gui_window_) == false )
  {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could set GUI desktop." );
    // return false;
  }

  const auto GUI_GAMEOVER_CARD_INFO =
    this->game->res_cfg_->get_string("GUI_GAMEOVER_CARD_INFO");
  if( this->game->card_info_box_.load_document_file(GUI_GAMEOVER_CARD_INFO) == false ) {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load resource file:", GUI_GAMEOVER_CARD_INFO );
    exit(NOM_EXIT_FAILURE);
    // return false;
  }

  auto card_text =
    this->game->hand[PlayerIndex::PLAYER_2].getSelectedCard().name;
  this->game->card_info_box_.set_message_text(card_text);
  this->game->card_info_box_.show();

  // Commence the countdown on the showing of results!
  this->transistion.start();

  nom::InputActionMapper state;

  auto pause_game( [=](const nom::Event& evt) {
    this->game->set_state(Game::State::Pause);
  });

  auto move_cursor_left( [=](const nom::Event& evt) {
    this->cursor_.move_left();
  });

  auto move_cursor_right( [=](const nom::Event& evt) {
    this->cursor_.move_right();
  });

  auto select_card( [=](const nom::Event& evt) {
    this->on_mouse_button_down(evt);
  });

  auto select_cards( [=](const nom::Event& evt) {
    this->selected_card =
      this->game->hand[PlayerIndex::PLAYER_2].getSelectedCard();

    // NOTE: Data validation (crash prevention)
    if( this->selected_card != Card::null ) {
      this->game->set_state(Game::State::ConfirmationDialog);
    } else {
      this->game->cursor_wrong->Play();
      this->game->set_state(Game::State::MainMenu);
    }
  });

  // ...Keyboard mappings...
  state.insert("pause_game", nom::KeyboardAction(SDLK_p), pause_game);
  state.insert( "move_cursor_left", nom::KeyboardAction(SDLK_LEFT),
                move_cursor_left );
  state.insert( "move_cursor_right", nom::KeyboardAction(SDLK_RIGHT),
                move_cursor_right );
  state.insert( "select_cards", nom::KeyboardAction(SDLK_SPACE), select_cards );

  // ...Mouse button & wheel mappings...

  state.insert( "select_card", nom::MouseButtonAction(nom::LEFT_MOUSE_BUTTON,
                InputState::RELEASED), select_card );
  state.insert( "select_cards",
                nom::MouseButtonAction(nom::LEFT_MOUSE_BUTTON, 2,
                InputState::RELEASED), select_cards );

  state.insert( "move_cursor_left",
                nom::MouseWheelAction(nom::MOUSE_WHEEL_UP),
                move_cursor_left );
  state.insert( "move_cursor_right",
                nom::MouseWheelAction(nom::MOUSE_WHEEL_DOWN),
                move_cursor_right );

  // ...Joystick mappings...
  auto& joystick_id = this->game->joystick_id_;

  state.insert( "pause_game",
                nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_START), pause_game );
  state.insert( "move_cursor_left",
                nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_DPAD_LEFT), move_cursor_left );
  state.insert( "move_cursor_right",
                nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_DPAD_RIGHT), move_cursor_right );
  state.insert( "select_cards",
                nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_A), select_cards );

  if( this->game->debug_game_ == true ) {
    auto restart_game( [=](const nom::Event& evt) {
      this->selected_card = this->game->hand[PlayerIndex::PLAYER_2].getSelectedCard();
      this->game->set_state(Game::State::CardsMenu);
    });

    state.insert("restart_game", nom::KeyboardAction(SDLK_RETURN), restart_game);

    state.insert( "restart_game", nom::GameControllerButtonAction(joystick_id,
                  nom::GameController::BUTTON_BACK), restart_game );
  } // end if DEBUG_GAME

  this->game->input_mapper.erase( "GameOverState" );
  this->game->input_mapper.insert( "GameOverState", state, true );
  this->game->input_mapper.activate_only( "GameOverState" );
  this->game->input_mapper.activate( "Game" );

    // ...Initialization of animation sprites...

  this->flash_action_sprite_ = std::make_shared<Sprite>();
  if( this->flash_action_sprite_ != nullptr ) {
    this->flash_action_sprite_->init_with_color(Color4i::White, CARD_DIMS);
    this->flash_action_sprite_->set_alpha(Color4i::ALPHA_TRANSPARENT);
  }
}

void GameOverState::on_exit(nom::void_ptr data)
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->game->winning_track->Stop();

  this->game->info_box_.close();
  this->game->card_info_box_.close();
  Rocket::Core::Factory::ClearStyleSheetCache();
  Rocket::Core::Factory::ClearTemplateCache();

  this->game->actions_.cancel_actions();
}

void GameOverState::on_pause( nom::void_ptr data )
{
  // Hide the cursor so that it doesn't show up during undesirable states such
  // as during the ConfirmationDialogState or Pause states.
  this->cursor_.set_frame(INTERFACE_CURSOR_HIDDEN);
}

void GameOverState::on_resume( nom::void_ptr data )
{
  // Restore the rendering of the player's cursor
  this->cursor_.set_frame(INTERFACE_CURSOR_SHOWN);

  nom::int32_ptr response = static_cast<nom::int32_ptr> (data);

  // "Yes" to verification of choice; we expect this response to be coming from
  // ConfirmationDialogState.
  if( response != nullptr && *response == 1 && this->game->state()->current_state_id() == Game::State::GameOver )
  {
    nom::Event ev;
    ev.type = nom::Event::USER_EVENT;
    ev.timestamp = nom::ticks();
    ev.user.code = GameEvent::AnimationEvent;
    ev.user.data1 = nullptr;
    ev.user.data2 = nullptr;
    ev.user.window_id = 0;
    this->game->evt_handler_.push_event(ev);
  }

  this->game->input_mapper.activate_only( "GameOverState" );
  this->game->input_mapper.activate( "Game" );
}

bool GameOverState::on_event(const nom::Event& ev)
{
  if( ev.type == nom::Event::USER_EVENT ) {
    this->on_user_event(ev);
    return true;
  }

  return false;
}

void GameOverState::on_mouse_button_down( const nom::Event& ev )
{
  nom::Point2i coords ( ev.mouse.x, ev.mouse.y ); // mouse input coordinates

  // Player hand selection checks
  for ( nom::int32 idx = 0; idx < this->game->hand[PlayerIndex::PLAYER_2].size(); idx++ )
  {
    IntRect player2_card_pos  ( PLAYER2_GAMEOVER_ORIGIN_X,
                                PLAYER2_GAMEOVER_ORIGIN_Y,
                                ( CARD_WIDTH ) * ( idx + 1 ),
                                CARD_WIDTH
                              );

    if( player2_card_pos.contains(coords) ) {

      // 1. Update cursor position
      // 2. Update player's selected card
      // 3. Update the card info message box
      // 4. Play sound event

      this->game->hand[PlayerIndex::PLAYER_2].set_position(idx);
      this->cursor_.set_position( Point2i(PLAYER2_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * idx, this->cursor_.position().y ) );

      Card selected_card = this->game->hand[PlayerIndex::PLAYER_2].cards[idx];
      this->game->card_info_box_.set_message_text( selected_card.name );

      this->game->cursor_move->Play();
      // We must break the loop here upon the end of a matching coords check
      // in order to prevent a nasty "last card stays permanently selected"
      // bug from cropping back up!
      break;
    }
  }
}

void GameOverState::on_user_event(const nom::Event& ev)
{
  Card selected_card =
    this->game->hand[PlayerIndex::PLAYER_2].getSelectedCard();

  if( ev.user.code == GameEvent::GUIEvent ) {
    NOM_LOG_DEBUG( TTCARDS_LOG_CATEGORY_EVENTS, "GameEvent::GUIEvent" );

    this->game->card_info_box_.set_message_text(selected_card.name);

    this->game->cursor_move->Play();
  } else if( ev.user.code == GameEvent::AnimationEvent ) {
    NOM_LOG_DEBUG( TTCARDS_LOG_CATEGORY_EVENTS, "GameEvent::AnimationEvent" );

    // NOTE: This should not be disabled until the total number of won cards
    // has been reached.
    this->game->card_info_box_.disable();

    nom::size_type card_pos =
      this->game->hand[PlayerIndex::PLAYER_2].position();

    auto player_id =
      this->game->hand[PlayerIndex::PLAYER_2].cards[card_pos].player_id;

    if( player_id != PlayerID::PLAYER_ID_1 ) {

      Card& pcard = this->game->hand[PlayerIndex::PLAYER_2].cards[card_pos];

      this->game->hand[PlayerIndex::PLAYER_2].cards[card_pos].player_id =
        PlayerID::PLAYER_ID_1;

      // ...Render a new card background based on the owner flip...

      auto old_renderer = pcard.card_renderer;
      NOM_ASSERT(old_renderer != nullptr);
      NOM_ASSERT(old_renderer->valid() == true);

      auto old_renderer_pos = old_renderer->position();

      auto new_renderer =
        tt::create_card_renderer(this->game->card_res_.get(), pcard);
      NOM_ASSERT(new_renderer != nullptr);
      NOM_ASSERT(new_renderer->valid() == true);

      pcard.card_renderer.reset(new_renderer);
      new_renderer->set_position(old_renderer_pos);

      Point2i card_pos;
      if( pcard.card_renderer != nullptr ) {
        card_pos = new_renderer->position();
      }

      auto flip_action =
        this->game->create_flip_card_action(this->flash_action_sprite_, card_pos);

      this->game->actions_.run_action(flip_action, [=]() {

        auto& sp = this->scale_action_sprite_;

        this->scale_action_sprite_ = new_renderer->rendered_card();
        this->scale_action_sprite_->set_position( new_renderer->position() );

        auto scale_action =
          this->create_scale_card_action(sp);

        this->game->actions_.run_action(scale_action, [=]() {
          Point2i delta;

          if( sp != nullptr ) {
            delta.x = 0;
            delta.y = GAME_RESOLUTION.h + sp->size().h;
          }

          this->game->actions_.cancel_action("move_card_action");

          auto move_card_action =
            nom::create_action<MoveByAction>(sp, delta, 0.5f);
          if( move_card_action != nullptr ) {
            move_card_action->set_name("move_card_action");
          }

          std::stringstream won_text_str;
          won_text_str << this->selected_card.name << " card acquired";
          this->game->info_box_.set_message_text( won_text_str.str() );

          this->game->actions_.run_action(move_card_action);
        });
      });

      this->game->card_flip->Play();
    } else {
      //this->game->info_box_.diable();
      this->game->cursor_wrong->Play();
    }

  }
}

void GameOverState::on_update( float delta_time )
{
  this->game->gui_window_.update();

  if ( this->transistion.ticks() > 1500 )
  {
    this->transistion.stop();
    this->show_results = true;
  }

  this->game->window.update();
}

void GameOverState::on_draw(nom::RenderWindow& target)
{
  this->game->gameover_background.draw(target);

  // Render top view
  auto p2_hand_idx = 0;
  Point2i p2_pos(Point2i::zero);
  for(  auto itr = this->game->hand[PlayerIndex::PLAYER_2].begin();
        itr != this->game->hand[PlayerIndex::PLAYER_2].end();
        ++itr )
  {
     p2_pos.x = PLAYER2_GAMEOVER_ORIGIN_X + (CARD_WIDTH * p2_hand_idx);
     p2_pos.y = PLAYER2_GAMEOVER_ORIGIN_Y;
     ++p2_hand_idx;

    auto card_renderer =
      itr->card_renderer;
    if( card_renderer != nullptr && card_renderer->valid() == true ) {
      card_renderer->render(target);
    }
  }

  // Render bottom view
  auto p1_hand_idx = 0;
  Point2i p1_pos(Point2i::zero);
  for(  auto itr = this->game->hand[PlayerIndex::PLAYER_1].begin();
        itr != this->game->hand[PlayerIndex::PLAYER_1].end();
        ++itr )
  {
     p1_pos.x = PLAYER1_GAMEOVER_ORIGIN_X + (CARD_WIDTH * p1_hand_idx);
     p1_pos.y = PLAYER1_GAMEOVER_ORIGIN_Y;
     ++p1_hand_idx;

    auto card_renderer =
      itr->card_renderer;
    if( card_renderer != nullptr && card_renderer->valid() == true ) {
      card_renderer->render(target);
    }
  }

  TT_RENDER_ACTION(this->flash_action_sprite_, "flip_card_action");
  TT_RENDER_ACTION(this->scale_action_sprite_, "scale_card_action");
  TT_RENDER_ACTION(this->scale_action_sprite_, "move_card_action");

  this->game->gui_window_.draw();

  this->cursor_.draw(target);
  if ( this->show_results == true )
  {
    //Card lost_card = this->game->hand[PlayerIndex::PLAYER_1].getSelectedCard();
//NOM_LOG_INFO ( TTCARDS, lost_card.getName() + "Card lost" );

    //nom::sleep ( 1000 );

    // Restart game
    //this->game->set_state ( Game::State::CardsMenu );
    this->transistion.stop();
    this->show_results = true;
  } // end if update
}

std::shared_ptr<nom::IActionObject> GameOverState::
create_scale_card_action(const std::shared_ptr<nom::Sprite>& sp)
{
  Size2f scale_factor(1.0f, 1.0f);
  Point2i delta(Point2i::zero);
  const real32 SCALE_CARD_DURATION = 1.0f;

  this->game->actions_.cancel_action("scale_card_action");

  scale_factor.w = 2.0f;
  scale_factor.h = 2.0f;

  auto scale_card_action0 =
    nom::create_action<ScaleByAction>(sp, scale_factor, SCALE_CARD_DURATION);

  if( sp != nullptr ) {

    if( sp->position().x >= (GAME_RESOLUTION.w / 2) ) {
      delta.x = -(GAME_RESOLUTION.w - sp->size().w) / 2;
    } else {
      delta.x = (GAME_RESOLUTION.w - sp->size().w) / 2;
    }

    delta.y = (GAME_RESOLUTION.h - sp->size().h) / 2;
  }

  auto scale_card_action1 =
    nom::create_action<MoveByAction>(sp, delta, SCALE_CARD_DURATION);

  nom::action_list group_list = { scale_card_action0, scale_card_action1 };

  auto scale_card_action =
    nom::create_action<GroupAction>(group_list);
  if( scale_card_action != nullptr ) {
    scale_card_action->set_name("scale_card_action");
  }

  return scale_card_action;
}

} // namespace tt
