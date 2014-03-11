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
#include "GameOverStateCursor.hpp"

GameOverStateCursor::GameOverStateCursor ( void )
{
NOM_LOG_TRACE( TTCARDS );
}

GameOverStateCursor::GameOverStateCursor  (
                                            const nom::SpriteSheet& sheet
                                          ) : Cursor ( sheet )
{
NOM_LOG_TRACE( TTCARDS );
}

GameOverStateCursor::GameOverStateCursor  (
                                            const std::string& filename
                                          ) : Cursor ( filename )
{
NOM_LOG_TRACE( TTCARDS );
}

GameOverStateCursor::~GameOverStateCursor ( void )
{
NOM_LOG_TRACE( TTCARDS );
}

void GameOverStateCursor::set_position_map ( CardHand* position )
{
  this->card_position.reset ( position, Free_CardHand );
}

nom::int32 GameOverStateCursor::position ( void )
{
  if ( this->card_position )
  {
    return this->card_position->position();
  }
  else
  {
    return -1;
  }
}

nom::int32 GameOverStateCursor::move_left ( void )
{
  if ( this->x() > PLAYER2_GAMEOVER_ORIGIN_X + ( CARD_WIDTH / 2 ) )
  {
    this->move ( -(CARD_WIDTH), 0 );
    this->previous();
  }

  return this->x();
}

nom::int32 GameOverStateCursor::move_right ( void )
{
  if ( this->x() < PLAYER2_GAMEOVER_ORIGIN_X + ( CARD_WIDTH * ( this->card_position->size() - 1 ) ) )
  {
    this->move ( (CARD_WIDTH), 0 );
    this->next();
  }

  return this->x();
}

void GameOverStateCursor::next ( void )
{
  if ( this->card_position )
  {
    this->card_position->next();

    nom::Event ev;
    ev.user.code = GameEvent::UIEvent;
    ev.user.data1 = nullptr;
    ev.user.data2 = nullptr;
    ev.user.window_id = 0;
    this->cursor_event.dispatch ( ev );
  }
}

void GameOverStateCursor::previous ( void )
{
  if ( this->card_position )
  {
    this->card_position->previous();

    nom::Event ev;
    ev.user.code = GameEvent::UIEvent;
    ev.user.data1 = nullptr;
    ev.user.data2 = nullptr;
    ev.user.window_id = 0;
    this->cursor_event.dispatch ( ev );
  }
}
