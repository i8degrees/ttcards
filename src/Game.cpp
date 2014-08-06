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

Game::Game() :
  audio_dev_{ nullptr },
  listener_{ nullptr },
  sound_buffers{ nullptr },
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
    nom::SDL2Logger::set_logging_priority( NOM, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    // Enable logging of all messages in the game
    nom::SDL2Logger::set_logging_priority( TTCARDS, nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );

    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_INPUT, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );
    // nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_TEST, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    // nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_EVENTS, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    // Disable logging of all messages from GameConfig except for fatal errs.
    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_CFG, nom::LogPriority::NOM_LOG_PRIORITY_ERROR );

    // Enable logging of game state function traces
    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_TRACE_STATES, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

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

Game::~Game()
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE );
}

bool Game::on_init( void )
{
  nom::Rectangle rectangle  ( IntRect( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT),
                              nom::Color4i::Gray
                            );

  nom::uint32 video_flags = 0;
  /* TODO: Use me when fixed-time-step is fully implemented
  nom::uint32 renderer_flags = SDL_RENDERER_PRESENTVSYNC;
  TODO */

  this->set_state_machine( new nom::StateMachine() );

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

  // Quick and dirty method of toggling the use of nomlib's audio subsystem
  // #undef NOM_USE_OPENAL

  // Initialize audio subsystem...
  #if defined( NOM_USE_OPENAL )
    this->audio_dev_ = new nom::AudioDevice();
    this->listener_ = new nom::Listener();
  #else
    this->audio_dev_ = new nom::NullAudioDevice();
    this->listener_ = new nom::NullListener();
  #endif // defined NOM_USE_OPENAL

  for( auto idx = 0; idx != NUM_SOUND_BUFFERS; ++idx )
  {
    #if defined( NOM_USE_OPENAL )
      this->sound_buffers[idx] = new nom::SoundBuffer();
    #else
      this->sound_buffers[idx] = new nom::NullSoundBuffer();
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
    this->cursor_move = new nom::Sound();
    this->cursor_cancel = new nom::Sound();
    this->cursor_wrong = new nom::Sound();
    this->card_place = new nom::Sound();
    this->card_flip = new nom::Sound();
    this->load_game = new nom::Sound();
    this->save_game = new nom::Sound();
    this->music_track = new nom::Music();
    this->winning_track = new nom::Music();

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
    this->cursor_move = new nom::NullSound();
    this->cursor_cancel = new nom::NullSound();
    this->cursor_wrong = new nom::NullSound();
    this->card_place = new nom::NullSound();
    this->card_flip = new nom::NullSound();
    this->load_game = new nom::NullSound();
    this->save_game = new nom::NullSound();
    this->music_track = new nom::NullMusic();
    this->winning_track = new nom::NullMusic();
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

  nom::EventCallback jumpto_continue_menu_state( [&] ( const nom::Event& evt )
    {
      this->set_state( Game::State::ContinueMenu );
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

  state.insert( "quit_game", nom::KeyboardAction( SDL_KEYDOWN, SDLK_ESCAPE ), quit_game );
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
    state.insert( "jumpto_continue_menu_state", nom::KeyboardAction( SDL_KEYDOWN, SDLK_0, KMOD_LGUI ), jumpto_continue_menu_state );
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

int32_t Game::Run( void )
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
      nom::Event evt;
      while( this->poll_event( evt ) )
      {
        this->on_event( evt );
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

    case Game::State::ContinueMenu:
    {
      this->state()->set_state( ContinueMenuStatePtr( new ContinueMenuState( this->game ) ), data );
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

  this->set_state( Game::State::CardsMenu );
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
