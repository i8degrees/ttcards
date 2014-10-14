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

ContinueMenuStateCursor::ContinueMenuStateCursor() :
  cursor_position_(0)
{
  // NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );
}

ContinueMenuStateCursor::~ContinueMenuStateCursor()
{
  // NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );
}

ContinueMenuStateCursor::ContinueMenuStateCursor  (
                                                    const nom::SpriteSheet& sheet
                                                  ) :
  AnimatedSprite(sheet),
  cursor_position_(0)
{
  // NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );
}

ContinueMenuStateCursor::ContinueMenuStateCursor  (
                                                    const std::string& filename
                                                  ) :
  AnimatedSprite(filename),
  cursor_position_(0)
{
  // NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );
}

void ContinueMenuStateCursor::set_position_map( const std::vector<nom::IntRect>& map )
{
  this->position_map_ = map;
  this->cursor_position_ = 0;
}

int ContinueMenuStateCursor::cursor_position()
{
  return this->cursor_position_;
}

int ContinueMenuStateCursor::first()
{
  return 0;
}

int ContinueMenuStateCursor::last()
{
  return this->position_map_.size();
}

bool ContinueMenuStateCursor::prev()
{
  int pos = this->cursor_position();          // Element position
  int first_pos = this->first();

  if( this->position().y > this->position_map_[first_pos].y ) {
    this->move( 0, -(this->position_map_[pos].h) );
    --cursor_position_;
    return true;
  }

  // NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_TEST, this->position().x );
  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_TEST, this->position().y );
  return false;
}

bool ContinueMenuStateCursor::next()
{
  int pos = this->cursor_position();          // Element position
  int last_pos = this->last() - 1;

  if( this->position().y < this->position_map_[last_pos].y )
  {
    this->move( 0, this->position_map_[pos].h );
    ++cursor_position_;
    return true;
  }

  // NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_TEST, this->position().x );
  NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_TEST, this->position().y );
  return false;
}

void ContinueMenuStateCursor::set_cursor_position(int pos)
{
  NOM_ASSERT( pos <= this->last() );

  if( pos <= this->last() ) {
    this->cursor_position_ = pos;

    // Update rendering coordinates
    this->set_position( this->position_map_[pos].position() );
  }
}
