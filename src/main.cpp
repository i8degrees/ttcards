/******************************************************************************
    main.cpp

  TTcards app execution loop

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"
#include "fps.h"

#include "SDL.h"

#ifdef EMSCRIPTEN
  #include "emscripten.h"
#endif

int main(int argc, char*argv[])
{
  unsigned int sdl_flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
  unsigned int video_flags = SDL_HWSURFACE | SDL_RLEACCEL | SDL_RESIZABLE | SDL_DOUBLEBUF;

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

  std::srand ( ( unsigned ) time ( 0 ) ); // Dependencies: before app state init

  Gfx engine ( sdl_flags ); // Second priority

  #ifndef EMSCRIPTEN
    engine.setIcon ( APP_ICON, GColor ( 0, 0, 0 ) ); // Dependencies: before video init
  #endif

  engine.SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, video_flags );

  engine.ChangeState( std::unique_ptr<CardsMenu>( new CardsMenu ( &engine ) ) );

  engine.Run(); // ...here we go!

  #ifdef EMSCRIPTEN
    //engine.Run(); // FIXME
  #else
    while ( engine.isRunning() )
    {
      engine.HandleInput ();
      engine.Update();
      engine.Draw();
    }
  #endif

  #ifdef DEBUG_TTCARDS_OBJ
    std::cout << "main():  " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  return EXIT_SUCCESS;
}
