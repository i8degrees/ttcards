/******************************************************************************

    TTcards game app execution stub

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAME_APP_HEADERS
#define GAME_APP_HEADERS

#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>

#include <SDL/SDL.h>

#include "nomlib/audio.hpp"
#include "nomlib/graphics.hpp"
#include "nomlib/sdl/utils.hpp"
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
