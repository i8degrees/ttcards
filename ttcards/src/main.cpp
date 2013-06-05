/******************************************************************************
    main.cpp

  TTcards app execution loop

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Display.h"
#include "ttcards.h"
#include "fps.h"

#include "SDL.h"

#ifdef EMSCRIPTEN
  #include "emscripten.h"
#endif

using namespace nom;

int main(int argc, char*argv[])
{
  unsigned int sdl_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
  unsigned int video_flags = SDL_SWSURFACE | SDL_RLEACCEL | SDL_RESIZABLE | SDL_DOUBLEBUF;

  unsigned int loops = 0; // globalTimer related
  unsigned int next_game_tick = 0; // globalTimer related

  #ifdef DEBUG_TTCARDS_OBJ
    std::cout << "main():  " << "Hello, world!" << "\n" << std::endl;
  #endif

  // Dependencies: First priority
  #ifndef EMSCRIPTEN
    // This isn't an absolute guarantee that we can do this reliably; we must use
    // argv[0] as we need to know the *starting* directory of where ttcards resides
    // from, not the current working directory in which it is executed from
    WORKING_DIR = OSXFs::getDirName ( argv[0] );
    OSXFs::setWorkingDir ( WORKING_DIR );
  #endif

  if ( argc > 1 )
  {
    if ( strcmp ( argv[1], "-e" ) == 0 || strcmp ( argv[1], "--export" ) == 0 )
    {
      Collection cards;

      if ( cards.ExportJSON ( "tcards.json" ) == false )
      {
        std::cout << "ERR: " << "Unknown failure to serialize JSON into cards.json" << std::endl;
        exit ( EXIT_FAILURE );
      }

      std::cout << "File cards.json successfully saved" << std::endl;
      exit ( EXIT_SUCCESS );
    }
    else if ( strcmp ( argv[1], "-h" ) == 0 || strcmp ( argv[1], "--help" ) == 0 )
    {
      std::cout << "\tttcards [ -h | --help ]" << std::endl;
      std::cout << "\tttcards [ -e | --export ]" << std::endl;
      exit ( EXIT_SUCCESS );
    }
  }

  std::srand ( ( unsigned ) time ( 0 ) ); // Dependencies: before app state init

  SDL_Display display ( sdl_flags );

  Gfx engine; // GameStates

  #ifndef EMSCRIPTEN
    display.setIcon ( APP_ICON ); // Dependencies: before video init
  #endif

  display.CreateWindow ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, video_flags );
  std::cout << display.getDisplayWidth() << " " << display.getDisplayHeight() << "\n";

  // Dependencies: after video init
  FPS fps; // timer for tracking frames per second
  fps.Start();

  next_game_tick = engine.getTicks();

  engine.ChangeState( std::unique_ptr<CardsMenu>( new CardsMenu ( &engine ) ) );

  engine.Run(); // ...here we go!

  #ifdef EMSCRIPTEN
    //engine.Run(); // FIXME
  #else
    while ( engine.isRunning() )
    {
      loops = 0;

      while ( engine.getTicks() > next_game_tick && loops < MAX_FRAMESKIP )
      {
        engine.HandleInput ();

        fps.Update();

        engine.Update ();
        engine.Draw ( display.getDisplay() );

        if ( engine.getShowFPS() )
          display.setTitle ( APP_NAME + " " + "-" + " " + std::to_string ( fps.getFPS() ) + " " + "fps" );
        else
          display.setTitle ( APP_NAME );

        next_game_tick += SKIP_TICKS;
        loops++;
      }
    }
  #endif

  #ifdef DEBUG_TTCARDS_OBJ
    std::cout << "main():  " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  return EXIT_SUCCESS;
}
