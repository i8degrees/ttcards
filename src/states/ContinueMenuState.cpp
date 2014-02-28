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

ContinueMenuState::ContinueMenuState  ( const nom::SDLApp::SharedPtr& object ) :
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
  nom::Gradient linear;
  Text option_text = nom::Text("Are you sure?\n\t\tYes\n\t\tNo", this->game->info_text, 12, nom::Text::Alignment::MiddleCenter);

  linear.set_start_color ( nom::Color4i::Gray );
  linear.set_end_color ( nom::Color4i::LightGray );
  linear.set_fill_direction ( nom::Gradient::FillDirection::Left );

  Point2i info_box_origin = Point2i( OPTION_BOX_ORIGIN_X, OPTION_BOX_ORIGIN_Y );
  Size2i info_box_size = Size2i( OPTION_BOX_WIDTH, OPTION_BOX_HEIGHT );

  this->info_box = nom::MessageBox  ( info_box_origin,
                                      info_box_size,
                                      nom::MessageBox::Style::Gray,
                                      linear
                                    );

  this->info_box.set_title ( nom::Text("CHOICE", this->game->info_small_text, 9, nom::Text::Alignment::TopLeft) );
  this->info_box.set_text ( option_text );

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
    default: /* Ignore non-mapped keys */ break;

    // Pause game
    case SDLK_p:
    {
      this->game->set_state( Game::State::Pause );
      break;
    }

    case SDLK_UP: this->cursor.move_up(); break;
    case SDLK_DOWN: this->cursor.move_down(); break;
    case SDLK_SPACE:
    {
      this->send_response();
      break;
    }
  } // end switch ( key )
}

void ContinueMenuState::on_mouse_left_button_down( const nom::Event& ev )
{
  Point2i mouse_input ( ev.mouse.x, ev.mouse.y ); // mouse input coordinates
  IntRect text_bounds = this->info_box.text_bounds();
  //IntRect text_bounds = IntRect ( option_text.position().x, option_text.position().y, option_text.width(), option_text.height() );
  //nom::int32 option_choice = this->cursor.position();

  if ( text_bounds.contains ( mouse_input ) )
  {
    // 1. Update cursor position
    // 2. Update player's selected choice
    // 3. Play sound event
    // 4. $$$ PROFIT $$$
    this->game->cursor_wrong.Play();
  }
}

void ContinueMenuState::on_mouse_middle_button_down( const nom::Event& ev )
{
  this->send_response();
}

void ContinueMenuState::on_mouse_wheel( const nom::Event& ev )
{
  if ( ev.wheel.y > 0 )
  {
    this->cursor.move_up();
  }
  else if (ev.wheel.y < 0 )
  {
    this->cursor.move_down();
  }
}

void ContinueMenuState::on_joy_button_down( const nom::Event& ev )
{
  switch ( ev.jbutton.button )
  {
    default: NOM_LOG_INFO ( TTCARDS, "FIXME: ContinueMenuState needs joystick implementation!" ); break;
  } // switch
}

void ContinueMenuState::on_user_event( const nom::UserEvent& ev )
{
  // Nothing to do; not the right event type for us!
  if ( ev.type != SDL_USEREVENT ) return;

  if ( ev.code == GameEvent::AudioEvent )
  {
    this->game->cursor_move.Play();
  }
}

void ContinueMenuState::on_update ( float delta_time )
{
  this->cursor.update();

  this->game->window.update();
}

void ContinueMenuState::on_draw ( nom::IDrawable::RenderTarget& target )
{
  this->info_box.draw ( target );
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
