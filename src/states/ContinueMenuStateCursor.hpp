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
#ifndef GAMEAPP_CONTINUE_MENU_STATE_CURSOR_HPP
#define GAMEAPP_CONTINUE_MENU_STATE_CURSOR_HPP

#include <iostream>
#include <string>
#include <memory>

#include <nomlib/graphics.hpp>
#include <nomlib/math.hpp>
#include <nomlib/gui.hpp>
#include <nomlib/system.hpp>

#include "config.hpp"

class ContinueMenuStateCursor: public nom::Cursor
{
  public:
    ContinueMenuStateCursor ( void );

    ContinueMenuStateCursor (
                              const nom::SpriteSheet& sheet
                            );

    ContinueMenuStateCursor (
                              const std::string& filename
                            );

    ~ContinueMenuStateCursor ( void );

    /// Set a new positioning object for this instance to use
    void set_position_map ( const nom::Coords& position_map );

    /// Obtain the current position we are at in the player's hand.
    nom::int32 position ( void );

    /// Move the cursor to the left.
    ///
    /// Returns the X coordinate position of the cursor after it has been moved.
    nom::int32 move_up ( void );

    /// Move the cursor to the right.
    ///
    /// Returns the X coordinate position of the cursor after it has been moved.
    nom::int32 move_down ( void );

  private:
    void next ( void );
    void previous ( void );
    nom::Coords option_position;
    nom::int32 cursor_position;
    nom::EventDispatcher cursor_event;
};

#endif // include guard defined
