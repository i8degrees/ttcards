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
#include "Game.hpp"

// Private headers
#include <Rocket/Debugger.h>

// Forward declarations
#include "CardCollection.hpp"
#include "CardResourceLoader.hpp"
#include "Board.hpp"
#include "helpers.hpp"
#include "states/GameOverState.hpp"
#include "states/ConfirmationDialogState.hpp"
#include "states/PauseState.hpp"
#include "states/CardsMenuState.hpp"
#include "states/PlayState.hpp"

//#include "states/States.hpp" // StateFactory

using namespace nom;

Game::Game( nom::int32 argc, char* argv[] ) :
  // SDLApp( OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS | OSX_DISABLE_FULLSCREEN_SPACES | INIT_ENGINE_FONTS ),
  SDLApp( OSX_DISABLE_MINIMIZE_ON_LOSS_FOCUS | OSX_DISABLE_FULLSCREEN_SPACES ),
  audio_dev_(nullptr),
  listener_(nullptr),
  cursor_move(nullptr),
  cursor_cancel(nullptr),
  cursor_wrong(nullptr),
  card_place(nullptr),
  card_flip(nullptr),
  load_game(nullptr),
  save_game(nullptr),
  theme_track_(nullptr),
  winning_track(nullptr),
  debug_game_(false),
  game(this)
{
  NOM_LOG_TRACE(TTCARDS_LOG_CATEGORY_TRACE);

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

  nom::init_rand( time(0) );

  #if ! defined( NDEBUG )  // Debug target build
    NOM_LOG_INFO( TTCARDS, "DEBUG build" );

    // Enable info log level and greater for our engine (deprecated)
    nom::SDL2Logger::set_logging_priority( NOM, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_APPLICATION, nom::LogPriority::NOM_LOG_PRIORITY_INFO );

    // Enable logging of all messages in the game
    nom::SDL2Logger::set_logging_priority( TTCARDS, nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );

    nom::SDL2Logger::set_logging_priority(  TTCARDS_LOG_CATEGORY_APPLICATION,
                                            LogPriority::NOM_LOG_PRIORITY_INFO );

    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_INPUT, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );
    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_TEST, nom::LogPriority::NOM_LOG_PRIORITY_INFO );

    // nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_EVENTS, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    // GameConfig debugging log levels;
    // NOM_LOG_PRIORITY_INFO will log only the file path of the configuration
    // file each time it is loaded
    // NOM_LOG_PRIORITY_DEBUG will log just the output data stream
    // NOM_LOG_PRIORITY_VERBOSE will log both the input and output data streams

    // nom::SDL2Logger::set_logging_priority(  TTCARDS_LOG_CATEGORY_CFG,
                                            // NOM_LOG_PRIORITY_DEBUG );
    nom::SDL2Logger::set_logging_priority(  TTCARDS_LOG_CATEGORY_CFG,
                                            NOM_LOG_PRIORITY_INFO );

    // Enable logging of game state function traces
    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_TRACE_STATES, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_GUI, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    // Game states debugging (engine scope)
    nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_SYSTEM, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    // Game states debugging (game scope)
    // nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_CARDS_MENU_STATE, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );
    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_GAME_OVER_STATE, nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );

    nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_CPU_PLAYER, nom::LogPriority::NOM_LOG_PRIORITY_DEBUG );

    // Extended diagnostics of the rendering subsystem; i.e.: allocation
    // counters, etc.
    // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_RENDER, nom::NOM_LOG_PRIORITY_DEBUG);

    // Animation subsystem
    nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ANIMATION, nom::NOM_LOG_PRIORITY_INFO);
    // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION_QUEUE, nom::NOM_LOG_PRIORITY_DEBUG);
    // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION_PLAYER, nom::NOM_LOG_PRIORITY_DEBUG);

    // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ANIMATION, nom::NOM_LOG_PRIORITY_VERBOSE);
    // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION_QUEUE, nom::NOM_LOG_PRIORITY_VERBOSE);
    // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION_PLAYER, nom::NOM_LOG_PRIORITY_VERBOSE);

    // Extended call stack diagnostics (i.e.: number of initialized Card objects)
    // nom::SDL2Logger::set_logging_priority( TTCARDS_LOG_CATEGORY_TRACE, nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );

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
        std::exit(NOM_EXIT_SUCCESS);
      } else if( std::string(argv[opt]) == "-h" || std::string(argv[opt]) == "--help" ) {

        std::cout << APP_NAME << " usage options:" << std::endl;
        std::cout << "[ --export <input_filename> <output_filename> ]" << std::endl;
        std::cout << "[ --config <output_filename> ]" << std::endl;
        std::cout << "[ -h, --help ]" << std::endl;
        std::cout << "[ -v, --version ]" << std::endl;
        std::exit(NOM_EXIT_SUCCESS);
      } // end help option
    } // end for argv[opt] loop
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

  // FIXME: The game crashes here for some reason
  // nom::shutdown_librocket();
}

bool Game::on_init()
{
  int render_driver = -1;
  uint32 window_flags = SDL_WINDOW_OPENGL;
  uint32 render_flags = SDL_RENDERER_ACCELERATED;

  // High-level file I/O interface for configuration files
  std::unique_ptr<nom::IValueDeserializer> fp =
    nom::make_unique<nom::JsonCppDeserializer>();
  NOM_ASSERT(fp != nullptr);

  if( this->config.load_file(TTCARDS_CONFIG_FILENAME, fp.get() ) == false ) {
    nom::DialogMessageBox(  "Critical Error",
                            "Could not load configuration file at: " +
                            TTCARDS_CONFIG_FILENAME );
    return false;
  }

  this->game->debug_game_ =
    this->game->config.get_bool("DEBUG_GAME");
  const bool ENABLE_VSYNC =
    this->config.get_bool32("ENABLE_VSYNC");
  const std::string ENABLE_VSYNC_STR =
    std::to_string(ENABLE_VSYNC);

  const std::string RENDER_SCALE_QUALITY =
    this->config.get_string("RENDER_SCALE_QUALITY");
  const int VIDEO_DISPLAY_INDEX =
    this->config.get_int("VIDEO_DISPLAY_INDEX");
  NOM_ASSERT(VIDEO_DISPLAY_INDEX >= 0);

  if( nom::set_hint(SDL_HINT_RENDER_VSYNC, ENABLE_VSYNC_STR) == false ) {
    NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not enable VSYNC." );
  }

  if( nom::set_hint(  SDL_HINT_RENDER_SCALE_QUALITY,
                      RENDER_SCALE_QUALITY) == false )
  {
    NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not set the renderer's scale quality to",
                  RENDER_SCALE_QUALITY );
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

  auto window_ret =
    this->window.create(  APP_NAME, RenderWindow::WINDOW_POS_CENTERED,
                          VIDEO_DISPLAY_INDEX, SCREEN_RESOLUTION, window_flags,
                          render_driver, render_flags );
  if( window_ret == false ) {
    nom::DialogMessageBox(  "Critical Error",
                            "Could not initialize rendering context and window." );
    return false;
  }

  this->window.set_window_icon ( this->config.get_string("APP_ICON") );

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

  // Try to set a sensible (optimal) refresh rate based on the display
  // capabilities when VSYNC is enabled.
  if( ENABLE_VSYNC == true ) {
    auto display_refresh_rate =
      this->window.refresh_rate();
    if( display_refresh_rate > 0 ) {
      this->frame_interval_ = display_refresh_rate;
    } else {
      // ...fall back to using the default value specified in our configuration
      this->frame_interval_ = this->config.get_int("FRAME_RATE");
      NOM_ASSERT(this->frame_interval_ > 0);
    }
  }

  // Initialize file roots for nomlib && libRocket to base file access from
  Rocket::Core::FileInterface* fs = nullptr;
  std::string fs_root;
  nom::File dir;

#if defined(NOM_PLATFORM_OSX)
    fs_root = dir.resource_path() + "/";
#elif defined(NOM_PLATFORM_WINDOWS)
    fs_root = this->working_directory + "\\";
#else
    // Has not been tested!
    fs_root = this->working_directory + "/Resources/";
#endif

  // Used by both internal and external libRocket interfaces; decorators,
  // libRocket-rendered fonts and so forth
  fs = new nom::RocketFileInterface( fs_root.c_str() );

  // Used by nom::BMFont interface (wholly unrelated to libRocket); this sets
  // the relative file root for filenames given in .FNT files to be loaded from
  nom::set_file_root(fs_root);

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

  if( this->gui_window_.load_font( this->config.get_string("GUI_TITLE_FONT") ) == false )
  {
    nom::DialogMessageBox(  "Critical Error",
                            "Could not load font file:" +
                            this->config.get_string("GUI_TITLE_FONT") );
    return false;
  }

  if( this->gui_window_.load_font( this->config.get_string("GUI_TEXT_FONT") ) == false )
  {
    nom::DialogMessageBox(  "Critical Error",
                            "Could not load font file:" +
                            this->config.get_string("GUI_TEXT_FONT") );
    return false;
  }

  Rocket::Core::DecoratorInstancer* decorator0 = new nom::DecoratorInstancerFinalFantasyFrame();
  Rocket::Core::Factory::RegisterDecoratorInstancer("final-fantasy-theme", decorator0 );
  decorator0->RemoveReference();

  DecoratorInstancerSprite* decorator1 = new nom::DecoratorInstancerSprite();
  Rocket::Core::Factory::RegisterDecoratorInstancer("sprite-sheet", decorator1 );
  decorator1->RemoveReference();

  this->set_state_machine( new nom::StateMachine() );

  // Commence the loading of game resources

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->card_font.load ( this->config.get_string("CARD_FONTFACE") ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.get_string("CARD_FONTFACE") );
      return false;
    }
  #else
    if( this->card_font.load ( this->config.get_string("CARD_FONTFACE_SCALE2X") ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.get_string("CARD_FONTFACE_SCALE2X") );
      return false;
    }
  #endif

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->background.load( this->config.get_string("BOARD_BACKGROUND"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.get_string("BOARD_BACKGROUND") );
    }
  #else
    if( this->background.load( this->config.get_string("BOARD_BACKGROUND_SCALE2X"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.get_string("BOARD_BACKGROUND_SCALE2X") );
    }
  #endif

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->scoreboard_font.load( this->config.get_string("SCORE_FONTFACE") ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.get_string("SCORE_FONTFACE") );
      return false;
    }
  #else
    if( this->scoreboard_font.load( this->config.get_string("SCORE_FONTFACE_SCALE2X") ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.get_string("SCORE_FONTFACE_SCALE2X") );
      return false;
    }
  #endif

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->gameover_font.load( this->config.get_string("GAMEOVER_FONTFACE") ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.get_string("GAMEOVER_FONTFACE") );
      return false;
    }
  #else
    if( this->gameover_font.load( this->config.get_string("GAMEOVER_FONTFACE_SCALE2X") ) == false )
    {
      NOM_LOG_ERR ( TTCARDS, "Could not load resource file: " + this->config.get_string("GAMEOVER_FONTFACE_SCALE2X") );
      return false;
    }
  #endif

  // Set both player's scoreboard fonts
  for( nom::uint32 idx = 0; idx < TOTAL_PLAYERS; ++idx ) {
    this->scoreboard_text[idx].set_font(&this->game->scoreboard_font);
  }

  // Initialize game over text
  this->game->gameover_text.set_font(&this->game->gameover_font);

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->gameover_background.load( this->config.get_string("GAMEOVER_BACKGROUND"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_ERR( TTCARDS, "Could not load resource file: " + this->config.get_string("GAMEOVER_BACKGROUND") );
      return false;
    }
  #else
    if( this->gameover_background.load( this->config.get_string("GAMEOVER_BACKGROUND_SCALE2X"), false, nom::Texture::Access::Streaming ) == false )
    {
      NOM_LOG_ERR( TTCARDS, "Could not load resource file: " + this->config.get_string("GAMEOVER_BACKGROUND_SCALE2X") );
      return false;
    }
  #endif

  nom::SpriteSheet cursor_frames;

  // Initialize interface cursor
  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( cursor_frames.load_file( this->config.get_string("INTERFACE_CURSOR_ATLAS") ) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not load sprite sheet:",
                    this->config.get_string("INTERFACE_CURSOR_ATLAS") );
      return false;
    }

    if( this->cursor_tex_.load( this->config.get_string("INTERFACE_CURSOR") ) == false ) {

      nom::DialogMessageBox( "Critical Error", "Could not load resource file: " +
                              this->config.get_string("INTERFACE_CURSOR") );
      return false;
    }
  #else
    if( cursor_frames.load_file( this->config.get_string("INTERFACE_CURSOR_ATLAS_SCALE2X") ) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not load sprite sheet:",
                    this->config.get_string("INTERFACE_CURSOR_ATLAS_SCALE2X") );
      return false;
    }

    if( this->cursor_tex_.load( this->config.get_string("INTERFACE_CURSOR_SCALE2X") ) == false ) {

      nom::DialogMessageBox(  "Critical Error", "Could not load resource file: " +
                              this->config.get_string("INTERFACE_CURSOR_SCALE2X") );
      return false;
    }
  #endif

  IntRect cursor_bounds(IntRect::zero);

  //

  cursor_bounds = cursor_frames.dimensions(SHEET_CURSOR_NONE);
  this->game->left_cursor_frames_.append_frame(cursor_bounds);

  cursor_bounds = cursor_frames.dimensions(SHEET_CURSOR_LEFT);
  this->game->left_cursor_frames_.append_frame(cursor_bounds);

  //

  cursor_bounds = cursor_frames.dimensions(SHEET_CURSOR_NONE);
  this->game->right_cursor_frames_.append_frame(cursor_bounds);

  cursor_bounds = cursor_frames.dimensions(SHEET_CURSOR_RIGHT);
  this->game->right_cursor_frames_.append_frame(cursor_bounds);

  //

  this->game->cursor_ =
    std::make_shared<SpriteBatch>();
  NOM_ASSERT(this->game->cursor_ != nullptr);
  this->game->cursor_->set_texture(this->game->cursor_tex_);
  this->game->cursor_->set_sprite_sheet(this->game->right_cursor_frames_);
  this->game->cursor_->set_frame(INTERFACE_CURSOR_HIDDEN);

  // blinking cursor animation
  const real32 CURSOR_BLINK_INTERVAL =
    this->game->config.get_real32("CURSOR_BLINK_INTERVAL");
  NOM_ASSERT(CURSOR_BLINK_INTERVAL > 0.0f);

  auto cursor_action =
    nom::create_action<SpriteBatchAction>(
      this->game->cursor_, CURSOR_BLINK_INTERVAL);
  NOM_ASSERT(cursor_action != nullptr);
  cursor_action->set_name("cursor_action");

  this->game->blinking_cursor_action_ =
    nom::create_action<RepeatForeverAction>(cursor_action);
  NOM_ASSERT(this->game->blinking_cursor_action_ != nullptr);

  // Initialize both player's card deck

  const std::string CARDS_DB =
    this->config.get_string("CARDS_DB");

  this->game->cards_db_[PLAYER1].reset( new CardCollection() );
  if( this->game->cards_db_[PLAYER1] == nullptr ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load the player's cards:",
                  "memory allocation failure." );
    return false;
  }

  if( this->game->cards_db_[PLAYER1]->load(CARDS_DB) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load the cards database for player 1 from:",
                  CARDS_DB );
    return false;
  }

  this->game->cards_db_[PLAYER2].reset( new CardCollection() );
  if( this->game->cards_db_[PLAYER2] == nullptr ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load the opponent's cards:",
                  "memory allocation failure." );
    return false;
  }

  if( this->game->cards_db_[PLAYER2]->load(CARDS_DB) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load the cards database for player 2 from:",
                  CARDS_DB );
    return false;
  }

  auto p1_db = this->game->cards_db_[PLAYER1].get();
  auto p2_db = this->game->cards_db_[PLAYER2].get();

  // Initialize the player's deck
  for( auto itr = p1_db->begin(); itr != p1_db->end(); ++itr ) {
    itr->setPlayerID(Card::PLAYER1);
    itr->setPlayerOwner(Card::PLAYER1);
  }

  // Initialize the opponent's deck
  for( auto itr = p2_db->begin(); itr != p2_db->end(); ++itr ) {
    itr->setPlayerID(Card::PLAYER2);
    itr->setPlayerOwner(Card::PLAYER2);
  }

  this->game->card_res_.reset( new CardResourceLoader() );
  NOM_ASSERT(this->game->card_res_ != nullptr);
  if( this->card_res_->load_file(&this->config, this->card_font) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not bootstrap card resources." );
    return false;
  }

  nom::SpriteSheet triad_frames;
  nom::Texture* triad_tex = new nom::Texture();
  NOM_ASSERT(triad_tex != nullptr);

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( triad_frames.load_file( this->game->config.get_string("TRIAD_SPINNER_ATLAS" ) ) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not load sprite sheet file:",
                    this->config.get_string("TRIAD_SPINNER") );
      return false;
    }

    if( triad_tex->load( this->config.get_string("TRIAD_SPINNER") ) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not load texture file: ",
                    this->config.get_string("TRIAD_SPINNER") );
      return false;
    }
  #else
    if( triad_frames.load_file( this->game->config.get_string("TRIAD_SPINNER_ATLAS_SCALE2X" ) ) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not load sprite sheet file:",
                    this->config.get_string("TRIAD_SPINNER_ATLAS_SCALE2X") );
      return false;
    }

    if( triad_tex->load( this->config.get_string("TRIAD_SPINNER_SCALE2X") ) == false ) {
      NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not load texture file: ",
                    this->config.get_string("TRIAD_SPINNER_SCALE2X") );
      return false;
    }
  #endif

  auto triad_start_frame =
    this->game->config.get_int("TRIAD_START_FRAME");
  if( triad_start_frame < 0 ) {
    triad_start_frame = 0;
  }

  this->game->triad_ =
    std::make_shared<SpriteBatch>();
  NOM_ASSERT(this->game->triad_ != nullptr);

  this->game->triad_->set_texture(triad_tex);
  this->game->triad_->set_sprite_sheet(triad_frames);
  this->game->triad_->set_frame(triad_start_frame);

  // actions init

  // blinking cursor animation
  const real32 TRIAD_FRAME_INTERVAL =
    this->game->config.get_real32("TRIAD_FRAME_INTERVAL");
  NOM_ASSERT(TRIAD_FRAME_INTERVAL > 0.0f);

  auto triad_action =
    nom::create_action<SpriteBatchAction>(
      this->game->triad_, TRIAD_FRAME_INTERVAL);
  NOM_ASSERT(triad_action != nullptr);

  this->game->triad_action_ =
    nom::create_action<RepeatForeverAction>(triad_action);
  NOM_ASSERT(this->game->triad_action_ != nullptr);

  // win, lose, draw text actions

  this->game->gameover_text.set_text("You Win!");
  this->game->won_text_sprite_ =
    std::make_shared<Sprite>( this->game->gameover_text.clone_texture() );
  NOM_ASSERT(this->game->won_text_sprite_ != nullptr);

  nom::set_alignment( this->game->won_text_sprite_.get(), Point2i::zero,
                      GAME_RESOLUTION, Anchor::MiddleCenter );
  this->game->won_text_sprite_->set_alpha(Color4i::ALPHA_TRANSPARENT);

  this->game->gameover_text.set_text("You Lose...");
  this->game->lost_text_sprite_ =
    std::make_shared<Sprite>( this->game->gameover_text.clone_texture() );
  NOM_ASSERT(this->game->lost_text_sprite_ != nullptr);

  nom::set_alignment( this->game->lost_text_sprite_.get(), Point2i::zero,
                      GAME_RESOLUTION, Anchor::MiddleCenter );
  this->game->lost_text_sprite_->set_alpha(Color4i::ALPHA_TRANSPARENT);

  this->game->gameover_text.set_text("Draw");
  this->game->tied_text_sprite_ =
    std::make_shared<Sprite>( this->game->gameover_text.clone_texture() );
  NOM_ASSERT(this->game->tied_text_sprite_ != nullptr);

  nom::set_alignment( this->game->tied_text_sprite_.get(), Point2i::zero,
                      GAME_RESOLUTION, Anchor::MiddleCenter );
  this->game->tied_text_sprite_->set_alpha(Color4i::ALPHA_TRANSPARENT);

  // Combo && Same flip text sprites
  this->game->gameover_text.set_text("Combo!");
  this->game->combo_text_sprite_ =
    std::make_shared<Sprite>( this->game->gameover_text.clone_texture() );
  NOM_ASSERT(this->game->combo_text_sprite_ != nullptr);

  nom::set_alignment( this->game->combo_text_sprite_.get(), Point2i::zero,
                      GAME_RESOLUTION, Anchor::MiddleRight );

  this->game->gameover_text.set_text("Same!");
  this->game->same_text_sprite_ =
    std::make_shared<Sprite>( this->game->gameover_text.clone_texture() );
  NOM_ASSERT(this->game->same_text_sprite_ != nullptr);

  nom::set_alignment( this->game->same_text_sprite_.get(), Point2i::zero,
                      GAME_RESOLUTION, Anchor::MiddleRight );

  // Initialize audio subsystem...
  if( this->game->config.get_bool("AUDIO_SFX") ||
      this->game->config.get_bool("AUDIO_TRACKS") ) {

    #if defined(NOM_USE_OPENAL)
      this->audio_dev_.reset( new nom::AudioDevice() );
      this->listener_.reset( new nom::Listener() );

      for( auto idx = 0; idx != NUM_SOUND_BUFFERS; ++idx ) {
        this->sound_buffers[idx].reset( new nom::SoundBuffer() );
      }
    #endif // defined NOM_USE_OPENAL
  }
  else {
    this->audio_dev_.reset( new nom::NullAudioDevice() );
    this->listener_.reset( new nom::NullListener() );

    for( auto idx = 0; idx != NUM_SOUND_BUFFERS; ++idx ) {
      this->sound_buffers[idx].reset( new nom::NullSoundBuffer() );
    }
  }

  // Load audio resources
  if( this->game->config.get_bool("AUDIO_SFX") ) {

    if ( this->sound_buffers[0]->load( this->config.get_string("CURSOR_MOVE") ) == false )
    {
      NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.get_string("CURSOR_MOVE") );
    }

    if ( this->sound_buffers[1]->load( this->config.get_string("CURSOR_CANCEL") ) == false )
    {
      NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.get_string("CURSOR_CANCEL") );
    }

    if ( this->sound_buffers[2]->load( this->config.get_string("CURSOR_WRONG") ) == false )
    {
      NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.get_string("CURSOR_WRONG") );
    }

    if ( this->sound_buffers[3]->load( this->config.get_string("CARD_PLACE") ) == false )
    {
      NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.get_string("CARD_PLACE") );
    }

    if ( this->sound_buffers[4]->load( this->config.get_string("CARD_FLIP") ) == false )
    {
      NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.get_string("CARD_FLIP") );
    }

    if ( this->sound_buffers[5]->load( this->config.get_string("SFX_LOAD_GAME") ) == false )
    {
      NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.get_string("SFX_LOAD_GAME") );
    }

    if ( this->sound_buffers[6]->load( this->config.get_string("SFX_SAVE_GAME") ) == false )
    {
      NOM_LOG_INFO ( TTCARDS, "Could not load resource file: " + this->config.get_string("SFX_SAVE_GAME") );
    }

    #if defined(NOM_USE_OPENAL)
      this->cursor_move.reset( new nom::Sound() );
      this->cursor_cancel.reset( new nom::Sound() );
      this->cursor_wrong.reset( new nom::Sound() );
      this->card_place.reset( new nom::Sound() );
      this->card_flip.reset( new nom::Sound() );
      this->load_game.reset( new nom::Sound() );
      this->save_game.reset( new nom::Sound() );
    #endif // defined NOM_USE_OPENAL

    this->cursor_move->setBuffer( *this->sound_buffers[0] );
    this->cursor_cancel->setBuffer( *this->sound_buffers[1] );
    this->cursor_wrong->setBuffer( *this->sound_buffers[2] );
    this->card_place->setBuffer( *this->sound_buffers[3] );
    this->card_flip->setBuffer( *this->sound_buffers[4] );
    this->load_game->setBuffer( *this->sound_buffers[5] );
    this->save_game->setBuffer( *this->sound_buffers[6] );
  } // end if AUDIO_SFX
  else {
    this->cursor_move.reset( new nom::NullSound() );
    this->cursor_cancel.reset( new nom::NullSound() );
    this->cursor_wrong.reset( new nom::NullSound() );
    this->card_place.reset( new nom::NullSound() );
    this->card_flip.reset( new nom::NullSound() );
    this->load_game.reset( new nom::NullSound() );
    this->save_game.reset( new nom::NullSound() );
  }

  if( this->game->config.get_bool("AUDIO_TRACKS") ) {

    const std::string MUSIC_THEME_TRACK =
      this->config.get_string("MUSIC_THEME_TRACK");
    const std::string MUSIC_WIN_TRACK =
      this->config.get_string("MUSIC_WIN_TRACK");

    if( this->sound_buffers[7]->load(MUSIC_THEME_TRACK) == false ) {
      NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not load resource file: ", MUSIC_THEME_TRACK );
    }

    if( this->sound_buffers[8]->load(MUSIC_WIN_TRACK) == false ) {
      NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_APPLICATION,
                    "Could not load resource file: ", MUSIC_WIN_TRACK );
    }

    #if defined(NOM_USE_OPENAL)
      this->theme_track_.reset( new nom::Music() );
      this->winning_track.reset( new nom::Music() );
    #endif // defined NOM_USE_OPENAL

    this->theme_track_->setBuffer( *this->sound_buffers[7] );
    this->winning_track->setBuffer( *this->sound_buffers[8] );
  } // end if AUDIO_TRACKS
  else {
    this->theme_track_.reset( new nom::NullMusic() );
    this->winning_track.reset( new nom::NullMusic() );
  }

  // ...Initialize our global input action bindings...

  // TODO: Most all of the input action bindings here should be re-initialized
  // at the start of a new game!

  this->game->input_mapper.clear();
  nom::InputActionMapper state;

  auto quit_game( [=](const nom::Event& evt) {
    this->on_app_quit(evt);
  });

  auto on_window_resized( [=](const nom::Event& evt) {
    this->game->on_window_resized(evt);
  });

  auto fps_counter( [=](const nom::Event& evt) {
    this->game->toggle_fps();
  });

  auto pause_music( [=](const nom::Event& evt) {
    this->game->pause_music();
  });

  auto mute_volume( [=](const nom::Event& evt) {
    this->game->mute_volume();
  });

  auto save_screenshot( [=](const nom::Event& evt) {
    this->game->save_screenshot();
  });

  auto reload_config( [=](const nom::Event& evt) {
    this->game->reload_config();
  });

  if( this->game->debug_game_ == true ) {
    auto jumpto_confirmation_dialog_state( [=](const nom::Event& evt) {
      this->set_state(Game::State::ConfirmationDialog);
    });

    auto jumpto_gameover_state( [=](const nom::Event& evt) {
      auto player1_win = new nom::uint32(GameOverType::Won);
      this->set_state(Game::State::GameOver, player1_win);
    });

    auto dump_board( [=](const nom::Event& evt) {
      this->game->dump_board();
    });

    auto dump_player0_hand( [=](const nom::Event& evt) {
      this->game->dump_hand(0);
    });

    auto dump_player1_hand( [=](const nom::Event& evt) {
      this->game->dump_hand(1);
    });

    auto dump_collection( [=](const nom::Event& evt) {
      this->game->dump_collection();
    });

    state.insert( "jumpto_confirmation_dialog_state",
                  nom::KeyboardAction(SDL_KEYDOWN, SDLK_0, KMOD_LGUI),
                  jumpto_confirmation_dialog_state );

    state.insert( "jumpto_gameover_state",
                  nom::KeyboardAction(SDL_KEYDOWN, SDLK_0),
                  jumpto_gameover_state );

    state.insert( "dump_board",
                  nom::KeyboardAction(SDL_KEYDOWN, SDLK_LEFTBRACKET, KMOD_LGUI),
                  dump_board );

    state.insert( "dump_player0_hand",
                  nom::KeyboardAction(SDL_KEYDOWN, SDLK_LEFTBRACKET),
                  dump_player0_hand );

    state.insert( "dump_player1_hand",
                  nom::KeyboardAction(SDL_KEYDOWN, SDLK_RIGHTBRACKET),
                  dump_player1_hand );

    state.insert( "dump_collection",
                  nom::KeyboardAction(  SDL_KEYDOWN, SDLK_RIGHTBRACKET,
                                        KMOD_LGUI ),
                  dump_collection );
  } // end if DEBUG_GAME

  // Conflicts with ConfirmationDialogState key binding
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

int Game::Run()
{
  auto delta_time = 0;
  auto elapsed_frames = 0;
  const std::string WINDOW_TITLE(APP_NAME);
  std::stringstream fps_title_prefix;
  std::stringstream fps_title_suffix;

  this->set_state(Game::State::CardsMenu);

  fps_title_prefix << WINDOW_TITLE << " " << "-" << " ";

  nom::Event evt;
  while( this->running() == true ) {

    delta_time = nom::ticks();

    while( this->poll_event(evt) ) {
      this->on_event(evt);
    }

    this->on_update(delta_time);
    this->game->actions_.update(delta_time);

    // Fix for GitHub Issue #9
    this->window.fill(nom::Color4i::Black);

    this->on_draw(this->window);

    if( this->show_fps() ) {

      // TODO: Update frame rate counter at one second intervals

      // TODO: Look into our FPS counter possibly being inaccurate -- why can
      // we never reach within the last ~5..7 frames of the set frame
      // interval??? Perhaps it has something to do with our sleep
      // granularity...?
      real32 frame_interval =
        elapsed_frames / (delta_time / 1000.0f);
      fps_title_suffix.str("");
      fps_title_suffix.precision(1);

      fps_title_suffix  << fps_title_prefix.str()
                        << std::fixed << frame_interval << " " << "FPS";
      this->window.set_window_title( fps_title_suffix.str() );
    } else {
      // Use the default window title string
      this->window.set_window_title(WINDOW_TITLE);
    }

    ++elapsed_frames;

    ttcards::set_frame_interval(this->frame_interval_);
  } // end while game is running

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
  this->game->theme_track_->togglePause();
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

void Game::reload_config()
{
  // High-level file I/O interface for configuration files
  std::unique_ptr<nom::IValueDeserializer> fp =
    nom::make_unique<nom::JsonCppDeserializer>();
  NOM_ASSERT(fp != nullptr);

  if( this->game->config.load_file(TTCARDS_CONFIG_FILENAME, fp.get() ) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not reload configuration file at:",
                  TTCARDS_CONFIG_FILENAME );
    return;
  }

  NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_APPLICATION,
                "Reloaded game configuration file:",
                TTCARDS_CONFIG_FILENAME );

  NOM_ASSERT(this->state() != nullptr);

  // NOTE: Prevents states crash
  if( this->state()->current_state_id() != Game::State::CardsMenu ) {
    this->set_state(Game::State::CardsMenu);
  }

  this->game->debug_game_ =
    this->game->config.get_bool("DEBUG_GAME");
}

void Game::dump_board( void )
{
  if( this->game->board_ != nullptr ) {
    this->game->board_->dump_values();
  }
}

void Game::dump_hand( nom::uint32 player_id )
{
  this->game->debug.ListCards( this->game->hand[player_id].cards );
}

void Game::dump_collection( void )
{
  if( this->game->cards_db_[PLAYER1] != nullptr ) {

    for(  auto itr = this->game->cards_db_[PLAYER1]->begin();
          itr != this->game->cards_db_[PLAYER1]->end();
          ++itr )
    {
      NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_APPLICATION, "FIXME" );
      // this->game->debug.ListCards(*itr);
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
