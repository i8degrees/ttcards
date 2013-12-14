/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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

using namespace nom;

App::App ( nom::int32 argc, char* argv[] )
{
  // Destination directory we descend into to locate game resources
  std::string working_directory;
  nom::File dir;

NOM_LOG_TRACE ( TTCARDS );

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
          exit ( EXIT_FAILURE );
        }
        else if ( dir.exists ( argv[opt + 1] ) == false )
        {
NOM_LOG_ERR ( TTCARDS, "File path for <input_filename> does not exist at: " + std::string(argv[opt + 1]) );
          exit ( EXIT_FAILURE );
        }
        else if ( cards.load( argv[opt + 1] ) == false )
        {
NOM_LOG_ERR ( TTCARDS, "Could not load game cards collection at: " + std::string(argv[opt + 1]) );
          exit ( EXIT_FAILURE );
        }

        if ( argv[opt + 2] == nullptr )
        {
NOM_LOG_ERR ( TTCARDS, "Missing parameter <output_filename> for export argument." );
          exit ( EXIT_FAILURE );
        }
        else if ( dir.exists ( argv[opt + 2] ) == true )
        {
NOM_LOG_ERR ( TTCARDS, "File path for <output_filename> already exists at: " + std::string(argv[opt + 2]) );
          exit ( EXIT_FAILURE );
        }
        else if ( cards.save( argv[opt + 2] ) == false )
        {
NOM_LOG_ERR ( TTCARDS, "Could not save the game cards collection at: " + std::string(argv[opt + 2]) );
          exit ( EXIT_FAILURE );
        }
        else
        {
NOM_LOG_INFO ( TTCARDS, "Game cards successfully saved at: " + std::string(argv[opt + 2]) );
          exit ( EXIT_SUCCESS );
        }
      } // end export cards option
      else if ( std::string(argv[opt]) == "--config" ) // Save configuration opt
      {
        if ( argv[opt + 1] == nullptr )
        {
NOM_LOG_ERR ( TTCARDS, "Missing parameter <output_filename> for config argument." );
          exit ( EXIT_FAILURE );
        }
        else if ( dir.exists ( argv[opt + 1] ) == true )
        {
NOM_LOG_ERR ( TTCARDS, "File path for <output_filename> already exists at: " + std::string(argv[opt + 1]) );
          exit ( EXIT_FAILURE );
        }

        GameConfig cfg;
        if ( cfg.load ( TTCARDS_CONFIG_FILENAME ) == false )
        {
NOM_LOG_ERR ( TTCARDS, "Could not load game configuration from file at: " + TTCARDS_CONFIG_FILENAME );
          exit ( EXIT_FAILURE );
        }

        if ( cfg.save ( argv[opt + 1] ) == false )
        {
NOM_LOG_ERR ( TTCARDS, "Could not save game configuration to file at: " + std::string(argv[opt + 1]) );
          exit ( EXIT_FAILURE );
        }
        else
        {
NOM_LOG_INFO ( TTCARDS, "Game configuration successfully saved at: " + std::string(argv[opt + 1]) );
          exit ( EXIT_SUCCESS );
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
    exit ( EXIT_SUCCESS );
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
  dir.set_path ( working_directory );
}

App::~App ( void )
{
NOM_LOG_TRACE ( TTCARDS );
}

bool App::onInit ( void )
{
  nom::Rectangle rectangle  ( nom::Coords ( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT ),
                              NOM_COLOR4U_GRAY
                            );

  nom::uint32 video_flags = SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_SHOWN;
  /* TODO: Use me when fixed-time-step is fully implemented
  nom::uint32 renderer_flags = SDL_RENDERER_PRESENTVSYNC;
  TODO */

  this->game = std::shared_ptr<GameObject> ( new GameObject );

  if ( this->game->config.load ( TTCARDS_CONFIG_FILENAME ) == false )
  {
    nom::DialogMessageBox ( "Critical Error", "Could not load configuration file at: " + TTCARDS_CONFIG_FILENAME );
    exit ( EXIT_FAILURE );
  }

#ifndef EMSCRIPTEN
  this->game->window.set_window_icon ( this->game->config.getString("APP_ICON") );
#endif

  // Obtain a list of available video modes so we can determine how to render
  // the game (scale factors, positioning, etc.).
  nom::VideoModeList modes = this->game->window.getVideoModes();

  for ( nom::uint32 idx = 0; idx != modes.size(); idx++ )
  {
    std::cout << modes[idx] << std::endl;
  }

  this->game->window.create ( "TTcards", SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, video_flags );

//this->game->window.set_window_title ( LOADING_TEXT );

  this->game->window.set_logical_resolution ( SCREEN_WIDTH, SCREEN_HEIGHT );

  // Commence the initialization of game objects
  this->game->menu_elements = nom::SpriteBatch ( "images/menu_elements.json" );
  this->game->cursor = nom::ui::Cursor ( "images/cursors.json" );
  this->game->cursor.set_position ( MENU_CARDS_CURSOR_ORIGIN_X, MENU_CARDS_CURSOR_ORIGIN_Y );

  // Commence the loading of game resources
  if ( this->game->info_text.load ( this->game->config.getString("INFO_FONTFACE"), nom::Color4u ( 255, 0, 255, 0 ), 0 ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("INFO_FONTFACE") );
    return false;
  }

  if ( this->game->info_text_gray.load ( this->game->config.getString("INFO_FONTFACE"), nom::Color4u ( 255, 0, 255, 0 ), 0 ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("INFO_FONTFACE") );
    return false;
  }

  if ( this->game->info_small_text.load ( this->game->config.getString("INFO_SMALL_FONTFACE"), nom::Color4u ( 255, 0, 255, 0 ), 0 ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("INFO_FONTFACE") );
    return false;
  }

  if ( this->game->menu_elements.load ( this->game->config.getString("MENU_ELEMENTS"), nom::Color4u ( 0, 0, 0 ), 0 ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("MENU_ELEMENTS") );
    return false;
  }

  if ( this->game->background.load( this->game->config.getString("BOARD_BACKGROUND"), /*NOM_COLOR4U_BLACK,*/ 0 ) == false )
  {
NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("BOARD_BACKGROUND") );
    rectangle.draw ( this->game->window );
  }
  else
  {
    if ( this->game->config.getString("SCALE_ALGORITHM") == "scale2x" )
    {
      this->game->background.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    }
    else if ( this->game->config.getString("SCALE_ALGORITHM") == "hqx" )
    {
      this->game->background.resize ( nom::Texture::ResizeAlgorithm::hq2x );
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

  if ( this->game->score_text[0].load ( this->game->config.getString("SCORE_FONTFACE"), NOM_COLOR4U_WHITE ) == true )
  {
    this->game->score_text[0].setColor ( NOM_COLOR4U_WHITE );
    this->game->score_text[0].setFontSize ( 48 * SCALE_FACTOR );
    this->game->score_text[0].setFontStyle ( nom::IFont::FontStyle::Italic );
    this->game->score_text[0].setRenderingStyle ( nom::IFont::RenderStyle::Blended );
    this->game->score_text[0].setFontOutline ( 1 );
  }
  else
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("SCORE_FONTFACE") );
    return false;
  }

  if ( this->game->score_text[1].load ( this->game->config.getString("SCORE_FONTFACE"), NOM_COLOR4U_WHITE ) == true )
  {
    this->game->score_text[1].setColor ( NOM_COLOR4U_WHITE );
    this->game->score_text[1].setFontSize ( 48 * SCALE_FACTOR );
    this->game->score_text[1].setFontStyle ( nom::IFont::FontStyle::Italic );
    this->game->score_text[1].setRenderingStyle ( nom::IFont::RenderStyle::Blended );
    this->game->score_text[1].setFontOutline ( 1 );
  }
  else
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("SCORE_FONTFACE") );
    return false;
  }

  if ( this->game->gameover_text.load ( this->game->config.getString("GAMEOVER_FONTFACE"), NOM_COLOR4U_WHITE ) == true )
  {
    this->game->gameover_text.setFontSize ( 48 * SCALE_FACTOR );
    this->game->gameover_text.setFontStyle ( nom::IFont::FontStyle::Italic );
    this->game->gameover_text.setRenderingStyle ( nom::IFont::RenderStyle::Blended );
    this->game->gameover_text.setFontOutline ( 1 );
  }
  else
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("GAMEOVER_FONTFACE") );
    return false;
  }

  if ( this->game->gameover_background.load ( this->game->config.getString("GAMEOVER_BACKGROUND"), true ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("GAMEOVER_BACKGROUND") );
    return false;
  }

  if ( this->game->cursor.load ( this->game->config.getString("INTERFACE_CURSOR"), NOM_COLOR4U_BLACK, true ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("INTERFACE_CURSOR") );
    return false;
  }

  if ( this->game->collection.load( this->game->config.getString("CARDS_DB") ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("CARDS_DB") );
    return false;
  }

  if ( this->game->card.load ( &this->game->config ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load CardView renderer" );
    return false;
  }

  // Rescale our game resources if necessary.
  if ( this->game->config.getString("SCALE_ALGORITHM") == "scale2x" )
  {
    this->game->info_text.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    this->game->info_text_gray.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    this->game->info_small_text.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    this->game->cursor.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    this->game->menu_elements.resize ( nom::Texture::ResizeAlgorithm::scale2x );
    this->game->gameover_background.resize ( nom::Texture::ResizeAlgorithm::scale2x );
  }
  else if ( this->game->config.getString("SCALE_ALGORITHM") == "hqx" )
  {
    this->game->info_text.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    this->game->info_text_gray.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    this->game->info_small_text.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    this->game->cursor.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    this->game->menu_elements.resize ( nom::Texture::ResizeAlgorithm::hq2x );
    this->game->gameover_background.resize ( nom::Texture::ResizeAlgorithm::hq2x );
  }

  // Load optional audio resources
  if ( this->game->cursor_move_buffer.load ( this->game->config.getString("CURSOR_MOVE") ) == false )
  {
NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("CURSOR_MOVE") );
  }

  if ( this->game->cursor_cancel_buffer.load ( this->game->config.getString("CURSOR_CANCEL") ) == false )
  {
NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("CURSOR_CANCEL") );
  }

  if ( this->game->cursor_wrong_buffer.load ( this->game->config.getString("CURSOR_WRONG") ) == false )
  {
NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("CURSOR_WRONG") );
  }

  if ( this->game->card_place_buffer.load ( this->game->config.getString("CARD_PLACE") ) == false )
  {
NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("CARD_PLACE") );
  }

  if ( this->game->card_flip_buffer.load ( this->game->config.getString("CARD_FLIP") ) == false )
  {
NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("CARD_FLIP") );
  }

  if ( this->game->load_game_buffer.load ( this->game->config.getString("SFX_LOAD_GAME") ) == false )
  {
NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("SFX_LOAD_GAME") );
  }

  if ( this->game->save_game_buffer.load ( this->game->config.getString("SFX_SAVE_GAME") ) == false )
  {
NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("SFX_SAVE_GAME") );
  }

  if ( this->game->music_buffer.load ( this->game->config.getString("MUSIC_TRACK") ) == false )
  {
NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("MUSIC_TRACK") );
  }

  if ( this->game->winning_buffer.load ( this->game->config.getString("MUSIC_WIN_TRACK") ) == false )
  {
NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("MUSIC_WIN_TRACK") );
  }

  this->game->cursor_move.setBuffer ( this->game->cursor_move_buffer );
  this->game->cursor_cancel.setBuffer ( this->game->cursor_cancel_buffer );
  this->game->cursor_wrong.setBuffer ( this->game->cursor_wrong_buffer );
  this->game->card_place.setBuffer ( this->game->card_place_buffer );
  this->game->card_flip.setBuffer ( this->game->card_flip_buffer );
  this->game->load_game.setBuffer ( this->game->load_game_buffer );
  this->game->save_game.setBuffer ( this->game->save_game_buffer );
  this->game->music_track.setBuffer ( this->game->music_buffer );
  this->game->winning_track.setBuffer ( this->game->winning_buffer );

  this->game->music_track.Play();

#if defined (NOM_DEBUG)
  this->game->music_track.Pause();
#endif

  this->Running(); // ...here we go!

  return true;
}

void App::onEvent ( SDL_Event* event )
{
  // Take care of our own events
  Input::HandleInput ( event );

  // Take care of each state's event
  nom::GameStates::onEvent ( event );
}

void App::onKeyDown ( nom::int32 key, nom::int32 mod, nom::uint32 window_id )
{
  switch ( key )
  {
    default: break;

#if defined (NOM_DEBUG)
    case SDLK_0:
    {
      if ( mod == KMOD_LGUI )
      {
        nom::GameStates::ChangeState ( ContinueMenuStatePtr ( new ContinueMenuState ( this->game ) ) );
      }
      else
      {
        nom::GameStates::ChangeState ( GameOverStatePtr ( new GameOverState ( this->game, GameOverType::Won ) ) );
      }
    }
    break;
#endif

    case SDLK_ESCAPE:
    case SDLK_q: this->onQuit(); break;

    case SDLK_p:
    {
      this->game->music_track.togglePause();
      this->game->winning_track.togglePause();
    }
    break;

    case SDLK_m:
    {
      float current_volume = this->game->listener.getVolume();
      if ( current_volume >= 100.0 )
        this->game->listener.setVolume ( 0.0 );
      else if ( current_volume <= 0.0 )
        this->game->listener.setVolume ( 100.0 );
    }
    break;

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
      std::string screenshot_filename = TTCARDS_DATA_DIR + "/" + "Screenshot_" + std::to_string ( this->ticks() ) + ".png";

      if ( this->game->window.save_screenshot( screenshot_filename ) == false )
      {
        NOM_LOG_ERR ( TTCARDS, "Could not save screenshot file:" + screenshot_filename );
        break;
      }

      NOM_LOG_INFO ( TTCARDS, "Saved screenshot: " + screenshot_filename );
      break;
    }
    break;

    case SDLK_r: // Start a new game
    {
      if ( this->game->config.load ( TTCARDS_CONFIG_FILENAME ) == false )
      {
NOM_LOG_ERR ( TTCARDS, "Could not reload configuration file at: " + TTCARDS_CONFIG_FILENAME );
        break;
      }
      nom::GameStates::ChangeState ( CardsMenuStatePtr( new CardsMenuState ( this->game ) ) );
    }
    break;

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

void App::onResize ( nom::int32 width, nom::int32 height )
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

int32_t App::Run ( void )
{
  unsigned int loops = 0;
  unsigned int next_game_tick = 0;
  nom::uint32 delta_time = 0;

  this->fps.start();

  next_game_tick = this->ticks();

  nom::GameStates::ChangeState( CardsMenuStatePtr( new CardsMenuState ( this->game ) ) );

  while ( this->isRunning() == true )
  {
    loops = 0;
    delta_time = this->ticks();

    while ( this->ticks() > next_game_tick && loops <= MAX_FRAMESKIP )
    {
      while ( this->PollEvents ( &event ) )
      {
        this->onEvent ( &event );
      }

      this->fps.update();

      nom::GameStates::update ( delta_time );
      nom::GameStates::draw ( this->game->window );

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

  return EXIT_SUCCESS;
}

int main ( int argc, char* argv[] )
{
  App engine ( argc, argv );

  if ( engine.onInit() == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not initialize game." );
    return EXIT_FAILURE;
  }

#ifdef EMSCRIPTEN
  // TODO
#else
  return engine.Run();
#endif

  // This is past the point of execution; all execution must reside within our
  // App class
}
