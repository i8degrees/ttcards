/******************************************************************************
    main.cpp

  TTcards game app execution

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Display.hpp"
#include "Game.hpp"
#include "FPS.hpp"

#include "SDL.h"

#ifdef EMSCRIPTEN
  #include "emscripten.h"
#endif

#include "SDL_App.hpp"
#include "GameStates.hpp"
#include "SDL_Image.hpp"

class App: public nom::SDL_App
{
  public:
    App ( void )
    {
      unsigned int video_flags = SDL_SWSURFACE | SDL_RLEACCEL | SDL_RESIZABLE | SDL_DOUBLEBUF;

      #ifdef DEBUG_GAME_OBJ
        std::cout << "main():  " << "Hello, world!" << "\n" << std::endl;
      #endif

      #ifndef EMSCRIPTEN
        display.setWindowIcon ( APP_ICON ); // Dependencies: before video init
      #endif

      display.createWindow ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, video_flags );

      //SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL / 3);
    }

    ~App ( void )
    {
      #ifdef DEBUG_GAME_OBJ
        std::cout << "main():  " << "Goodbye cruel world!" << "\n" << std::endl;
      #endif
    }

    void onEvent ( SDL_Event *event )
    {
      SDL_Input::HandleInput ( event ); /// First, take care of our own object's event
                                        /// requests; this allows us to use SDL even in
                                        /// headless mode.
                                        /// (no fancy dangled graphical-subsystem)
      nom::GameStates::onEvent ( event ); /// Take care of each game state's events
    }

    /// Default quit app event handlers
    void onKeyDown ( int32_t key, int32_t mod )
    {
      switch ( key )
      {
        case SDLK_ESCAPE:
        case SDLK_q: this->onQuit(); break;
        case SDLK_EQUALS: this->toggleFPS(); break;
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

    void onResize ( int32_t width, int32_t height )
    {

      if ( this->isFullScreen() )
      {
        display.toggleFullScreenWindow ( 0, 0 );
        this->setFullScreen ( false );
      }
      else
      {
        display.toggleFullScreenWindow ( 0, 0 );
        this->setFullScreen ( true );
      }
    }

    void onQuit ( void )
    {
      this->Quit();
    }

    int32_t Run ( void )
    {
      unsigned int loops = 0; // globalTimer related
      unsigned int next_game_tick = 0; // globalTimer related
      float delta_time = 0; // TODO; this is a stub out

      this->fps.Start();

      next_game_tick = getTicks();

      nom::GameStates::ChangeState( std::unique_ptr<CardsMenu>( new CardsMenu() ) );

      Running(); // ...here we go!

      SDL_Event event;

      while ( isRunning() == true )
      {
        loops = 0;

        while ( getTicks() > next_game_tick && loops < MAX_FRAMESKIP )
        {
          while ( SDL_PollEvent ( &event ) )
          {
            onEvent ( &event );
          }

          this->fps.Update();

          nom::GameStates::Update ( delta_time ); // FIXME; this is a stub out
          nom::GameStates::Draw ( display.get() );

          if ( getShowFPS() )
            display.setWindowTitle ( APP_NAME + " " + "-" + " " + std::to_string ( this->fps.getFPS() ) + " " + "fps" );
          else
            display.setWindowTitle ( APP_NAME );

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
    nom::SDL_Display display;
    nom::FPS fps; // timer for tracking frames per second
};

int main(int argc, char*argv[])
{
  // Dependencies: First priority
  #ifndef EMSCRIPTEN
    // This isn't an absolute guarantee that we can do this reliably; we must use
    // argv[0] as we need to know the *starting* directory of where ttcards resides
    // from, not the current working directory in which it is executed from
    WORKING_DIR = nom::OSXFS::getDirName ( argv[0] );
    nom::OSXFS::setWorkingDir ( WORKING_DIR );
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

  App engine; // GameStates

  #ifdef EMSCRIPTEN
    // FIXME
  #else
    return engine.Run();
  #endif
}
