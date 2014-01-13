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

ContinueMenuState::ContinueMenuState ( const std::shared_ptr<GameObject>& object )
{
NOM_LOG_TRACE ( TTCARDS );

  this->game = object;
}

ContinueMenuState::~ContinueMenuState ( void )
{
NOM_LOG_TRACE ( TTCARDS );
}

void ContinueMenuState::onInit ( void )
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

//NOM_DUMP_VAR ( this->game->info_text.getMultiLineTextWidth("Are you sure?\nYes\nNo") );
//nom::int32 text_width = this->game->info_text.getFontWidth();
//nom::int32 text_height = this->game->info_text.getFontHeight();
//NOM_DUMP_VAR ( text_width );
//NOM_DUMP_VAR ( text_height );
//NOM_DUMP_VAR ( this->game->info_text.getNewline() );

  // Initialize interface cursor
  this->cursor = ContinueMenuStateCursor ( "images/cursors.json" );

  if ( this->cursor.load ( this->game->config.getString("INTERFACE_CURSOR"), 0 ) == true )
  {
    if ( this->game->config.getString("SCALE_ALGORITHM") == "scale2x" )
    {
      this->cursor.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    }
    else if ( this->game->config.getString("SCALE_ALGORITHM") == "hqx" )
    {
      this->cursor.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    }

      this->position_map = nom::Coords  (
                                          OPTION_BOX_ORIGIN_Y + ( OPTION_BOX_HEIGHT / 2 ),
                                          OPTION_BOX_ORIGIN_Y + ( OPTION_BOX_HEIGHT / 2 )
                                          +
                                          ( option_text.height() / 2 )
                                        );

NOM_DUMP_VAR(option_text.height());
    this->cursor.set_position_map ( position_map );

    this->cursor.setSize ( CURSOR_WIDTH, CURSOR_HEIGHT );

    this->cursor.set_position (
                                ( OPTION_BOX_ORIGIN_X ) - CURSOR_WIDTH,
                                ( OPTION_BOX_ORIGIN_Y ) + ( OPTION_BOX_HEIGHT / 2 )
                              );

    this->cursor.set_frame ( INTERFACE_CURSOR_RIGHT );
  }
  else // EPIC FAIL
  {
#if defined (NOM_DEBUG)
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("INTERFACE_CURSOR") );
#else
    nom::DialogMessageBox ( "Error", "Could not load resource file: " + this->game->config.getString("INTERFACE_CURSOR") );
#endif
    exit ( NOM_EXIT_FAILURE );
  }
}

void ContinueMenuState::onExit ( void )
{
  // Stub
}

void ContinueMenuState::onKeyDown ( nom::int32 key, nom::int32 mod, nom::uint32 window_id )
{
  switch ( key )
  {
    default: break;

    // Resume previous state by exiting this one
    case SDLK_p: nom::GameStates::PopState(); break;

    case SDLK_UP: this->cursor.move_up(); break;
    case SDLK_DOWN: this->cursor.move_down(); break;
    case SDLK_SPACE:
    {
      nom::int32 choice = this->cursor.position();

      if ( choice == 0 ) // Yes
      {
        nom::GameStates::PopState ( choice );
      }
      else if ( choice == 1 ) // No
      {
        nom::GameStates::PopState();
      }
      else
      {
        this->game->cursor_wrong.Play();
      }
    }
    break;
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

void ContinueMenuState::update ( float delta_time )
{
  this->cursor.update();

  this->game->window.update();
}

void ContinueMenuState::draw ( nom::IDrawable::RenderTarget target )
{
  this->info_box.draw ( target );

  this->cursor.draw ( target );
}
