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
  nom::File dir;

TTCARDS_LOG_CLASSINFO;

// These definitions are influenced at build time with CMake options and serve
// to help determine the path of game resources
#if defined ( OSXAPP ) // OSX Application Bundle
  WORKING_DIR = nom::getBundleResourcePath();
#else // Potentially customized layout (POSIX hierarchy by default)
  WORKING_DIR = TTCARDS_INSTALL_PREFIX + "/share/ttcards/Resources/";
#endif

  // Change the working directory to whatever WORKING_DIR has been set to
  dir.setPath ( WORKING_DIR );

  // Command line arguments
  if ( argc > 1 )
  {
    if ( strcmp ( argv[1], "-e" ) == 0 || strcmp ( argv[1], "--export" ) == 0 )
    {
      CardCollection cards;
      if ( cards.size() < 1 )
      {
        if ( cards.LoadJSON ( CARDS_DB ) == false )
        {
          std::cout << "ERR: " << "Unknown failure to load cards database collection." << std::endl;
          exit ( EXIT_FAILURE );
        }
      }

      if ( cards.ExportJSON ( TTCARDS_DATA_DIR + "/" + "cards.json" ) == false )
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
      std::cout << "\tttcards [ -v | --version ]" << std::endl;
      std::cout << "\tttcards [ -e | --export ]" << std::endl;
      exit ( EXIT_SUCCESS );
    }
    else if ( strcmp ( argv[1], "-v" ) == 0 || strcmp ( argv[1], "--version" ) == 0 )
    {
      std::cout << APP_NAME << " version " << TTCARDS_VERSION_MAJOR << "." << TTCARDS_VERSION_MINOR << "." << TTCARDS_VERSION_PATCH << " by Jeffrey Carpenter" << std::endl;
      exit ( EXIT_SUCCESS );
    }
  }
}

App::~App ( void )
{
TTCARDS_LOG_CLASSINFO;
}

bool App::onInit ( void )
{
  nom::Rectangle rectangle  ( nom::Coords ( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT ),
                              nom::Color::Gray
                            );

  unsigned int video_flags = SDL_SWSURFACE | SDL_RLEACCEL | SDL_DOUBLEBUF;

  this->game = std::shared_ptr<GameObject> ( new GameObject );

#ifndef EMSCRIPTEN
  this->game->context.setWindowIcon ( APP_ICON );
#endif

  this->game->context.createWindow ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, video_flags );

  this->enableKeyRepeat ( 100, SDL_DEFAULT_REPEAT_INTERVAL / 3 );

  this->game->context.setWindowTitle( LOADING_TEXT );

  this->game->info_text.load ( INFO_FONTFACE, nom::Color ( 110, 144, 190 ), true );
  this->game->info_small_text.load ( INFO_SMALL_FONTFACE, nom::Color ( 110, 144, 190 ), true );

  // Provide a backup in case our pretty background image resource is not found
  if ( ! this->game->background.load( BOARD_BACKGROUND, nom::Color::Black, true ) )
  {
    rectangle.Draw ( this->game->context.get() );
  }
  else
  {
    this->game->background.Draw( this->game->context.get() );
  }
  this->game->context.Update();

  // We are not able to see the screen update of anything we do here unless we
  // poll for an events afterwards. Oddly enough, this issue only happens when
  // we are launching from an application bundle under OS X -- normal binaries
  // do not need this workaround.
  this->PollEvents( &event ); // Possible bug in SDL for OS X ..?

  this->game->collection.LoadJSON ( CARDS_DB );

  this->game->info_text_gray.load ( INFO_FONTFACE, nom::Color ( 110, 144, 190 ), true );

  this->game->score_text.load ( SCORE_FONTFACE, nom::Color::White, 36 );
  this->game->gameover_text.load ( SCORE_FONTFACE, 36 );

  this->game->gameover_background.load ( GAMEOVER_BACKGROUND, nom::Color::Black, true, 0 ); // use no transparency

  this->game->cursor = nom::ui::Cursor ( MENU_CARDS_CURSOR_ORIGIN_X, MENU_CARDS_CURSOR_ORIGIN_Y, CURSOR_WIDTH, CURSOR_HEIGHT );
  this->game->cursor.load ( INTERFACE_CURSOR, nom::Color::Black, true );
  this->game->cursor.setSheetDimensions ( 78, 16, 0, 0 );

  this->game->card.load();
  this->game->board.clear();

  this->game->sound_buffer.load ( CURSOR_MOVE );
  this->game->cursor_move.setBuffer ( this->game->sound_buffer );

  this->game->sound_buffer.load ( CURSOR_CANCEL );
  this->game->cursor_cancel.setBuffer ( this->game->sound_buffer );

  this->game->sound_buffer.load ( CURSOR_WRONG );
  this->game->cursor_wrong.setBuffer ( this->game->sound_buffer );

  this->game->sound_buffer.load ( CARD_PLACE );
  this->game->card_place.setBuffer ( this->game->sound_buffer );

  this->game->sound_buffer.load ( CARD_FLIP );
  this->game->card_flip.setBuffer ( this->game->sound_buffer );

  this->game->music_buffer.load ( MUSIC_TRACK );
  this->game->music_track.setBuffer ( this->game->music_buffer );

  this->game->winning_buffer.load ( MUSIC_WINNING_TRACK );
  this->game->winning_track.setBuffer ( this->game->winning_buffer );

  this->game->music_track.Play();

#ifdef TTCARDS_DEBUG
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
      this->game->music_track.togglePause(); this->game->winning_track.togglePause();
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

    case SDLK_s:
    {
      nom::Image image;
      image.save ( TTCARDS_DATA_DIR + "/" + "Screenshot_" + std::to_string ( getTicks() ) + ".bmp", this->game->context.get() );
      break;
    }

    // Start new game
    case SDLK_r: nom::GameStates::ChangeState ( CardsMenuStatePtr( new CardsMenuState ( this->game ) ) ); break;

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
