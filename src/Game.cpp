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
#include <iostream>
#include <string>

#include <Rocket/Debugger.h>

#include "Game.hpp"

#include "states/GameOverState.hpp"
#include "states/ConfirmationDialogState.hpp"
#include "states/PauseState.hpp"
#include "states/CardsMenuState.hpp"
#include "states/PlayState.hpp"

//#include "states/States.hpp" // StateFactory

using namespace nom;

Game::Game() :
  audio_dev_{ nullptr },
  listener_{ nullptr },
  cursor_move{ nullptr },
  cursor_cancel{ nullptr },
  cursor_wrong{ nullptr },
  card_place{ nullptr },
  card_flip{ nullptr },
  load_game{ nullptr },
  save_game{ nullptr },
  music_track{ nullptr },
  winning_track{ nullptr },
  game { this, tt::free_game }
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );
}

Game::Game( nom::int32 argc, char* argv[] ) :
  // SDLApp( OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS | OSX_DISABLE_FULLSCREEN_SPACES | INIT_ENGINE_FONTS ),
  SDLApp( OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS | OSX_DISABLE_FULLSCREEN_SPACES ),
  game { this, tt::free_game }
{
  std::ostringstream ttcards_version;

  ttcards_version << APP_NAME << " " << "v" << TTCARDS_VERSION_MAJOR << "."
      << TTCARDS_VERSION_MINOR << "." << TTCARDS_VERSION_PATCH << "-d"
      << std::endl;

  //this->state_factory = new States();

  nom::File dir;

  if( nom::init_third_party(0) == false )
  {
    nom::DialogMessageBox(  "Critical Error",
                            "Could not load third party libraries" );
    exit(NOM_EXIT_FAILURE);
  }
  atexit(nom::quit); // Clean up memory associated with nomlib

  // nom::init_rand( nom::hires_counter() );

  #if ! defined( NDEBUG )  // Debug target build
    NOM_LOG_INFO( TTCARDS, "DEBUG build" );

    // Enable info log level and greater for our engine (deprecated)
    nom::SDL2Logger::set_logging_priority( NOM, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    // Enable logging of all messages in the game
    nom::SDL2Logger::set_logging_priority( TTCARDS, nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );

    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_INPUT, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );
    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_TEST, nom::LogPriority::NOM_LOG_PRIORITY_INFO );

    // nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_EVENTS, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    // Disable logging of all messages from GameConfig except for fatal errs.
    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_CFG, nom::LogPriority::NOM_LOG_PRIORITY_WARN );

    // Enable logging of game state function traces
    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_TRACE_STATES, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_GUI, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    // Game states debugging (engine scope)
    nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_SYSTEM, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    // Game states debugging (game scope)
    // nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_CARDS_MENU_STATE, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_CPU_PLAYER, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );
  #else // NDEBUG -- release target build

    NOM_LOG_INFO( TTCARDS, "RELEASE build" );

    // Disable logging of all messages but critical log level (these are fatal)
    // under the scope of our engine, nomlib.
    nom::SDL2Logger::set_logging_priority( NOM, nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL );

    // Disable logging of all messages but critical log level (these are fatal)
    // under the scope of the game.
    nom::SDL2Logger::set_logging_priority( TTCARDS, nom::LogPriority::NOM_LOG_PRIORITY_CRITICAL );

  #endif

  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );

  // Command line arguments
  if ( argc > 1 )
  {
    for ( nom::int32 opt = 1; opt < argc; opt++ )
    {
      // Save cards collection at user defined path
      if ( std::string(argv[opt]) == "--export" )
      {
        CardCollection cards;

        if ( argv[opt + 1] == nullptr )
        {
          NOM_LOG_ERR ( TTCARDS, "Missing parameter <input_filename> for export argument." );
          exit ( NOM_EXIT_FAILURE );
        }
        else if ( dir.exists ( argv[opt + 1] ) == false )
        {
          NOM_LOG_ERR ( TTCARDS, "File path for <input_filename> does not exist at: " + std::string(argv[opt + 1]) );
          exit ( NOM_EXIT_FAILURE );
        }
        else if ( cards.load( argv[opt + 1] ) == false )
        {
          NOM_LOG_ERR ( TTCARDS, "Could not load game cards collection at: " + std::string(argv[opt + 1]) );
          exit ( NOM_EXIT_FAILURE );
        }

        if ( argv[opt + 2] == nullptr )
        {
          NOM_LOG_ERR ( TTCARDS, "Missing parameter <output_filename> for export argument." );
          exit ( NOM_EXIT_FAILURE );
        }
      #if ! defined( NOM_DEBUG ) // Allow overwriting of file I/O if in dev mode
        else if ( dir.exists ( argv[opt + 2] ) == true )
        {
          NOM_LOG_ERR ( TTCARDS, "File path for <output_filename> already exists at: " + std::string(argv[opt + 2]) );
          exit ( NOM_EXIT_FAILURE );
        }
      #endif
        else if ( cards.save( argv[opt + 2] ) == false )
        {
          NOM_LOG_ERR ( TTCARDS, "Could not save the game cards collection at: " + std::string(argv[opt + 2]) );
          exit ( NOM_EXIT_FAILURE );
        }
        else
        {
          NOM_LOG_INFO ( TTCARDS, "Game cards successfully saved at: " + std::string(argv[opt + 2]) );
          exit ( NOM_EXIT_SUCCESS );
        }
      } // end export cards option
      // else if ( std::string(argv[opt]) == "--config" ) // Save configuration opt
      // {
      //   if ( argv[opt + 1] == nullptr )
      //   {
      //     NOM_LOG_ERR ( TTCARDS, "Missing parameter <output_filename> for config argument." );
      //     exit ( NOM_EXIT_FAILURE );
      //   }
      // #if ! defined( NOM_DEBUG ) // Allow overwriting of file I/O if in dev mode
      //   else if ( dir.exists ( argv[opt + 1] ) == true )
      //   {
      //     NOM_LOG_ERR ( TTCARDS, "File path for <output_filename> already exists at: " + std::string(argv[opt + 1]) );
      //     exit ( NOM_EXIT_FAILURE );
      //   }
      // #endif

      //   GameConfig cfg;
      //   if ( cfg.load ( TTCARDS_CONFIG_FILENAME ) == false )
      //   {
      //     NOM_LOG_ERR ( TTCARDS, "Could not load game configuration from file at: " + TTCARDS_CONFIG_FILENAME );
      //     exit ( NOM_EXIT_FAILURE );
      //   }

      //   if ( cfg.save ( argv[opt + 1] ) == false )
      //   {
      //     NOM_LOG_ERR ( TTCARDS, "Could not save game configuration to file at: " + std::string(argv[opt + 1]) );
      //     exit ( NOM_EXIT_FAILURE );
      //   }
      //   else
      //   {
      //     NOM_LOG_INFO ( TTCARDS, "Game configuration successfully saved at: " + std::string(argv[opt + 1]) );
      //     exit ( NOM_EXIT_SUCCESS );
      //   }
      // } // end save config option
      else if ( std::string(argv[opt]) == "-v" || std::string(argv[opt]) == "--version" )
      {
        std::cout << ttcards_version.str() << std::endl;
        // TODO: Copyright info
      } // end version option
      else // -h, --help
      {
        std::cout << APP_NAME << " usage options:" << std::endl;
        std::cout << "[ --export <input_filename> <output_filename> ]" << std::endl;
        std::cout << "[ --config <output_filename> ]" << std::endl;
        std::cout << "[ -h, --help ]" << std::endl;
        std::cout << "[ -v, --version ]" << std::endl;
      } // end help option
    } // end for argv[opt] loop

    // If we have got this far, we assume command execution was successful
    exit(NOM_EXIT_SUCCESS);
  } // end argc > 1

  working_directory = dir.resource_path();

  // Change the working directory to whatever working_directory has been set to
  //
  // Note that it is important that we do not mess with the working directory
  // path until after our command line arguments have been processed, so that we
  // do not unintentionally mess up relative paths!
  dir.set_path (working_directory);
}

Game::~Game()
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );

  this->gui_window_.shutdown();

  nom::shutdown_librocket();
}

bool Game::on_init( void )
{
  int render_driver = -1;
  uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
  uint32 render_flags = SDL_RENDERER_ACCELERATED;
  /* TODO: Use me when fixed-time-step is fully implemented
  nom::uint32 renderer_flags = SDL_RENDERER_PRESENTVSYNC;
  TODO */

  this->set_state_machine( new nom::StateMachine() );

  if ( this->config.load ( TTCARDS_CONFIG_FILENAME ) == false )
  {
    nom::DialogMessageBox( "Critical Error", "Could not load configuration file at: " + TTCARDS_CONFIG_FILENAME );
    return false;
  }

  if( nom::set_hint( SDL_HINT_RENDER_VSYNC, "0" ) == false )
  {
    NOM_LOG_INFO( NOM, "Could not disable vertical refresh." );
  }

  if( nom::set_hint(  SDL_HINT_RENDER_SCALE_QUALITY,
                      this->config.getString("RENDER_SCALE_QUALITY") ) == false )
  {
    NOM_LOG_INFO( NOM, "Could not set rendering scale quality." );
  }

  // We only can support a OpenGL capable rendering driver at the moment; see
  // nom::RocketSDL2RenderInterface.
  render_driver = nom::available_render_driver("opengl");
  if( render_driver == -1 )
  {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION,
                  "Could not find an OpenGL rendering driver." );
    return false;
  }

  if( this->window.create( "TTcards", SCREEN_WIDTH, SCREEN_HEIGHT,
      window_flags, render_driver, render_flags ) == false )
  {
    nom::DialogMessageBox(  "Critical Error",
                            "Could not initialize rendering context and window." );
    return false;
  }

  this->window.set_window_icon ( this->config.getString("APP_ICON") );

  if( nom::RocketSDL2RenderInterface::gl_init(  this->window.size().w,
                                                this->window.size().h ) == false )
  {
    nom::DialogMessageBox( "Critical Error", "Could not initialize OpenGL." );
    return false;
  }

  // Allow for automatic rescaling of the output window based on the aspect
  // ratio (i.e.: handle fullscreen resizing); this will use letterboxing
  // when the aspect ratio is greater than what is available, or side-bars
  // when the aspect ratio is less than.
  this->window.set_logical_size( GAME_RESOLUTION.w, GAME_RESOLUTION.h );

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    this->window.set_scale( nom::Point2f(2,2) );
  #else
    this->window.set_scale( nom::Point2f(1,1) );
  #endif

  // Initialize libRocket's file interface
  Rocket::Core::FileInterface* fs = nullptr;

  // Use Resources from the project root instead of the application bundle when
  // in our native development environment; this is to let us quickly reload
  // modified configuration files (RML, RCSS) without the need to remember to
  // copy the files manually or 'make install'.
  //
  // The game uses resources from its application bundle, which are copied over
  // from the Resources directory during the build process ('make install').
  // The resources from the application bundle should be regarded as temporary,
  // as they are potentially overwritten with each new build, so they should not
  // be modified.
  //
  // This will have to do until I get around to cleaning up the build system...
  //
  // Note that Resources/config.json is not taken care of by the configuration
  // here -- still points to ~/Documents/ttcards/.... I've decided to give
  // [fswatch](https://github.com/emcrisostomo/fswatch) a try for automatic
  // copying of this file. See also: bin/fswatch.sh
  #if defined(NOM_PLATFORM_OSX) && ! defined(NDEBUG)
    fs = new nom::RocketFileInterface( "../../../../Resources/" );
  #elif defined(NOM_PLATFORM_WINDOWS) && ! defined(NDEBUG)
    fs = new nom::RocketFileInterface( this->working_directory + "\\" );
  #else
    // Stub code
    fs = new nom::RocketFileInterface( this->working_directory + "/Resources/" );
  #endif

  Rocket::Core::SystemInterface* sys =
    new RocketSDL2SystemInterface();

  if( nom::init_librocket( fs, sys ) == false )
  {
    nom::DialogMessageBox( "Critical Error", "Could not initialize libRocket." );
    return false;
  }

  Rocket::Core::RenderInterface* renderer =
    new nom::RocketSDL2RenderInterface( &this->window );

  #if ! defined( NDEBUG )
    this->gui_window_.enable_debugger();
  #endif

  if( this->gui_window_.create_context( "main",
      this->window.size(), renderer ) == false )
  {
    nom::DialogMessageBox( "Critical Error", "Could not initialize GUI context." );
    return false;
  }

  // Resize libRocket's visual debugger window to fit within our game's tiny
  // resolution
  #if ! defined( NDEBUG )

    // Visual debugger is too large when we are dealing with the original game
    // resolution
    #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
      // 32 is the height of the visual debugger's menu; as per
      // Rocket/Debugger/MenuSource.h
      Size2i debugger_dims( 150, (this->gui_window_.size().h / 2) + 32 );
      this->gui_window_.set_debugger_size(debugger_dims);
    #endif

  #endif

  if( this->gui_window_.load_font( this->config.getString("GUI_TITLE_FONT") ) == false )
  {
    nom::DialogMessageBox(  "Critical Error",
                            "Could not load font file:" +
                            this->config.getString("GUI_TITLE_FONT") );
    return false;
  }

  if( this->gui_window_.load_font( this->config.getString("GUI_TEXT_FONT") ) == false )
  {
    nom::DialogMessageBox(  "Critical Error",
                            "Could not load font file:" +
                            this->config.getString("GUI_TEXT_FONT") );
    return false;
  }

  Rocket::Core::DecoratorInstancer* decorator0 = new nom::DecoratorInstancerFinalFantasyFrame();
  Rocket::Core::Factory::RegisterDecoratorInstancer("final-fantasy-theme", decorator0 );
  decorator0->RemoveReference();

  DecoratorInstancerSprite* decorator1 = new nom::DecoratorInstancerSprite();
  Rocket::Core::Factory::RegisterDecoratorInstancer("sprite-sheet", decorator1 );
  decorator1->RemoveReference();

  // Commence the loading of game resources

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    this->cursor =
      nom::AnimatedSprite( this->config.getString("INTERFACE_CURSOR_ATLAS") );

    if( this->card_font.load ( this->config.getString("CARD_FONTFACE") ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.getString("CARD_FONTFACE") );
      return false;
    }
  #else
    this->cursor =
      nom::AnimatedSprite( this->config.getString("INTERFACE_CURSOR_ATLAS_SCALE2X") );

    if( this->card_font.load ( this->config.getString("CARD_FONTFACE_SCALE2X") ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.getString("CARD_FONTFACE_SCALE2X") );
      return false;
    }
  #endif

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->background.load( this->config.getString("BOARD_BACKGROUND"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("BOARD_BACKGROUND") );
    }
  #else
    if( this->background.load( this->config.getString("BOARD_BACKGROUND_SCALE2X"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("BOARD_BACKGROUND_SCALE2X") );
    }
  #endif

  if( this->scoreboard_font.load( this->config.getString("SCORE_FONTFACE") ) == false )
  {
    NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.getString("SCORE_FONTFACE") );
    return false;
  }
  this->scoreboard_font.set_outline(1);

  if( this->gameover_font.load( this->config.getString("GAMEOVER_FONTFACE") ) == false )
  {
    NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.getString("GAMEOVER_FONTFACE") );
    return false;
  }
  this->gameover_font.set_outline(1);

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->gameover_background.load( this->config.getString("GAMEOVER_BACKGROUND"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_ERR( TTCARDS, "Could not load resource file: " + this->config.getString("GAMEOVER_BACKGROUND") );
      return false;
    }
  #else
    if( this->gameover_background.load( this->config.getString("GAMEOVER_BACKGROUND_SCALE2X"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_ERR( TTCARDS, "Could not load resource file: " + this->config.getString("GAMEOVER_BACKGROUND_SCALE2X") );
      return false;
    }
  #endif

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->cursor.load( this->config.getString("INTERFACE_CURSOR"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_ERR( TTCARDS, "Could not load resource file: " + this->config.getString("INTERFACE_CURSOR") );
      return false;
    }
  #else
    if( this->cursor.load( this->config.getString("INTERFACE_CURSOR_SCALE2X"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_ERR( TTCARDS, "Could not load resource file: " + this->config.getString("INTERFACE_CURSOR_SCALE2X") );
      return false;
    }
  #endif

  if ( this->collection.load( this->config.getString("CARDS_DB") ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.getString("CARDS_DB") );
    return false;
  }

  if ( this->card.load( &this->config, this->card_font ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load CardView renderer" );
    return false;
  }

  // Quick and dirty method of toggling the use of nomlib's audio subsystem
  // #undef NOM_USE_OPENAL

  // Initialize audio subsystem...
  #if defined( NOM_USE_OPENAL )
    this->audio_dev_.reset( new nom::AudioDevice() );
    this->listener_.reset( new nom::Listener() );
  #else
    this->audio_dev_.reset( new nom::NullAudioDevice() );
    this->listener_.reset( new nom::NullListener() );
  #endif // defined NOM_USE_OPENAL

  for( auto idx = 0; idx != NUM_SOUND_BUFFERS; ++idx )
  {
    #if defined( NOM_USE_OPENAL )
      this->sound_buffers[idx].reset( new nom::SoundBuffer() );
    #else
      this->sound_buffers[idx].reset( new nom::NullSoundBuffer() );
    #endif // defined NOM_USE_OPENAL
  }

  // Load optional audio resources
  if ( this->sound_buffers[0]->load( this->config.getString("CURSOR_MOVE") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("CURSOR_MOVE") );
  }

  if ( this->sound_buffers[1]->load( this->config.getString("CURSOR_CANCEL") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("CURSOR_CANCEL") );
  }

  if ( this->sound_buffers[2]->load( this->config.getString("CURSOR_WRONG") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("CURSOR_WRONG") );
  }

  if ( this->sound_buffers[3]->load( this->config.getString("CARD_PLACE") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("CARD_PLACE") );
  }

  if ( this->sound_buffers[4]->load( this->config.getString("CARD_FLIP") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("CARD_FLIP") );
  }

  if ( this->sound_buffers[5]->load( this->config.getString("SFX_LOAD_GAME") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("SFX_LOAD_GAME") );
  }

  if ( this->sound_buffers[6]->load( this->config.getString("SFX_SAVE_GAME") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("SFX_SAVE_GAME") );
  }

  if ( this->sound_buffers[7]->load( this->config.getString("MUSIC_TRACK") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("MUSIC_TRACK") );
  }

  if ( this->sound_buffers[8]->load( this->config.getString("MUSIC_WIN_TRACK") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("MUSIC_WIN_TRACK") );
  }

  #if defined( NOM_USE_OPENAL )
    this->cursor_move.reset( new nom::Sound() );
    this->cursor_cancel.reset( new nom::Sound() );
    this->cursor_wrong.reset( new nom::Sound() );
    this->card_place.reset( new nom::Sound() );
    this->card_flip.reset( new nom::Sound() );
    this->load_game.reset( new nom::Sound() );
    this->save_game.reset( new nom::Sound() );
    this->music_track.reset( new nom::Music() );
    this->winning_track.reset( new nom::Music() );

    this->cursor_move->setBuffer( *this->sound_buffers[0] );
    this->cursor_cancel->setBuffer( *this->sound_buffers[1] );
    this->cursor_wrong->setBuffer( *this->sound_buffers[2] );
    this->card_place->setBuffer( *this->sound_buffers[3] );
    this->card_flip->setBuffer( *this->sound_buffers[4] );
    this->load_game->setBuffer( *this->sound_buffers[5] );
    this->save_game->setBuffer( *this->sound_buffers[6] );
    this->music_track->setBuffer( *this->sound_buffers[7] );
    this->winning_track->setBuffer( *this->sound_buffers[8] );
  #else
    this->cursor_move.reset( new nom::NullSound() );
    this->cursor_cancel.reset( new nom::NullSound() );
    this->cursor_wrong.reset( new nom::NullSound() );
    this->card_place.reset( new nom::NullSound() );
    this->card_flip.reset( new nom::NullSound() );
    this->load_game.reset( new nom::NullSound() );
    this->save_game.reset( new nom::NullSound() );
    this->music_track.reset( new nom::NullMusic() );
    this->winning_track.reset( new nom::NullMusic() );
  #endif // defined NOM_USE_OPENAL

  this->music_track->Play();

  #if defined (NOM_DEBUG)
    this->music_track->Pause();
  #endif

  this->game->input_mapper.clear();

  nom::InputActionMapper state;

  nom::EventCallback quit_game( [&] ( const nom::Event& evt ) { this->on_app_quit( evt ); } );
  nom::EventCallback on_window_resized( [&] ( const nom::Event& evt ) { this->game->on_window_resized( evt ); } );
  nom::EventCallback fps_counter( [&] ( const nom::Event& evt ) { this->game->toggle_fps(); } );
  nom::EventCallback pause_music( [&] ( const nom::Event& evt ) { this->game->pause_music(); } );
  nom::EventCallback mute_volume( [&] ( const nom::Event& evt ) { this->game->mute_volume(); } );
  nom::EventCallback save_screenshot( [&] ( const nom::Event& evt ) { this->game->save_screenshot(); } );
  nom::EventCallback reload_config( [&] ( const nom::Event& evt ) { this->game->reload_config(); } );

  // Debugging helpers

  nom::EventCallback jumpto_confirmation_dialog_state( [&] ( const nom::Event& evt )
    {
      this->set_state( Game::State::ConfirmationDialog );
    }
  );

  nom::EventCallback jumpto_gameover_state( [&] ( const nom::Event& evt )
    {
      nom::uint32_ptr player1_win = new nom::uint32 (GameOverType::Won);
      this->set_state( Game::State::GameOver, player1_win );
    }
  );
  nom::EventCallback dump_board( [&] ( const nom::Event& evt ) { this->game->dump_board(); } );
  nom::EventCallback dump_player0_hand( [&] ( const nom::Event& evt ) { this->game->dump_hand(0); } );
  nom::EventCallback dump_player1_hand( [&] ( const nom::Event& evt ) { this->game->dump_hand(1); } );
  nom::EventCallback dump_collection( [&] ( const nom::Event& evt ) { this->game->dump_collection(); } );

  // Conflicts with ConfirmationDialogState key binding
  // state.insert( "quit_game", nom::KeyboardAction( SDL_KEYDOWN, SDLK_ESCAPE ), quit_game );
  state.insert( "quit_game", nom::KeyboardAction( SDL_KEYDOWN, SDLK_q ), quit_game );

  #if defined( NOM_PLATFORM_OSX )
    state.insert( "on_window_resize", nom::KeyboardAction( SDL_KEYDOWN, SDLK_f, KMOD_LGUI ), on_window_resized );
  #else
    state.insert( "on_window_resize", nom::KeyboardAction( SDL_KEYDOWN, SDLK_f, KMOD_LCTRL ), on_window_resized );
  #endif

  state.insert( "fps_counter", nom::KeyboardAction( SDL_KEYDOWN, SDLK_BACKSLASH ), fps_counter );
  state.insert( "pause_music", nom::KeyboardAction( SDL_KEYDOWN, SDLK_m, KMOD_LSHIFT ), pause_music );
  state.insert( "mute_volume", nom::KeyboardAction( SDL_KEYDOWN, SDLK_m ), mute_volume );
  state.insert( "save_screenshot", nom::KeyboardAction( SDL_KEYDOWN, SDLK_F1 ), save_screenshot );
  state.insert( "reload_config", nom::KeyboardAction( SDL_KEYDOWN, SDLK_r ), reload_config );

  // TODO: Change to:
  // #if ! defined( NDEBUG ) // Debug build
  #if defined( TTCARDS_DEBUG_GAME_STATE )
    state.insert( "jumpto_confirmation_dialog_state", nom::KeyboardAction( SDL_KEYDOWN, SDLK_0, KMOD_LGUI ), jumpto_confirmation_dialog_state );
    state.insert( "jumpto_gameover_state", nom::KeyboardAction( SDL_KEYDOWN, SDLK_0 ), jumpto_gameover_state );

    state.insert( "dump_player0_hand", nom::KeyboardAction( SDL_KEYDOWN, SDLK_LEFTBRACKET ), dump_player0_hand );
    state.insert( "dump_player1_hand", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RIGHTBRACKET ), dump_player1_hand );
    state.insert( "dump_board", nom::KeyboardAction( SDL_KEYDOWN, SDLK_LEFTBRACKET, KMOD_LGUI ), dump_board );
    state.insert( "dump_collection", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RIGHTBRACKET, KMOD_LGUI ), dump_collection );
  #endif

  this->game->input_mapper.insert( "Game", state, true );

  return true;
}

void Game::on_event( const nom::Event& ev )
{
  // First, our own events (global key bindings):
  SDLApp::on_event( ev );

  // Then, key, mouse, joystick, etc. events registered for the current input
  // context:
  this->input_mapper.on_event( ev );
}

void Game::on_window_resized( const nom::Event& ev )
{
  this->window.toggle_fullscreen();
}

int32_t Game::Run( void )
{
  unsigned int loops = 0;
  unsigned int next_game_tick = 0;
  nom::uint32 delta_time = 0;
  nom::uint32 delay_time = this->config.getInteger("DELAY_TIME");

  this->fps.start();

  next_game_tick = this->ticks();

  this->set_state( Game::State::CardsMenu );

  while ( this->running() == true )
  {
    loops = 0;
    delta_time = this->ticks();

    while( delta_time > next_game_tick && loops <= MAX_FRAMESKIP )
    {
      nom::Event evt;
      while( this->poll_event( evt ) )
      {
        this->on_event( evt );
      }

      this->fps.update();

      this->on_update( delta_time );

      // Fix for GitHub Issue #9
      this->window.fill(nom::Color4i::Black);

      this->on_draw( this->window );

      if ( this->show_fps() )
      {
        this->window.set_window_title ( APP_NAME + " " + "-" + " " + ( this->fps.asString() ) + " " + "fps" );
      }
      else
      {
        this->window.set_window_title ( APP_NAME );
      }

      next_game_tick += SKIP_TICKS;
      loops++;

      // Sleep away spare tick; improve CPU usage dramatically.
      //
      // See config.json notes on DELAY_TIME.
      if( this->fps.fps() >= TICKS_PER_SECOND ) {
        nom::sleep(delay_time);
      }
    }
  }

  return NOM_EXIT_SUCCESS;
}

void Game::set_state( nom::uint32 id, nom::void_ptr data )
{
  switch ( id )
  {
    default: /* Ignore unknown identifiers */ break;

    case Game::State::CardsMenu:
    {
      this->state()->set_state( CardsMenuStatePtr( new CardsMenuState( this->game ) ), data );
      break;
    }

    case Game::State::Play:
    {
      this->state()->set_state( PlayStatePtr( new PlayState( this->game ) ), data );
      break;
    }

    case Game::State::GameOver:
    {
      this->state()->set_state( GameOverStatePtr( new GameOverState( this->game, data ) ), data );
      break;
    }

    case Game::State::Pause:
    {
      this->state()->set_state( PauseStatePtr( new PauseState( this->game ) ), data );
      break;
    }

    case Game::State::ConfirmationDialog:
    {
      this->state()->set_state( ConfirmationDialogStatePtr( new ConfirmationDialogState( this->game ) ), data );
      break;
    }
  }
}

void Game::pause_music( void )
{
  this->game->music_track->togglePause();
  this->game->winning_track->togglePause();
}

void Game::mute_volume( void )
{
  // Global volume level
  float current_volume = this->game->listener_->getVolume();

  if( current_volume >= 100.0 )
  {
    this->game->listener_->setVolume( 0.0 );
  }
  else if( current_volume <= 0.0 )
  {
    this->game->listener_->setVolume( 100.0 );
  }
}

void Game::save_screenshot( void )
{
  nom::Path p;
  std::string filename = TTCARDS_DATA_DIR + p.native() + "Screenshot.png";

  if( this->game->window.save_screenshot( filename ) == false )
  {
    NOM_LOG_ERR( TTCARDS, "Could not save screen-shot: " + filename );
  }
}

void Game::reload_config( void )
{
  if( this->game->config.load( TTCARDS_CONFIG_FILENAME ) == false )
  {
    NOM_LOG_ERR( TTCARDS, "Could not reload configuration file at: " + TTCARDS_CONFIG_FILENAME );
    return;
  }

  NOM_LOG_INFO( TTCARDS, "Reloaded game configuration file: " + TTCARDS_CONFIG_FILENAME );

  // Prevents states crash
  if( this->state() != nullptr && this->state()->current_state_id() != Game::State::CardsMenu ) {
    this->set_state( Game::State::CardsMenu );
  }
}

void Game::dump_board( void )
{
  this->game->board.list();
}

void Game::dump_hand( nom::uint32 player_id )
{
  this->game->debug.ListCards( this->game->hand[player_id].cards );
}

void Game::dump_collection( void )
{
  this->game->debug.ListCards( this->game->collection.cards );
}

namespace tt {

void free_game ( Game* game )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );

  // Fixes double delete issues that result otherwise
  //if ( game != nullptr ) delete game;
}

} // namespace tt
