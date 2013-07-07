/******************************************************************************

    Final Fantasy VIII Triple Triad Remake

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "Game.hpp"

Game::Game ( CardHand player1_hand )
{
  #ifdef DEBUG_GAME_OBJ
    std::cout << "Game::Game (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->hand[0] = player1_hand;

  this->turn = 0;
  this->cursor_locked = false;

  this->collection.cards.clear();
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

void Game::Pause ( void )
{
  std::cout << "\n" << "Game state Paused" << "\n";
}

void Game::Resume ( void )
{
  std::cout << "\n" << "Game state Resumed" << "\n";
}

void Game::onInit ( void )
{
  nom::int32 idx = 0; // for loop iterations

  this->collection.LoadJSON ( CARDS_DB );

  this->board.Init ( &this->rules );

  this->background.loadFromImage ( BOARD_BACKGROUND, nom::Color ( nom::Color::Black ), true, 0 );

  this->score_text.Load ( SCORE_FONTFACE, 32 );
  this->score_text.setTextColor ( nom::Color ( 255, 255, 255 ) ); // white

  this->gameOver_text.Load ( SCORE_FONTFACE, 36 ); // temp font
  this->gameOver_text.setTextColor ( nom::Color ( 255, 255, 255 ) ); // color: red

  this->info_text.Load ( INFO_FONTFACE, nom::Color ( 110, 144, 190 ), true );
  this->info_small_text.Load ( INFO_SMALL_FONTFACE, nom::Color ( 110, 144, 190 ), true );

  this->cursor = nom::SDL_Cursor ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y, CURSOR_WIDTH, CURSOR_HEIGHT );
  this->cursor.Load ( INTERFACE_CURSOR, nom::Color ( 0, 0, 0 ), true );
  this->cursor.setSheetDimensions ( 78, 16, 0, 0 );
  this->cursor.setSheetID ( INTERFACE_CURSOR_NONE ); // default cursor image
  this->cursor.setState ( 0 ); // default state; player hand select

  //this->music.LoadMusicTrack ( MUSIC_TRACK );

  this->player[0].Init ( &this->hand[0] );
  this->player[0].setPosition ( PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y );

  this->player[1].Init ( &this->hand[1] );
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

  this->rules.setRules ( 1 );

  this->msgbox.push_back ( nom::Color ( 41, 41, 41 ) ); // top1
  this->msgbox.push_back ( nom::Color ( 133, 133, 133 ) ); // top2

  this->msgbox.push_back ( nom::Color ( 41, 41, 41 ) ); // left1
  this->msgbox.push_back ( nom::Color ( 133, 133, 133 ) ); // left2

  this->msgbox.push_back ( nom::Color ( 57, 57, 57 ) ); // bottom1
  this->msgbox.push_back ( nom::Color ( 41, 41, 41 ) ); // bottom2

  this->msgbox.push_back ( nom::Color ( 57, 57, 57 ) ); // right1
  this->msgbox.push_back ( nom::Color ( 41, 41, 41 ) ); // right2

  #ifndef DEBUG_GAME
    this->debug_box.disable ();
  #endif

  linear.setStartColor ( nom::Color ( 67, 67, 67, 255 ) );
  linear.setEndColor ( nom::Color ( 99, 99, 99, 255 ) );

  this->debug_box.Init ( 170, 8, 43, 20, msgbox, linear );
  this->info_box.Init ( 104, 194, 176, 24, msgbox, linear );

  #ifdef DEBUG_GAME
    this->debugCardsNoRuleset();
    //this->debugCardsSameRuleset();
  #endif

  //this->music.PlayMusicTrack ( -1 );
  //this->music.PauseMusic ();

  this->player[0].setID ( Card::PLAYER1 );
  this->player[1].setID ( Card::PLAYER2 );

  this->player_turn ( 0 );

  //update.Start();
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

  this->hand[player].clearSelectedCard();

  this->unlockSelectedCard();

  if ( this->get_turn() == 0 )
    this->player_turn ( 1 );
  else if ( this->get_turn() == 1 )
    this->player_turn ( 0 );
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
    coords = getCursorBoardPos ( this->cursor.getX(), this->cursor.getY() );

    if ( coords.x != -1 && coords.y != -1 )
      selectedCard = this->board.getCard ( coords.x, coords.y );
  }
  // player hand selection state
  else
    selectedCard = this->hand[player_turn].getSelectedCard();

  // Debug helping info MessageBox display
  if ( this->debug_box.isEnabled() == true )
  {
    if ( selectedCard.getID() != 0 )
    {
      this->info_text.setText ( std::to_string ( selectedCard.getID() ) );
      signed int text_width = this->info_text.getTextWidth ();

      this->debug_box.Draw ( video_buffer );

      this->info_text.setPosition ( ( SCREEN_WIDTH - text_width ) / 2, 10 );
      this->info_text.Update();
      this->info_text.Draw ( video_buffer );
    }
  }

  // (Southern) informational MessageBox display (selected / active card's name)
  if ( selectedCard.getName().length() != 0 || selectedCard.getName() != "\0" )
  {
    this->info_text.setText ( selectedCard.getName() );
    unsigned int text_width = this->info_text.getTextWidth();
    this->info_small_text.setText ( "INFO" );

    this->info_box.Draw ( video_buffer );

    this->info_text.setPosition ( ( SCREEN_WIDTH - text_width ) / 2, 196 );
    this->info_text.Update();
    this->info_text.Draw ( video_buffer );

    this->info_small_text.setPosition ( 108, 194 );
    this->info_small_text.Update();
    this->info_small_text.Draw ( video_buffer );
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

  this->hand[turn].clearSelectedCard ();
  this->hand[turn].selectCard ( this->hand[turn].cards.front() );

  this->cursor.setState ( 0 );
  this->cursor.setPosition ( this->player_cursor_coords[player_turn].x, this->player_cursor_coords[player_turn].y );
}

// helper method for cursor input selection
void Game::unlockSelectedCard ( void )
{
  this->cursor.setState ( 0 ); // player card select

  this->resetCursor();

  this->lockCursor ( false );
}

// helper method for cursor input selection
void Game::lockSelectedCard ( void )
{
  nom::Coords coords; // temp container var to hold cursor pos mapping coords

  this->cursor.setState ( 1 ); // board select

  if ( this->isCursorLocked() == false )
  {
    if ( get_turn() == 0 )
      this->cursor.setPosition ( CURSOR_ORIGIN_X-16, CURSOR_ORIGIN_Y ); // FIXME
    else if ( get_turn() == 1 )
      this->cursor.setPosition ( CURSOR_ORIGIN_X+16, CURSOR_ORIGIN_Y ); // FIXME

    this->lockCursor ( true );
  }
  else
  {
    coords = getCursorBoardPos ( this->cursor.getX(), this->cursor.getY() );
    if ( coords.x != -1 && coords.y != -1 )
      this->moveTo ( coords.x, coords.y );

    this->unlockSelectedCard();
  }
}

// Helper method for updating board with player's selected card
void Game::moveTo ( unsigned int x, unsigned int y )
{
  Card selected;

  for ( nom::uint32 turn = 0; turn < TOTAL_PLAYERS; turn++ )
  {
    selected = this->hand[turn].getSelectedCard();

    if ( selected.getID() != 0 )
    {
      if ( this->board.getStatus ( x, y ) == false )
      {
        if ( this->get_turn() == turn )
        {
          this->board.updateStatus ( x, y, this->hand[turn].getSelectedCard() );
          this->hand[turn].removeCard ( this->hand[turn].getSelectedCard() );

          std::vector<std::pair<int, int>> grid = board.checkBoard ( x, y );

          if ( grid.empty() == false )
          {
            if ( rules.getRules() == 0 )
            {
              board.flipCard ( grid[0].first, grid[0].second, turn + 1 );
            }
          }

          if ( rules.getRules() != 0 )
          {
            for ( nom::ulong g = 0; g < grid.size(); g++ )
            {
              board.flipCard ( grid[g].first, grid[g].second, turn + 1 );

              std::vector<std::pair<int, int>> tgrid = board.checkBoard ( grid[g].first, grid[g].second );

               // temporary workaround until a more proper solution is found
              if ( rules.getRules() == 2 || rules.getRules() == 4 )
                continue;
              else
              {
                for ( nom::ulong tg = 0; tg < tgrid.size(); tg++ )
                {
                  board.flipCard( tgrid[tg].first, tgrid[tg].second, turn + 1 );
                }
              }
            }
          }
          this->endTurn();
        }
      }
    }
  }
}

void Game::onKeyDown ( int32_t key, int32_t mod )
{
  switch ( key )
  {
    default: break;

    case SDLK_p: /* Pause State ... */ break;
    case SDLK_m: /*this->music.togglePlayingMusic();*/ break;

    case SDLK_e: this->endTurn(); break;
    case SDLK_LEFTBRACKET: debugListCards ( mod ); break;
    case SDLK_RIGHTBRACKET: debugListCollection( mod ); break;
    case SDLK_d: if ( mod == KMOD_LMETA ) this->removePlayerCard(); break;

    case SDLK_i: debugBox(); break;
    case SDLK_r: nom::GameStates::ChangeState ( std::unique_ptr<CardsMenu>( new CardsMenu ) ); break;

    case SDLK_LEFT: this->moveCursorLeft(); break;
    case SDLK_RIGHT: this->moveCursorRight(); break;
    case SDLK_UP: this->moveCursorUp(); break;
    case SDLK_DOWN: this->moveCursorDown(); break;

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
  for ( hand_index = 0; hand_index < this->hand[ player_turn ].size(); hand_index++ )
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
      this->hand[ player_turn ].selectCard ( this->hand[ player_turn ].cards[ hand_index ] );

      // Updates Cursor Position
      this->cursor.setPosition ( this->player_cursor_coords[ player_turn ].x, this->player_cursor_coords[ player_turn ].y + ( CARD_HEIGHT / 2 ) * hand_index );

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

void Game::onMouseWheel ( bool up, bool down )
{
  if ( this->cursor.getState() == 0 )
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
    if ( this->cursor.getY() <= this->cursor_coords_map[idx].y )
      return this->cursor_coords_map[idx].x;
    else // catch all safety switch
      // assume we are at the last position in the index when all else fails
      pos = MAX_PLAYER_HAND;
  }

  return pos;
}

void Game::moveCursorLeft ( void )
{
  if ( this->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->cursor.getX() > BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) )
      this->cursor.move ( -( CARD_WIDTH ), 0 );
  }
}

void Game::moveCursorRight ( void )
{
  if ( this->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->cursor.getX() < BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) )
      this->cursor.move ( ( CARD_WIDTH ), 0 );
  }
}

void Game::moveCursorUp ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->cursor.getState() == 0 )
  {
    if ( this->cursor.getY() > PLAYER1_CURSOR_ORIGIN_Y )
    {
      this->cursor.move ( 0, -( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->hand[player_turn].selectCard ( this->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->cursor.getY() > BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) )
      this->cursor.move ( 0, -( CARD_HEIGHT ) );
  }
}

void Game::moveCursorDown ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->cursor.getState() == 0 )
  {
    if ( this->cursor.getY() < ( CARD_HEIGHT / 2 ) * ( this->hand[player_turn].size() ) )
    {
      this->cursor.move ( 0, ( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->hand[player_turn].selectCard ( this->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->cursor.getY() < BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) )
      this->cursor.move ( 0, ( CARD_HEIGHT ) );
  }
}

void Game::updateCursor ( void )
{
  if ( this->get_turn() == 0 ) // player1
    this->cursor.setSheetID ( INTERFACE_CURSOR_RIGHT );
  else // player2
    this->cursor.setSheetID ( INTERFACE_CURSOR_LEFT );

  this->cursor.Update();
}

void Game::drawCursor ( void* video_buffer )
{
  this->cursor.Draw ( video_buffer );
}

// Scoring: board_card_count + player_card_count
void Game::updateScore ( void )
{
  unsigned int hand_count = 0; // player hand total count
  unsigned int board_count = 0; // board card total count
  unsigned int turn = 0;

  for ( turn = 0; turn < TOTAL_PLAYERS; turn++ )
  {
    board_count = this->board.getPlayerCount ( turn + 1 );

    hand_count = this->hand[turn].size();

    this->player[turn].setScore ( board_count + hand_count );
  }
}

void Game::drawScore ( void *video_buffer )
{
  this->score_text.setText ( std::to_string ( player[0].getScore() ) );
  this->score_text.setPosition ( PLAYER1_SCORE_ORIGIN_X, PLAYER1_SCORE_ORIGIN_Y );
  this->score_text.Update();
  this->score_text.Draw ( video_buffer );

  this->score_text.setText ( std::to_string ( player[1].getScore() ) );
  this->score_text.setPosition ( PLAYER2_SCORE_ORIGIN_X, PLAYER2_SCORE_ORIGIN_Y );
  this->score_text.Update();
  this->score_text.Draw ( video_buffer );
}

void Game::Update ( float delta_time )
{
  this->updateCursor();

  if ( this->get_turn() == 0 ) // player1
  {
    this->player_rect.setPosition ( nom::Coords ( 320, 0, 16, 16 ) );
    this->player_rect.setColor ( nom::Color ( 188, 203, 236 ) );
  }
  else // player2
  {
    this->player_rect.setPosition ( nom::Coords ( 40, 0, 16, 16 ) );
    this->player_rect.setColor ( nom::Color ( 222, 196, 205 ) );
  }

  this->debug_box.Update();
  this->info_box.Update();

  this->updateScore();

  this->context.Update();
}

void Game::Draw ( void *video_buffer )
{
  this->background.Draw ( video_buffer );

  this->board.Draw ( video_buffer );

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
  if ( this->board.getCount () >= 9 || this->hand[PLAYER1].size() == 0 || this->hand[PLAYER2].size() == 0 )
  {
    std::vector<Card> winning_cards;
    winning_cards.clear();

    if ( this->player[PLAYER1].getScore() > this->player[PLAYER2].getScore() ) // player 1 wins
    {
      this->gameOver_text.setText ( "Player 1 wins!" );
      signed int width = this->gameOver_text.getTextWidth ();
      this->gameOver_text.setPosition ( ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
      this->gameOver_text.Update();
      this->gameOver_text.Draw ( video_buffer );

      for ( nom::int32 i = 0; i < this->hand[PLAYER2].size(); i++ )
        winning_cards.push_back ( this->hand[PLAYER2].cards[i] );

      for ( int y = 0; y < BOARD_GRID_HEIGHT; y++ )
      {
        for ( int x = 0; x < BOARD_GRID_WIDTH; x++ )
        {
          Card card = this->board.getCard ( x, y );

          if ( card.getPlayerOwner() == Card::PLAYER2 )
            winning_cards.push_back ( card );
        }
      }

      this->context.Update();

      this->update.Start();

      while ( this->update.getTicks() < 1000 )
      {
        // Do nothing loop for 1 second
      }

      this->update.Stop();

      nom::GameStates::ChangeState ( std::unique_ptr<GameOver>( new GameOver( winning_cards, 1 ) ) );
    }
    else if ( this->player[PLAYER2].getScore() > this->player[PLAYER1].getScore() ) // player 2 wins
    {
      this->gameOver_text.setText ( "Player 2 wins!" );
      signed int width = this->gameOver_text.getTextWidth ();
      this->gameOver_text.setPosition ( ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
      this->gameOver_text.Update();
      this->gameOver_text.Draw ( video_buffer );

      for ( nom::int32 i = 0; i < this->hand[PLAYER1].size(); i++ )
        winning_cards.push_back ( this->hand[PLAYER1].cards[i] );

      for ( nom::int32 y = 0; y < BOARD_GRID_HEIGHT; y++ )
      {
        for ( nom::int32 x = 0; x < BOARD_GRID_WIDTH; x++ )
        {
          Card card = this->board.getCard ( x, y );

          if ( card.getPlayerOwner() == Card::PLAYER1 )
            winning_cards.push_back ( card );
        }
      }

      this->context.Update();

      this->update.Start();

      while ( this->update.getTicks() < 1000 )
      {
        // Do nothing loop for 1 second
      }

      this->update.Stop();

      nom::GameStates::ChangeState ( std::unique_ptr<GameOver>( new GameOver( winning_cards, 2 ) ) );
    }
    else if ( this->player[PLAYER1].getScore() == this->player[PLAYER2].getScore() )  // player tie
    {
      this->gameOver_text.setText ( "Tie!" );
      signed int width = this->gameOver_text.getTextWidth ();
      this->gameOver_text.setPosition ( ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
      this->gameOver_text.Update();
      this->gameOver_text.Draw ( video_buffer );

      winning_cards.clear();

      this->context.Update();

      this->update.Start();

      while ( this->update.getTicks() < 1000 )
      {
        // Do nothing loop for 1 second
      }

      this->update.Stop();

      nom::GameStates::ChangeState ( std::unique_ptr<GameOver>( new GameOver( winning_cards, 3 ) ) );
    }
    else // Undefined
    {
      std::cout << "ERR in isGameOver()" << "\n";
      exit(1);
    }
  }
}
