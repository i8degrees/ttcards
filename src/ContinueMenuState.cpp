/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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

ContinueMenuState::ContinueMenuState ( std::shared_ptr<GameObject> object )
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

  linear.setStartColor ( nom::Color ( 67, 67, 67, 255 ) );
  linear.setEndColor ( nom::Color ( 99, 99, 99, 255 ) );

  this->info_box = nom::ui::MessageBox  (
                                          OPTION_BOX_ORIGIN_X, OPTION_BOX_ORIGIN_Y,
                                          OPTION_BOX_WIDTH, OPTION_BOX_HEIGHT,
                                          nom::ui::FrameStyle::Gray, linear
                                        );

  //this->game->info_text.setNewline ( 8 );

  nom::int32 text_width = this->game->info_text.getFontWidth();
  nom::int32 text_height = this->game->info_text.getFontHeight();

NOM_DUMP_VAR ( text_width );
NOM_DUMP_VAR ( text_height );
NOM_DUMP_VAR ( this->game->info_text.getNewline() );

  this->game->info_text.setPosition ( nom::Coords( ( SCREEN_WIDTH - text_width ) / 2, ( SCREEN_HEIGHT - text_height ) / 2 - 16  ) );
  this->game->info_small_text.setPosition ( nom::Coords( OPTION_BOX_ORIGIN_X + 4, OPTION_BOX_ORIGIN_Y ) );

  this->game->info_text.setText ( "Are you sure?\nYes\nNo" );
  this->game->info_small_text.setText ( "CHOICE" );
}

void ContinueMenuState::onExit ( void )
{
  // Stub
}

void ContinueMenuState::onKeyDown ( nom::int32 key, nom::int32 mod )
{
  switch ( key )
  {
    default: break;

    // Resume previous state by exiting this one
    case SDLK_p: nom::GameStates::PopState(); break;
  }
}

void ContinueMenuState::Update ( float delta_time )
{
  this->info_box.Update();
  this->game->info_text.Update();
  this->game->info_small_text.Update();

  this->game->context.Update();
}

void ContinueMenuState::Draw ( void* video_buffer )
{
  this->info_box.Draw( video_buffer );
  this->game->info_text.Draw( video_buffer );

  this->game->info_small_text.Draw( video_buffer );
}
