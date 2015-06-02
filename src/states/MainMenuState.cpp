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
#include "MainMenuState.hpp"

// Private headers
#include "helpers.hpp"

// Forward declarations
#include "Game.hpp"

using namespace nom;

namespace tt {

MainMenuState::MainMenuState(nom::SDLApp* object) :
  nom::IState(Game::State::MainMenu),
  game( NOM_SCAST(Game*, object) )
{
  NOM_LOG_TRACE(TTCARDS_LOG_CATEGORY_TRACE_STATES);
}

MainMenuState::~MainMenuState()
{
  NOM_LOG_TRACE(TTCARDS_LOG_CATEGORY_TRACE_STATES);
}

void MainMenuState::on_init(nom::void_ptr data)
{
  NOM_LOG_TRACE(TTCARDS_LOG_CATEGORY_TRACE_STATES);

  // auto cfg = this->game->config_.get();

  Point2i align_text_pos(Point2i::zero);
  Point2i offset(Point2i::zero);

  // TODO: This menu text should only be shown when there is one or more saved
  // games in the player's local "Saved Games" directory
  this->menu_text_[0] = Text();
  this->menu_text_[0].set_font(&this->game->menu_font_);
  this->menu_text_[0].set_text_size(22);
  this->menu_text_[0].set_text("Continue");

  align_text_pos =
    nom::alignment_rect(  this->menu_text_[0].size(), Point2i::zero,
                          GAME_RESOLUTION, nom::Anchor::MiddleCenter );

  this->menu_text_[0].set_position(align_text_pos);

  //

  this->menu_text_[1] = Text();
  this->menu_text_[1].set_font(&this->game->menu_font_);
  this->menu_text_[1].set_text_size(22);
  this->menu_text_[1].set_text("New Game");

  offset.y = offset.y + this->menu_text_[0].size().h;
  align_text_pos =
    nom::alignment_rect(  this->menu_text_[1].size(), offset,
                          GAME_RESOLUTION, nom::Anchor::MiddleCenter );

  this->menu_text_[1].set_position(align_text_pos);

  //

  this->menu_text_[2] = Text();
  this->menu_text_[2].set_font(&this->game->menu_font_);
  this->menu_text_[2].set_text_size(22);
  this->menu_text_[2].set_text("Load Game");

  offset.y = offset.y + this->menu_text_[1].size().h;
  align_text_pos =
    nom::alignment_rect(  this->menu_text_[2].size(), offset,
                          GAME_RESOLUTION, nom::Anchor::MiddleCenter );

  this->menu_text_[2].set_position(align_text_pos);

  //

  this->menu_text_[3] = Text();
  this->menu_text_[3].set_font(&this->game->menu_font_);
  this->menu_text_[3].set_text_size(22);
  this->menu_text_[3].set_text("View Cards");

  offset.y = offset.y + this->menu_text_[2].size().h;
  align_text_pos =
    nom::alignment_rect(  this->menu_text_[3].size(), offset,
                          GAME_RESOLUTION, nom::Anchor::MiddleCenter );

  this->menu_text_[3].set_position(align_text_pos);

  //

  this->menu_text_[4] = Text();
  this->menu_text_[4].set_font(&this->game->menu_font_);
  this->menu_text_[4].set_text_size(22);
  this->menu_text_[4].set_text("Options");

  offset.y = offset.y + this->menu_text_[3].size().h;
  align_text_pos =
    nom::alignment_rect(  this->menu_text_[4].size(), offset,
                          GAME_RESOLUTION, nom::Anchor::MiddleCenter );

  this->menu_text_[4].set_position(align_text_pos);

  //

  this->menu_text_[5] = Text();
  this->menu_text_[5].set_font(&this->game->menu_font_);
  this->menu_text_[5].set_text_size(22);
  this->menu_text_[5].set_text("Credits");

  offset.y = offset.y + this->menu_text_[4].size().h;
  align_text_pos =
    nom::alignment_rect(  this->menu_text_[5].size(), offset,
                          GAME_RESOLUTION, nom::Anchor::MiddleCenter );

  this->menu_text_[5].set_position(align_text_pos);

  //

  this->menu_text_[6] = Text();
  this->menu_text_[6].set_font(&this->game->menu_font_);
  this->menu_text_[6].set_text_size(22);
  this->menu_text_[6].set_text("Quit");

  offset.y = offset.y + this->menu_text_[5].size().h;
  align_text_pos =
    nom::alignment_rect(  this->menu_text_[6].size(), offset,
                          GAME_RESOLUTION, nom::Anchor::MiddleCenter );

  this->menu_text_[6].set_position(align_text_pos);

  // ...Initialize game cursor...

  IntRect cursor_pos(IntRect::zero);
  for(  auto cursor_index = 0;
        cursor_index != tt::TOTAL_MENU_ENTRIES;
        ++cursor_index )
  {
    Size2i menu_text_dims =
      this->menu_text_[cursor_index].size();

    Point2i align_pos =
      nom::alignment_rect(  this->game->cursor_->size(), Point2i::zero,
                            GAME_RESOLUTION, nom::Anchor::MiddleCenter );

    cursor_pos.w = CURSOR_WIDTH;
    cursor_pos.h = CURSOR_HEIGHT;

    cursor_pos.x = align_pos.x - menu_text_dims.w;

    // Roughly aligned in the middle of the menu text entry
    cursor_pos.y =
      // (align_pos.y + (CARD_HEIGHT / 2) * cursor_index);
      (align_pos.y + (menu_text_dims.h) * cursor_index);

    this->cursor_coords_map_.push_back(cursor_pos);
  }

  this->game->cursor_->set_position( this->cursor_coords_map_[0].position() );
  this->game->cursor_->set_sprite_sheet(this->game->right_cursor_frames_);
  this->game->cursor_->set_frame(INTERFACE_CURSOR_SHOWN);

  nom::InputActionMapper state;

  auto cursor_prev( [=](const nom::Event& evt) {
    this->cursor_prev();
  });

  auto cursor_next( [=](const nom::Event& evt) {
    this->cursor_next();
  });

  auto confirm_selection( [=](const nom::Event& evt) {
    this->on_confirm_selection(evt);
  });

  auto confirm_mouse_cursor_move( [=](const nom::Event& evt) {
    this->on_mouse_button_up(evt);
  });

  // ...Keyboard input mappings...

  state.insert("cursor_prev", nom::KeyboardAction(SDLK_UP), cursor_prev);
  state.insert("cursor_next", nom::KeyboardAction(SDLK_DOWN), cursor_next);
  state.insert( "confirm_selection",
                nom::KeyboardAction(SDLK_RETURN), confirm_selection );

  // ...Mouse wheel input mappings...

  state.insert( "cursor_prev",
                nom::MouseWheelAction(nom::MOUSE_WHEEL_UP), cursor_prev );
  state.insert( "cursor_next",
                nom::MouseWheelAction(nom::MOUSE_WHEEL_DOWN), cursor_next );

  state.insert( "confirm_mouse_cursor_move",
                nom::MouseButtonAction( nom::LEFT_MOUSE_BUTTON,
                nom::InputState::RELEASED ), confirm_mouse_cursor_move );

  // ...Joystick input mappings...
  auto& joystick_id = this->game->joystick_id_;

  state.insert( "cursor_prev",
                nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_DPAD_UP), cursor_prev );
  state.insert( "cursor_next", nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_DPAD_DOWN), cursor_next );
  state.insert( "confirm_selection",
                nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_START), confirm_selection );

  this->game->input_mapper.erase("MainMenuState");
  this->game->input_mapper.insert("MainMenuState", state, true);
  this->game->input_mapper.activate_only("MainMenuState");
  this->game->input_mapper.activate("Game");
}

void MainMenuState::on_exit(nom::void_ptr data)
{
  NOM_LOG_TRACE(TTCARDS_LOG_CATEGORY_TRACE_STATES);

  this->game->actions_.cancel_actions();
}

void MainMenuState::on_pause(nom::void_ptr data)
{
  NOM_LOG_TRACE(TTCARDS_LOG_CATEGORY_TRACE_STATES);

  // Hide the cursor so that it doesn't show up during undesirable states such
  // as during the ConfirmationDialogState or Pause states.
  this->game->cursor_->set_frame(INTERFACE_CURSOR_HIDDEN);
}

void MainMenuState::on_resume(nom::void_ptr data)
{
  NOM_LOG_TRACE(TTCARDS_LOG_CATEGORY_TRACE_STATES);

  // Restore the rendering of the game cursor
  this->game->cursor_->set_frame(INTERFACE_CURSOR_SHOWN);
}

void MainMenuState::on_update(nom::real32 delta_time)
{
  this->update_cursor();

  this->game->window.update();
}

void MainMenuState::on_draw(nom::RenderWindow& target)
{
#if 1
  this->game->window.fill(nom::Color4i::Black);
#else
  this->game->gameover_background.draw(target);
#endif

  for(  auto itr = this->menu_text_.begin();
        itr != this->menu_text_.end();
        ++itr )
  {
    (itr)->second.draw(target);
  }

  TT_RENDER_SPRITE(this->game->cursor_);
}

// Private scope

bool MainMenuState::on_event(const nom::Event& evt)
{
  return false;
}

void MainMenuState::on_mouse_button_up(const nom::Event& evt)
{
  Point2i mouse_input(evt.mouse.x, evt.mouse.y);
  IntRect menu_text_bounds(IntRect::zero);

  for(  auto itr = this->menu_text_.begin();
        itr != this->menu_text_.end();
        ++itr )
  {
    menu_text_bounds.x = (itr)->second.position().x;
    menu_text_bounds.y = (itr)->second.position().y;
    menu_text_bounds.w = (itr)->second.size().w;
    menu_text_bounds.h = (itr)->second.size().h;

    if( menu_text_bounds.contains(mouse_input) ) {

      this->set_cursor_position( (itr)->first );
      this->game->cursor_move->Play();
      this->on_confirm_selection(evt);
      break;
    }
  }
}

void MainMenuState::update_cursor()
{
  this->game->cursor_->set_frame(INTERFACE_CURSOR_SHOWN);
}

int MainMenuState::cursor_position()
{
  int pos = this->cursor_pos_; // The card's position index

  if( pos < TOTAL_MENU_ENTRIES && pos >= 0 ) {
    return pos;
  } else {
    return -1;
  }
}

void MainMenuState::set_cursor_position(int pos)
{
  if( pos < TOTAL_MENU_ENTRIES && pos >= 0 ) {
    auto cursor_pos =
      this->cursor_coords_map_[pos].position();

    this->cursor_pos_ = pos;

    this->game->cursor_->set_position(cursor_pos);
  }
}

void MainMenuState::cursor_prev()
{
  int pos = this->cursor_pos_; // The card's position index
  Point2i move_to_offset(Point2i::zero);

  // Move up if the game cursor is before the first menu entry
  if( this->cursor_pos_ > 0 ) {

    --this->cursor_pos_;
    move_to_offset.x = 0;
    move_to_offset.y = -(this->cursor_coords_map_.at(pos).h);
    this->game->cursor_->translate(move_to_offset);

    this->game->cursor_move->Play();
  }
}

void MainMenuState::cursor_next()
{
  int pos = this->cursor_pos_; // The card's position index
  Point2i move_to_offset(Point2i::zero);

  // Move down if the game cursor is not at the last menu entry
  if( this->cursor_pos_ < (TOTAL_MENU_ENTRIES - 1) ) {

    ++this->cursor_pos_;
    move_to_offset.x = 0;
    move_to_offset.y = this->cursor_coords_map_.at(pos).h;
    this->game->cursor_->translate(move_to_offset);

    this->game->cursor_move->Play();
  }
}

void MainMenuState::on_confirm_selection(const nom::Event& evt)
{
  const real32 FADE_OUT_DURATION = 1.0f;
  int cursor_pos = this->cursor_position();

  switch(cursor_pos)
  {
    case MENU_TEXT_INVALID:
    default:
    {
      NOM_ASSERT(cursor_pos != -1);
    } break;

    case MENU_TEXT_CONTINUE:
    {
      // TODO: This is stub code and needs to be implemented; continuing will
      // pick up from where the player last left off.
      auto fade_transistion( [=]() {
        this->game->load_game->Play();
        this->game->set_state(Game::State::CardsMenu);
      });

      this->game->fade_window_out(  FADE_OUT_DURATION, Color4i::White,
                                    SCREEN_RESOLUTION, fade_transistion );
    } break;

    case MENU_TEXT_NEW_GAME:
    {
      auto fade_transistion( [=]() {
        this->game->load_game->Play();
        this->game->set_state(Game::State::CardsMenu);
      });

      this->game->fade_window_out(  FADE_OUT_DURATION, Color4i::Black,
                                    SCREEN_RESOLUTION, fade_transistion );
    } break;

    case MENU_TEXT_LOAD_GAME:
    {
      // TODO: Implement!
      this->game->cursor_wrong->Play();
    } break;

    case MENU_TEXT_VIEW_CARDS:
    {
      // TODO: Implement!
      this->game->cursor_wrong->Play();
    } break;

    case MENU_TEXT_OPTIONS:
    {
      // TODO: Implement!
      this->game->cursor_wrong->Play();
    } break;

    case MENU_TEXT_CREDITS:
    {
      // TODO: Implement!
      this->game->cursor_wrong->Play();
    } break;

    case MENU_TEXT_QUIT:
    {
      auto completion_func( [=]() {
        this->game->on_game_quit(evt);
      });

      this->game->fade_window_out(  FADE_OUT_DURATION, Color4i::Black,
                                    SCREEN_RESOLUTION );
    } break;
  }
}

} // namespace tt
