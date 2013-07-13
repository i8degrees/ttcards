/******************************************************************************

    TTcards game app execution

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include <iostream>
#include <string>
#include <cstdlib>

#ifdef EMSCRIPTEN
  #include "emscripten.h"
#endif

#include <SDL/SDL.h>

#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

#include "CardsMenuState.h"
#include "cfg.h"

class App: public nom::SDL_App
{
  public:
    App ( nom::int32 argc, char* argv[] )
    {
      unsigned int video_flags = SDL_SWSURFACE | SDL_RLEACCEL | SDL_RESIZABLE | SDL_DOUBLEBUF;

#ifdef DEBUG_GAME_OBJ
  std::cout << "main():  " << "Hello, world!" << "\n" << std::endl;
#endif

#ifndef EMSCRIPTEN

// This isn't an absolute guarantee that we can do this reliably; we must use
// argv[0] as we need to know the *starting* directory of where ttcards resides
// from, not the current working directory in which it is executed from
#ifdef TTCARDS_RELEASE
  WORKING_DIR = "/usr/local/share/ttcards/";
#else
  WORKING_DIR = nom::OSXFS::getDirName ( argv[0] );
#endif

  nom::OSXFS::setWorkingDir ( WORKING_DIR );

#endif // EMSCRIPTEN

  // Command line arguments
  if ( argc > 1 )
  {
    if ( strcmp ( argv[1], "-e" ) == 0 || strcmp ( argv[1], "--export" ) == 0 )
    {
      Collection cards;

      if ( cards.ExportJSON ( "cards.json" ) == false )
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

  std::srand ( ( unsigned ) time ( 0 ) );

#ifndef EMSCRIPTEN
  display.setWindowIcon ( APP_ICON );
#endif

      this->display.createWindow ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, video_flags );

      this->enableKeyRepeat ( 100, SDL_DEFAULT_REPEAT_INTERVAL / 3 );
    }

    ~App ( void )
    {
#ifdef DEBUG_GAME_OBJ
  std::cout << "main():  " << "Goodbye cruel world!" << "\n" << std::endl;
#endif
    }

    void onEvent ( SDL_Event *event )
    {
      // Take care of our own events
      SDL_Input::HandleInput ( event );

      // Take care of each state's event
      nom::GameStates::onEvent ( event );
    }

    void onKeyDown ( int32_t key, int32_t mod )
    {
      switch ( key )
      {
        case SDLK_ESCAPE:
        case SDLK_q: this->onQuit(); break;
        case SDLK_BACKSLASH: this->toggleFPS(); break;
        case SDLK_f: this->onResize ( 0, 0 ); break;
        case SDLK_s:
        {
          nom::SDL_Image image;
          image.saveToFile ( "Screenshot_" + std::to_string ( getTicks() ) + ".bmp", display.get() );
          break;
        }
        default: break;
      }
    }

    // Default resize app event handler
    void onResize ( int32_t width, int32_t height )
    {
      if ( this->isFullScreen() )
      {
        this->display.toggleFullScreenWindow ( 0, 0 );
        this->setFullScreen ( false );
      }
      else
      {
        this->display.toggleFullScreenWindow ( 0, 0 );
        this->setFullScreen ( true );
      }
    }

    int32_t Run ( void )
    {
      unsigned int loops = 0;
      unsigned int next_game_tick = 0;
      float delta_time = 0; // TODO; this is a stub out

      this->fps.Start();

      next_game_tick = getTicks();

      nom::GameStates::ChangeState( std::unique_ptr<CardsMenu>( new CardsMenu() ) );

      this->Running(); // ...here we go!

      while ( this->isRunning() == true )
      {
        loops = 0;

        while ( this->getTicks() > next_game_tick && loops <= MAX_FRAMESKIP )
        {
          while ( this->PollEvents ( &event ) )
          {
            this->onEvent ( &event );
          }

          this->fps.Update();

          nom::GameStates::Update ( delta_time ); // FIXME; this is a stub out
          nom::GameStates::Draw ( this->display.get() );

          if ( this->getShowFPS() )
            this->display.setWindowTitle ( APP_NAME + " " + "-" + " " + std::to_string ( this->fps.getFPS() ) + " " + "fps" );
          else
            this->display.setWindowTitle ( APP_NAME );

          next_game_tick += SKIP_TICKS;
          loops++;

          // FIXME: this is a lazy patch to keep CPU cycles down; on my system,
          // usage drops from 99% to ~22..30%
          if ( this->fps.getFPS() >= TICKS_PER_SECOND )
            this->fps.sleep ( 50 );
        }
      }

      return EXIT_SUCCESS;
    }

  private:
    /// display context
    nom::SDL_Display display;
    /// timer for tracking frames per second
    nom::FPS fps;
    /// Input events
    SDL_Event event;
};

int main ( int argc, char* argv[] )
{
  App engine ( argc, argv );

#ifdef EMSCRIPTEN
  // TODO
#else
  return engine.Run();
#endif

  // This is past the point of execution; all execution must reside within our
  // App class
}
