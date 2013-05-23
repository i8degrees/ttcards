/******************************************************************************
    main.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"

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

  #ifndef EMSCRIPTEN
    // This isn't an absolute guarantee that we can do this reliably; we must use
    // argv[0] as we need to know the *starting* directory of where ttcards resides
    // from, not the current working directory in which it is executed from
    WORKING_DIR = OSXFs::getDirName ( argv[0] );
    OSXFs::setWorkingDir ( WORKING_DIR );
  #endif

  std::srand ( ( unsigned ) time ( 0 ) ); // needs to exec before CardHand, Player, Board, etc.

  Gfx engine ( sdl_flags );

  #ifndef EMSCRIPTEN
    engine.setIcon ( APP_ICON, GColor ( 0, 0, 0 ) );
  #endif

  engine.SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, video_flags );

  engine.ChangeState( std::unique_ptr<TTcards>( new TTcards ( &engine) ) );

  engine.Run(); // Set running state to true

  #ifdef EMSCRIPTEN
    //engine.Run(); // FIXME
  #else
    while ( engine.isRunning() == true )
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
