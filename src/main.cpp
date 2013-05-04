/******************************************************************************
    main.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"

#ifdef EMSCRIPTEN
  #include "emscripten.h"
#endif

int main(int argc, char *argv[])
{
  unsigned int sdl_flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
  unsigned int video_flags = SDL_HWSURFACE | SDL_RLEACCEL | SDL_RESIZABLE | SDL_DOUBLEBUF;

  #ifdef DEBUG_TTCARDS_OBJ
    std::cout << "main():  " << "Hello, world!" << "\n" << std::endl;
  #endif

  std::srand ( ( unsigned ) time ( 0 ) ); // needs to exec before CardHand, Player, Board, etc.

  Gfx engine = ( sdl_flags ); // rendering interface instance
  engine.SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, video_flags );

  TTcards app; // needs to be declared before Gfx instance

  if ( app.Init ( &engine ) == true )
  {
    #ifdef EMSCRIPTEN
      app.Run(); // FIXME
    #else
      while ( app.IsRunning() != false )
      {
        app.Run();
      }
    #endif
  }

  #ifdef DEBUG_TTCARDS_OBJ
    std::cout << "main():  " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  return EXIT_SUCCESS;
}
