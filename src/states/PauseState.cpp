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
#include "PauseState.hpp"

using namespace nom;

PauseState::PauseState ( const nom::SDLApp::shared_ptr& object ) :
  nom::IState { Game::State::Pause, nom::IState::StateFlags::BackRender },
  game { NOM_DYN_SHARED_PTR_CAST( Game, object) },
  info_box_window{ nullptr },
  info_box{ nullptr }
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

PauseState::~PauseState ( void )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  NOM_DELETE_PTR( this->info_box_window );
}

void PauseState::on_init ( nom::void_ptr data )
{
  Point2i info_box_origin = Point2i( PAUSE_BOX_ORIGIN_X, PAUSE_BOX_ORIGIN_Y );
  Size2i info_box_size = Size2i( PAUSE_BOX_WIDTH, PAUSE_BOX_HEIGHT );

  // This widget's coordinates will be relative to the top-level widget
  this->info_box_window = new nom::UIWidget( this->game->gui_window_ );

  this->info_box = new nom::MessageBox  (
                                          this->info_box_window,
                                          -1,
                                          info_box_origin,
                                          info_box_size
                                        );

  this->info_box->set_decorator( new nom::FinalFantasyDecorator() );

  this->info_box->set_title( "PAUSE", this->game->info_small_text, nom::DEFAULT_FONT_SIZE );
  this->info_box->set_message( SHORT_VERSION_INFO, this->game->info_text, nom::DEFAULT_FONT_SIZE );

  this->blink_update.start();

  this->info_box_window->insert_child( this->info_box );
}

void PauseState::on_exit ( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

void PauseState::on_pause ( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

void PauseState::on_resume ( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

void PauseState::on_key_down( const nom::Event& ev )
{
  switch ( ev.key.sym )
  {
    default: break;

    // Exit pause state; resume previous state
    case SDLK_p: this->game->pop_state(); break;
  }
}

void PauseState::on_joy_button_down( const nom::Event& ev )
{
  switch ( ev.jbutton.button )
  {
    default: break;

    // Exit pause state; resume previous state
    case nom::PSXBUTTON::START: this->game->pop_state(); break;
  }
}

void PauseState::on_update ( float delta_time )
{
  this->info_box->set_title_text( "PAUSE" );

  if ( this->blink_update.ticks() > 800 )
  {
    this->blink_update.stop();
    this->info_box->set_title_text( "" );
    this->blink_text = true;
  }

  this->game->window.update();
}

void PauseState::on_draw ( nom::IDrawable::RenderTarget& target )
{
  this->info_box_window->draw( target );

  if ( this->blink_text )
  {
    this->info_box->set_title_text( "" );
    this->blink_update.start();
    this->blink_text = false;
  }
}
