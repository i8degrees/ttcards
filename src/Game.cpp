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
#include "Game.hpp"

Game::Game ( std::shared_ptr<GameObject> object )
{
  #ifdef DEBUG_GAME_OBJ
    std::cout << "Game::Game (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->state = object;

  this->turn = 0;
  this->cursor_locked = false;

  // this->state->hand[0] is initialized for us in the CardsMenu state
  this->state->hand[1].clear();
}

Game::~Game ( void )
{
  #ifdef DEBUG_GAME_OBJ
    std::cout << "Game::~Game (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif
}

void Game::onExit ( void )
{
  std::cout << "\n" << "Game state onExit" << "\n";
}

void Game::onInit ( void )
{
  std::vector<nom::Color> msgbox;
  nom::Gradient linear;
  nom::OpenAL::SoundBuffer sound_buffer;
  nom::int32 idx = 0; // for loop iterations

  this->state->score_text.setColor ( nom::Color::White );
  this->state->gameOver_text.setColor ( nom::Color::White );

  this->state->cursor.setPosition ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y );
  this->state->cursor.setSheetID ( INTERFACE_CURSOR_NONE ); // default cursor image
  this->state->cursor.setState ( 0 ); // default state; player hand select

  this->state->rules.setRules ( 1 );
  this->state->board = Board ( this->state->rules, this->state->card );

  this->player[0] = Player ( &this->state->hand[0], this->state );
  this->player[0].setPosition ( PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y );

  this->player[1] = Player ( &this->state->hand[1], this->state );
  this->player[1].setPosition ( PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y );

  // player1, player2 cursor X, Y coords
  this->player_cursor_coords[0] = nom::Coords ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y );
  this->player_cursor_coords[1] = nom::Coords ( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y );

  for ( idx = 0; idx < MAX_PLAYER_HAND; idx++ )
    this->cursor_coords_map[idx] = nom::Coords ( idx, this->player_cursor_coords[0].y + ( CARD_HEIGHT / 2 * idx ) );

  // Cursor X, Y coords mapping for placing cards on board
  this->board_coords_map[0] = nom::Coords ( 0, 0, BOARD_ORIGIN_X + ( CARD_WIDTH * 1), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) );
  this->board_coords_map[1] = nom::Coords ( 1, 0, BOARD_ORIGIN_X + ( CARD_WIDTH * 2), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) );
  this->board_coords_map[2] = nom::Coords ( 2, 0, BOARD_ORIGIN_X + ( CARD_WIDTH * 3), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) );

  this->board_coords_map[3] = nom::Coords ( 0, 1, BOARD_ORIGIN_X + ( CARD_WIDTH * 1), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) );
  this->board_coords_map[4] = nom::Coords ( 1, 1, BOARD_ORIGIN_X + ( CARD_WIDTH * 2), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) );
  this->board_coords_map[5] = nom::Coords ( 2, 1, BOARD_ORIGIN_X + ( CARD_WIDTH * 3), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) );

  this->board_coords_map[6] = nom::Coords ( 0, 2, BOARD_ORIGIN_X + ( CARD_WIDTH * 1), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) );
  this->board_coords_map[7] = nom::Coords ( 1, 2, BOARD_ORIGIN_X + ( CARD_WIDTH * 2), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) );
  this->board_coords_map[8] = nom::Coords ( 2, 2, BOARD_ORIGIN_X + ( CARD_WIDTH * 3), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) );

  msgbox.push_back ( nom::Color ( 41, 41, 41 ) ); // top1
  msgbox.push_back ( nom::Color ( 133, 133, 133 ) ); // top2

  msgbox.push_back ( nom::Color ( 41, 41, 41 ) ); // left1
  msgbox.push_back ( nom::Color ( 133, 133, 133 ) ); // left2

  msgbox.push_back ( nom::Color ( 57, 57, 57 ) ); // bottom1
  msgbox.push_back ( nom::Color ( 41, 41, 41 ) ); // bottom2

  msgbox.push_back ( nom::Color ( 57, 57, 57 ) ); // right1
  msgbox.push_back ( nom::Color ( 41, 41, 41 ) ); // right2

  linear.setStartColor ( nom::Color ( 67, 67, 67, 255 ) );
  linear.setEndColor ( nom::Color ( 99, 99, 99, 255 ) );

  this->info_box = nom::MessageBox  ( INFO_BOX_ORIGIN_X, INFO_BOX_ORIGIN_Y,
                                      INFO_BOX_WIDTH, INFO_BOX_HEIGHT,
                                      msgbox, linear
                                    );

  this->debug_box = nom::MessageBox ( DEBUG_BOX_ORIGIN_X, DEBUG_BOX_ORIGIN_Y,
                                      DEBUG_BOX_WIDTH, DEBUG_BOX_HEIGHT,
                                      msgbox, linear
                                    );

#ifndef DEBUG_GAME
  this->debug_box.disable ();
#endif

#ifdef DEBUG_GAME
  //this->debugCardsNoRuleset();
  //this->debugCardsSameRuleset();
#endif

  this->state->hand[1].randomize ( this->state->collection );

  this->player[0].setID ( Card::PLAYER1 );
  this->player[1].setID ( Card::PLAYER2 );

  this->player_turn ( 0 );

/*
  sound_buffer.loadFromFile ( CURSOR_MOVE );
  this->cursor_move.setBuffer ( sound_buffer );

  sound_buffer.loadFromFile ( CURSOR_CANCEL );
  this->cursor_cancel.setBuffer ( sound_buffer );

  sound_buffer.loadFromFile ( CURSOR_WRONG );
  this->cursor_wrong.setBuffer ( sound_buffer );

  sound_buffer.loadFromFile ( CARD_FLIP );
  this->card_flip.setBuffer ( sound_buffer );

  sound_buffer.loadFromFile ( CARD_PLACE );
  this->card_place.setBuffer ( sound_buffer );

  this->music_buffer.loadFromFile ( MUSIC_TRACK );
  this->music_track.setBuffer ( this->music_buffer );
  this->music_track.Play();
  this->music_track.Pause();
*/
}

void Game::onKeyDown ( int32_t key, int32_t mod )
{
  switch ( key )
  {
    default: break;

    case SDLK_p: this->music_track.togglePause(); /* Pause State ... */ break;
    case SDLK_m: /*this->music.togglePlayingMusic();*/ break;
    case SDLK_e: this->endTurn(); break;
    case SDLK_LEFTBRACKET: debugListCards ( mod ); break;
    case SDLK_RIGHTBRACKET: debugListCollection( mod ); break;
    case SDLK_d: this->removePlayerCard(); break;
    case SDLK_i: debugBox(); break;

    case SDLK_r: nom::GameStates::ChangeState ( std::unique_ptr<CardsMenu>( new CardsMenu ( this->state ) ) ); break;

    case SDLK_LEFT:
    {
      if ( mod == KMOD_LSHIFT ) // increase card rank attribute by + 1
      {
        this->debugModifyCardRank ( true, WEST );
      }
      else if ( mod == KMOD_LCTRL ) // decrease card rank attribute by - 1
      {
        this->debugModifyCardRank ( false, WEST );
      }
      else
        this->moveCursorLeft();
    }
    break;

    case SDLK_RIGHT:
    {
      if ( mod == KMOD_LSHIFT ) // increase card rank attribute by + 1
      {
        this->debugModifyCardRank ( true, EAST );
      }
      else if ( mod == KMOD_LCTRL ) // decrease card rank attribute by - 1
      {
        this->debugModifyCardRank ( false, EAST );
      }
      else
        this->moveCursorRight();
    }
    break;

    case SDLK_UP:
    {
      if ( mod == KMOD_LSHIFT ) // increase card rank attribute by + 1
      {
        this->debugModifyCardRank ( true, NORTH );
      }
      else if ( mod == KMOD_LCTRL ) // decrease card rank attribute by - 1
      {
        this->debugModifyCardRank ( false, NORTH );
      }
      else
        this->moveCursorUp();
    }
    break;

    case SDLK_DOWN:
    {
      if ( mod == KMOD_LSHIFT ) // increase card rank attribute by + 1
      {
        this->debugModifyCardRank ( true, SOUTH );
      }
      else if ( mod == KMOD_LCTRL ) // decrease card rank attribute by - 1
      {
        this->debugModifyCardRank ( false, SOUTH );
      }
      else
        this->moveCursorDown();
    }
    break;

    case SDLK_x: this->unlockSelectedCard(); break;
    case SDLK_SPACE: this->lockSelectedCard(); break;

    // move selected card to grid[0][0]
    case SDLK_1: this->moveTo ( 0, 0 ); break;

    // move selected card to grid[1][0]
    case SDLK_2: this->moveTo ( 1, 0 ); break;

    // move selected card to grid[2][0]
    case SDLK_3: this->moveTo ( 2, 0 ); break;

    // move selected card to grid[0][1]
    case SDLK_4: this->moveTo ( 0, 1 ); break;

    // move selected card to grid[1][1]
    case SDLK_5: this->moveTo ( 1, 1 ); break;

    // move selected card to grid[2][1]
    case SDLK_6: this->moveTo ( 2, 1 ); break;

    // move selected card to grid[0][2]
    case SDLK_7: this->moveTo ( 0, 2 ); break;

    // move selected card to grid[1][2]
    case SDLK_8: this->moveTo ( 1, 2 ); break;

    // move selected card to grid[2][2] if possible
    case SDLK_9: this->moveTo ( 2, 2 ); break;
  } // end key switch
}

void Game::onMouseLeftButtonDown ( nom::int32 x, nom::int32 y )
{
  nom::int32 hand_index = 0; // iterator
  uint32_t player_turn = this->get_turn();

  nom::Coords coords ( x, y ); // temp container var to hold mouse mapping coords
  nom::Coords player_coords = player[player_turn].getPosition(); // Player cursor origin coordinates

  // Player hand selection checks
  for ( hand_index = 0; hand_index < this->state->hand[ player_turn ].size(); hand_index++ )
  {
    if  (
          x <= ( player_coords.x + CARD_WIDTH ) &&
          x >= ( player_coords.x ) &&
          // hand_index+1 because we start the loop iterator at zero; mouse check
          // calculation is invalid at this number whereas it is not for the
          // actions that take place if said check yields true
          y <= ( player_coords.y + ( CARD_HEIGHT / 2 ) * ( hand_index + 1 ) ) &&
          y >= ( player_coords.y )
        )
    {

      // Update player's selected card
      this->state->hand[ player_turn ].selectCard ( this->state->hand[ player_turn ].cards[ hand_index ] );

      // Updates Cursor Position
      this->state->cursor.setPosition ( this->player_cursor_coords[ player_turn ].x, this->player_cursor_coords[ player_turn ].y + ( CARD_HEIGHT / 2 ) * hand_index );

      //this->cursor_move.Play();

      // We must break the loop here upon the end of a matching coords check
      // in order to prevent a nasty "last card stays permanently selected"
      // bug from cropping back up!
      break;
    }
  }

  // Board grid coords check; player is attempting to place a card on the board
  // when the player hand coords check above comes back false
  coords = this->getCursorBoardPos ( x, y );

  // Attempts to move card onto board; validity checking is performed within
  // the following method call
  if ( coords.x != -1 && coords.y != -1 ) // undefined if -1, -1
    this->moveTo ( coords.x, coords.y );
}

void Game::onMouseRightButtonDown ( nom::int32 x, nom::int32 y )
{
  this->lockSelectedCard();
}

void Game::onMouseWheel ( bool up, bool down )
{
  if ( this->state->cursor.getState() == 0 )
  {
    if ( up )
      this->moveCursorUp();
    else if ( down )
      this->moveCursorDown();
  }
}

void Game::onJoyButtonDown ( int32_t which, int32_t button )
{
  switch ( button )
  {
    // Debug helpers
    case nom::PSXBUTTON::L1: this->endTurn(); break;

    case nom::PSXBUTTON::UP: this->moveCursorUp(); break;
    case nom::PSXBUTTON::RIGHT: this->moveCursorRight(); break;
    case nom::PSXBUTTON::DOWN: this->moveCursorDown(); break;
    case nom::PSXBUTTON::LEFT: this->moveCursorLeft(); break;

    case nom::PSXBUTTON::TRIANGLE: /* TODO */ break;
    case nom::PSXBUTTON::CIRCLE: this->unlockSelectedCard(); break;
    case nom::PSXBUTTON::CROSS: this->lockSelectedCard(); break;

    default: break;
  }
}

unsigned int Game::get_turn ( void )
{
  return this->turn;
}

void Game::player_turn ( unsigned int player )
{
  this->turn = player;

  this->resetCursor();
}

// Helper method for incrementing to next player's turn
void Game::endTurn ( void )
{
  unsigned int player = get_turn();

  this->unlockSelectedCard();

  if ( this->get_turn() == 0 )
  {
    this->state->hand[PLAYER1].clearSelectedCard();
    this->player_turn ( 1 );
  }
  else if ( this->get_turn() == 1 )
  {
    this->state->hand[PLAYER2].clearSelectedCard();
    this->player_turn ( 0 );
  }
}

// Interface Helper method; shows Card's ID number in a message box for both cursor
// states; player's hand and placed board cards -- debug handling included
void Game::showCardInfoBox ( void* video_buffer )
{
  Card selectedCard; // temp container var to hold our card info (ID, name)
  nom::Coords coords; // temp container var to hold cursor pos mapping coords

  unsigned int player_turn = get_turn();

  // board selection state
  if ( this->isCursorLocked() == true )
  {
    coords = getCursorBoardPos ( this->state->cursor.getX(), this->state->cursor.getY() );

    if ( coords.x != -1 && coords.y != -1 )
      selectedCard = this->state->board.getCard ( coords.x, coords.y );
  }
  // player hand selection state
  else
    selectedCard = this->state->hand[player_turn].getSelectedCard();

  // Debug helping info MessageBox display
  if ( this->debug_box.isEnabled() == true )
  {
    if ( selectedCard.getID() != 0 )
    {
      this->state->info_text.setText ( std::to_string ( selectedCard.getID() ) );
      nom::int32 text_width = this->state->info_text.getFontWidth ();

      this->debug_box.Draw ( video_buffer );

      this->state->info_text.setPosition ( ( SCREEN_WIDTH - text_width ) / 2, DEBUG_BOX_TEXT_ORIGIN_Y );
      this->state->info_text.Update();
      this->state->info_text.Draw ( video_buffer );
    }
  }

  // (Southern) informational MessageBox display (selected / active card's name)
  if ( selectedCard.getName().length() != 0 || selectedCard.getName() != "\0" )
  {
    this->state->info_text.setText ( selectedCard.getName() );
    nom::int32 text_width = this->state->info_text.getFontWidth();
    this->state->info_small_text.setText ( "INFO" );

    this->info_box.Draw ( video_buffer );

    this->state->info_text.setPosition ( ( SCREEN_WIDTH - text_width ) / 2, INFO_BOX_TEXT_ORIGIN_Y );
    this->state->info_text.Update();
    this->state->info_text.Draw ( video_buffer );

    this->state->info_small_text.setPosition ( INFO_BOX_SMALL_TEXT_ORIGIN_X, INFO_BOX_SMALL_TEXT_ORIGIN_Y );
    this->state->info_small_text.Update();
    this->state->info_small_text.Draw ( video_buffer );
  }
}

bool Game::isCursorLocked ( void )
{
  if ( this->cursor_locked == true )
    return true;
  else
    return false;
}

void Game::lockCursor ( bool lock )
{
  this->cursor_locked = lock;
}

// Helper method for resetting cursor related input
void Game::resetCursor ( void )
{
  unsigned int player_turn = get_turn();

  this->state->hand[turn].selectCard ( this->state->hand[turn].cards.front() );

  this->state->cursor.setState ( 0 );
  this->state->cursor.setPosition ( this->player_cursor_coords[player_turn].x, this->player_cursor_coords[player_turn].y );
}

// helper method for cursor input selection
void Game::unlockSelectedCard ( void )
{
  this->state->cursor.setState ( 0 ); // player card select

  this->resetCursor();

  this->lockCursor ( false );

  this->cursor_cancel.Play();
}

// helper method for cursor input selection
void Game::lockSelectedCard ( void )
{
  nom::Coords coords; // temp container var to hold cursor pos mapping coords

  this->state->cursor.setState ( 1 ); // board select

  if ( this->isCursorLocked() == false )
  {
    if ( get_turn() == 0 )
      this->state->cursor.setPosition ( CURSOR_ORIGIN_X-16, CURSOR_ORIGIN_Y ); // FIXME
    else if ( get_turn() == 1 )
      this->state->cursor.setPosition ( CURSOR_ORIGIN_X+16, CURSOR_ORIGIN_Y ); // FIXME

    this->lockCursor ( true );
  }
  else
  {
    coords = getCursorBoardPos ( this->state->cursor.getX(), this->state->cursor.getY() );
    if ( coords.x != -1 && coords.y != -1 )
      this->moveTo ( coords.x, coords.y );

    this->unlockSelectedCard();
  }
}

// Helper method for updating board with player's selected card
void Game::moveTo ( unsigned int x, unsigned int y )
{
  Card selected;
  nom::uint32 player_turn = this->get_turn();

  selected = this->state->hand[ player_turn ].getSelectedCard();

  if ( selected.getID() != 0 )
  {
    if ( this->state->board ( x, y ) != 0 )
      this->cursor_wrong.Play();

    if ( this->state->board ( x, y ) == false )
    {
      this->state->board.updateStatus ( x, y, this->state->hand[ player_turn ].getSelectedCard() );
      this->state->hand[ player_turn ].removeCard ( this->state->hand[ player_turn ].getSelectedCard() );

      this->card_place.Play();

      std::vector<std::pair<int, int>> grid = this->state->board.checkBoard ( x, y );

      if ( grid.empty() == false )
      {
        if ( this->state->rules.getRules() == 0 )
        {
          this->state->board.flipCard ( grid[0].first, grid[0].second, player_turn + 1 );
        }
      }

      if ( this->state->rules.getRules() != 0 )
      {
        for ( nom::ulong g = 0; g < grid.size(); g++ )
        {
          this->state->board.flipCard ( grid[g].first, grid[g].second, player_turn + 1 );

          std::vector<std::pair<int, int>> tgrid = this->state->board.checkBoard ( grid[g].first, grid[g].second );

          // temporary workaround until a more proper solution is found
          if ( this->state->rules.getRules() == 2 || this->state->rules.getRules() == 4 )
            continue;
          else
          {
            for ( nom::ulong tg = 0; tg < tgrid.size(); tg++ )
            {
              this->state->board.flipCard( tgrid[tg].first, tgrid[tg].second, player_turn + 1 );
            }
          }
        }
      }
      this->endTurn();
    }
  }
}

// Default return value is -1, -1 AKA undefined
//
// Translates global bounds coordinates onto local board grid bounds coords
//
// TODO / STUB:
//
//    0, 0    0, 1    0, 2
//    0, 1    1, 1    2, 1
//    0, 2    1, 2    2, 2
//
//
nom::Coords Game::getCursorBoardPos ( nom::int32 x, nom::int32 y )
{
  nom::int32 idx = 0; // iterator
  nom::Coords pos ( -1, -1 ); // ...when all else fails, default to undefined

  for ( idx = 0; idx < ( BOARD_GRID_WIDTH * BOARD_GRID_HEIGHT ); idx++ )
  {
    if ( x <= this->board_coords_map[idx].width && x >= BOARD_ORIGIN_X && y <= this->board_coords_map[idx].height && y >= BOARD_ORIGIN_Y )
      return this->board_coords_map[idx];
  }

  return pos;
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
unsigned int Game::getCursorPos ( void )
{
  unsigned int pos = 0;
  nom::int32 idx = 0;

  for ( idx = 0; idx < MAX_PLAYER_HAND; idx++ )
  {
    if ( this->state->cursor.getY() <= this->cursor_coords_map[idx].y )
      return this->cursor_coords_map[idx].x;
    else // catch all safety switch
      // assume we are at the last position in the index when all else fails
      pos = MAX_PLAYER_HAND;
  }

  return pos;
}

void Game::moveCursorLeft ( void )
{
  if ( this->state->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->state->cursor.getX() > BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) )
      this->state->cursor.move ( -( CARD_WIDTH ), 0 );
  }
  this->cursor_move.Play();
}

void Game::moveCursorRight ( void )
{
  if ( this->state->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->state->cursor.getX() < BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) )
      this->state->cursor.move ( ( CARD_WIDTH ), 0 );
  }
  this->cursor_move.Play();
}

void Game::moveCursorUp ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->state->cursor.getState() == 0 )
  {
    if ( this->state->cursor.getY() > PLAYER1_CURSOR_ORIGIN_Y )
    {
      this->state->cursor.move ( 0, -( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->state->hand[player_turn].selectCard ( this->state->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->state->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->state->cursor.getY() > BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) )
      this->state->cursor.move ( 0, -( CARD_HEIGHT ) );
  }
  this->cursor_move.Play();
}

void Game::moveCursorDown ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->state->cursor.getState() == 0 )
  {
    if ( this->state->cursor.getY() < ( CARD_HEIGHT / 2 ) * ( this->state->hand[player_turn].size() ) )
    {
      this->state->cursor.move ( 0, ( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->state->hand[player_turn].selectCard ( this->state->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->state->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->state->cursor.getY() < BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) )
      this->state->cursor.move ( 0, ( CARD_HEIGHT ) );
  }
  this->cursor_move.Play();
}

void Game::updateCursor ( void )
{
  if ( this->get_turn() == 0 ) // player1
    this->state->cursor.setSheetID ( INTERFACE_CURSOR_RIGHT );
  else // player2
    this->state->cursor.setSheetID ( INTERFACE_CURSOR_LEFT );

  this->state->cursor.Update();
}

void Game::drawCursor ( void* video_buffer )
{
  this->state->cursor.Draw ( video_buffer );
}

// Scoring: board_card_count + player_card_count
void Game::updateScore ( void )
{
  unsigned int hand_count = 0; // player hand total count
  unsigned int board_count = 0; // board card total count
  unsigned int turn = 0;

  for ( turn = 0; turn < TOTAL_PLAYERS; turn++ )
  {
    board_count = this->state->board.getPlayerCount ( turn + 1 );

    hand_count = this->state->hand[turn].size();

    this->player[turn].setScore ( board_count + hand_count );
  }
}

void Game::drawScore ( void *video_buffer )
{
  this->state->score_text.setText ( std::to_string ( player[0].getScore() ) );
  this->state->score_text.setPosition ( PLAYER1_SCORE_ORIGIN_X, PLAYER1_SCORE_ORIGIN_Y );
  this->state->score_text.Update();
  this->state->score_text.Draw ( video_buffer );

  this->state->score_text.setText ( std::to_string ( player[1].getScore() ) );
  this->state->score_text.setPosition ( PLAYER2_SCORE_ORIGIN_X, PLAYER2_SCORE_ORIGIN_Y );
  this->state->score_text.Update();
  this->state->score_text.Draw ( video_buffer );
}

void Game::Update ( nom::uint32 delta_time )
{
  this->updateCursor();

  if ( this->get_turn() == 0 ) // player1
  {
    this->player_rect.setPosition ( nom::Coords ( PLAYER1_INDICATOR_ORIGIN_X, PLAYER1_INDICATOR_ORIGIN_Y, PLAYER_INDICATOR_WIDTH, PLAYER_INDICATOR_HEIGHT ) );
    this->player_rect.setColor ( nom::Color ( 188, 203, 236 ) );
  }
  else // player2
  {
    this->player_rect.setPosition ( nom::Coords ( PLAYER2_INDICATOR_ORIGIN_X, PLAYER2_INDICATOR_ORIGIN_Y, PLAYER_INDICATOR_WIDTH, PLAYER_INDICATOR_HEIGHT ) );
    this->player_rect.setColor ( nom::Color ( 222, 196, 205 ) );
  }

  this->debug_box.Update();
  this->info_box.Update();

  this->updateScore();

  this->state->context.Update();
}

void Game::Draw ( void *video_buffer )
{
  this->state->background.Draw ( video_buffer );

  this->state->board.Draw ( video_buffer );

  this->player[0].Draw ( video_buffer );
  this->player[1].Draw ( video_buffer );

  this->player_rect.Draw ( video_buffer );

  this->drawCursor ( video_buffer );

  this->showCardInfoBox ( video_buffer );

  this->drawScore ( video_buffer );

  // FIXME: We keep game over check logic here in order to allow for the last
  // card placed to be shown to the player
  //
  // Game Over States
  // game / round is over when board card count >= 9
  if ( this->state->board.getCount () >= 9 || this->state->hand[PLAYER1].size() == 0 || this->state->hand[PLAYER2].size() == 0 )
  {
    std::vector<Card> winning_cards;
    winning_cards.clear();

    if ( this->player[PLAYER1].getScore() > this->player[PLAYER2].getScore() ) // player 1 wins
    {
      this->state->gameOver_text.setText ( "Player 1 wins!" );
      nom::int32 width = this->state->gameOver_text.getFontWidth();

      this->state->gameOver_text.setPosition ( nom::Coords ( ( SCREEN_WIDTH - width ) / 2, SCREEN_HEIGHT / 2 ) );
      this->state->gameOver_text.Update();
      this->state->gameOver_text.Draw ( video_buffer );

      for ( nom::int32 i = 0; i < this->state->hand[PLAYER2].size(); i++ )
        winning_cards.push_back ( this->state->hand[PLAYER2].cards[i] );

      for ( int y = 0; y < BOARD_GRID_HEIGHT; y++ )
      {
        for ( int x = 0; x < BOARD_GRID_WIDTH; x++ )
        {
          Card card = this->state->board.getCard ( x, y );

          if ( card.getPlayerOwner() == Card::PLAYER2 )
            winning_cards.push_back ( card );
        }
      }

      this->state->context.Update();

      this->update.Start();

      while ( this->update.getTicks() < 1000 )
      {
        // Do nothing loop for 1 second
      }

      this->update.Stop();

      nom::GameStates::ChangeState ( std::unique_ptr<GameOver>( new GameOver( this->state ) ) );
    }
    else if ( this->player[PLAYER2].getScore() > this->player[PLAYER1].getScore() ) // player 2 wins
    {
      this->state->gameOver_text.setText ( "Player 2 wins!" );
      nom::int32 width = this->state->gameOver_text.getFontWidth();
      this->state->gameOver_text.setPosition ( nom::Coords ( ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 ) );
      this->state->gameOver_text.Update();
      this->state->gameOver_text.Draw ( video_buffer );

      for ( nom::int32 i = 0; i < this->state->hand[PLAYER1].size(); i++ )
        winning_cards.push_back ( this->state->hand[PLAYER1].cards[i] );

      for ( nom::int32 y = 0; y < BOARD_GRID_HEIGHT; y++ )
      {
        for ( nom::int32 x = 0; x < BOARD_GRID_WIDTH; x++ )
        {
          Card card = this->state->board.getCard ( x, y );

          if ( card.getPlayerOwner() == Card::PLAYER1 )
            winning_cards.push_back ( card );
        }
      }

      this->state->context.Update();

      this->update.Start();

      while ( this->update.getTicks() < 1000 )
      {
        // Do nothing loop for 1 second
      }

      this->update.Stop();

      nom::GameStates::ChangeState ( std::unique_ptr<GameOver>( new GameOver ( this->state ) ) );
    }
    else if ( this->player[PLAYER1].getScore() == this->player[PLAYER2].getScore() )  // player tie
    {
      this->state->gameOver_text.setText ( "Tie!" );
      nom::int32 width = this->state->gameOver_text.getFontWidth();
      this->state->gameOver_text.setPosition ( nom::Coords ( ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 ) );
      this->state->gameOver_text.Update();
      this->state->gameOver_text.Draw ( video_buffer );

      winning_cards.clear();

      this->state->context.Update();

      this->update.Start();

      while ( this->update.getTicks() < 1000 )
      {
        // Do nothing loop for 1 second
      }

      this->update.Stop();

      nom::GameStates::ChangeState ( std::unique_ptr<GameOver>( new GameOver( this->state ) ) );
    }
    else // Undefined
    {
      std::cout << "ERR in isGameOver()" << "\n";
      exit(1);
    }
  }
}
