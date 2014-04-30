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
#include "PlayState.hpp"

using namespace nom;

PlayState::PlayState ( const nom::SDLApp::shared_ptr& object ) :
  game { NOM_PTR_CAST( Game, object) }
{
  NOM_LOG_TRACE( TTCARDS );

  this->turn = 0;
  this->cursor_locked = false;
  this->skip_turn = false;
  this->gameover_state = GameOverType::NotOver;
}

PlayState::~PlayState ( void )
{
  NOM_LOG_TRACE( TTCARDS );
}

void PlayState::on_exit ( nom::void_ptr data ) {}

void PlayState::on_pause ( nom::void_ptr data )
{
  std::cout << "\n" << "PlayState Paused" << "\n";
}

void PlayState::on_resume ( nom::void_ptr data )
{
  std::cout << "\n" << "PlayState Resumed" << "\n";
}

void PlayState::on_init ( nom::void_ptr data )
{
  // UI elements below are used in the construction of both dialog message boxes!
  nom::Window* window = nullptr;

  Point2i info_box_origin = Point2i( INFO_BOX_ORIGIN_X, INFO_BOX_ORIGIN_Y );
  Size2i info_box_size = Size2i( INFO_BOX_WIDTH, INFO_BOX_HEIGHT );
  Point2i debug_box_origin = Point2i( DEBUG_BOX_ORIGIN_X, DEBUG_BOX_ORIGIN_Y );
  Size2i debug_box_size = Size2i( DEBUG_BOX_WIDTH, DEBUG_BOX_HEIGHT );

  while ( this->game->hand[0].size() < MAX_PLAYER_HAND )
  {
    this->game->hand[0].shuffle ( 8, 10, this->game->collection );
  }

  // Clear CPUPlayer's hand (we will initialize it here soon enough
  this->game->hand[1].clear();

  // Random seeding for picking out whose turn it is initially
  nom::int32 seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine rand_generator ( seed );
  std::uniform_int_distribution<nom::int32> distribution ( 0, TOTAL_PLAYERS - 1 );

  this->game->cursor.set_position ( Point2i(PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y) );
  this->game->cursor.set_frame ( INTERFACE_CURSOR_NONE ); // default cursor image
  this->game->cursor.set_state ( 0 ); // default state; player hand select

  this->game->rules.setRules ( 1 );
  this->game->board = Board ( this->game->rules, &this->game->card );

  this->player[0] = Player ( &this->game->hand[0], &this->game->card, this->game->rules );
  this->player[0].set_position( Point2i( PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y ) );

  this->player[1] = Player ( &this->game->hand[1], &this->game->card, this->game->rules );
  this->player[1].set_position( Point2i( PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y ) );

  // player1, player2 cursor X, Y coords
  this->player_cursor_coords[0] = nom::Point2i( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y );
  this->player_cursor_coords[1] = nom::Point2i( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y );

  // Compute the player's scoreboard X, Y coordinate positions
  this->player_scoreboard[0] = Point2i( PLAYER1_SCORE_ORIGIN_X, PLAYER1_SCORE_ORIGIN_Y );
  this->player_scoreboard[1] = Point2i( PLAYER2_SCORE_ORIGIN_X, PLAYER2_SCORE_ORIGIN_Y );

  // Set both player's scoreboard fonts
  for ( nom::uint32 idx = 0; idx < TOTAL_PLAYERS; ++idx )
  {
    this->scoreboard_text[idx].set_font( &this->game->scoreboard_font );
    this->scoreboard_text[idx].set_text_size ( 48 * SCALE_FACTOR );
    this->scoreboard_text[idx].set_style ( nom::Text::Style::Italic );
  }

  // Initialize game over text
  this->gameover_text.set_font( &this->game->gameover_font );
  this->gameover_text.set_text_size( 48 * SCALE_FACTOR );
  this->gameover_text.set_style( nom::Text::Style::Italic );

  for ( nom::int32 idx = 0; idx < MAX_PLAYER_HAND; idx++ )
  {
    this->cursor_coords_map[idx] = nom::Point2i( idx, this->player_cursor_coords[0].y + ( CARD_HEIGHT / 2 * idx ) );
  }

  window = new nom::Window( debug_box_origin, debug_box_size );

  // Our gradient-filled background colors; from starting color to ending color.
  nom::Gradient::Colors g_colors = { nom::Color4i::Gray, nom::Color4i::LightGray };

  // Construct the gradient used as the backdrop of the dialog message box.
  window->set_shape( new nom::Gradient( g_colors, debug_box_origin, debug_box_size, nom::Point2i( 0,0 ), nom::Gradient::FillDirection::Left ) );

  window->set_shape( new nom::GrayFrame( debug_box_origin, debug_box_size ) );

  this->debug_box = nom::MessageBox::unique_ptr (
                                                  new nom::MessageBox(
                                                    window,
                                                    debug_box_origin,
                                                    debug_box_size
                                                  )
                                                );

  // Title label text *must* be ALL CAPITALS; the small bitmap font used only
  // has the glyphs for the capital letters of the alphabet.
  this->debug_box->set_title_label( nom::Text("INFO.", &this->game->info_small_text, 9, nom::Text::Alignment::TopLeft) );

  // Initialize message label text to sane defaults; see also:
  // ::on_update_info_dialogs method.
  this->debug_box->set_message_label( nom::Text("", &this->game->info_text, -1, nom::Text::Alignment::MiddleCenter ) );

  #if ! defined ( NOM_DEBUG )
    this->debug_box->disable();
  #endif

  // Re-declare the nom::Window object with new dimensions for our second
  // nom::MessageBox object.
  window = new nom::Window( info_box_origin, info_box_size );

  // Construct the gradient used as the backdrop of the dialog message box.
  window->set_shape( new nom::Gradient( g_colors, info_box_origin, info_box_size, nom::Point2i( 0,0 ), nom::Gradient::FillDirection::Left ) );
  window->set_shape( new nom::GrayFrame( info_box_origin, info_box_size ) );

  this->info_box = nom::MessageBox::unique_ptr  (
                                                  new nom::MessageBox(
                                                    window,
                                                    info_box_origin,
                                                    info_box_size
                                                  )
                                                );

  // Title label text *must* be ALL CAPITALS; the small bitmap font used only
  // has the glyphs for the capital letters of the alphabet.
  this->info_box->set_title_label( nom::Text("INFO.", &this->game->info_small_text, 9, nom::Text::Alignment::TopLeft ) );

  // Initialize message label text to sane defaults; see also:
  // ::on_update_info_dialogs method.
  this->info_box->set_message_label( nom::Text("", &this->game->info_text, -1, nom::Text::Alignment::MiddleCenter ) );

  while ( this->game->hand[1].size() < MAX_PLAYER_HAND )
  {
    this->game->hand[1].shuffle ( 1, 10, this->game->collection );
  }

  // Initialize player cards to their respective defaults; this lets us know not
  // only whose cards they are originally but also presently -- critical in card
  // flipping, scoreboard keeping and end of game tallying logic.
  this->player[0].setID ( Card::PLAYER1 );
  this->player[1].setID ( Card::PLAYER2 );

  // Update both player scores now that we have the player scoreboard X, Y
  // origins calculated for rendering.
  //
  // Assuming a player hand of five, this should always assign a starting score
  // value of 5 for each player initially.
  this->updateScore();

  // Set whose turn it is initially using a random number generator with equal
  // odds -- 50/50 chance that you will have the first move!
  this->player_turn ( nom::rand ( 0, TOTAL_PLAYERS - 1 ) );

  // Initialize our animation state timers
  this->player_timer[1].setFrameRate ( 500 );
  this->cursor_blink.start();
  this->blink_cursor = false;

  // FIXME:
  // delete window;
}

void PlayState::on_key_down( const nom::Event& ev )
{
  nom::uint32 player_turn = get_turn();

  switch ( ev.key.sym )
  {
    default: /* Ignore unmapped keys */ break;

    // Pause game
    case SDLK_p:
    {
      this->game->set_state( Game::State::Pause );
      break;
    }

    case SDLK_s: // Save current game
    {
      if ( this->game->hand[0].save( USER_PLAYER1_FILENAME ) == false )
      {
        NOM_LOG_ERR ( TTCARDS, "Unable to save game data at: " + USER_PLAYER1_FILENAME );
        this->game->cursor_wrong.Play();
        break;
      }

      NOM_LOG_INFO ( TTCARDS, "Saved player 1 hand data at: " + std::string(USER_PLAYER1_FILENAME) );

      if ( this->game->hand[1].save( USER_PLAYER2_FILENAME ) == false )
      {
        NOM_LOG_ERR ( TTCARDS, "Unable to save game data at: " + USER_PLAYER2_FILENAME );
        this->game->cursor_wrong.Play();
        break;
      }

      NOM_LOG_INFO ( TTCARDS, "Saved player 2 hand data at: " + std::string(USER_PLAYER2_FILENAME) );

      if ( this->game->board.save( USER_BOARD_FILENAME ) == false )
      {
        NOM_LOG_ERR ( TTCARDS, "Unable to save game data at: " + USER_BOARD_FILENAME );
        this->game->cursor_wrong.Play();
        break;
      }

      NOM_LOG_INFO ( TTCARDS, "Saved board data at: " + std::string(USER_BOARD_FILENAME) );

      this->game->save_game.Play(); // Successful saved game!
    }
    break;

    case SDLK_l: // Load saved game
    {
      if ( ev.key.mod == KMOD_LGUI ) // Special game load (player1 always wins!)
      {
        if ( this->game->hand[0].load ( "Debug" + path.native() + "player1_unbeatable.json" ) == false )
        {
          NOM_LOG_ERR ( TTCARDS, "Unable to load game data from: " + std::string("player1_unbeatable.json") );
          this->game->cursor_wrong.Play();
          break;
        }
        NOM_LOG_INFO ( TTCARDS, "Loaded player 1 data from: " + std::string("player1_unbeatable.json") );
      }
      else // Normal game load for player1
      {
        if ( this->game->hand[0].load( USER_PLAYER1_FILENAME ) == false )
        {
          NOM_LOG_ERR ( TTCARDS, "Unable to load game data from: " + USER_PLAYER1_FILENAME );
          this->game->cursor_wrong.Play();
          break;
        }
        NOM_LOG_INFO ( TTCARDS, "Loaded player 1 data from: " + std::string(USER_PLAYER1_FILENAME) );
      }
      if ( this->game->hand[1].load( USER_PLAYER2_FILENAME ) == false )
      {
        NOM_LOG_ERR ( TTCARDS, "Unable to load game data from: " + USER_PLAYER2_FILENAME );
        this->game->cursor_wrong.Play();
        break;
      }
      NOM_LOG_INFO ( TTCARDS, "Loaded player 2 data from: " + std::string(USER_PLAYER2_FILENAME) );

      if ( this->game->board.load( USER_BOARD_FILENAME ) == false )
      {
        NOM_LOG_ERR ( TTCARDS, "Unable to load game data from: " + USER_BOARD_FILENAME );
        this->game->cursor_wrong.Play();
        break;
      }

      NOM_LOG_INFO ( TTCARDS, "Loaded board data from: " + std::string(USER_BOARD_FILENAME) );

      // Successful load of saved game!
      this->updateScore();
      this->resetCursor();

      this->game->load_game.Play();
    }
    break;

#if defined (NOM_DEBUG)
    case SDLK_e: // Full control over the other player's move
    {
      if ( ev.key.mod == KMOD_LGUI ) // Return control over to the other player
      {
        this->skip_turn = false;
        this->player[1].set_state ( PlayerState::Reserved );
        this->endTurn();
      }
      else
      {
        this->skip_turn = true;
        this->player[1].set_state ( PlayerState::Debug );
        this->endTurn();
      }
    }
    break;
#endif

    case SDLK_d:
    {
      this->game->hand[player_turn].erase ( this->game->hand[player_turn].getSelectedCard() );

      this->game->cursor.set_position ( Point2i(this->player_cursor_coords[player_turn].x, this->player_cursor_coords[player_turn].y) );
    }
    break;

    case SDLK_i:
    {
      if ( this->debug_box->enabled() == true )
      {
        this->debug_box->disable();
      }
      else
      {
        this->debug_box->enable();
      }
    }
    break;

    case SDLK_LEFT:
    {
      if ( ev.key.mod == KMOD_LSHIFT ) // increase card rank attribute by + 1
      {
        this->game->hand[ player_turn ].modifyCardRank ( true, WEST );
      }
      else if ( ev.key.mod == KMOD_LCTRL ) // decrease card rank attribute by - 1
      {
        this->game->hand[ player_turn ].modifyCardRank ( false, WEST );
      }
      else
      {
        this->moveCursorLeft();
      }
    }
    break;

    case SDLK_RIGHT:
    {
      if ( ev.key.mod == KMOD_LSHIFT ) // increase card rank attribute by + 1
      {
        this->game->hand[ player_turn ].modifyCardRank ( true, EAST );
      }
      else if ( ev.key.mod == KMOD_LCTRL ) // decrease card rank attribute by - 1
      {
        this->game->hand[ player_turn ].modifyCardRank ( false, EAST );
      }
      else
      {
        this->moveCursorRight();
      }
    }
    break;

    case SDLK_UP:
    {
      if ( ev.key.mod == KMOD_LSHIFT ) // increase card rank attribute by + 1
      {
        this->game->hand[ player_turn ].modifyCardRank ( true, NORTH );
      }
      else if ( ev.key.mod == KMOD_LCTRL ) // decrease card rank attribute by - 1
      {
        this->game->hand[ player_turn ].modifyCardRank ( false, NORTH );
      }
      else
      {
        this->moveCursorUp();
      }
    }
    break;

    case SDLK_DOWN:
    {
      if ( ev.key.mod == KMOD_LSHIFT ) // increase card rank attribute by + 1
      {
        this->game->hand[ player_turn ].modifyCardRank ( true, SOUTH );
      }
      else if ( ev.key.mod == KMOD_LCTRL ) // decrease card rank attribute by - 1
      {
        this->game->hand[ player_turn ].modifyCardRank ( false, SOUTH );
      }
      else
      {
        this->moveCursorDown();
      }
    }
    break;

    case SDLK_x: this->unlockSelectedCard(); break;
    case SDLK_SPACE: this->lockSelectedCard(); break;

    // move selected card to grid[0][0]
    case SDLK_1: if ( player_turn != PLAYER2 ) this->moveTo ( 0, 0 ); break;

    // move selected card to grid[1][0]
    case SDLK_2: if ( player_turn != PLAYER2 ) this->moveTo ( 1, 0 ); break;

    // move selected card to grid[2][0]
    case SDLK_3: if ( player_turn != PLAYER2 ) this->moveTo ( 2, 0 ); break;

    // move selected card to grid[0][1]
    case SDLK_4: if ( player_turn != PLAYER2 ) this->moveTo ( 0, 1 ); break;

    // move selected card to grid[1][1]
    case SDLK_5: if ( player_turn != PLAYER2 ) this->moveTo ( 1, 1 ); break;

    // move selected card to grid[2][1]
    case SDLK_6: if ( player_turn != PLAYER2 ) this->moveTo ( 2, 1 ); break;

    // move selected card to grid[0][2]
    case SDLK_7: if ( player_turn != PLAYER2 ) this->moveTo ( 0, 2 ); break;

    // move selected card to grid[1][2]
    case SDLK_8: if ( player_turn != PLAYER2 ) this->moveTo ( 1, 2 ); break;

    // move selected card to grid[2][2] if possible
    case SDLK_9: if ( player_turn != PLAYER2 ) this->moveTo ( 2, 2 ); break;
  } // end key switch
}

void PlayState::on_mouse_left_button_down( const nom::Event& ev )
{
  uint32 player_turn = this->get_turn(); // Ignore player2 mouse input

  // Player cursor positioning
  Point2i player_pos = this->player[player_turn].position();

  // mouse input coordinates
  Point2i mouse_input ( ev.mouse.x, ev.mouse.y );

  // Rectangle bounds of player's card
  IntRect card_bounds;

  // Disable mouse input if we are not controlling the other player
  if ( this->skip_turn == false )
  {
    if ( this->get_turn() != 0 ) return;
  }

  // Player hand selection checks; we must calculate the valid bounds of any
  // given card in the player's hand -- the current player position combined
  // with the known card size yields rectangular coordinates we can compare.
  for ( nom::int32 idx = 0; idx < this->game->hand[ player_turn ].size(); idx++ )
  {
    card_bounds = IntRect ( player_pos.x,
                            player_pos.y,
                            CARD_WIDTH,
                            ( CARD_HEIGHT / 2  ) * ( idx + 1 )
                          );

    // If the mouse coordinates match within the card bounds, we have a match!
    if ( card_bounds.contains( mouse_input ) )
    {
      // 1. Update player's selected card
      // 2. Update cursor position
      // 3. Play sound event
      this->game->hand[ player_turn ].selectCard ( this->game->hand[ player_turn ].cards[ idx ] );

      this->game->cursor.set_position ( Point2i(this->player_cursor_coords[ player_turn ].x, this->player_cursor_coords[ player_turn ].y + ( CARD_HEIGHT / 2 ) * idx) );

      this->game->cursor_move.Play();

      // We must break the loop here upon the end of a matching coords check
      // in order to prevent a nasty "last card stays permanently selected"
      // bug from cropping back up!
      break;
    }
  }

  // Board grid coords check; player is attempting to place a card on the board
  // when the player hand coords check above comes back false
  IntRect mouse_map = this->game->board.getGlobalBounds ( ev.mouse.x, ev.mouse.y );

  // Attempts to move card onto board; validity checking is performed within
  // the following method call
  if ( mouse_map != nom::IntRect::null ) // undefined if -1, -1
  {
    this->moveTo ( mouse_map.x, mouse_map.y );
  }
}

void PlayState::on_mouse_right_button_down( const nom::Event& ev )
{
  // Stub
}

void PlayState::on_mouse_wheel( const nom::Event& ev )
{
  // Do not check mouse wheel state unless it is a valid event; we receive
  // invalid data here if we do not check for this.
  if( ev.type != SDL_MOUSEWHEEL ) return;

  if ( this->game->cursor.state() == 0 ) // Player's hand mode
  {
    if ( ev.wheel.y > 0 )
    {
      this->moveCursorUp();
    }
    else if ( ev.wheel.y < 0 )
    {
      this->moveCursorDown();
    }
  }
}

void PlayState::on_joy_button_down( const nom::Event& ev )
{
  switch ( ev.jbutton.button )
  {
    default: break;

    case nom::PSXBUTTON::START:
    {
      this->game->set_state( Game::State::Pause);
      break;
    }

    case nom::PSXBUTTON::UP: this->moveCursorUp(); break;
    case nom::PSXBUTTON::RIGHT: this->moveCursorRight(); break;
    case nom::PSXBUTTON::DOWN: this->moveCursorDown(); break;
    case nom::PSXBUTTON::LEFT: this->moveCursorLeft(); break;

    case nom::PSXBUTTON::TRIANGLE: /* TODO */ break;
    case nom::PSXBUTTON::CIRCLE: this->unlockSelectedCard(); break;
    case nom::PSXBUTTON::CROSS: this->lockSelectedCard(); break;
  }
}

unsigned int PlayState::get_turn ( void )
{
  return this->turn;
}

void PlayState::player_turn ( unsigned int player )
{
  this->turn = player;

  this->resetCursor();
}

// Helper method for incrementing to next player's turn
void PlayState::endTurn ( void )
{
  this->updateScore();

  this->unlockSelectedCard();

  this->game->hand[PLAYER1].clearSelectedCard();
  this->game->hand[PLAYER2].clearSelectedCard();

  if ( this->get_turn() == PLAYER1 )
  {
    this->player_turn ( PLAYER2 );
  }
  else if ( this->get_turn() == PLAYER2 )
  {
    this->player_turn ( PLAYER1 );
  }
}

void PlayState::on_update_info_dialogs( void )
{
  nom::uint32 player_turn = get_turn();
  Card selected_card; // temp container var to hold our card info (ID, name)
  nom::IntRect coords; // temp container var to hold cursor pos mapping coords

  // Board selection state
  if ( this->isCursorLocked() == true )
  {
    coords = this->game->board.getGlobalBounds ( this->game->cursor.x(), this->game->cursor.y() );
    if ( coords != nom::IntRect::null )
    {
      selected_card = this->game->board.get ( coords.x, coords.y );
    }
  }
  else // Player hand selection state
  {
    // Allow watching both players make their card selections for DEBUG
    // (think: development) builds.
    #if defined ( NOM_DEBUG )
        selected_card = this->game->hand[player_turn].getSelectedCard();
    #else // Do not show the actions of other players
        selected_card = this->game->hand[PLAYER1].getSelectedCard();
    #endif
  }

  if ( selected_card.getID() != BAD_CARD_ID )
  {
    std::string card_id = selected_card.get_id_string();
    std::string card_name = selected_card.getName();

    // (Northern) debug info box
    this->debug_box->set_message_text( card_id );

    // (Southern) info card box
    this->info_box->set_message_text( card_name );
  }
}

bool PlayState::isCursorLocked ( void )
{
  if ( this->cursor_locked == true )
  {
    return true;
  }
  else
  {
    return false;
  }
}

void PlayState::lockCursor ( bool lock )
{
  this->cursor_locked = lock;
}

// Helper method for resetting cursor related input
void PlayState::resetCursor ( void )
{
  unsigned int player_turn = get_turn();

  this->game->hand[player_turn].selectCard ( this->game->hand[player_turn].cards.front() );

  this->game->cursor.set_state ( 0 );
  this->game->cursor.set_position ( Point2i(this->player_cursor_coords[0].x, this->player_cursor_coords[0].y) );

  // Only set the position of the game interface cursor for player2 when we are
  // controlling him
  if ( this->skip_turn == true )
  {
    this->game->cursor.set_position ( Point2i(this->player_cursor_coords[player_turn].x, this->player_cursor_coords[player_turn].y) );
  }
}

// helper method for cursor input selection
void PlayState::unlockSelectedCard ( void )
{
  this->game->cursor.set_state ( 0 ); // player card select

  this->resetCursor();

  this->lockCursor ( false );

  this->game->cursor_cancel.Play();
}

// helper method for cursor input selection
void PlayState::lockSelectedCard ( void )
{
  nom::IntRect coords; // temp container var to hold cursor pos mapping coords

  this->game->cursor.set_state ( 1 ); // board select

  if ( this->isCursorLocked() == false )
  {
    if ( get_turn() == 0 )
    {
      this->game->cursor.set_position ( Point2i(CURSOR_ORIGIN_X-16, CURSOR_ORIGIN_Y) ); // FIXME
    }
    else if ( get_turn() == 1 )
    {
      this->game->cursor.set_position ( Point2i(CURSOR_ORIGIN_X+16, CURSOR_ORIGIN_Y) ); // FIXME
    }

    this->lockCursor ( true );
  }
  else
  {
    coords = this->game->board.getGlobalBounds ( this->game->cursor.x(), this->game->cursor.y() );

    if ( coords != nom::IntRect::null )
    {
      this->moveTo ( coords.x, coords.y );
    }

    this->unlockSelectedCard();
  }
}

// Helper method for updating board with player's selected card
void PlayState::moveTo ( unsigned int x, unsigned int y )
{
  Card selected;
  nom::uint32 player_turn = this->get_turn();

  selected = this->game->hand[ player_turn ].getSelectedCard();

  if ( player_turn == PLAYER1 )
  {
    std::vector<BoardTile> adj = this->game->board.find_adjacent ( x, y );

    // Dump returned list of cards
    nom::uint32 line_number = 1;
    for ( auto idx = 0; idx < adj.size(); idx++ )
    {
      Card tile = adj[idx].tile();

      if ( tile.getPlayerID() != Card::PLAYER1 )
      {
        nom::IntRect pos = adj[idx].bounds();
        nom::uint32 element = adj[idx].element();

        std::cout << line_number
                  << ". "
                  << tile.getName()
                  << " is at pos "
                  << pos.x
                  << ", "
                  << pos.y
                  << " ("
                  << pos.w
                  << "x"
                  << pos.h
                  << ") "
                  << " with an element ID of "
                  << element
                  << "."
                  << "\n";

        line_number++;
      }
    }

    /*
        for ( nom::int32 cols = y; cols < BOARD_GRID_HEIGHT; cols++ )
        {
          for ( nom::int32 rows = x; rows < BOARD_GRID_WIDTH; rows++ )
          {
            if ( rows != 0 )
            {

            }
          }
        }
        */
  }

  //std::cout << "\n";

  if ( selected.getID() != BAD_CARD_ID )
  {
    if ( player_turn == PLAYER1 && this->game->board ( x, y ) != BAD_CARD_ID )
    {
      this->game->cursor_wrong.Play();
    }

    if ( this->game->board ( x, y ) == BAD_CARD_ID )
    {
      this->game->board.updateStatus ( x, y, this->game->hand[ player_turn ].getSelectedCard() );
      this->game->hand[ player_turn ].erase ( this->game->hand[ player_turn ].getSelectedCard() );

      this->game->card_place.Play();

      std::vector<std::pair<int, int>> grid = this->game->board.checkBoard ( x, y );

      if ( grid.empty() == false )
      {
        if ( this->game->rules.getRules() == 0 )
        {
          this->game->board.flipCard ( grid[0].first, grid[0].second, player_turn + 1 );
          this->game->card_flip.Play();
        }
      }

      if ( this->game->rules.getRules() != 0 )
      {
        for ( nom::int32 g = 0; g < grid.size(); g++ )
        {
          this->game->board.flipCard ( grid[g].first, grid[g].second, player_turn + 1 );
          this->game->card_flip.Play();

          std::vector<std::pair<int, int>> tgrid = this->game->board.checkBoard ( grid[g].first, grid[g].second );

          // temporary workaround until a more proper solution is found
          if ( this->game->rules.getRules() == 2 || this->game->rules.getRules() == 4 )
            continue;
          else
          {
            for ( nom::int32 tg = 0; tg < tgrid.size(); tg++ )
            {
              this->game->board.flipCard( tgrid[tg].first, tgrid[tg].second, player_turn + 1 );
              this->game->card_flip.Play();
            }
          }
        }
      }
      this->endTurn();
    }
  }
}

// Helper method for obtaining card hand index position based off given origin
// coords definitions, creating us an ID map, initialized early on within the
// encapsulating class
//
// Works with both player1 and player2 coords mapping
//
// cursor_coords_map
//   [ index, y coordinate value ]
//
unsigned int PlayState::getCursorPos ( void )
{
  unsigned int pos = 0;
  nom::int32 idx = 0;

  for ( idx = 0; idx < MAX_PLAYER_HAND; idx++ )
  {
    if ( this->game->cursor.y() <= this->cursor_coords_map[idx].y )
      return this->cursor_coords_map[idx].x;
    else // catch all safety switch
      // assume we are at the last position in the index when all else fails
      pos = MAX_PLAYER_HAND;
  }

  return pos;
}

void PlayState::moveCursorLeft ( void )
{
  if ( this->game->cursor.state() == 1 ) // locked cursor to board select mode
  {
    if ( this->game->cursor.x() > BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) )
      this->game->cursor.move ( -( CARD_WIDTH ), 0 );
  }
  this->game->cursor_move.Play();
}

void PlayState::moveCursorRight ( void )
{
  if ( this->game->cursor.state() == 1 ) // locked cursor to board select mode
  {
    if ( this->game->cursor.x() < BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) )
      this->game->cursor.move ( ( CARD_WIDTH ), 0 );
  }
  this->game->cursor_move.Play();
}

void PlayState::moveCursorUp ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->game->cursor.state() == 0 )
  {
    if ( this->game->cursor.y() > PLAYER1_CURSOR_ORIGIN_Y )
    {
      this->game->cursor.move ( 0, -( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->game->hand[player_turn].selectCard ( this->game->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->game->cursor.state() == 1 ) // locked cursor to board select mode
  {
    if ( this->game->cursor.y() > BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) )
      this->game->cursor.move ( 0, -( CARD_HEIGHT ) );
  }
  this->game->cursor_move.Play();
}

void PlayState::moveCursorDown ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->game->cursor.state() == 0 )
  {
    if ( this->game->cursor.y() < ( CARD_HEIGHT / 2 ) * ( this->game->hand[player_turn].size() ) )
    {
      this->game->cursor.move ( 0, ( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->game->hand[player_turn].selectCard ( this->game->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->game->cursor.state() == 1 ) // locked cursor to board select mode
  {
    if ( this->game->cursor.y() < BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) )
      this->game->cursor.move ( 0, ( CARD_HEIGHT ) );
  }
  this->game->cursor_move.Play();
}

void PlayState::updateCursor ( void )
{
  if ( this->game->cursor.state() == 1 )
  {
    if ( this->cursor_blink.ticks() > 192 ) // Blinky blink!
    {
      this->cursor_blink.stop();
      this->game->cursor.set_frame ( INTERFACE_CURSOR_NONE );
      this->blink_cursor = true;
    }
  }

  if ( this->get_turn() == PLAYER1 && this->blink_cursor == false ) // player1
  {
    this->game->cursor.set_frame ( INTERFACE_CURSOR_RIGHT );
  }
  // Only show interface cursor for player2 when we are controlling him
  else if ( this->skip_turn == true && this->get_turn() == PLAYER2 && this->blink_cursor == false )
  {
    this->game->cursor.set_frame ( INTERFACE_CURSOR_LEFT );
  }

  this->game->cursor.update();
}

void PlayState::drawCursor ( nom::IDrawable::RenderTarget& target )
{
  this->game->cursor.draw ( target );

  if ( this->blink_cursor )
  {
    this->game->cursor.set_frame ( INTERFACE_CURSOR_RIGHT );
    this->cursor_blink.start();
    this->blink_cursor = false;
  }
}

void PlayState::updateScore ( void )
{
  for ( nom::uint32 players = 0; players < TOTAL_PLAYERS; players++ )
  {
    // Number of cards player owns on the board
    nom::uint32 board_count = this->game->board.getPlayerCount ( players + 1 );

    // Number of cards player has remaining
    nom::uint32 hand_count = this->game->hand[players].size();

    this->player[players].setScore ( board_count + hand_count );

    // Update the font responsible for rendering the score
    this->scoreboard_text[players].set_text ( this->player[players].getScoreAsString() );
    this->scoreboard_text[players].set_position ( nom::Point2i (this->player_scoreboard[players].x, this->player_scoreboard[players].y) );
  }
}

void PlayState::on_update ( float delta_time )
{
  this->game->board.update();

  this->updateCursor();

  this->on_update_info_dialogs();

  this->player[0].update();
  this->player[1].update();

  // Player two animation effect
  if ( ! ( this->player_timer[1].ticks() + this->player_timer[1].framerate() >= delta_time ) )
  {
    this->player_timer[1].start();

    // Only show player2 animation when we are not controlling him
    if ( this->skip_turn == false )
    {
      nom::uint32 rand_pick = nom::rand ( 0, this->game->hand[PLAYER2].size() - 1 );
      this->game->hand[PLAYER2].selectCard ( this->game->hand[PLAYER2].cards[ rand_pick ] );
    }
  }

  if ( this->get_turn() == 0 ) // player1
  {
    this->player_rect = nom::Rectangle ( nom::IntRect( PLAYER1_INDICATOR_ORIGIN_X, PLAYER1_INDICATOR_ORIGIN_Y, PLAYER_INDICATOR_WIDTH, PLAYER_INDICATOR_HEIGHT), nom::Color4i( 188, 203, 236 ) );
  }
  else // player2
  {
    this->player_rect = nom::Rectangle ( nom::IntRect( PLAYER2_INDICATOR_ORIGIN_X, PLAYER2_INDICATOR_ORIGIN_Y, PLAYER_INDICATOR_WIDTH, PLAYER_INDICATOR_HEIGHT), nom::Color4i( 222, 196, 205 ) );

    // Skipping a turn like this is only available in debug versions
    if ( this->skip_turn == false )
    {
      nom::IntRect board_edges[4];

      board_edges[0].x = 0;
      board_edges[0].y = 0;

      board_edges[1].x = 2;
      board_edges[1].y = 0;

      board_edges[2].x = 0;
      board_edges[2].y = 2;

      board_edges[3].x = 2;
      board_edges[3].y = 2;

      nom::int32 edge_pick = nom::rand ( 0, 3 );

      nom::uint32 rand_pick = nom::rand ( 0, this->game->hand[1].size() - 1 );
      this->game->hand[1].selectCard ( this->game->hand[1].cards[ rand_pick ] );

      if ( this->game->board.getStatus ( board_edges[0].x, board_edges[0].y ) == BAD_CARD_ID )
      {
        this->moveTo ( board_edges[ edge_pick ].x, board_edges[ edge_pick ].y );
      }
      else if ( this->game->board.getStatus ( board_edges[1].x, board_edges[1].y ) == BAD_CARD_ID )
      {
        this->moveTo ( board_edges[1].x, board_edges[1].y );
      }
      else if ( this->game->board.getStatus ( board_edges[2].x, board_edges[2].y ) == BAD_CARD_ID )
      {
        this->moveTo ( board_edges[2].x, board_edges[2].y );
      }
      else if ( this->game->board.getStatus ( board_edges[3].x, board_edges[3].y ) == BAD_CARD_ID )
      {
        this->moveTo ( board_edges[3].x, board_edges[3].y );
      }
      else
      {
        nom::int32 moveX = nom::rand ( 0, 2 );
        nom::int32 moveY = nom::rand ( 0, 2 );
        nom::uint32 rand_pick = nom::rand ( 0, this->game->hand[1].size() - 1 );
        this->game->hand[1].selectCard ( this->game->hand[1].cards[ rand_pick ] );

        this->moveTo ( moveX, moveY );
      } // end board_edges choice
    } // player2
  } // end player1

  this->game->window.update();
}

void PlayState::on_draw ( nom::IDrawable::RenderTarget& target )
{
  this->game->background.draw ( target );

  this->game->board.draw ( target );

  this->player[0].draw ( target );
  this->player[1].draw ( target );

  this->player_rect.draw ( target );

  this->drawCursor ( target );

  this->debug_box->draw( target );
  this->info_box->draw( target );

  // Draw each player's scoreboard
  this->scoreboard_text[0].draw ( target );
  this->scoreboard_text[1].draw ( target );

  // FIXME: We keep game over check logic here in order to allow for the last
  // card placed to be shown to the player
  //
  // Game Over States
  // game / round is over when board card count >= 9
  if ( this->game->board.getCount () >= 9 || this->game->hand[ PLAYER1 ].size() == 0 || this->game->hand[ PLAYER2 ].size() == 0 )
  {
    if ( this->player[ PLAYER1 ].getScore() > this->player[ PLAYER2 ].getScore() )
    {
      this->gameover_state = GameOverType::Won;
      this->gameover_text.set_color ( nom::Color4i::White );
      this->gameover_text.set_text ( "You win!" );
    }
    else if ( this->player[ PLAYER1 ].getScore() < this->player[ PLAYER2 ].getScore() )
    {
      this->gameover_state = GameOverType::Lost;
      this->gameover_text.set_color ( nom::Color4i::White );
      this->gameover_text.set_text ( "You lose..." );
    }
    else // Assume a draw
    {
      this->gameover_state = GameOverType::Tie;
      this->gameover_text.set_color ( nom::Color4i::White );
      this->gameover_text.set_text ( "Tie!" );
    }

    nom::Point2i pos = nom::Point2i ( 0, 0 );
    nom::Size2i size = nom::Size2i ( SCREEN_WIDTH, SCREEN_HEIGHT );

    this->gameover_text.set_position ( pos );
    this->gameover_text.set_size ( size );
    this->gameover_text.set_alignment ( nom::Text::Alignment::MiddleCenter );
    this->gameover_text.draw ( target );
    this->game->window.update();

    // Chill for a second
    nom::sleep ( 1000 );
    if ( this->gameover_state == GameOverType::Tie && this->game->rules.getRules() != CardRules::SuddenDeath )
    {
      this->game->set_state( Game::State::CardsMenu );
    }
    else
    {
      nom::uint32_ptr data = new nom::uint32 ( this->gameover_state );
      this->game->set_state( Game::State::GameOver, data );
    }
  }
}
