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
#include "ContinueMenuState.hpp"

// Forward declarations
#include "Game.hpp"

using namespace nom;

ContinueMenuState::ContinueMenuState  ( const nom::SDLApp::shared_ptr& object ) :
  nom::IState( Game::State::ContinueMenu, nom::IState::Flags::BackRender, nom::IState::Type::Child ),
  game( NOM_DYN_SHARED_PTR_CAST( Game, object) )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

ContinueMenuState::~ContinueMenuState()
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

void ContinueMenuState::on_init( nom::void_ptr data )
{
  if( this->game->question_box_.set_context(&this->game->gui_window_) == false )
  {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could set GUI desktop." );
    // return false;
  }

  if( this->game->question_box_.load_document_file( this->game->config.getString("GUI_QBOX") ) == false )
  {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could not load file:",
                  this->game->config.getString("GUI_QBOX") );
    // return false;
  }

  this->game->question_box_.set_title_text("CHOICE");

  this->game->question_box_.show();

  // Initialize interface cursor
  this->cursor_ = ContinueMenuStateCursor("images/cursors.json");

  if( this->cursor_.load( this->game->config.getString("INTERFACE_CURSOR"), false, nom::Texture::Access::Streaming ) == false )
  {
    // EPIC FAIL
    nom::DialogMessageBox ( "Critical Error", "Could not load resource file: " + this->game->config.getString("INTERFACE_CURSOR") );
    exit ( NOM_EXIT_FAILURE );
  }

  if ( this->game->config.getString("SCALE_ALGORITHM") == "scale2x" )
  {
    this->cursor_.resize( nom::Texture::ResizeAlgorithm::scale2x );
  }
  else if ( this->game->config.getString("SCALE_ALGORITHM") == "hqx" )
  {
    this->cursor_.resize( nom::Texture::ResizeAlgorithm::hq2x );
  }

  this->cursor_.set_frame(INTERFACE_CURSOR_RIGHT);

  // Build offset coordinate map for the game cursor; this is necessary for
  // syncing key, mouse wheel and joystick input.
  Point2i cursor_offset;
  Rocket::Core::ElementList tags;
  Rocket::Core::Element* top =
    this->game->question_box_.document()->GetElementById("content");

  NOM_ASSERT(top != nullptr);
  if( top != nullptr ) {
    Rocket::Core::ElementUtilities::GetElementsByTagName(tags, top, "answer");

    // NOM_DUMP( tags.size() );
    Rocket::Core::Vector2f position;
    Rocket::Core::Vector2f size;
    IntRect row;
    std::vector<IntRect> cursor_map;
    for(auto itr = tags.begin(); itr != tags.end(); ++itr)
    {
      // NOM_DUMP( (*itr)->GetId().CString() );

      position = (*itr)->GetAbsoluteOffset(Rocket::Core::Box::PADDING);
      size = (*itr)->GetBox().GetSize(Rocket::Core::Box::PADDING);

      row.x = position.x - this->cursor_.size().w;
      row.y = position.y + (size.y / 2);
      row.w = size.x;
      row.h = size.y;
      cursor_map.push_back(row);

      NOM_DUMP_VAR(TTCARDS_LOG_CATEGORY_TEST, "position_map:", row);
    }

    this->cursor_.set_position_map(cursor_map);

    // Starting origin for game cursor
    this->cursor_.set_position( cursor_map[0].position() );
  }

  // Starting game cursor offset is on the 'no' element
  this->cursor_.set_cursor_position(1);

  nom::InputActionMapper state;

  // FIXME:
  // nom::InputActionMapper key_bindings, gamepad_bindings;

  nom::EventCallback pause_game( [&] (const nom::Event& evt) {
    this->game->set_state(Game::State::Pause);
  });

  nom::EventCallback cursor_prev( [&] (const nom::Event& evt) {
    if( this->cursor_.prev() ) {
      this->game->cursor_move->Play();
    }
  });

  nom::EventCallback cursor_next( [&] (const nom::Event& evt) {
    if( this->cursor_.next() ) {
      this->game->cursor_move->Play();
    }
  });

  nom::EventCallback select( [&] (const nom::Event& evt) {
    this->send_response();
  });

  nom::EventCallback cancel( [&] (const nom::Event& evt) {
    this->game->state()->pop_state(nullptr);
  });

  // Equivalent to 'cursor_prev' and 'cursor_next' actions
  nom::EventCallback mouse_click( [&] (const nom::Event& evt) {
    this->on_mouse_button_up(evt);
  });

  // Equivalent to 'select' action
  nom::EventCallback mouse_select( [&] (const nom::Event& evt) {
    this->on_mouse_button_dblclick(evt);
  });

  // Keyboard mappings
  state.insert( "pause_game", nom::KeyboardAction( SDL_KEYDOWN, SDLK_p ), pause_game );
  state.insert( "cursor_prev", nom::KeyboardAction( SDL_KEYDOWN, SDLK_UP ), cursor_prev );
  state.insert( "cursor_next", nom::KeyboardAction( SDL_KEYDOWN, SDLK_DOWN ), cursor_next );
  state.insert( "select", nom::KeyboardAction( SDL_KEYDOWN, SDLK_SPACE ), select );
  state.insert( "cancel", nom::KeyboardAction( SDL_KEYDOWN, SDLK_ESCAPE ), cancel );

  // Mouse button mappings
  state.insert( "click", nom::MouseButtonAction( SDL_MOUSEBUTTONUP, SDL_BUTTON_LEFT ), mouse_click );
  state.insert( "select", nom::MouseButtonAction( SDL_MOUSEBUTTONUP, SDL_BUTTON_LEFT, 2 ), mouse_select );

  // Mouse wheel mappings
  state.insert( "cursor_prev", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_Y, nom::MouseWheelAction::UP ), cursor_prev );
  state.insert( "cursor_next", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_Y, nom::MouseWheelAction::DOWN ), cursor_next );

  // Joystick button mappings
  state.insert( "pause_game", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::START ), pause_game );
  state.insert( "cursor_prev", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::UP ), cursor_prev );
  state.insert( "cursor_next", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::DOWN ), cursor_next );
  state.insert( "select", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::CROSS ), select );
  state.insert( "cancel", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::CIRCLE ), cancel );

  this->game->input_mapper.erase( "ContinueMenuState" );
  this->game->input_mapper.insert( "ContinueMenuState", state, true );

  // FIXME:
  // this->game->input_mapper.insert( "ContinueMenuState", key_bindings, true );
  // this->game->input_mapper.insert( "ContinueMenuState", gamepad_bindings, true );

  this->game->input_mapper.activate_only( "ContinueMenuState" );
  this->game->input_mapper.activate( "Game" );
}

void ContinueMenuState::on_exit( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->game->question_box_.close();
  Rocket::Core::Factory::ClearStyleSheetCache();
  Rocket::Core::Factory::ClearTemplateCache();
}

void ContinueMenuState::on_resume( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->game->input_mapper.activate_only( "ContinueMenuState" );
  this->game->input_mapper.activate( "Game" );
}

// void ContinueMenuState::on_user_event(const nom::Event& ev)
// {
//   NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_EVENTS );

//   // Nothing to do; not the right event type for us!
//   if( ev.type != SDL_USEREVENT )
//   {
//     return;
//   }

//   if( ev.user.code == GameEvent::AudioEvent )
//   {
//     this->game->cursor_move->Play();
//   }
// }

void ContinueMenuState::on_mouse_button_up(const nom::Event& ev)
{
  using namespace Rocket::Core;

  Element* yes_response =
    this->game->question_box_.document()->GetElementById("yes");
  Element* no_response =
    this->game->question_box_.document()->GetElementById("no");

  NOM_ASSERT( yes_response != nullptr );
  NOM_ASSERT( no_response != nullptr );

  if( yes_response == nullptr || no_response == nullptr ) return;

  Vector2f mouse_coords(ev.mouse.x, ev.mouse.y);

  if( yes_response->IsPointWithinElement(mouse_coords) ) {
    if( this->cursor_.prev() ) {
      this->game->cursor_move->Play();
    }
  }
  else if( no_response->IsPointWithinElement(mouse_coords) ) {
    if( this->cursor_.next() ) {
      this->game->cursor_move->Play();
    }
  }
}

void ContinueMenuState::on_mouse_button_dblclick(const nom::Event& ev)
{
  using namespace Rocket::Core;

  Element* yes_response =
    this->game->question_box_.document()->GetElementById("yes");
  Element* no_response =
    this->game->question_box_.document()->GetElementById("no");

  NOM_ASSERT( yes_response != nullptr );
  NOM_ASSERT( no_response != nullptr );

  if( yes_response == nullptr || no_response == nullptr ) return;

  Vector2f mouse_coords(ev.mouse.x, ev.mouse.y);

  if( yes_response->IsPointWithinElement(mouse_coords) ) {
    this->send_response();
    return;
  }

  if( no_response->IsPointWithinElement(mouse_coords) ) {
    this->send_response();
    return;
  }
}

void ContinueMenuState::on_update( float delta_time )
{
  this->game->gui_window_.update();

  this->cursor_.update();

  this->game->window.update();
}

void ContinueMenuState::on_draw( nom::RenderWindow& target )
{
  this->game->gui_window_.draw();

  this->cursor_.draw(target);
}

void ContinueMenuState::send_response()
{
  // We will use the positioning of the cursor to map user's response;
  //
  // Position zero (0) will generate a "Yes" response.
  // Position one (1) will generate a "No" response.
  //
  // We pass the response along to whomever called us (this state) as we
  // exit stage right.
  int choice = this->cursor_.cursor_position();

  if( choice == 0 )
  {
    nom::int32_ptr response = new nom::int32(1);
    this->game->state()->pop_state(response);
  }
  else if( choice == 1 )
  {
    nom::int32_ptr response = nullptr;
    this->game->state()->pop_state(response);
  }
  else
  {
    // This should never be heard!
    this->game->cursor_wrong->Play();
  }
}
