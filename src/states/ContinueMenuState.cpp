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

using namespace nom;

ContinueMenuState::ContinueMenuState  ( const nom::SDLApp::shared_ptr& object ) :
  nom::IState{ Game::State::ContinueMenu, nom::IState::Flags::BackRender, nom::IState::Type::Child },
  game { NOM_DYN_SHARED_PTR_CAST( Game, object) },
  question_box_window{ nullptr },
  question_box{ nullptr }
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

ContinueMenuState::~ContinueMenuState()
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  NOM_DELETE_PTR( this->question_box_window );
}

void ContinueMenuState::on_init( nom::void_ptr data )
{
  Point2i question_box_origin = Point2i( OPTION_BOX_ORIGIN_X, OPTION_BOX_ORIGIN_Y );
  Size2i question_box_size = Size2i( OPTION_BOX_WIDTH, OPTION_BOX_HEIGHT );

  // This widget's coordinates will be relative to the top-level widget
  this->question_box_window = new nom::UIWidget( this->game->gui_window_ );

  this->question_box = new nom::QuestionDialogBox (
                                                    this->question_box_window,
                                                    -1,
                                                    question_box_origin,
                                                    question_box_size
                                                  );

  this->question_box->set_decorator( new nom::FinalFantasyDecorator() );

  // The font used for choice text labels
  this->question_box->set_font( this->game->info_text );

  this->question_box->set_title( "CHOICE", this->game->info_small_text, nom::DEFAULT_FONT_SIZE );

  // We need to use the height of the text font later for cursor calculations.
  this->question_box->set_message( "Are you sure?", this->game->info_text, nom::DEFAULT_FONT_SIZE );
  this->question_box->set_message_alignment( nom::Anchor::TopCenter );

  std::string yes_label( "Yes" );
  std::string no_label( "No " );

  this->question_box->append_choice( yes_label );
  this->question_box->append_choice( no_label );
  this->question_box->set_selection( 1 ); // Match the starting position of
                                          // the cursor

  NOM_CONNECT_UIWIDGET_EVENT( this->question_box, nom::UIEvent::MOUSE_DOWN, this->on_gui_mouse_down );
  NOM_CONNECT_UIWIDGET_EVENT( this->question_box, nom::UIEvent::MOUSE_DCLICK, this->on_gui_mouse_dclick );
  NOM_CONNECT_UIWIDGET_EVENT( this->question_box, nom::UIEvent::MOUSE_WHEEL, this->on_gui_mouse_wheel );
  NOM_CONNECT_UIWIDGET_EVENT( this->question_box, nom::UIEvent::KEY_DOWN, this->on_gui_key_down );

  // Initialize interface cursor
  this->cursor = ContinueMenuStateCursor ( "images/cursors.json" );

  if ( this->cursor.load( this->game->config.getString("INTERFACE_CURSOR"), false, nom::Texture::Access::Streaming ) == false )
  {
    // EPIC FAIL
    nom::DialogMessageBox ( "Critical Error", "Could not load resource file: " + this->game->config.getString("INTERFACE_CURSOR") );
    exit ( NOM_EXIT_FAILURE );
  }

  if ( this->game->config.getString("SCALE_ALGORITHM") == "scale2x" )
  {
    this->cursor.resize( nom::Texture::ResizeAlgorithm::scale2x );
  }
  else if ( this->game->config.getString("SCALE_ALGORITHM") == "hqx" )
  {
    this->cursor.resize( nom::Texture::ResizeAlgorithm::hq2x );
  }

  this->position_map = Point2i  (
                                  OPTION_BOX_ORIGIN_Y + ( OPTION_BOX_HEIGHT / 2 ),
                                  OPTION_BOX_ORIGIN_Y + ( OPTION_BOX_HEIGHT / 2 )
                                  +
                                  ( this->question_box->message_bounds().h / 2 )
                                );

  this->cursor.set_position_map ( position_map );

  this->cursor.set_size ( Size2i ( CURSOR_WIDTH, CURSOR_HEIGHT ) );

  this->cursor.set_position ( Point2i (
                              ( OPTION_BOX_ORIGIN_X ) - CURSOR_WIDTH,
                              ( OPTION_BOX_ORIGIN_Y ) + ( OPTION_BOX_HEIGHT / 2 )
                                      )
                            );

  this->cursor.set_frame ( INTERFACE_CURSOR_RIGHT );

  // Match starting position index of question box selection
  this->cursor.move_down();

  this->question_box_window->insert_child( this->question_box );

  nom::InputActionMapper state;

  // FIXME:
  // nom::InputActionMapper key_bindings, gamepad_bindings;

  nom::EventCallback pause_game( [&] ( const nom::Event& evt ) { this->game->set_state( Game::State::Pause ); } );
  nom::EventCallback move_cursor_up( [&] ( const nom::Event& evt ) { this->cursor.move_up(); } );
  nom::EventCallback move_cursor_down( [&] ( const nom::Event& evt ) { this->cursor.move_down(); } );
  nom::EventCallback select_choice( [&] ( const nom::Event& ev )
    {
      this->send_response();
    }
  );

  nom::EventCallback cancel_choice( [&] ( const nom::Event& evt )
    {
      this->game->state()->pop_state( nullptr );
    }
  );

  // Keyboard mappings
  state.insert( "pause_game", nom::KeyboardAction( SDL_KEYDOWN, SDLK_p ), pause_game );

  // Joystick button mappings
  state.insert( "pause_game", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::START ), pause_game );
  state.insert( "move_cursor_up", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::UP ), move_cursor_up );
  state.insert( "move_cursor_down", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::DOWN ), move_cursor_down );
  state.insert( "select_choice", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::CROSS ), select_choice );
  state.insert( "cancel_choice", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::CIRCLE ), cancel_choice );

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
}

void ContinueMenuState::on_resume( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->game->input_mapper.activate_only( "ContinueMenuState" );
  this->game->input_mapper.activate( "Game" );
}

void ContinueMenuState::on_user_event( const nom::Event& ev )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_EVENTS );

  // Nothing to do; not the right event type for us!
  if ( ev.type != SDL_USEREVENT )
  {
    return;
  }

  if ( ev.user.code == GameEvent::AudioEvent )
  {
    this->game->cursor_move->Play();
  }
}

void ContinueMenuState::on_gui_key_down( const UIWidgetEvent& ev )
{
  nom::Event evt = ev.event();

  if( evt.type != SDL_KEYDOWN ) return;

  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_INPUT, ev.index() );
  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_INPUT, ev.text() );

  switch( ev.index() )
  {
    default:
    {
      // Do nothing
      break;
    }

    case 0:
    {
      this->cursor.move_up();
      break;
    }

    case 1:
    {
      this->cursor.move_down();
      break;
    }
  }

  if( evt.type == SDL_KEYDOWN && evt.key.sym == SDLK_SPACE )
  {
    this->send_response();
  }
}

void ContinueMenuState::on_gui_mouse_down( const nom::UIWidgetEvent& ev )
{
  // nom::Event evt = ev.event();

  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_EVENTS );
  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_INPUT, ev.index() );
  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_INPUT, ev.text() );

   // Obtain the option label text chosen by index.
  switch( ev.index() )
  {
    default:
    {
      // Do nothing
      break;
    }

    case 0:
    {
      this->cursor.move_up();
      break;
    }

    case 1:
    {
      this->cursor.move_down();
      break;
    }
  }
}

void ContinueMenuState::on_gui_mouse_dclick( const nom::UIWidgetEvent& ev )
{
  // nom::Event evt = ev.event();

  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_EVENTS );
  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_INPUT, ev.index() );
  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_INPUT, ev.text() );

  // Obtain the option label text chosen by index.
  switch( ev.index() )
  {
    default:
    {
      // Do nothing
      break;
    }

    case 0:
    {
      this->send_response();
      break;
    }

    case 1:
    {
      this->send_response();
      break;
    }
  }
}

void ContinueMenuState::on_gui_mouse_wheel( const nom::UIWidgetEvent& ev )
{
  nom::Event evt = ev.event();

  // Do not check mouse wheel state unless it is a valid event; we receive
  // invalid data here if we do not check for this.
  if( evt.type != SDL_MOUSEWHEEL ) return;

  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_INPUT, ev.index() );
  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_INPUT, ev.text() );

   // Obtain the option label text chosen by index.
  switch( ev.index() )
  {
    default:
    {
      // Do nothing
      break;
    }

    case 0:
    {
      this->cursor.move_up();
      break;
    }

    case 1:
    {
      this->cursor.move_down();
      break;
    }
  }
}

bool ContinueMenuState::on_event( const nom::Event& ev )
{
  assert( this->question_box_window != nullptr );

  if( this->question_box_window != nullptr )
  {
    return this->question_box_window->process_event( ev );
  }

  return false;
}

void ContinueMenuState::on_update( float delta_time )
{
  this->question_box_window->update();

  this->cursor.update();

  this->game->window.update();
}

void ContinueMenuState::on_draw( nom::RenderWindow& target )
{
  this->question_box_window->draw( target );

  this->cursor.draw ( target );
}

void ContinueMenuState::send_response( void )
{
  // We will use the positioning of the cursor to map user's response;
  //
  // Position zero (0) will generate a "Yes" response.
  // Position one (1) will generate a "No" response.
  //
  // We pass the response along to whomever called us (this state) as we
  // exit stage right.
  nom::int32 choice = this->cursor.cursor_position();

  if ( choice == 0 )
  {
    nom::int32_ptr response = new nom::int32( 1 );
    this->game->state()->pop_state( response );
  }
  else if ( choice == 1 )
  {
    nom::int32_ptr response = nullptr;
    this->game->state()->pop_state( response );
  }
  else
  {
    // This should never be heard!
    this->game->cursor_wrong->Play();
  }
}
