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

ContinueMenuState::ContinueMenuState  ( const nom::SDLApp::SharedPtr& object ) :
  nom::IState { Game::State::ContinueMenu, 0, nom::IState::StateFlags::BackRender },
  game { std::dynamic_pointer_cast<Game> (object) }
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
  nom::Text option_text = nom::Text("Are you sure?\nYes\nNo", this->game->info_text, 12, nom::Text::Alignment::MiddleCenter);

  linear.set_start_color ( nom::Color4i::Gray );
  linear.set_end_color ( nom::Color4i::LightGray );
  linear.set_fill_direction ( nom::Gradient::FillDirection::Left );

  this->info_box = nom::MessageBox  (
                                          OPTION_BOX_ORIGIN_X, OPTION_BOX_ORIGIN_Y,
                                          OPTION_BOX_WIDTH, OPTION_BOX_HEIGHT,
                                          nom::MessageBox::Style::Gray, linear
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

  this->position_map = nom::Coords  (
                                      OPTION_BOX_ORIGIN_Y + ( OPTION_BOX_HEIGHT / 2 ),
                                      OPTION_BOX_ORIGIN_Y + ( OPTION_BOX_HEIGHT / 2 )
                                      +
                                      ( option_text.height() / 2 )
                                    );

  this->cursor.set_position_map ( position_map );

  this->cursor.setSize ( CURSOR_WIDTH, CURSOR_HEIGHT );

  this->cursor.set_position (
                              ( OPTION_BOX_ORIGIN_X ) - CURSOR_WIDTH,
                              ( OPTION_BOX_ORIGIN_Y ) + ( OPTION_BOX_HEIGHT / 2 )
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

void ContinueMenuState::onKeyDown ( nom::int32 key, nom::int32 mod, nom::uint32 window_id )
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

    case SDLK_UP: this->cursor.move_up(); break;
    case SDLK_DOWN: this->cursor.move_down(); break;
    case SDLK_SPACE:
    {
      // We will use the positioning of the cursor to map user's response
      nom::int32 choice = this->cursor.position();

      if ( choice == 0 ) // Yes; response will be 0 (TRUE)
      {
        nom::int32_ptr response = new nom::int32 (0);
        this->game->pop_state(response);
      }
      else if ( choice == 1 ) // No; response will be 1 (FALSE)
      {
        nom::int32_ptr response = new nom::int32 (1);
        this->game->pop_state(response);
      }
      else
      {
        this->game->cursor_wrong.Play();
      }
      break;
    }
  }
}

void ContinueMenuState::onMouseLeftButtonDown ( nom::int32 x, nom::int32 y, nom::uint32 window_id )
{
  nom::Coords coords ( x, y ); // mouse input coordinates

  // Player hand selection checks
  nom::int32 option_choice = this->cursor.position();

NOM_DUMP_VAR(option_choice);

  if ( this->position_map.intersects ( coords ) )
  {
    NOM_DUMP_VAR("b00bs");
    // 1. Update cursor position
    // 2. Update player's selected card
    // 3. Update the card info message box
    // 4. Play sound event

    //this->game->cursor_move.Play();
  }
}

void ContinueMenuState::onMouseWheel ( nom::int32 x, nom::int32 y, nom::uint32 window_id )
{
  if ( y > 0 )
  {
    this->cursor.move_up();
  }
  else if (y < 0 )
  {
    this->cursor.move_down();
  }
}

void ContinueMenuState::onJoyButtonDown ( nom::int32 which, nom::int32 button )
{
  switch ( button )
  {
    default: NOM_LOG_INFO ( TTCARDS, "FIXME: ContinueMenuState needs joystick implementation!" ); break;
  } // switch
}

void ContinueMenuState::onUserEvent ( nom::uint32 type, nom::int32 code, void* data1, void* data2 )
{
  if ( type == SDL_USEREVENT && code == static_cast<nom::int32> ( nom::EventDispatcher::UserEvent::UI ) )
  {
    this->game->cursor_move.Play();
  }
}

void ContinueMenuState::on_update ( float delta_time )
{
  this->cursor.update();

  this->game->window.update();
}

void ContinueMenuState::on_draw ( nom::IDrawable::RenderTarget target )
{
  this->info_box.draw ( target );
  this->cursor.draw ( target );
}
