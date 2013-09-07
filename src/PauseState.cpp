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
#include "PauseState.hpp"

PauseState::PauseState ( std::shared_ptr<GameObject> object )
{
NOM_LOG_TRACE ( TTCARDS );

  this->game = object;
}

PauseState::~PauseState ( void )
{
NOM_LOG_TRACE ( TTCARDS );
}

void PauseState::onInit ( void )
{
  nom::Gradient linear;

  linear.setStartColor ( nom::Color ( 67, 67, 67, 255 ) );
  linear.setEndColor ( nom::Color ( 99, 99, 99, 255 ) );

  this->info_box = nom::ui::MessageBox  (
                                          PAUSE_BOX_ORIGIN_X, PAUSE_BOX_ORIGIN_Y,
                                          PAUSE_BOX_WIDTH, PAUSE_BOX_HEIGHT,
                                          nom::ui::FrameStyle::Gray, linear
                                        );


  this->info_box.setWindowTitleFont ( &this->game->info_small_text );
  this->info_box.setLabelFont ( &this->game->info_text );
  this->info_box.setLabel ( SHORT_VERSION_INFO );
  this->info_box.setLabelTextAlignment ( nom::TextAlignment::MiddleCenter );

  this->update.start();
}

void PauseState::onExit ( void )
{
  std::cout << "\n" << "PauseState state onExit" << "\n";
}

void PauseState::Pause ( void )
{
  std::cout << "\n" << "PauseState state Paused" << "\n";
}

void PauseState::Resume ( void )
{
  std::cout << "\n" << "PauseState state Resumed" << "\n";
}

void PauseState::onKeyDown ( nom::int32 key, nom::int32 mod )
{
  switch ( key )
  {
    default: break;

    // Exit pause state; resume previous state
    case SDLK_p: nom::GameStates::PopState(); break;
  }
}

void PauseState::Update ( float delta_time )
{
  this->info_box.Update();

  this->info_box.setWindowTitle ( "PAUSE" );

  if ( this->update.ticks() > 800 )
  {
    this->update.stop();
    this->info_box.setWindowTitle ( "" );
    this->blink_text = true;
  }

  this->game->context.Update();
}

void PauseState::Draw ( void* video_buffer )
{
  this->info_box.Draw( video_buffer );

  if ( this->blink_text )
  {
    this->info_box.setWindowTitle ( "" );
    this->update.start();
    this->blink_text = false;
  }
}
