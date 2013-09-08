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
#include "PlayState.hpp"

PlayState::PlayState ( std::shared_ptr<GameObject> object )
{
NOM_LOG_TRACE ( TTCARDS );

  this->game = object;

  this->turn = 0;
  this->cursor_locked = false;
  this->skip_turn = false;
  this->gameover_state = GameOverType::NotOver;

  // this->game->hand[0] is initialized for us in the CardsMenu state
  this->game->hand[1].clear();
}

PlayState::~PlayState ( void )
{
NOM_LOG_TRACE ( TTCARDS );
}

void PlayState::onExit ( void ) {}

void PlayState::Pause ( void )
{
  std::cout << "\n" << "PlayState Paused" << "\n";
}

void PlayState::Resume ( void )
{
  std::cout << "\n" << "PlayState Resumed" << "\n";
}

void PlayState::onInit ( void )
{
  nom::Gradient linear;

  // Random seeding for picking out whose turn it is initially
  nom::uint64 seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine rand_generator ( seed );
  std::uniform_int_distribution<nom::uint32> distribution ( 0, TOTAL_PLAYERS - 1 );

  this->game->cursor.setPosition ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y );
  this->game->cursor.setSheetID ( INTERFACE_CURSOR_NONE ); // default cursor image
  this->game->cursor.setState ( 0 ); // default state; player hand select

  this->game->rules.setRules ( 1 );
  this->game->board = Board ( this->game->rules, &this->game->card );

  this->player[0] = Player ( &this->game->hand[0], &this->game->card, this->game->rules );
  this->player[0].setPosition ( PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y );

  this->player[1] = Player ( &this->game->hand[1], &this->game->card, this->game->rules );
  this->player[1].setPosition ( PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y );

  // player1, player2 cursor X, Y coords
  this->player_cursor_coords[0] = nom::Coords ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y );
  this->player_cursor_coords[1] = nom::Coords ( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y );

  // Compute the player's scoreboard X, Y coordinate positions
  this->player_scoreboard[0].setPosition ( PLAYER1_SCORE_ORIGIN_X, PLAYER1_SCORE_ORIGIN_Y );
  this->player_scoreboard[1].setPosition ( PLAYER2_SCORE_ORIGIN_X, PLAYER2_SCORE_ORIGIN_Y );

  for ( nom::int32 idx = 0; idx < MAX_PLAYER_HAND; idx++ )
  {
    this->cursor_coords_map[idx] = nom::Coords ( idx, this->player_cursor_coords[0].y + ( CARD_HEIGHT / 2 * idx ) );
  }

  linear.setStartColor ( nom::Color ( 67, 67, 67, 255 ) );
  linear.setEndColor ( nom::Color ( 99, 99, 99, 255 ) );

  this->info_box = nom::ui::MessageBox  (
                                          INFO_BOX_ORIGIN_X, INFO_BOX_ORIGIN_Y,
                                          INFO_BOX_WIDTH, INFO_BOX_HEIGHT,
                                          nom::ui::FrameStyle::Gray, linear
                                        );

  this->debug_box = nom::ui::MessageBox (
                                          DEBUG_BOX_ORIGIN_X, DEBUG_BOX_ORIGIN_Y,
                                          DEBUG_BOX_WIDTH, DEBUG_BOX_HEIGHT,
                                          nom::ui::FrameStyle::Gray, linear
                                        );

  this->info_box.setWindowTitleFont ( &this->game->info_small_text );
  this->info_box.setLabelFont ( &this->game->info_text );
  this->info_box.setLabelTextAlignment ( nom::TextAlignment::MiddleCenter );

  this->debug_box.setWindowTitleFont ( &this->game->info_small_text );
  this->debug_box.setLabelFont ( &this->game->info_text );
  this->debug_box.setLabelTextAlignment ( nom::TextAlignment::MiddleCenter );

#if ! defined ( DEBUG )
  this->debug_box.disable();
#endif

  this->game->hand[1].randomize ( 1, 10, this->game->collection );

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
  this->player_turn ( nom::randomInteger ( 0, TOTAL_PLAYERS - 1 ) );

  // Initialize our animation state timers
  this->player_timer[1].setFrameRate ( 320 ); // 250 is also nice, I think!
  this->cursor_blink.start();
  this->blink_cursor = false;
}

void PlayState::onKeyDown ( int32_t key, int32_t mod )
{
  nom::uint32 player_turn = get_turn();

  switch ( key )
  {
    default: break;

    case SDLK_p: // Pause game
    {
      nom::GameStates::PushState ( PauseStatePtr( new PauseState ( this->game ) ) );
    }
    break;

    case SDLK_s: // Save current game
    {
      if ( this->game->hand[0].save( USER_PLAYER1_FILENAME ) == false )
      {
NOM_LOG_ERR ( TTCARDS, "Unable to save game data at: " + USER_PLAYER1_FILENAME );
        this->game->cursor_wrong.Play();
        break;
      }

      if ( this->game->hand[1].save( USER_PLAYER2_FILENAME ) == false )
      {
NOM_LOG_ERR ( TTCARDS, "Unable to save game data at: " + USER_PLAYER2_FILENAME );
        this->game->cursor_wrong.Play();
        break;
      }

      if ( this->game->board.save( USER_BOARD_FILENAME ) == false )
      {
NOM_LOG_ERR ( TTCARDS, "Unable to save game data at: " + USER_BOARD_FILENAME );
        this->game->cursor_wrong.Play();
        break;
      }

      this->game->save_game.Play(); // Successful saved game!
    }
    break;

    case SDLK_l: // Load saved game
    {
      if ( mod == KMOD_LMETA ) // Special game load (player1 always wins!)
      {
        if ( this->game->hand[0].load ( "Debug" + path.native() + "player1_unbeatable.json" ) == false )
        {
// FIXME
//NOM_LOG_ERR ( TTCARDS, "Unable to load game data at: " + "player1_unbeatable.json" );
NOM_LOG_ERR ( TTCARDS, "Unable to load game data at: player1_unbeatable.json" );
          this->game->cursor_wrong.Play();
          break;
        }
      }
      else // Normal game load for player1
      {
        if ( this->game->hand[0].load( USER_PLAYER1_FILENAME ) == false )
        {
NOM_LOG_ERR ( TTCARDS, "Unable to load game data at: " + USER_PLAYER1_FILENAME );
          this->game->cursor_wrong.Play();
          break;
        }
      }

      if ( this->game->hand[1].load( USER_PLAYER2_FILENAME ) == false )
      {
NOM_LOG_ERR ( TTCARDS, "Unable to load game data at: " + USER_PLAYER2_FILENAME );
        this->game->cursor_wrong.Play();
        break;
      }

      if ( this->game->board.load( USER_BOARD_FILENAME ) == false )
      {
NOM_LOG_ERR ( TTCARDS, "Unable to load game data at: " + USER_BOARD_FILENAME );
        this->game->cursor_wrong.Play();
        break;
      }

      // Successful load of saved game!
      this->updateScore();
      this->resetCursor();
      this->game->load_game.Play();
    }
    break;

#if defined ( DEBUG )
    case SDLK_e: // Full control over the other player's move
    {
      if ( mod == KMOD_LMETA ) // Return control over to the other player
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

      this->game->cursor.setPosition ( this->player_cursor_coords[player_turn].x, this->player_cursor_coords[player_turn].y );
    }
    break;

    case SDLK_i:
    {
      if ( this->debug_box.isEnabled() == true )
      {
        this->debug_box.disable();
      }
      else
      {
        this->debug_box.enable();
      }
    }
    break;

    case SDLK_LEFT:
    {
      if ( mod == KMOD_LSHIFT ) // increase card rank attribute by + 1
      {
        this->game->hand[ player_turn ].modifyCardRank ( true, WEST );
      }
      else if ( mod == KMOD_LCTRL ) // decrease card rank attribute by - 1
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
      if ( mod == KMOD_LSHIFT ) // increase card rank attribute by + 1
      {
        this->game->hand[ player_turn ].modifyCardRank ( true, EAST );
      }
      else if ( mod == KMOD_LCTRL ) // decrease card rank attribute by - 1
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
      if ( mod == KMOD_LSHIFT ) // increase card rank attribute by + 1
      {
        this->game->hand[ player_turn ].modifyCardRank ( true, NORTH );
      }
      else if ( mod == KMOD_LCTRL ) // decrease card rank attribute by - 1
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
      if ( mod == KMOD_LSHIFT ) // increase card rank attribute by + 1
      {
        this->game->hand[ player_turn ].modifyCardRank ( true, SOUTH );
      }
      else if ( mod == KMOD_LCTRL ) // decrease card rank attribute by - 1
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

void PlayState::onMouseLeftButtonDown ( nom::int32 x, nom::int32 y )
{
  nom::uint32 player_turn = this->get_turn();
  nom::Coords coords ( x, y ); // mouse input coordinates
  nom::Coords player_coords = this->player[player_turn].getPosition(); // Player cursor origin coordinates

  // Disable mouse input if we are not controlling the other player
  if ( this->skip_turn == false )
  {
    if ( this->get_turn() != 0 ) return;
  }

  // Player hand selection checks
  for ( nom::int32 idx = 0; idx < this->game->hand[ player_turn ].size(); idx++ )
  {
    player_coords = nom::Coords (
                                  player_coords.x, player_coords.y,
                                  CARD_WIDTH, ( CARD_HEIGHT / 2  ) * ( idx + 1 )
                                );

    if ( player_coords.intersects ( coords ) )
    {
      // 1. Update player's selected card
      // 2. Update cursor position
      // 3. Play sound event
      this->game->hand[ player_turn ].selectCard ( this->game->hand[ player_turn ].cards[ idx ] );

      this->game->cursor.setPosition ( this->player_cursor_coords[ player_turn ].x, this->player_cursor_coords[ player_turn ].y + ( CARD_HEIGHT / 2 ) * idx );

      this->game->cursor_move.Play();

      // We must break the loop here upon the end of a matching coords check
      // in order to prevent a nasty "last card stays permanently selected"
      // bug from cropping back up!
      break;
    }
  }

  // Board grid coords check; player is attempting to place a card on the board
  // when the player hand coords check above comes back false
  coords = this->game->board.getGlobalBounds ( x, y );

  // Attempts to move card onto board; validity checking is performed within
  // the following method call
  if ( coords != nom::Coords::null ) // undefined if -1, -1
  {
    this->moveTo ( coords.x, coords.y );
  }
}

void PlayState::onMouseRightButtonDown ( nom::int32 x, nom::int32 y )
{
  // Stub
}

void PlayState::onMouseWheel ( bool up, bool down )
{
  if ( this->game->cursor.getState() == 0 ) // Player's hand mode
  {
    if ( up )
    {
      this->moveCursorUp();
    }
    else if ( down )
    {
      this->moveCursorDown();
    }
  }
}

void PlayState::onJoyButtonDown ( int32_t which, int32_t button )
{
  switch ( button )
  {
    default: break;

    case nom::PSXBUTTON::START:
    {
      nom::GameStates::PushState ( PauseStatePtr( new PauseState ( this->game ) ) );
    }
    break;

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

void PlayState::updateMessageBoxes ( void )
{
  nom::uint32 player_turn = get_turn();
  Card selected_card; // temp container var to hold our card info (ID, name)
  nom::Coords coords; // temp container var to hold cursor pos mapping coords

  // board selection state
  if ( this->isCursorLocked() == true )
  {
    coords = this->game->board.getGlobalBounds ( this->game->cursor.getX(), this->game->cursor.getY() );
    if ( coords != nom::Coords::null )
    {
      selected_card = this->game->board.get ( coords.x, coords.y );
    }
  }
  else // player hand selection state
  {
#if defined (DEBUG) // Allow watching both players make their card selections
    selected_card = this->game->hand[player_turn].getSelectedCard();
#else // Do not show the actions of other players
    selected_card = this->game->hand[PLAYER1].getSelectedCard();
#endif
  }

  // Debug helping info MessageBox display
  if ( selected_card.getID() != 0 )
  {
    this->debug_box.setLabel ( std::to_string ( selected_card.getID() ) );
  }

  // (Southern) informational MessageBox display (selected / active card's name)
  if ( selected_card.getName().length() != 0 )
  {
    this->info_box.setLabel ( selected_card.getName() );
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

  this->game->cursor.setState ( 0 );
  this->game->cursor.setPosition ( this->player_cursor_coords[0].x, this->player_cursor_coords[0].y );

  // Only set the position of the game interface cursor for player2 when we are
  // controlling him
  if ( this->skip_turn == true )
  {
    this->game->cursor.setPosition ( this->player_cursor_coords[player_turn].x, this->player_cursor_coords[player_turn].y );
  }
}

// helper method for cursor input selection
void PlayState::unlockSelectedCard ( void )
{
  this->game->cursor.setState ( 0 ); // player card select

  this->resetCursor();

  this->lockCursor ( false );

  this->game->cursor_cancel.Play();
}

// helper method for cursor input selection
void PlayState::lockSelectedCard ( void )
{
  nom::Coords coords; // temp container var to hold cursor pos mapping coords

  this->game->cursor.setState ( 1 ); // board select

  if ( this->isCursorLocked() == false )
  {
    if ( get_turn() == 0 )
      this->game->cursor.setPosition ( CURSOR_ORIGIN_X-16, CURSOR_ORIGIN_Y ); // FIXME
    else if ( get_turn() == 1 )
      this->game->cursor.setPosition ( CURSOR_ORIGIN_X+16, CURSOR_ORIGIN_Y ); // FIXME

    this->lockCursor ( true );
  }
  else
  {
    coords = this->game->board.getGlobalBounds ( this->game->cursor.getX(), this->game->cursor.getY() );

    if ( coords != nom::Coords::null )
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
    Cards adjacent_cards = this->game->board.find_adjacent ( x, y );

    // Dump returned list of cards
    this->game->debug.ListCards ( adjacent_cards );
  }

  if ( selected.getID() != 0 )
  {
    if ( player_turn == PLAYER1 && this->game->board ( x, y ) != 0 )
    {
      this->game->cursor_wrong.Play();
    }

    if ( this->game->board ( x, y ) == false )
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
        for ( nom::ulong g = 0; g < grid.size(); g++ )
        {
          this->game->board.flipCard ( grid[g].first, grid[g].second, player_turn + 1 );
          this->game->card_flip.Play();

          std::vector<std::pair<int, int>> tgrid = this->game->board.checkBoard ( grid[g].first, grid[g].second );

          // temporary workaround until a more proper solution is found
          if ( this->game->rules.getRules() == 2 || this->game->rules.getRules() == 4 )
            continue;
          else
          {
            for ( nom::ulong tg = 0; tg < tgrid.size(); tg++ )
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
    if ( this->game->cursor.getY() <= this->cursor_coords_map[idx].y )
      return this->cursor_coords_map[idx].x;
    else // catch all safety switch
      // assume we are at the last position in the index when all else fails
      pos = MAX_PLAYER_HAND;
  }

  return pos;
}

void PlayState::moveCursorLeft ( void )
{
  if ( this->game->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->game->cursor.getX() > BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) )
      this->game->cursor.move ( -( CARD_WIDTH ), 0 );
  }
  this->game->cursor_move.Play();
}

void PlayState::moveCursorRight ( void )
{
  if ( this->game->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->game->cursor.getX() < BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) )
      this->game->cursor.move ( ( CARD_WIDTH ), 0 );
  }
  this->game->cursor_move.Play();
}

void PlayState::moveCursorUp ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->game->cursor.getState() == 0 )
  {
    if ( this->game->cursor.getY() > PLAYER1_CURSOR_ORIGIN_Y )
    {
      this->game->cursor.move ( 0, -( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->game->hand[player_turn].selectCard ( this->game->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->game->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->game->cursor.getY() > BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) )
      this->game->cursor.move ( 0, -( CARD_HEIGHT ) );
  }
  this->game->cursor_move.Play();
}

void PlayState::moveCursorDown ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->game->cursor.getState() == 0 )
  {
    if ( this->game->cursor.getY() < ( CARD_HEIGHT / 2 ) * ( this->game->hand[player_turn].size() ) )
    {
      this->game->cursor.move ( 0, ( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->game->hand[player_turn].selectCard ( this->game->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->game->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->game->cursor.getY() < BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) )
      this->game->cursor.move ( 0, ( CARD_HEIGHT ) );
  }
  this->game->cursor_move.Play();
}

void PlayState::updateCursor ( void )
{
  if ( this->game->cursor.getState() == 1 )
  {
    if ( this->cursor_blink.ticks() > 192 ) // Blinky blink!
    {
      this->cursor_blink.stop();
      this->game->cursor.setSheetID ( INTERFACE_CURSOR_NONE );
      this->blink_cursor = true;
    }
  }

  if ( this->get_turn() == PLAYER1 && this->blink_cursor == false ) // player1
  {
    this->game->cursor.setSheetID ( INTERFACE_CURSOR_RIGHT );
  }
  // Only show interface cursor for player2 when we are controlling him
  else if ( this->skip_turn == true && this->get_turn() == PLAYER2 && this->blink_cursor == false )
  {
    this->game->cursor.setSheetID ( INTERFACE_CURSOR_LEFT );
  }

  this->game->cursor.update();
}

void PlayState::drawCursor ( void* video_buffer )
{
  this->game->cursor.draw ( video_buffer );

  if ( this->blink_cursor )
  {
    this->game->cursor.setSheetID ( INTERFACE_CURSOR_RIGHT );
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
    this->game->score_text[players].setText ( this->player[players].getScoreAsString() );
    this->game->score_text[players].setPosition ( nom::Coords ( this->player_scoreboard[players].x, this->player_scoreboard[players].y ) );
    this->game->score_text[players].Update();
  }
}

void PlayState::Update ( float delta_time )
{
  this->game->board.update();

  this->updateCursor();

  this->updateMessageBoxes();

  this->info_box.Update();
  this->debug_box.Update();

  // Player two animation effect
  if ( this->player_timer[1].ticks() + this->player_timer[1].framerate() > SDL_GetTicks() )
  {
    // return
  }
  else
  {
    this->player_timer[1].start();

    // Only show player2 animation when we are not controlling him
    if ( this->skip_turn == false )
    {
      nom::uint32 rand_pick = nom::randomInteger ( 0, this->game->hand[PLAYER2].size() );
      this->game->hand[PLAYER2].selectCard ( this->game->hand[PLAYER2].cards[ rand_pick ] );
    }
  }

  if ( this->get_turn() == 0 ) // player1
  {
    this->player_rect.setPosition ( nom::Coords ( PLAYER1_INDICATOR_ORIGIN_X, PLAYER1_INDICATOR_ORIGIN_Y, PLAYER_INDICATOR_WIDTH, PLAYER_INDICATOR_HEIGHT ) );
    this->player_rect.setColor ( nom::Color ( 188, 203, 236 ) );
  }
  else // player2
  {
    this->player_rect.setPosition ( nom::Coords ( PLAYER2_INDICATOR_ORIGIN_X, PLAYER2_INDICATOR_ORIGIN_Y, PLAYER_INDICATOR_WIDTH, PLAYER_INDICATOR_HEIGHT ) );
    this->player_rect.setColor ( nom::Color ( 222, 196, 205 ) );

    // Skipping a turn like this is only available in debug versions
    if ( this->skip_turn == false )
    {
      nom::Coords board_edges[3];

      board_edges[0].x = 0;
      board_edges[0].y = 0;

      board_edges[1].x = 2;
      board_edges[1].y = 0;

      board_edges[2].x = 0;
      board_edges[2].y = 2;

      board_edges[3].x = 2;
      board_edges[3].y = 2;

      nom::int32 edge_pick = nom::randomInteger ( 0, 3 );

      nom::uint32 rand_pick = nom::randomInteger ( 0, this->game->hand[1].size() );
      this->game->hand[1].selectCard ( this->game->hand[1].cards[ rand_pick ] );

      if ( this->game->board.getStatus ( board_edges[0].x, board_edges[0].y ) == false )
      {
        this->moveTo ( board_edges[ edge_pick ].x, board_edges[ edge_pick ].y );
      }
      else if ( this->game->board.getStatus ( board_edges[1].x, board_edges[1].y ) == false )
      {
        this->moveTo ( board_edges[1].x, board_edges[1].y );
      }
      else if ( this->game->board.getStatus ( board_edges[2].x, board_edges[2].y ) == false )
      {
        this->moveTo ( board_edges[2].x, board_edges[2].y );
      }
      else if ( this->game->board.getStatus ( board_edges[3].x, board_edges[3].y ) == false )
      {
        this->moveTo ( board_edges[3].x, board_edges[3].y );
      }
      else
      {
        nom::int32 moveX = nom::randomInteger ( 0, 2 );
        nom::int32 moveY = nom::randomInteger ( 0, 2 );
        nom::uint32 rand_pick = nom::randomInteger ( 0, this->game->hand[1].size() );
        this->game->hand[1].selectCard ( this->game->hand[1].cards[ rand_pick ] );

        this->moveTo ( moveX, moveY );
      } // end board_edges choice
    } // player2
  } // end player1

  this->game->context.Update();
}

void PlayState::Draw ( void *video_buffer )
{
  this->game->background.Draw ( video_buffer );

  this->game->board.draw ( video_buffer );

  this->player[0].Draw ( video_buffer );
  this->player[1].Draw ( video_buffer );

  this->player_rect.Draw ( video_buffer );

  this->drawCursor ( video_buffer );

  this->info_box.Draw ( video_buffer );
  this->debug_box.Draw ( video_buffer );

  // Draw each player's scoreboard
  this->game->score_text[0].Draw ( video_buffer );
  this->game->score_text[1].Draw ( video_buffer );

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
      this->game->gameover_text.setColor ( nom::Color::White );
      this->game->gameover_text.setText ( "You win!" );
    }
    else if ( this->player[ PLAYER1 ].getScore() < this->player[ PLAYER2 ].getScore() )
    {
      this->gameover_state = GameOverType::Lost;
      this->game->gameover_text.setColor ( nom::Color::White );
      this->game->gameover_text.setText ( "You lose..." );
    }
    else // Assume a draw
    {
      this->gameover_state = GameOverType::Tie;
      this->game->gameover_text.setColor ( nom::Color::White );
      this->game->gameover_text.setText ( "Tie!" );
    }

    nom::int32 width = this->game->gameover_text.getFontWidth();
    this->game->gameover_text.setPosition ( nom::Coords ( ( SCREEN_WIDTH - width ) / 2, SCREEN_HEIGHT / 2 ) );
    this->game->gameover_text.Update();
    this->game->gameover_text.Draw ( video_buffer );
    this->game->context.Update();

    nom::sleep ( 1000 ); // Chill for a second

    if ( this->gameover_state == GameOverType::Tie && this->game->rules.getRules() != CardRules::SuddenDeath )
    {
      nom::GameStates::ChangeState ( CardsMenuStatePtr ( new CardsMenuState ( this->game ) ) );
    }
    else
    {
      nom::GameStates::ChangeState ( GameOverStatePtr( new GameOverState( this->game, this->gameover_state ) ) );
    }
  }
}
