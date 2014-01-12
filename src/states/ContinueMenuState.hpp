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
#ifndef GAMEAPP_CONTINUE_MENU_STATE_HEADERS
#define GAMEAPP_CONTINUE_MENU_STATE_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include <nomlib/graphics.hpp>
#include <nomlib/gui.hpp>

#include "config.hpp"
#include "ContinueMenuStateCursor.hpp"
#include "CardsMenuState.hpp"
#include "GameObject.hpp"

class ContinueMenuState: public nom::IState
{
  public:
    ContinueMenuState ( std::shared_ptr<GameObject> object );
    ~ContinueMenuState ( void );

  private:
    void onInit ( void );
    void onExit ( void );

    void onKeyDown ( nom::int32 key, nom::int32 mod, nom::uint32 window_id );
    void onMouseLeftButtonDown ( nom::int32 x, nom::int32 y, nom::uint32 window_id );
    void onMouseWheel ( nom::int32 x, nom::int32 y, nom::uint32 window_id );
    void onJoyButtonDown ( nom::int32 which, nom::int32 button );
    void onUserEvent ( nom::uint32 type, nom::int32 code, void* data1, void* data2 );

    void update ( float delta_time );
    void draw ( nom::IDrawable::RenderTarget target );

    std::shared_ptr<GameObject> game;

    nom::MessageBox info_box;
    ContinueMenuStateCursor cursor;

    nom::Coords position_map;
};

// Convenience declarations for changing state
typedef std::unique_ptr<ContinueMenuState> ContinueMenuStatePtr;

#endif // include guard defined
