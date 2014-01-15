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
#include <cstdlib>

#ifdef EMSCRIPTEN
  #include <emscripten.h>
#endif

#include "Game.hpp"

#include "states/GameOverState.hpp"
#include "states/ContinueMenuState.hpp"
#include "states/PauseState.hpp"
#include "states/CardsMenuState.hpp"
#include "states/PlayState.hpp"

//#include "states/States.hpp" // StateFactory

Game::Game ( void ) :
  game { this, tt::free_game }
{
  NOM_LOG_TRACE(NOM);
}

Game::Game ( nom::int32 argc, char* argv[] )  :
  game { this, tt::free_game }
{
  //this->state_factory = new States();

  // Destination directory we descend into to locate game resources
  std::string working_directory;
  nom::File dir;

  NOM_LOG_TRACE( TTCARDS );

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
        else if ( dir.exists ( argv[opt + 2] ) == true )
        {
NOM_LOG_ERR ( TTCARDS, "File path for <output_filename> already exists at: " + std::string(argv[opt + 2]) );
          exit ( NOM_EXIT_FAILURE );
        }
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
        else if ( dir.exists ( argv[opt + 1] ) == true )
        {
NOM_LOG_ERR ( TTCARDS, "File path for <output_filename> already exists at: " + std::string(argv[opt + 1]) );
          exit ( NOM_EXIT_FAILURE );
        }

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
        std::cout << APP_NAME << " version " << TTCARDS_VERSION_MAJOR << "." << TTCARDS_VERSION_MINOR << "." << TTCARDS_VERSION_PATCH << " by Jeffrey Carpenter" << std::endl;
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

// These definitions are influenced at build time with CMake options and serve
// to help determine the path of game resources
#if defined (NOM_PLATFORM_OSX)
  // We only do "OS X Application Bundle" build types under Mac OS X platform
  working_directory = nom::getBundleResourcePath();
#elif defined (NOM_PLATFORM_WINDOWS)
  // Assume the Resources subdirectory is in a sub-directory of wherever the
  // executable is launched from
  working_directory = dir.path(argv[0]) + path.native() + "Resources" + path.native();
#else // Potentially customized layout (POSIX hierarchy by default)
  // FIXME
  working_directory = TTCARDS_INSTALL_PREFIX + path.native() + "share" + path.native() + "ttcards" + path.native() + "Resources";
#endif

  // Change the working directory to whatever working_directory has been set to
  //
  // Note that it is important that we do not mess with the working directory
  // path until after our command line arguments have been processed, so that we
  // do not unintentionally mess up relative paths!
  dir.set_path (working_directory);

  if ( nom::init_third_party(0) == false )
  {
    nom::DialogMessageBox ( "Critical Error", "Could not load third party libraries" );
    exit(NOM_EXIT_FAILURE);
  }

  atexit(nom::quit); // Clean up memory associated with nomlib
}

Game::~Game ( void )
{
  NOM_LOG_TRACE ( TTCARDS );
}

bool Game::on_init ( void )
{
  nom::Rectangle rectangle  ( nom::Coords ( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT ),
                              nom::Color4i::Gray
                            );

  nom::uint32 video_flags = 0;
  /* TODO: Use me when fixed-time-step is fully implemented
  nom::uint32 renderer_flags = SDL_RENDERER_PRESENTVSYNC;
  TODO */

  if ( this->game->config.load ( TTCARDS_CONFIG_FILENAME ) == false )
  {
    nom::DialogMessageBox ( "Critical Error", "Could not load configuration file at: " + TTCARDS_CONFIG_FILENAME );
    exit ( NOM_EXIT_FAILURE );
  }

  // Obtain a list of available video modes so we can determine how to render
  // the game (scale factors, positioning, etc.).
  nom::VideoModeList modes = this->game->window.getVideoModes();

  for ( nom::uint32 idx = 0; idx != modes.size(); idx++ )
  {
    std::cout << modes[idx] << std::endl;
  }

  this->game->window.create ( "TTcards", SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, video_flags );
  this->game->window.set_window_icon ( this->game->config.getString("APP_ICON") );
//this->game->window.set_window_title ( LOADING_TEXT );

  this->game->window.set_logical_size ( SCREEN_WIDTH, SCREEN_HEIGHT );

  // Commence the initialization of game objects
  this->game->menu_elements = nom::SpriteBatch ( "images/menu_elements.json" );
  this->game->cursor = nom::Cursor ( "images/cursors.json" );
  this->game->cursor.set_position ( MENU_CARDS_CURSOR_ORIGIN_X, MENU_CARDS_CURSOR_ORIGIN_Y );

  // Commence the loading of game resources
  if ( this->game->info_text.load ( this->game->config.getString("INFO_FONTFACE"), 0 ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("INFO_FONTFACE") );
    return false;
  }

  if ( this->game->info_small_text.load ( this->game->config.getString("INFO_SMALL_FONTFACE"), 0 ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("INFO_SMALL_FONTFACE") );
    return false;
  }

  if ( this->game->card_font.load ( this->game->config.getString("CARD_FONTFACE"), 0 ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("CARD_FONTFACE") );
    return false;
  }

  if ( this->game->menu_elements.load ( this->game->config.getString("MENU_ELEMENTS"), 0 ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("MENU_ELEMENTS") );
    return false;
  }

  if ( this->game->background.load( this->game->config.getString("BOARD_BACKGROUND"), 0 ) == false )
  {
NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("BOARD_BACKGROUND") );
    rectangle.draw ( this->game->window );
  }
  else
  {
    if ( this->game->config.getString("SCALE_ALGORITHM") == "scale2x" )
    {
      // FIXME: this->game->background.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    }
    else if ( this->game->config.getString("SCALE_ALGORITHM") == "hqx" )
    {
      // FIXME: this->game->background.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    }

//this->game->background.draw( this->game->window );
  }

//this->game->window.update();

  // Possible bug in SDL for OS X
  //
  // We are not able to see the screen update of anything we do here unless we
  // poll for an events afterwards. Oddly enough, this issue only happens when
  // we are launching from an application bundle under OS X -- normal binaries
  // do not need this workaround.
//this->PollEvents( &event );

  if ( this->game->scoreboard_font.load ( this->game->config.getString("SCORE_FONTFACE"), 0 ) == true )
  {
    this->game->scoreboard_font.set_outline ( 1 );
  }
  else
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("SCORE_FONTFACE") );
    return false;
  }

  if ( this->game->gameover_font.load ( this->game->config.getString("GAMEOVER_FONTFACE"), 0 ) == true )
  {
    this->game->gameover_font.set_outline ( 1 );
  }
  else
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("GAMEOVER_FONTFACE") );
    return false;
  }

  if ( this->game->gameover_background.load ( this->game->config.getString("GAMEOVER_BACKGROUND"), 0 ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("GAMEOVER_BACKGROUND") );
    return false;
  }

  if ( this->game->cursor.load ( this->game->config.getString("INTERFACE_CURSOR"), 0 ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("INTERFACE_CURSOR") );
    return false;
  }

  if ( this->game->collection.load( this->game->config.getString("CARDS_DB") ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("CARDS_DB") );
    return false;
  }

  if ( this->game->card.load ( &this->game->config, this->game->card_font ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load CardView renderer" );
    return false;
  }

  // Rescale our game resources if necessary.
  if ( this->game->config.getString("SCALE_ALGORITHM") == "scale2x" )
  {
/* FIXME
    this->game->info_text.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    this->game->info_text_gray.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    this->game->info_small_text.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    this->game->cursor.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    this->game->menu_elements.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    this->game->gameover_background.resize ( nom::Texture::ResizeAlgorithm::scale2x );
*/
  }
  else if ( this->game->config.getString("SCALE_ALGORITHM") == "hqx" )
  {
/* FIXME
    this->game->info_text.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    this->game->info_text_gray.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    this->game->info_small_text.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    this->game->cursor.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    this->game->menu_elements.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    this->game->gameover_background.resize ( nom::Texture::ResizeAlgorithm::hq2x );
*/
  }

  // Load optional audio resources
  if ( this->game->sound_buffers[0].load ( this->game->config.getString("CURSOR_MOVE") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("CURSOR_MOVE") );
  }

  if ( this->game->sound_buffers[1].load ( this->game->config.getString("CURSOR_CANCEL") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("CURSOR_CANCEL") );
  }

  if ( this->game->sound_buffers[2].load ( this->game->config.getString("CURSOR_WRONG") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("CURSOR_WRONG") );
  }

  if ( this->game->sound_buffers[3].load ( this->game->config.getString("CARD_PLACE") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("CARD_PLACE") );
  }

  if ( this->game->sound_buffers[4].load ( this->game->config.getString("CARD_FLIP") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("CARD_FLIP") );
  }

  if ( this->game->sound_buffers[5].load ( this->game->config.getString("SFX_LOAD_GAME") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("SFX_LOAD_GAME") );
  }

  if ( this->game->sound_buffers[6].load ( this->game->config.getString("SFX_SAVE_GAME") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("SFX_SAVE_GAME") );
  }

  if ( this->game->sound_buffers[7].load ( this->game->config.getString("MUSIC_TRACK") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("MUSIC_TRACK") );
  }

  if ( this->game->sound_buffers[8].load ( this->game->config.getString("MUSIC_WIN_TRACK") ) == false )
  {
    NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("MUSIC_WIN_TRACK") );
  }

  this->game->cursor_move.setBuffer ( this->game->sound_buffers[0] );
  this->game->cursor_cancel.setBuffer ( this->game->sound_buffers[1] );
  this->game->cursor_wrong.setBuffer ( this->game->sound_buffers[2] );
  this->game->card_place.setBuffer ( this->game->sound_buffers[3] );
  this->game->card_flip.setBuffer ( this->game->sound_buffers[4] );
  this->game->load_game.setBuffer ( this->game->sound_buffers[5] );
  this->game->save_game.setBuffer ( this->game->sound_buffers[6] );
  this->game->music_track.setBuffer ( this->game->sound_buffers[7] );
  this->game->winning_track.setBuffer ( this->game->sound_buffers[8] );

  this->game->music_track.Play();

#if defined (NOM_DEBUG)
  this->game->music_track.Pause();
#endif

  return true;
}

void Game::onKeyDown ( nom::int32 key, nom::int32 mod, nom::uint32 window_id )
{
  switch ( key )
  {
    default: break;

#if defined (NOM_DEBUG)
    case SDLK_0:
    {
      if ( mod == KMOD_LGUI )
      {
        this->game->set_state( Game::State::ContinueMenu );
      }
      else
      {
        nom::uint32_ptr player1_win = new nom::uint32 (GameOverType::Won);
        this->game->set_state( Game::State::GameOver, player1_win );
      }
    }
    break;
#endif

    case SDLK_ESCAPE:
    case SDLK_q: this->on_quit(); break;

    // Audio control
    case SDLK_m:
    {
      if ( mod == KMOD_LSHIFT ) // Pause music
      {
        this->game->music_track.togglePause();
        this->game->winning_track.togglePause();
        break;
      }

      // Global volume level in game
      float current_volume = this->game->listener.getVolume();
      if ( current_volume >= 100.0 )
      {
        this->game->listener.setVolume ( 0.0 );
      }
      else if ( current_volume <= 0.0 )
      {
        this->game->listener.setVolume ( 100.0 );
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
      if ( mod == KMOD_LGUI || ( mod & KMOD_LCTRL && mod & KMOD_LGUI ) ) this->onResize ( 0, 0 ); break;
      #else
      if ( mod == KMOD_LCTRL ) this->onResize ( 0, 0 ); break;
      #endif
      break;
    }
    case SDLK_F1:
    {
      nom::Path p;
      std::string screenshot_filename = TTCARDS_DATA_DIR + p.native() + "Screenshot.png";

      if ( this->game->window.save_screenshot( screenshot_filename ) == false )
      {
        NOM_LOG_ERR ( TTCARDS, "Could not save screenshot" );
        break;
      }
    }
    break;

    case SDLK_r: // Start a new game
    {
      if ( this->game->config.load ( TTCARDS_CONFIG_FILENAME ) == false )
      {
NOM_LOG_ERR ( TTCARDS, "Could not reload configuration file at: " + TTCARDS_CONFIG_FILENAME );
        break;
      }

      this->game->set_state( Game::State::CardsMenu );
      break;
    }

    case SDLK_LEFTBRACKET:
    {
      if ( mod == KMOD_LGUI )
        this->game->board.list();
      else
        this->game->debug.ListCards ( this->game->hand[0].cards );
    }
    break;

    case SDLK_RIGHTBRACKET:
    {
      if ( mod == KMOD_LGUI )
        this->game->debug.ListCards ( this->game->collection.cards );
      else
        this->game->debug.ListCards ( this->game->hand[1].cards );
    }
    break;
  }
}

void Game::onResize ( nom::int32 width, nom::int32 height )
{
  this->game->window.toggle_fullscreen();

  // FIXME: This is a temporary patch until we figure out why our window does
  // not automatically resize itself to previous dimensions after entering
  // full-screen.
  //
  // This particular bug only happens on the Windows platform, and does nothing
  // on OS X! Could this be related to GitHub Issue #9 ..?
  #if defined (NOM_PLATFORM_WINDOWS)
    this->game->window.set_size ( SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2 );
  #endif
}

int32_t Game::Run ( void )
{
  unsigned int loops = 0;
  unsigned int next_game_tick = 0;
  nom::uint32 delta_time = 0;

  this->fps.start();

  next_game_tick = this->ticks();

  this->game->set_state( Game::State::CardsMenu );

  while ( this->running() == true )
  {
    loops = 0;
    delta_time = this->ticks();

    while ( this->ticks() > next_game_tick && loops <= MAX_FRAMESKIP )
    {
      while ( this->poll_events ( &event ) )
      {
        this->on_event ( &event );
      }

      this->fps.update();

      this->on_update ( delta_time );

      // Fix for GitHub Issue #9
      this->game->window.fill(nom::Color4i::Black);

      this->on_draw ( this->game->window );

      if ( this->show_fps() )
      {
        this->game->window.set_window_title ( APP_NAME + " " + "-" + " " + ( this->fps.asString() ) + " " + "fps" );
      }
      else
      {
        this->game->window.set_window_title ( APP_NAME );
      }

      next_game_tick += SKIP_TICKS;
      loops++;
    }
  }

  return NOM_EXIT_SUCCESS;
}

void Game::set_state ( uint32 id, nom::void_ptr data )
{
  switch ( id )
  {
    default: /* Ignore unknown identifiers */ break;

    case State::CardsMenu:
    {
      SDLApp::set_state( CardsMenuStatePtr( new CardsMenuState( this->game ) ), data );
      break;
    }

    case State::Play:
    {
      SDLApp::set_state( PlayStatePtr( new PlayState( this->game ) ), data );
      break;
    }

    case State::GameOver:
    {
      SDLApp::set_state( GameOverStatePtr( new GameOverState( this->game, data ) ), data );
      break;
    }

    case State::Pause:
    {
      // TODO: explain why this is needed
/*
      if ( this->game->state_id() != Game::State::Pause && this->game->state_id() != Game::State::GameOver )
      {
        this->push_state( PauseStatePtr( new PauseState( this->game ) ), data );
      }
      else
      {
        // FIXME: Game crashes if we un-pause the game while in GameOverState;
        // by checking for the absence of the state is a workaround patch.
        this->pop_state();
      }
*/
      this->push_state( PauseStatePtr( new PauseState( this->game ) ), data );
      break;
    }

    case State::ContinueMenu:
    {
      SDLApp::push_state( ContinueMenuStatePtr( new ContinueMenuState( this->game ) ), data );
      break;
    }
  }
}

namespace tt {

void free_game ( Game* game )
{
  NOM_LOG_TRACE(TTCARDS);

  // Fixes double delete issues that result otherwise
  //if ( game != nullptr ) delete game;
}

} // namespace tt
