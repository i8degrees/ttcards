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

App::App ( nom::int32 argc, char* argv[] )
{
  // destination directory we descend into to locate game resources
  std::string working_directory;
  nom::File dir;

NOM_LOG_TRACE ( TTCARDS );

// These definitions are influenced at build time with CMake options and serve
// to help determine the path of game resources
#if defined ( OSXAPP ) // OSX Application Bundle
  working_directory = nom::getBundleResourcePath();
#else // Potentially customized layout (POSIX hierarchy by default)
  working_directory = TTCARDS_INSTALL_PREFIX + path.native() + "share" + path.native() + "ttcards" + path.native() + "Resources";
#endif

  // Change the working directory to whatever WORKING_DIR has been set to
  dir.setPath ( working_directory );

  // Command line arguments
  if ( argc > 1 )
  {
    if ( strcmp ( argv[1], "-e" ) == 0 || strcmp ( argv[1], "--export" ) == 0 )
    {
      CardCollection cards;

      // Load cards collection from an existing file path
      if ( argv[3] == nullptr )
      {
NOM_LOG_ERR ( TTCARDS, "Missing parameter <input_filename>." );
        exit ( EXIT_FAILURE );
      }
      else if ( dir.exists ( argv[3] ) == false )
      {
// FIXME
//NOM_LOG_ERR ( TTCARDS, "File path does not exist: " + argv[3] );
NOM_LOG_ERR ( TTCARDS, "File path does not exist." );
        exit ( EXIT_FAILURE );
      }

      if ( cards.load( argv[3] ) == false )
      {
NOM_LOG_ERR ( TTCARDS, "Could not load the cards collection." );
        exit ( EXIT_FAILURE );
      }

      // Save cards collection at user defined path
      if ( argv[2] == nullptr )
      {
NOM_LOG_ERR ( TTCARDS, "Missing parameter <output_filename>." );
        exit ( EXIT_FAILURE );
      }

      if ( dir.exists ( argv[2] ) == true )
      {
// FIXME
//NOM_LOG_ERR ( TTCARDS, "File path already exists: " + argv[2] );
NOM_LOG_ERR ( TTCARDS, "File path already exists." );
        exit ( EXIT_FAILURE );
      }

      if ( cards.save( TTCARDS_DATA_DIR + "/" + argv[2] ) == false )
      {
// FIXME
//NOM_LOG_ERR ( TTCARDS, "Could not serialize data in file: " + TTCARDS_DATA_DIR + path.native() + argv[2] );
NOM_LOG_ERR ( TTCARDS, "Could not serialize cards collection." );
        exit ( EXIT_FAILURE );
      }
// FIXME
//NOM_LOG_INFO ( TTCARDS, "File " + argv[2] + " successfully saved" );
NOM_LOG_INFO ( TTCARDS, "File export successful." );
    } // end  export option
    else if ( strcmp ( argv[1], "-h" ) == 0 || strcmp ( argv[1], "--help" ) == 0 )
    {
      std::cout << "\tttcards [ -h | --help ]" << std::endl;
      std::cout << "\tttcards [ -v | --version ]" << std::endl;
      std::cout << "\tttcards [ -e | --export <output_filename> <input_filename> ]" << std::endl;
    } // end help option
    else if ( strcmp ( argv[1], "-v" ) == 0 || strcmp ( argv[1], "--version" ) == 0 )
    {
      std::cout << APP_NAME << " version " << TTCARDS_VERSION_MAJOR << "." << TTCARDS_VERSION_MINOR << "." << TTCARDS_VERSION_PATCH << " by Jeffrey Carpenter" << std::endl;
    } // end version option

    // If we have got this far, we assume command execution was successful
    exit ( EXIT_SUCCESS );
  } // end argument & parameter checks
}

App::~App ( void )
{
NOM_LOG_TRACE ( TTCARDS );
}

bool App::onInit ( void )
{
  nom::Rectangle rectangle  ( nom::Coords ( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT ),
                              nom::Color::Gray
                            );

  unsigned int video_flags = SDL_SWSURFACE | SDL_RLEACCEL | SDL_DOUBLEBUF;

  this->game = std::shared_ptr<GameObject> ( new GameObject );

  if ( this->game->config.load( TTCARDS_DATA_DIR + "/config.json" ) == false )
  {
    NOM_LOG_ERR ( TTCARDS, "Could not load configuration file: " + TTCARDS_DATA_DIR + "/config.json" );

    nom::DialogMessageBox ( "Critical Error", "Could not load configuration file: " + TTCARDS_DATA_DIR + "/config.json" );
    exit ( EXIT_FAILURE );
  }

#ifndef EMSCRIPTEN
  this->game->context.setWindowIcon ( this->game->config.getString("APP_ICON") );
#endif

  // Obtain a list of available video modes so we can determine how to render
  // the game (scale factors, positioning, etc.).
  nom::VideoModeList modes = this->game->context.getVideoModes();

  for ( nom::uint32 idx = 0; idx != modes.size(); idx++ )
  {
    std::cout << modes[idx] << std::endl;
  }

  this->game->context.createWindow ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, video_flags );

  this->enableKeyRepeat ( 100, SDL_DEFAULT_REPEAT_INTERVAL / 3 );

  this->game->context.setWindowTitle( LOADING_TEXT );

  // Commence the initialization of game objects
  this->game->menu_elements = nom::Sprite ( MENU_ELEMENT_WIDTH, MENU_ELEMENT_HEIGHT );
  this->game->menu_elements.setSheetDimensions ( 58, 16, 0, 0 );

  this->game->cursor = nom::ui::Cursor ( MENU_CARDS_CURSOR_ORIGIN_X, MENU_CARDS_CURSOR_ORIGIN_Y, CURSOR_WIDTH, CURSOR_HEIGHT );
  this->game->cursor.setSheetDimensions ( 78, 16, 0, 0 );

  // Commence the loading of game resources
  if ( this->game->info_text.load ( this->game->config.getString("INFO_FONTFACE"), nom::Color ( 110, 144, 190 ), true ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("INFO_FONTFACE") );
    return false;
  }

  if ( this->game->info_text_gray.load ( this->game->config.getString("INFO_FONTFACE"), nom::Color ( 110, 144, 190 ), true ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("INFO_FONTFACE") );
    return false;
  }

  if ( this->game->info_small_text.load ( this->game->config.getString("INFO_SMALL_FONTFACE"), nom::Color ( 110, 144, 190 ), true ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("INFO_FONTFACE") );
    return false;
  }

  if ( this->game->menu_elements.load ( this->game->config.getString("MENU_ELEMENTS"), nom::Color ( 0, 0, 0 ), true ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("MENU_ELEMENTS") );
    return false;
  }

  if ( this->game->background.load( this->game->config.getString("BOARD_BACKGROUND"), nom::Color::Black, true ) == false )
  {
NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->game->config.getString("BOARD_BACKGROUND") );
    rectangle.Draw ( this->game->context.get() );
  }
  else
  {

    if ( this->game->config.getString("SCALE_ALGORITHM") == "scale2x" )
    {
      this->game->background.resize ( nom::ResizeAlgorithm::scale2x );
    }
    else if ( this->game->config.getString("SCALE_ALGORITHM") == "hqx" )
    {
      this->game->background.resize ( nom::ResizeAlgorithm::hq2x );
    }

    this->game->background.Draw( this->game->context.get() );
  }

  this->game->context.Update();

  // Possible bug in SDL for OS X
  //
  // We are not able to see the screen update of anything we do here unless we
  // poll for an events afterwards. Oddly enough, this issue only happens when
  // we are launching from an application bundle under OS X -- normal binaries
  // do not need this workaround.
  this->PollEvents( &event );

  if ( this->game->score_text[0].load ( this->game->config.getString("SCORE_FONTFACE"), nom::Color::White ) == true )
  {
    this->game->score_text[0].setColor ( nom::Color::White );
    this->game->score_text[0].setFontSize ( 36 );
  }
  else
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("SCORE_FONTFACE") );
    return false;
  }

  if ( this->game->score_text[1].load ( this->game->config.getString("SCORE_FONTFACE"), nom::Color::White ) == true )
  {
    this->game->score_text[1].setColor ( nom::Color::White );
    this->game->score_text[1].setFontSize ( 36 );
  }
  else
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("SCORE_FONTFACE") );
    return false;
  }

  if ( this->game->gameover_text.load ( this->game->config.getString("GAMEOVER_FONTFACE"), false ) == true )
  {
    this->game->gameover_text.setFontSize ( 36 );
  }
  else
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("GAMEOVER_FONTFACE") );
    return false;
  }

  if ( this->game->gameover_background.load ( this->game->config.getString("GAMEOVER_BACKGROUND"), nom::Color::Black, true, 0 ) == false )
  {
NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->game->config.getString("GAMEOVER_BACKGROUND") );
    return false;
  }

  if ( this->game->cursor.load ( this->game->config.getString("INTERFACE_CURSOR"), nom::Color::Black, true ) == false )
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
    return false;
  }

  // Rescale our game resources if necessary.
  if ( this->game->config.getString("SCALE_ALGORITHM") == "scale2x" )
  {
    this->game->info_text.resize ( nom::ResizeAlgorithm::scale2x );
    this->game->info_text_gray.resize ( nom::ResizeAlgorithm::scale2x );
    this->game->info_small_text.resize ( nom::ResizeAlgorithm::scale2x );
    this->game->cursor.resize ( nom::ResizeAlgorithm::scale2x );
    this->game->menu_elements.resize ( nom::ResizeAlgorithm::scale2x );
    this->game->gameover_background.resize ( nom::ResizeAlgorithm::scale2x );
  }
  else if ( this->game->config.getString("SCALE_ALGORITHM") == "hqx" )
  {
    this->game->info_text.resize ( nom::ResizeAlgorithm::hq2x );
    this->game->info_text_gray.resize ( nom::ResizeAlgorithm::hq2x );
    this->game->info_small_text.resize ( nom::ResizeAlgorithm::hq2x );
    this->game->cursor.resize ( nom::ResizeAlgorithm::hq2x );
    this->game->menu_elements.resize ( nom::ResizeAlgorithm::hq2x );
    this->game->gameover_background.resize ( nom::ResizeAlgorithm::hq2x );
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

#ifdef NOM_DEBUG
  this->game->music_track.Pause();
#endif

  this->Running(); // ...here we go!

  return true;
}

void App::onEvent ( SDL_Event *event )
{
  // Take care of our own events
  Input::HandleInput ( event );

  // Take care of each state's event
  nom::GameStates::onEvent ( event );
}

void App::onKeyDown ( int32_t key, int32_t mod )
{
  switch ( key )
  {
    default: break;

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

    case SDLK_BACKSLASH: this->toggleFPS(); break;
    case SDLK_f: if ( mod == KMOD_LMETA ) this->onResize ( 0, 0 ); break;

    case SDLK_F1:
    {
      nom::Image image;

       std::string screenshot_filename = TTCARDS_DATA_DIR + "/" + "Screenshot_" + std::to_string ( getTicks() ) + ".bmp";

      if ( image.save ( screenshot_filename, this->game->context.get() ) == false )
      {
NOM_LOG_ERR ( TTCARDS, "Could not save screenshot file:" + screenshot_filename );
        break;
      }
NOM_LOG_INFO ( TTCARDS, "Saved screenshot: " + screenshot_filename );
    }
    break;

    case SDLK_r: // Start a new game
    {
      if ( mod == KMOD_LMETA ) // Reload configuration
      {
        if ( this->game->config.load( TTCARDS_DATA_DIR + "/config.json" ) == true )
        {
NOM_LOG_INFO ( TTCARDS, "Reloaded configuration file: " + TTCARDS_DATA_DIR + "/config.json" );
        }
        else
        {
NOM_LOG_ERR ( TTCARDS, "Could not reload configuration file: " + TTCARDS_DATA_DIR + "/config.json" );
        }
        break;
      }
      else
      {
        nom::GameStates::ChangeState ( CardsMenuStatePtr( new CardsMenuState ( this->game ) ) );
      }
    }
    break;

    case SDLK_LEFTBRACKET:
    {
      if ( mod == KMOD_LMETA )
        this->game->board.List();
      else
        this->game->debug.ListCards ( this->game->hand[0].cards );
    }
    break;

    case SDLK_RIGHTBRACKET:
    {
      if ( mod == KMOD_LMETA )
        this->game->debug.ListCards ( this->game->collection.cards );
      else
        this->game->debug.ListCards ( this->game->hand[1].cards );
    }
    break;
  }
}

void App::onResize ( int32_t width, int32_t height )
{
  if ( this->isFullScreen() )
  {
    this->game->context.toggleFullScreenWindow ( 0, 0 );
    this->setFullScreen ( false );
  }
  else
  {
    this->game->context.toggleFullScreenWindow ( 0, 0 );
    this->setFullScreen ( true );
  }
}

int32_t App::Run ( void )
{
  unsigned int loops = 0;
  unsigned int next_game_tick = 0;
  nom::uint32 delta_time = 0; // TODO; this is a stub out

  this->fps.Start();

  next_game_tick = this->getTicks();

  nom::GameStates::ChangeState( CardsMenuStatePtr( new CardsMenuState ( this->game ) ) );

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
      nom::GameStates::Draw ( this->game->context.get() );

      if ( this->getShowFPS() )
      {
        this->game->context.setWindowTitle ( APP_NAME + " " + "-" + " " + std::to_string ( this->fps.getFPS() ) + " " + "fps" );
      }
      else
      {
        this->game->context.setWindowTitle ( APP_NAME );
      }

      next_game_tick += SKIP_TICKS;
      loops++;

      // FIXME: this is a lazy patch to keep CPU cycles down; on my system,
      // usage drops from 99% to ~22..30%
      if ( this->fps.getFPS() >= TICKS_PER_SECOND )
      {
        nom::sleep ( 50 );
      }
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
