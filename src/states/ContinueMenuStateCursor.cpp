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
#include "ContinueMenuStateCursor.hpp"

ContinueMenuStateCursor::ContinueMenuStateCursor ( void ) :
  cursor_position_{ 0 }
{
  // NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );
}

ContinueMenuStateCursor::~ContinueMenuStateCursor ( void )
{
  // NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );
}

ContinueMenuStateCursor::ContinueMenuStateCursor  (
                                                    const nom::SpriteSheet& sheet
                                                  ) :
  AnimatedSprite{ sheet },
  cursor_position_{ 0 }
{
  // NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );
}

ContinueMenuStateCursor::ContinueMenuStateCursor  (
                                                    const std::string& filename
                                                  ) :
  AnimatedSprite{ filename },
  cursor_position_{ 0 }
{
  // NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );
}

void ContinueMenuStateCursor::set_position_map ( const nom::Point2i& position_map )
{
  this->option_position = position_map;
  this->cursor_position_ = 0;
}

int ContinueMenuStateCursor::cursor_position( void )
{
  return this->cursor_position_;
}

nom::int32 ContinueMenuStateCursor::move_up ( void )
{
  if ( this->position().y > this->option_position.x )
  {
    this->move ( 0, -(16) );
    this->previous();
  }

  //NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_GUI, this->position().y );
  return this->position().y;
}

nom::int32 ContinueMenuStateCursor::move_down ( void )
{
  if ( this->position().y < 128 /*this->option_position.y*/ ) // FIXME
  {
    this->move ( 0, 16 );
    this->next();
  }

  //NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_GUI, this->position().y );
  return this->position().y;
}

void ContinueMenuStateCursor::next ( void )
{
  this->cursor_position_++;
  //NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_GUI, this->cursor_position_);

  nom::Event ev;
  ev.user.code = GameEvent::AudioEvent;
  ev.user.data1 = nullptr;
  ev.user.data2 = nullptr;
  ev.user.window_id = 0;
  this->cursor_event.dispatch ( ev );
}

void ContinueMenuStateCursor::previous ( void )
{
  this->cursor_position_--;
  //NOM_DUMP_VAR(TTCARDS_LOG_CATEGORY_GUI, this->cursor_position_);

  nom::Event ev;
  ev.user.code = GameEvent::AudioEvent;
  ev.user.data1 = nullptr;
  ev.user.data2 = nullptr;
  ev.user.window_id = 0;
  this->cursor_event.dispatch ( ev );
}
