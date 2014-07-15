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

#include "Game.hpp"

#include "states/GameOverState.hpp"
#include "states/ContinueMenuState.hpp"
#include "states/PauseState.hpp"
#include "states/CardsMenuState.hpp"
#include "states/PlayState.hpp"

//#include "states/States.hpp" // StateFactory

using namespace nom;

Game::Game ( void ) :
  game { this, tt::free_game }
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );
}

Game::Game ( nom::int32 argc, char* argv[] ) :
  // SDLApp( OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS | OSX_DISABLE_FULLSCREEN_SPACES | INIT_ENGINE_FONTS ),
  SDLApp( OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS | OSX_DISABLE_FULLSCREEN_SPACES ),
  game { this, tt::free_game }
{
  std::ostringstream ttcards_version;

  ttcards_version << APP_NAME << " " << "v" << TTCARDS_VERSION_MAJOR << "."
      << TTCARDS_VERSION_MINOR << "." << TTCARDS_VERSION_PATCH << "-d"
      << std::endl;

  //this->state_factory = new States();

  // Destination directory we descend into to locate game resources
  std::string working_directory;
  nom::File dir;

  if ( nom::init_third_party(0) == false )
  {
    nom::DialogMessageBox ( "Critical Error", "Could not load third party libraries" );
    exit(NOM_EXIT_FAILURE);
  }

  atexit(nom::quit); // Clean up memory associated with nomlib

  #if ! defined( NDEBUG )  // Debug target build
    NOM_LOG_INFO( TTCARDS, "DEBUG build" );

    // Enable info log level and greater for our engine (deprecated)
    nom::SDL2Logger::set_logging_priority( NOM, SDL_LOG_PRIORITY_INFO );

    // Enable logging of all messages in the game
    nom::SDL2Logger::set_logging_priority( TTCARDS, SDL_LOG_PRIORITY_VERBOSE );

    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_DEBUG );
    // nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_TEST, SDL_LOG_PRIORITY_DEBUG );

    // Disable logging of all messages from GameConfig except for fatal errs.
    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_CFG, SDL_LOG_PRIORITY_ERROR );

    // Enable logging of game state function traces
    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_TRACE_STATES, SDL_LOG_PRIORITY_DEBUG );

  #else // NDEBUG -- release target build

    NOM_LOG_INFO( TTCARDS, "RELEASE build" );

    // Disable logging of all messages but critical log level (these are fatal)
    // under the scope of our engine, nomlib.
    nom::SDL2Logger::set_logging_priority( NOM, SDL_LOG_PRIORITY_CRITICAL );

    // Disable logging of all messages but critical log level (these are fatal)
    // under the scope of the game.
    nom::SDL2Logger::set_logging_priority( TTCARDS, SDL_LOG_PRIORITY_CRITICAL );

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
      else if ( std::string(argv[opt]) == "--config" ) // Save configuration opt
      {
        if ( argv[opt + 1] == nullptr )
        {
          NOM_LOG_ERR ( TTCARDS, "Missing parameter <output_filename> for config argument." );
          exit ( NOM_EXIT_FAILURE );
        }
      #if ! defined( NOM_DEBUG ) // Allow overwriting of file I/O if in dev mode
        else if ( dir.exists ( argv[opt + 1] ) == true )
        {
          NOM_LOG_ERR ( TTCARDS, "File path for <output_filename> already exists at: " + std::string(argv[opt + 1]) );
          exit ( NOM_EXIT_FAILURE );
        }
      #endif

        GameConfig cfg;
        if ( cfg.load ( TTCARDS_CONFIG_FILENAME ) == false )
        {
          NOM_LOG_ERR ( TTCARDS, "Could not load game configuration from file at: " + TTCARDS_CONFIG_FILENAME );
          exit ( NOM_EXIT_FAILURE );
        }

        if ( cfg.save ( argv[opt + 1] ) == false )
        {
          NOM_LOG_ERR ( TTCARDS, "Could not save game configuration to file at: " + std::string(argv[opt + 1]) );
          exit ( NOM_EXIT_FAILURE );
        }
        else
        {
          NOM_LOG_INFO ( TTCARDS, "Game configuration successfully saved at: " + std::string(argv[opt + 1]) );
          exit ( NOM_EXIT_SUCCESS );
        }
      } // end save config option
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

Game::~Game ( void )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );
}

bool Game::on_init ( void )
{
  nom::Rectangle rectangle  ( IntRect( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT),
                              nom::Color4i::Gray
                            );

  nom::uint32 video_flags = 0;
  /* TODO: Use me when fixed-time-step is fully implemented
  nom::uint32 renderer_flags = SDL_RENDERER_PRESENTVSYNC;
  TODO */

  if ( this->config.load ( TTCARDS_CONFIG_FILENAME ) == false )
  {
    nom::DialogMessageBox ( "Critical Error", "Could not load configuration file at: " + TTCARDS_CONFIG_FILENAME );
    exit ( NOM_EXIT_FAILURE );
  }

  // Obtain a list of available video modes so we can determine how to render
  // the game (scale factors, positioning, etc.).
  nom::VideoModeList modes = this->window.getVideoModes();

  for ( nom::uint32 idx = 0; idx != modes.size(); idx++ )
  {
    std::cout << modes[idx] << std::endl;
  }

  // Original scale (384x224)
  //this->window.create ( "TTcards", SCREEN_WIDTH * 1, SCREEN_HEIGHT * 1, video_flags );

  // Enhanced scaling (784x448)
  this->window.create ( "TTcards", SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, video_flags );

  // Almost-HD resolution scaling (1152x672)
  //this->window.create ( "TTcards", SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, video_flags );

  this->window.set_window_icon ( this->config.getString("APP_ICON") );
//this->window.set_window_title ( LOADING_TEXT );

  this->window.set_logical_size( SCREEN_WIDTH, SCREEN_HEIGHT );

  // Top level (parent) widget; coordinates are relative to our game's
  // resolution (window)
  this->gui_window_ = new nom::UIWidget (
                                          nom::Point2i::zero,
                                          this->window.size()
                                        );

  // Commence the initialization of game objects
  this->menu_elements = nom::SpriteBatch ( "images/menu_elements.json" );
  this->cursor = nom::AnimatedSprite( "images/cursors.json" );
  this->cursor.set_position ( Point2i(MENU_CARDS_CURSOR_ORIGIN_X, MENU_CARDS_CURSOR_ORIGIN_Y) );

  // Commence the loading of game resources
  if( this->info_text.load( this->config.getString("INFO_FONTFACE") ) == false )
  {
    NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.getString("INFO_FONTFACE") );
    return false;
  }

  if( this->info_small_text.load( this->config.getString("INFO_SMALL_FONTFACE") ) == false )
  {
    NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.getString("INFO_SMALL_FONTFACE") );
    return false;
  }

  if( this->card_font.load ( this->config.getString("CARD_FONTFACE") ) == false )
  {
    NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.getString("CARD_FONTFACE") );
    return false;
  }

  if( this->menu_elements.load( this->config.getString("MENU_ELEMENTS"), false, nom::Texture::Access::Streaming ) == false )
  {
    NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.getString("MENU_ELEMENTS") );
    return false;
  }

  if( this->background.load( this->config.getString("BOARD_BACKGROUND"), false, nom::Texture::Access::Streaming ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("BOARD_BACKGROUND") );
    rectangle.draw ( this->window );
  }
  else
  {
    if ( this->config.getString("SCALE_ALGORITHM") == "scale2x" )
    {
      this->background.resize( nom::Texture::ResizeAlgorithm::scale2x );
    }
    else if ( this->config.getString("SCALE_ALGORITHM") == "hqx" )
    {
      this->background.resize( nom::Texture::ResizeAlgorithm::hq2x );
    }

//this->background.draw( this->window );
  }

//this->window.update();

  // Possible bug in SDL for OS X
  //
  // We are not able to see the screen update of anything we do here unless we
  // poll for an events afterwards. Oddly enough, this issue only happens when
  // we are launching from an application bundle under OS X -- normal binaries
  // do not need this workaround.
//this->PollEvents( &event );

  if( this->scoreboard_font.load( this->config.getString("SCORE_FONTFACE") ) == true )
  {
    this->scoreboard_font.set_outline ( 1 );
  }
  else
  {
    NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.getString("SCORE_FONTFACE") );
    return false;
  }

  if( this->gameover_font.load( this->config.getString("GAMEOVER_FONTFACE") ) == true )
  {
    this->gameover_font.set_outline ( 1 );
  }
  else
  {
    NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.getString("GAMEOVER_FONTFACE") );
    return false;
  }

  if ( this->gameover_background.load( this->config.getString("GAMEOVER_BACKGROUND"), false, nom::Texture::Access::Streaming ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.getString("GAMEOVER_BACKGROUND") );
    return false;
  }

  if ( this->cursor.load( this->config.getString("INTERFACE_CURSOR"), false, nom::Texture::Access::Streaming ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.getString("INTERFACE_CURSOR") );
    return false;
  }

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

  // Rescale our game resources if necessary.
  if ( this->config.getString("SCALE_ALGORITHM") == "scale2x" )
  {
    // FIXME: (see nomlib's "feature/Image_Resize" branch):
    // this->info_text.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    // this->info_text_gray.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    // this->info_small_text.resize ( nom::Texture::ResizeAlgorithm::scale2x );

    this->cursor.resize( nom::Texture::ResizeAlgorithm::scale2x );
    this->menu_elements.resize( nom::Texture::ResizeAlgorithm::scale2x );
    this->gameover_background.resize( nom::Texture::ResizeAlgorithm::scale2x );
  }
  else if ( this->config.getString("SCALE_ALGORITHM") == "hqx" )
  {
    // FIXME: (see nomlib's "feature/Image_Resize" branch):
    // this->info_text.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    // this->info_text_gray.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    // this->info_small_text.resize ( nom::Texture::ResizeAlgorithm::hq2x );

    this->cursor.resize( nom::Texture::ResizeAlgorithm::hq2x );
    this->menu_elements.resize( nom::Texture::ResizeAlgorithm::hq2x );
    this->gameover_background.resize( nom::Texture::ResizeAlgorithm::hq2x );
  }

  // Load optional audio resources
  if ( this->sound_buffers[0].load ( this->config.getString("CURSOR_MOVE") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("CURSOR_MOVE") );
  }

  if ( this->sound_buffers[1].load ( this->config.getString("CURSOR_CANCEL") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("CURSOR_CANCEL") );
  }

  if ( this->sound_buffers[2].load ( this->config.getString("CURSOR_WRONG") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("CURSOR_WRONG") );
  }

  if ( this->sound_buffers[3].load ( this->config.getString("CARD_PLACE") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("CARD_PLACE") );
  }

  if ( this->sound_buffers[4].load ( this->config.getString("CARD_FLIP") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("CARD_FLIP") );
  }

  if ( this->sound_buffers[5].load ( this->config.getString("SFX_LOAD_GAME") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("SFX_LOAD_GAME") );
  }

  if ( this->sound_buffers[6].load ( this->config.getString("SFX_SAVE_GAME") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("SFX_SAVE_GAME") );
  }

  if ( this->sound_buffers[7].load ( this->config.getString("MUSIC_TRACK") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("MUSIC_TRACK") );
  }

  if ( this->sound_buffers[8].load ( this->config.getString("MUSIC_WIN_TRACK") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.getString("MUSIC_WIN_TRACK") );
  }

  this->cursor_move.setBuffer ( this->sound_buffers[0] );
  this->cursor_cancel.setBuffer ( this->sound_buffers[1] );
  this->cursor_wrong.setBuffer ( this->sound_buffers[2] );
  this->card_place.setBuffer ( this->sound_buffers[3] );
  this->card_flip.setBuffer ( this->sound_buffers[4] );
  this->load_game.setBuffer ( this->sound_buffers[5] );
  this->save_game.setBuffer ( this->sound_buffers[6] );
  this->music_track.setBuffer ( this->sound_buffers[7] );
  this->winning_track.setBuffer ( this->sound_buffers[8] );

  this->music_track.Play();

#if defined (NOM_DEBUG)
  this->music_track.Pause();
#endif
  return true;
}

void Game::on_key_down( const nom::Event& ev )
{
  switch ( ev.key.sym )
  {
    default: break;

#if defined (NOM_DEBUG)
    case SDLK_0:
    {
      if ( ev.key.mod == KMOD_LGUI )
      {
        this->set_state( Game::State::ContinueMenu );
      }
      else
      {
        nom::uint32_ptr player1_win = new nom::uint32 (GameOverType::Won);
        this->set_state( Game::State::GameOver, player1_win );
      }
    }
    break;
#endif

    case SDLK_ESCAPE:
    case SDLK_q: this->on_app_quit( ev ); break;

    // Audio control
    case SDLK_m:
    {
      if ( ev.key.mod == KMOD_LSHIFT ) // Pause music
      {
        this->music_track.togglePause();
        this->winning_track.togglePause();
        break;
      }

      // Global volume level in game
      float current_volume = this->listener.getVolume();
      if ( current_volume >= 100.0 )
      {
        this->listener.setVolume ( 0.0 );
      }
      else if ( current_volume <= 0.0 )
      {
        this->listener.setVolume ( 100.0 );
      }

      break;
    }

    case SDLK_BACKSLASH: this->toggle_fps(); break;

    case SDLK_f:
    {
      // Platform dependent key bindings for full-screen toggling:
      //
      //  Mac OS X: Command + Control + F, Command + F
      //  Linux, Windows & all other platforms: Control + F
      #if defined ( NOM_PLATFORM_OSX )
        if ( ev.key.mod == KMOD_LGUI || ( ev.key.mod & KMOD_LCTRL && ev.key.mod & KMOD_LGUI ) )
        {
          this->on_window_resized( ev );
          break;
        }
      #else
        if ( ev.key.mod == KMOD_LCTRL )
        {
          this->on_window_resized( ev );
        }
        break;
      #endif

      break;
    }
    case SDLK_F1:
    {
      nom::Path p;
      std::string screenshot_filename = TTCARDS_DATA_DIR + p.native() + "Screenshot.png";

      if ( this->window.save_screenshot( screenshot_filename ) == false )
      {
        NOM_LOG_ERR ( TTCARDS, "Could not save screenshot" );
        break;
      }
    }
    break;

    case SDLK_r: // Start a new game
    {
      if ( this->config.load ( TTCARDS_CONFIG_FILENAME ) == false )
      {
NOM_LOG_ERR ( TTCARDS, "Could not reload configuration file at: " + TTCARDS_CONFIG_FILENAME );
        break;
      }

      this->set_state( Game::State::CardsMenu );
      break;
    }

    case SDLK_LEFTBRACKET:
    {
      if ( ev.key.mod == KMOD_LGUI )
        this->board.list();
      else
        this->debug.ListCards ( this->hand[0].cards );
    }
    break;

    case SDLK_RIGHTBRACKET:
    {
      if ( ev.key.mod == KMOD_LGUI )
        this->debug.ListCards ( this->collection.cards );
      else
        this->debug.ListCards ( this->hand[1].cards );
    }
    break;
  }
}

void Game::on_window_resized( const nom::Event& ev )
{
  this->window.toggle_fullscreen();

  // FIXME: This is a temporary patch until we figure out why our window does
  // not automatically resize itself to previous dimensions after entering
  // full-screen.
  //
  // This particular bug only happens on the Windows platform, and does nothing
  // on OS X! Could this be related to GitHub Issue #9 ..?
  #if defined (NOM_PLATFORM_WINDOWS)
    this->window.set_size ( SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2 );
  #endif
}

int32_t Game::Run ( void )
{
  unsigned int loops = 0;
  unsigned int next_game_tick = 0;
  nom::uint32 delta_time = 0;

  this->fps.start();

  next_game_tick = this->ticks();

  this->set_state( Game::State::CardsMenu );

  while ( this->running() == true )
  {
    loops = 0;
    delta_time = this->ticks();

    while ( this->ticks() > next_game_tick && loops <= MAX_FRAMESKIP )
    {
      while( this->poll_event( this->event ) )
      {
        this->on_event( this->event );
      }

      this->fps.update();

      this->on_update ( delta_time );

      // Fix for GitHub Issue #9
      this->window.fill(nom::Color4i::Black);

      this->on_draw ( this->window );

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

      // Sleep away the spare ticks (improves CPU usage dramatically)
      nom::sleep(50);
    }
  }

  return NOM_EXIT_SUCCESS;
}

void Game::set_state ( nom::uint32 id, nom::void_ptr data )
{
  switch ( id )
  {
    default: /* Ignore unknown identifiers */ break;

    case Game::State::CardsMenu:
    {
      SDLApp::set_state( CardsMenuStatePtr( new CardsMenuState( this->game ) ), data );
      break;
    }

    case Game::State::Play:
    {
      SDLApp::set_state( PlayStatePtr( new PlayState( this->game ) ), data );
      break;
    }

    case Game::State::GameOver:
    {
      SDLApp::set_state( GameOverStatePtr( new GameOverState( this->game, data ) ), data );
      break;
    }

    case Game::State::Pause:
    {
      this->push_state( PauseStatePtr( new PauseState( this->game ) ), data );
      break;
    }

    case Game::State::ContinueMenu:
    {
      this->push_state( ContinueMenuStatePtr( new ContinueMenuState( this->game ) ), data );
      break;
    }
  }
}

namespace tt {

void free_game ( Game* game )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );

  // Fixes double delete issues that result otherwise
  //if ( game != nullptr ) delete game;
}

} // namespace tt
