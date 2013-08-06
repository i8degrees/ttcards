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
#ifndef GAME_APP_HEADERS
#define GAME_APP_HEADERS

#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

#include <SDL/SDL.h>

#include <nomlib/audio.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/sdl/utils.hpp>
#include <nomlib/system.hpp>

#include "CardsMenuState.h"
#include "version.hpp" // CMake auto-generated header file
#include "GameObject.hpp"
#include "cfg.h"

class App: public nom::SDL_App // "is-a" relationship
{
  public:
    App ( nom::int32 argc, char* argv[] );
    ~App ( void );

    /// Handle app & state events
    void onEvent ( SDL_Event *event );

    /// Handle key events
    void onKeyDown ( int32_t key, int32_t mod );

    /// Event handler for resize app request
    void onResize ( int32_t width, int32_t height );

    /// Run app loop
    nom::int32 Run ( void );

  private:
    /// Display context
    nom::Display display;
    /// Timer for tracking frames per second
    nom::FPS fps;
    /// Input events
    SDL_Event event;
    /// Audio subsystem
    nom::OpenAL::AudioDevice dev;
    /// Master volume control
    nom::OpenAL::Listener listener;

    std::shared_ptr<GameObject> state;
};


#endif // GAME_APP_HEADERS defined
