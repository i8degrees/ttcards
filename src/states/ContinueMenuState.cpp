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
  nom::IState { Game::State::ContinueMenu, nom::IState::StateFlags::BackRender },
  game { NOM_PTR_CAST( Game, object) }
{
  NOM_LOG_TRACE( TTCARDS );
}

ContinueMenuState::~ContinueMenuState ( void )
{
  NOM_LOG_TRACE( TTCARDS );
}

void ContinueMenuState::on_init ( nom::void_ptr data )
{
  // Our gradient-filled background colors; from starting color to ending color.
  nom::Gradient::Colors g_colors = { nom::Color4i::Gray, nom::Color4i::LightGray };

  nom::Window* window = nullptr;

  Point2i question_box_origin = Point2i( OPTION_BOX_ORIGIN_X, OPTION_BOX_ORIGIN_Y );
  Size2i question_box_size = Size2i( OPTION_BOX_WIDTH, OPTION_BOX_HEIGHT );

  window = new nom::Window( question_box_origin, question_box_size );
  window->set_shape( new nom::Gradient( g_colors, question_box_origin, question_box_size, nom::Point2i( 0,0 ), nom::Gradient::FillDirection::Left ) );
  window->set_shape( new nom::GrayFrame( question_box_origin, question_box_size ) );

  this->question_box = nom::QuestionDialogBox::unique_ptr (
                                                            new nom::QuestionDialogBox(
                                                              window,
                                                              question_box_origin,
                                                              question_box_size
                                                          )
                                                      );

  this->question_box->set_title_label( nom::Text("CHOICE", &this->game->info_small_text, -1, nom::Text::Alignment::TopLeft) );

  // We need to use the height of the text font later for cursor calculations.
  Text option_text = nom::Text( "Are you sure?", &this->game->info_text, -1, nom::Text::Alignment::TopCenter );
  this->question_box->set_message_label( option_text );

  nom::Text yes_label( nom::Text( "Yes", &this->game->info_text, -1, nom::Text::Alignment::MiddleCenter ) );

  // Appending a single space character onto "No" text label so that it lines up
  // with our "Yes" text label.
  nom::Text no_label( nom::Text( "No ", &this->game->info_text, -1, nom::Text::Alignment::MiddleCenter ) );

  this->question_box->append_choice( yes_label );
  this->question_box->append_choice( no_label );
  this->question_box->set_selection( 1 );

  this->question_box->register_event_listener( nom::UIEventType::MOUSE_SELECTION, nom::UIEventCallback( [&] ( nom::UIEvent& ev ) { this->on_mouse_event( ev ); } ) );
  this->question_box->register_event_listener( nom::UIEventType::MOUSE_DOWN, nom::UIEventCallback( [&] ( nom::UIEvent& ev ) { this->on_mouse_event( ev ); } ) );
  this->question_box->register_event_listener( nom::UIEventType::MOUSE_DCLICK, nom::UIEventCallback( [&] ( nom::UIEvent& ev ) { this->on_mouse_dclick( ev ); } ) );
  this->question_box->register_event_listener( nom::UIEventType::MOUSE_WHEEL, nom::UIEventCallback( [&] ( nom::UIEvent& ev ) { this->on_wheel( ev ); } ) );
  this->question_box->register_event_listener( nom::UIEventType::KEY_SELECTION, nom::UIEventCallback( [&] ( nom::UIEvent& ev ) { this->on_key_event( ev ); } ) );

  // Initialize interface cursor
  this->cursor = ContinueMenuStateCursor ( "images/cursors.json" );

  if ( this->cursor.load ( this->game->config.getString("INTERFACE_CURSOR"), 0 ) == false )
  {
    // EPIC FAIL
    nom::DialogMessageBox ( "Critical Error", "Could not load resource file: " + this->game->config.getString("INTERFACE_CURSOR") );
    exit ( NOM_EXIT_FAILURE );
  }

  if ( this->game->config.getString("SCALE_ALGORITHM") == "scale2x" )
  {
    //this->cursor.resize ( nom::Texture::ResizeAlgorithm::scale2x );
  }
  else if ( this->game->config.getString("SCALE_ALGORITHM") == "hqx" )
  {
    //this->cursor.resize ( nom::Texture::ResizeAlgorithm::hq2x );
  }

  this->position_map = Point2i  (
                                  OPTION_BOX_ORIGIN_Y + ( OPTION_BOX_HEIGHT / 2 ),
                                  OPTION_BOX_ORIGIN_Y + ( OPTION_BOX_HEIGHT / 2 )
                                  +
                                  ( option_text.height() / 2 )
                                );

  this->cursor.set_position_map ( position_map );

  this->cursor.set_size ( Size2i ( CURSOR_WIDTH, CURSOR_HEIGHT ) );

  this->cursor.set_position ( Point2i (
                              ( OPTION_BOX_ORIGIN_X ) - CURSOR_WIDTH,
                              ( OPTION_BOX_ORIGIN_Y ) + ( OPTION_BOX_HEIGHT / 2 )
                                      )
                            );

  this->cursor.set_frame ( INTERFACE_CURSOR_RIGHT );

  // FIXME:
  // delete window;
}

void ContinueMenuState::on_exit ( nom::void_ptr data )
{
  // Stub
}

void ContinueMenuState::on_resume ( nom::void_ptr data )
{
  // Stub
}

void ContinueMenuState::on_key_down( const nom::Event& ev )
{
  switch ( ev.key.sym )
  {
    default:
    {
      // Ignore non-mapped keys
      break;
    }

    // Pause game
    case SDLK_p:
    {
      this->game->set_state( Game::State::Pause );
      break;
    }

    // case SDLK_UP: this->cursor.move_up(); break;
    // case SDLK_DOWN: this->cursor.move_down(); break;
    case SDLK_SPACE:
    {
      this->send_response();
      break;
    }
  } // end switch ( key )
}

void ContinueMenuState::on_mouse_wheel( const nom::Event& ev )
{
  // Do not check mouse wheel state unless it is a valid event; we receive
  // invalid data here if we do not check for this.
  // if( ev.type != SDL_MOUSEWHEEL ) return;

  // if ( ev.wheel.y > 0 )
  // {
  //   this->cursor.move_up();
  // }
  // else if (ev.wheel.y < 0 )
  // {
  //   this->cursor.move_down();
  // }
}

void ContinueMenuState::on_joy_button_down( const nom::Event& ev )
{
  switch ( ev.jbutton.button )
  {
    default: NOM_LOG_INFO ( TTCARDS, "FIXME: ContinueMenuState needs joystick implementation!" ); break;
  } // switch
}

void ContinueMenuState::on_user_event( const nom::Event& ev )
{
  NOM_LOG_TRACE( NOM );

  // Nothing to do; not the right event type for us!
  if ( ev.type != SDL_USEREVENT )
  {
    return;
  }

  if ( ev.user.code == GameEvent::AudioEvent )
  {
    this->game->cursor_move.Play();
  }
}

void ContinueMenuState::on_key_event( const nom::UIEvent& ev )
{
  nom::Event event = ev.event();

  // if( event.type != SDL_KEYDOWN ) return;

  switch( event.key.sym )
  {
    default: break;

    case SDLK_UP: this->cursor.move_up(); break;
    case SDLK_DOWN: this->cursor.move_down(); break;

    case SDLK_SPACE:
    {
      // this->send_response();
      break;
    }
  } // end switch ( key )
}

void ContinueMenuState::on_mouse_event( const nom::UIEvent& ev )
{
  NOM_LOG_TRACE( NOM );
  NOM_DUMP( ev.index() );
  NOM_DUMP( ev.text() );

   // Obtain the option label text chosen by index.
  switch( ev.index() )
  {
    default: /* Do nothing */ break;

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

void ContinueMenuState::on_mouse_dclick( const nom::UIEvent& ev )
{
  NOM_LOG_TRACE( NOM );
  NOM_DUMP( ev.index() );
  NOM_DUMP( ev.text() );

   // Obtain the option label text chosen by index.
  switch( ev.index() )
  {
    default: /* Do nothing */ break;

    case 0:
    case 1:
    {
      this->send_response();
      break;
    }
  }
}

void ContinueMenuState::on_wheel( const nom::UIEvent& ev )
{
  nom::Event event = ev.event();

  // Do not check mouse wheel state unless it is a valid event; we receive
  // invalid data here if we do not check for this.
  if( event.type != SDL_MOUSEWHEEL ) return;

  int selection = this->question_box->selection();

  if ( event.wheel.y > 0 && selection > 0 ) // Up
  {
    this->cursor.move_up();
  }
  else if (event.wheel.y < 0 && selection < this->question_box->items_size() )
  {
    this->cursor.move_down();
  }
}

void ContinueMenuState::on_event( const nom::Event& ev )
{
  this->question_box->process_event( ev );
}

void ContinueMenuState::on_update ( float delta_time )
{
  this->cursor.update();

  this->game->window.update();
}

void ContinueMenuState::on_draw ( nom::IDrawable::RenderTarget& target )
{
  this->question_box->draw( target );

  this->cursor.draw ( target );
}

void ContinueMenuState::send_response ( void )
{
  // We will use the positioning of the cursor to map user's response;
  //
  // Position zero (0) will generate a "Yes" response.
  // Position one (1) will generate a "No" response.
  //
  // We pass the response along to whomever called us (this state) as we
  // exit stage right.
  nom::int32 choice = this->cursor.position();

  if ( choice == 0 )
  {
    nom::int32_ptr response = new nom::int32 (1);
    this->game->pop_state(response);
  }
  else if ( choice == 1 )
  {
    nom::int32_ptr response = nullptr;
    this->game->pop_state(response);
  }
  else
  {
    // This should never be heard!
    this->game->cursor_wrong.Play();
  }
}
