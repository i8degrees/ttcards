/******************************************************************************
    ttcards.cpp

  Final Fantasy VIII Triple Triad Remake

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"

using namespace nom;

TTcards::TTcards ( nom::GameApp *engine, CardHand player1_hand )
{
  #ifdef DEBUG_TTCARDS_OBJ
    std::cout << "TTcards::TTcards (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->engine = engine; // initialize rendering interface
  this->hand[0] = player1_hand;

  this->background = NULL;

  this->turn = 0;
  this->cursor_locked = false;

  this->collection.cards.clear();

  this->Init();
}

TTcards::~TTcards ( void )
{
  #ifdef DEBUG_TTCARDS_OBJ
    std::cout << "TTcards::~TTcards (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->background )
    SDL_FreeSurface ( this->background );

  this->background = NULL;

  if ( this->engine )
    this->engine = NULL;
}

bool TTcards::Init ( void )
{
  this->Load();

  #ifdef DEBUG_TTCARDS
    this->debugCardsNoRuleset();
    //this->debugCardsSameRuleset();
  #endif

  //this->music.PlayMusicTrack ( -1 );
  //this->music.PauseMusic ();

  this->player[0].setID ( Card::PLAYER1 );
  this->player[1].setID ( Card::PLAYER2 );

  this->player_turn ( 0 );

  //update.Start();

  //SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL / 3);

  return true;
}

void TTcards::Pause ( void )
{
  std::cout << "\n" << "TTcards state Paused" << "\n";
}

void TTcards::Resume ( void )
{
  std::cout << "\n" << "TTcards state Resumed" << "\n";
}

void TTcards::Load ( void )
{
  unsigned int idx = 0; // for loop iterations

  this->collection.LoadJSON ( CARDS_DB );

  this->board.Init ( &this->card, &this->rules );
  this->background = (SDL_Surface*) Gfx::LoadImage ( BOARD_BACKGROUND );

  this->score_text.Load ( SCORE_FONTFACE, 32 );
  this->score_text.setTextColor ( nom::Color ( 255, 255, 255 ) ); // white

  this->gameOver_text.Load ( SCORE_FONTFACE, 36 ); // temp font
  this->gameOver_text.setTextColor ( nom::Color ( 255, 255, 255 ) ); // color: red

  this->info_text.Load ( INFO_FONTFACE, nom::Color ( 110, 144, 190 ), 16, 16 );
  this->info_small_text.Load ( INFO_SMALL_FONTFACE, nom::Color ( 110, 144, 190 ), 16, 16 );

  this->cursor = nom::SDL_Cursor ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y, CURSOR_WIDTH, CURSOR_HEIGHT );
  this->cursor.Load ( INTERFACE_CURSOR, nom::Color ( 0, 0, 0 ) );
  this->cursor.setSheetID ( INTERFACE_CURSOR_NONE ); // default cursor image
  this->cursor.setState ( 0 ); // default state; player hand select

  //this->music.LoadMusicTrack ( MUSIC_TRACK );

  this->player[0].Init ( &this->hand[0], &this->card );
  this->player[0].setXY ( PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y );

  this->player[1].Init ( &this->hand[1], &this->card );
  this->player[1].setXY ( PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y );

  // player1, player2 cursor X, Y coords
  this->player_cursor_coords[0].setCoords ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y );
  this->player_cursor_coords[1].setCoords ( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y );

  for ( idx = 0; idx < MAX_PLAYER_HAND; idx++ )
    this->cursor_coords_map[idx].setCoords ( idx, this->player_cursor_coords[0].getY() + ( CARD_HEIGHT / 2 * idx ) );

  // Cursor X, Y coords mapping for placing cards on board
  this->board_coords_map[0].setCoords ( 0, 0, BOARD_ORIGIN_X + ( CARD_WIDTH * 1), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) );
  this->board_coords_map[1].setCoords ( 1, 0, BOARD_ORIGIN_X + ( CARD_WIDTH * 2), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) );
  this->board_coords_map[2].setCoords ( 2, 0, BOARD_ORIGIN_X + ( CARD_WIDTH * 3), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) );

  this->board_coords_map[3].setCoords ( 0, 1, BOARD_ORIGIN_X + ( CARD_WIDTH * 1), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) );
  this->board_coords_map[4].setCoords ( 1, 1, BOARD_ORIGIN_X + ( CARD_WIDTH * 2), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) );
  this->board_coords_map[5].setCoords ( 2, 1, BOARD_ORIGIN_X + ( CARD_WIDTH * 3), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) );

  this->board_coords_map[6].setCoords ( 0, 2, BOARD_ORIGIN_X + ( CARD_WIDTH * 1), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) );
  this->board_coords_map[7].setCoords ( 1, 2, BOARD_ORIGIN_X + ( CARD_WIDTH * 2), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) );
  this->board_coords_map[8].setCoords ( 2, 2, BOARD_ORIGIN_X + ( CARD_WIDTH * 3), BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) );

  this->rules.setRules ( 1 );

  this->msgbox[0].setColor ( 41, 41, 41 ); // top1
  this->msgbox[1].setColor ( 133, 133, 133 ); // top2

  this->msgbox[2].setColor ( 41, 41, 41 ); // left1
  this->msgbox[3].setColor ( 133, 133, 133 ); // left2

  this->msgbox[4].setColor ( 57, 57, 57 ); // bottom1
  this->msgbox[5].setColor ( 41, 41, 41 ); // bottom2

  this->msgbox[6].setColor ( 57, 57, 57 ); // right1
  this->msgbox[7].setColor ( 41, 41, 41 ); // right2

  #ifndef DEBUG_TTCARDS
    this->debug_box.disable ();
  #endif

  for ( unsigned int i = 0; i < 8; i++ )
  {
    this->info_box.setBorder ( msgbox[i] );
    this->debug_box.setBorder ( msgbox[i] );
  }

  this->info_box.setBackground ( &linear );
  this->debug_box.setBackground ( &linear );

  drawableRects.push_back ( new nom::Rectangle ( nom::Coords ( 320, 0, 16, 16 ), nom::Color ( 188, 203, 236 ) ) );
  drawableRects.push_back ( new nom::Rectangle ( nom::Coords ( 40, 0, 16, 16 ), nom::Color ( 222, 196, 205 ) ) );

  //return true;
}

// These cards should be discarded from player's hand ( MAX_HAND = 5 )
void TTcards::debugCardsDiscard ( void )
{
  this->hand[0].addCard ( this->collection.cards[88] ); // Carbuncle
  this->hand[0].addCard ( this->collection.cards[24] ); // TriFace

  this->hand[1].addCard ( this->collection.cards[88] ); // Carbuncle
}

// Debug player hand set for no and combo rulesets
void TTcards::debugCardsNoRuleset ( void )
{
/*
  this->hand[0].addCard ( this->collection.cards[89] ); // Diablos
  this->hand[0].addCard ( this->collection.cards[109] ); // Squall
  this->hand[0].addCard ( this->collection.cards[99] ); // Ward
  this->hand[0].addCard ( this->collection.cards[84] ); // Ifrit [pos 3]
  this->hand[0].addCard ( this->collection.cards[16] ); // Thrustaevis
*/
  this->hand[1].addCard ( this->collection.cards[20] ); // Jelleye
  this->hand[1].addCard ( this->collection.cards[88] ); // Carbuncle
  this->hand[1].addCard ( this->collection.cards[24] ); // TriFace
  this->hand[1].addCard ( this->collection.cards[66] ); // Propagator
  this->hand[1].addCard ( this->collection.cards[50] ); // Malboro
}

// Debug player hand set for same rulesets
void TTcards::debugCardsSameRuleset ( void )
{
  this->hand[0].addCard ( this->collection.cards[89] ); // Diablos
  this->hand[0].addCard ( this->collection.cards[109] ); // Squall
  this->hand[0].addCard ( this->collection.cards[99] ); // Ward
  this->hand[0].addCard ( this->collection.cards[84] ); // Ifrit [pos 3]
  //this->hand[0].addCard ( this->collection.cards[16] ); // Thrustaevis
  this->hand[0].addCard ( this->collection.cards[60] ); // Iguion

  this->hand[1].addCard ( this->collection.cards[20] ); // Jelleye
  this->hand[1].addCard ( this->collection.cards[2] ); // Bite Bug
  //this->hand[1].addCard ( this->collection.cards[88] ); // Carbuncle
  this->hand[1].addCard ( this->collection.cards[5] ); // Gayla
  this->hand[1].addCard ( this->collection.cards[63] ); // Oilboyle
  this->hand[1].addCard ( this->collection.cards[77] ); // Chubby Chocobo
  //this->hand[1].addCard ( this->collection.cards[50] ); // Malboro
}

// debug helper method
void TTcards::removePlayerCard ( void )
{
  unsigned int player_turn = get_turn();

  hand[player_turn].removeCard ( hand[player_turn].getSelectedCard() );
  hand[player_turn].clearSelectedCard();
  hand[player_turn].selectCard ( hand[player_turn].cards.front() );

  this->cursor.setXY ( this->player_cursor_coords[player_turn].getX(), this->player_cursor_coords[player_turn].getY() );
}

// Debug -- input events helper method
void TTcards::debugBox ( void )
{
  if ( this->debug_box.isEnabled() == true )
    this->debug_box.disable ();
  else
    this->debug_box.enable ();
}

// Debug -- input events helper method
void TTcards::debugListCards ( SDLMod mod )
{
  if ( mod == KMOD_LMETA )
    this->debug.ListCards ( this->hand[1].cards );
  else
    this->debug.ListCards ( this->hand[0].cards );
}

// Debug -- input events helper method
void TTcards::debugListCollection ( SDLMod mod )
{
  if ( mod == KMOD_LMETA )
    this->debug.ListCards ( this->collection.cards );
  else
    this->board.List();
}

unsigned int TTcards::get_turn ( void )
{
  return this->turn;
}

void TTcards::player_turn ( unsigned int player )
{
  this->turn = player;

  this->resetCursor();
}

// Helper method for incrementing to next player's turn
void TTcards::endTurn ( void )
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
void TTcards::showCardInfoBox ( void* video_buffer )
{
  Card selectedCard; // temp container var to hold our card info (ID, name)
  nom::Coords coords; // temp container var to hold cursor pos mapping coords

  unsigned int player_turn = get_turn();

  // board selection state
  if ( this->isCursorLocked() == true )
  {
    coords = getCursorBoardPos ( this->cursor.getX(), this->cursor.getY() );

    if ( coords.getX() != -1 && coords.getY() != -1 )
      selectedCard = this->board.getCard ( coords.getX(), coords.getY() );
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

      this->debug_box.Draw ( video_buffer, 170, 8, 43, 20 ); // 86x20 @ 140, 8

      this->info_text.setXY ( ( SCREEN_WIDTH - text_width ) / 2, 10 );
      this->info_text.Draw ( video_buffer );
    }
  }

  // (Southern) informational MessageBox display (selected / active card's name)
  if ( selectedCard.getName().length() != 0 || selectedCard.getName() != "\0" )
  {
    this->info_text.setText ( selectedCard.getName() );
    unsigned int text_width = this->info_text.getTextWidth();
    this->info_small_text.setText ( "INFO" );

    this->info_box.Draw ( video_buffer, 104, 194, 176, 24 );

    this->info_text.setXY ( ( SCREEN_WIDTH - text_width ) / 2, 196 );
    this->info_text.Draw ( video_buffer );

    this->info_small_text.setXY ( 108, 194 );
    this->info_small_text.Draw ( video_buffer );
  }
}

bool TTcards::isCursorLocked ( void )
{
  if ( this->cursor_locked == true )
    return true;
  else
    return false;
}

void TTcards::lockCursor ( bool lock )
{
  this->cursor_locked = lock;
}

// Helper method for resetting cursor related input
void TTcards::resetCursor ( void )
{
  unsigned int player_turn = get_turn();

  this->hand[turn].clearSelectedCard ();
  this->hand[turn].selectCard ( this->hand[turn].cards.front() );

  this->cursor.setState ( 0 );
  this->cursor.setXY ( this->player_cursor_coords[player_turn].getX(), this->player_cursor_coords[player_turn].getY() );
}

// helper method for cursor input selection
void TTcards::unlockSelectedCard ( void )
{
  this->cursor.setState ( 0 ); // player card select

  this->resetCursor();

  this->lockCursor ( false );
}

// helper method for cursor input selection
void TTcards::lockSelectedCard ( void )
{
  nom::Coords coords; // temp container var to hold cursor pos mapping coords

  this->cursor.setState ( 1 ); // board select

  if ( this->isCursorLocked() == false )
  {
    if ( get_turn() == 0 )
      this->cursor.setXY ( CURSOR_ORIGIN_X-16, CURSOR_ORIGIN_Y ); // FIXME
    else if ( get_turn() == 1 )
      this->cursor.setXY ( CURSOR_ORIGIN_X+16, CURSOR_ORIGIN_Y ); // FIXME

    this->lockCursor ( true );
  }
  else
  {
    coords = getCursorBoardPos ( this->cursor.getX(), this->cursor.getY() );
    if ( coords.getX() != -1 && coords.getY() != -1 )
      this->moveTo ( coords.getX(), coords.getY() );

    this->unlockSelectedCard();
  }
}

// Helper method for updating board with player's selected card
void TTcards::moveTo ( unsigned int x, unsigned int y )
{
  Card selected;

  for ( int turn = 0; turn < TOTAL_PLAYERS; turn++ )
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
            for ( int g = 0; g < grid.size(); g++ )
            {
              board.flipCard ( grid[g].first, grid[g].second, turn + 1 );

              std::vector<std::pair<int, int>> tgrid = board.checkBoard ( grid[g].first, grid[g].second );

               // temporary workaround until a more proper solution is found
              if ( rules.getRules() == 2 || rules.getRules() == 4 )
                continue;
              else
              {
                for ( int tg = 0; tg < tgrid.size(); tg++ )
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

void TTcards::onExit ( void )
{
  this->engine->Quit();
}

void TTcards::onResize ( unsigned int width, unsigned int height )
{
  nom::SDL_Display display;

  if ( this->engine->isFullScreen() )
  {
    display.toggleFullScreenWindow ( 0, 0 );
    this->engine->setFullScreen ( false );
  }
  else
  {
    display.toggleFullScreenWindow ( 0, 0 );
    this->engine->setFullScreen ( true );
  }
}

void TTcards::HandleInput ( void )
{
  SDL_Event event;

  while ( SDL_PollEvent ( &event ) )
    SDLInput::Input ( &event );
}

void TTcards::onKeyDown ( SDLKey key, SDLMod mod )
{
  switch ( key )
  {
    case SDLK_ESCAPE:
    case SDLK_q: this->onExit(); break;
    case SDLK_f: this->onResize ( 0, 0 ); break;

    case SDLK_p: /* Pause State ... */ break;
    case SDLK_m: /*this->music.togglePlayingMusic();*/ break;
    case SDLK_EQUALS: this->engine->toggleFPS(); break;

    case SDLK_e: this->endTurn(); break;
    case SDLK_LEFTBRACKET: debugListCards ( mod ); break;
    case SDLK_RIGHTBRACKET: debugListCollection( mod ); break;
    case SDLK_d: if ( mod == KMOD_LMETA ) this->removePlayerCard(); break;

    case SDLK_i: debugBox(); break;
    case SDLK_r: nom::GameStates::PopStateThenChangeState ( std::unique_ptr<CardsMenu>( new CardsMenu ( this->engine ) ) ); break;

    case SDLK_LEFT: this->moveCursorLeft(); break;
    case SDLK_RIGHT: this->moveCursorRight(); break;
    case SDLK_UP: this->moveCursorUp(); break;
    case SDLK_DOWN: this->moveCursorDown(); break;

    case SDLK_x: this->unlockSelectedCard(); break;
    case SDLK_SPACE: this->lockSelectedCard(); break;

    default: break;
  }
}

void TTcards::onMouseLeftButtonDown ( unsigned int x, unsigned int y )
{
  nom::Coords coords ( x, y ); // temp container var to hold cursor pos mapping coords
  unsigned int player_turn = get_turn();

  std::pair<int, int> player_coords = player[player_turn].getXY(); // PLAYER ORIGIN XY

  // player hand selection checks
  if ( x <= ( std::get<0>(player_coords) + CARD_WIDTH ) && x >= ( std::get<0>(player_coords) ) && y <= ( std::get<1>(player_coords) + ( CARD_HEIGHT / 2 ) * 1 ) && y >= ( std::get<1>(player_coords) ) )
  {
    hand[player_turn].selectCard ( hand[player_turn].cards[0] );

    // Updates Cursor Position
    if ( hand[player_turn].cards[0].getID() != 0 )
      this->cursor.setXY ( this->player_cursor_coords[player_turn].getX(), this->player_cursor_coords[player_turn].getY() + ( CARD_HEIGHT / 2 ) * 0 );
  }

  else if ( x <= ( std::get<0>(player_coords) + CARD_WIDTH ) && x >= ( std::get<0>(player_coords) ) && y <= ( std::get<1>(player_coords) + ( CARD_HEIGHT / 2 ) * 2 ) && y >= ( std::get<1>(player_coords) ) )
  {
    hand[player_turn].selectCard ( hand[player_turn].cards[1] );

    // Updates Cursor Position
    if ( hand[player_turn].cards[1].getID() != 0 )
      this->cursor.setXY ( this->player_cursor_coords[player_turn].getX(), this->player_cursor_coords[player_turn].getY() + ( CARD_HEIGHT / 2 ) * 1 );
  }

  else if ( x <= ( std::get<0>(player_coords) + CARD_WIDTH ) && x >= ( std::get<0>(player_coords) ) && y <= ( std::get<1>(player_coords) + ( CARD_HEIGHT / 2 ) * 3 ) && y >= ( std::get<1>(player_coords) ) )
  {
    hand[player_turn].selectCard ( hand[player_turn].cards[2] );

    // Updates Cursor Position
    if ( hand[player_turn].cards[2].getID() != 0 )
      this->cursor.setXY ( this->player_cursor_coords[player_turn].getX(), this->player_cursor_coords[player_turn].getY() + ( CARD_HEIGHT / 2 ) * 2 );
  }

  else if ( x <= ( std::get<0>(player_coords) + CARD_WIDTH ) && x >= ( std::get<0>(player_coords) ) && y <= ( std::get<1>(player_coords) + ( CARD_HEIGHT / 2 ) * 4 ) && y >= ( std::get<1>(player_coords) ) )
  {
    hand[player_turn].selectCard ( hand[player_turn].cards[3] );

    // Updates Cursor Position
    if ( hand[player_turn].cards[3].getID() != 0 )
      this->cursor.setXY ( this->player_cursor_coords[player_turn].getX(), this->player_cursor_coords[player_turn].getY() + ( CARD_HEIGHT / 2 ) * 3 );
  }

  else if ( x <= ( std::get<0>(player_coords) + CARD_WIDTH ) && x >= ( std::get<0>(player_coords) ) && y <= ( std::get<1>(player_coords) + ( CARD_HEIGHT / 2 ) * 5 ) && y >= ( std::get<1>(player_coords) ) )
  {
    hand[player_turn].selectCard ( hand[player_turn].cards[4] );

    // Updates Cursor Position
    if ( hand[player_turn].cards[4].getID() != 0 )
      this->cursor.setXY ( this->player_cursor_coords[player_turn].getX(), this->player_cursor_coords[player_turn].getY() + ( CARD_HEIGHT / 2 ) * 4 );
  }

  // board grid checks of players
  coords = this->getCursorBoardPos ( x, y );

  if ( coords.getX() != -1 && coords.getY() != -1 )
    this->moveTo ( coords.getX(), coords.getY() );
}

void TTcards::onMouseWheel ( bool up, bool down )
{
  if ( this->cursor.getState() == 0 )
  {
    if ( up )
      this->moveCursorUp();
    else if ( down )
      this->moveCursorDown();
  }
}

void TTcards::onJoyButtonDown ( unsigned int which, unsigned int button )
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

nom::Coords TTcards::getCursorBoardPos ( unsigned int x, unsigned int y )
{
  unsigned int idx = 0;
  nom::Coords pos ( -1, -1 ); // ...when all else fails, default to undefined

  for ( idx = 0; idx < ( BOARD_GRID_WIDTH * BOARD_GRID_HEIGHT ); idx++ )
  {
    if ( x <= this->board_coords_map[idx].getWidth() && x >= BOARD_ORIGIN_X && y <= this->board_coords_map[idx].getHeight() && y >= BOARD_ORIGIN_Y )
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
unsigned int TTcards::getCursorPos ( void )
{
  unsigned int pos = 0;
  unsigned int idx = 0;

  for ( idx = 0; idx < MAX_PLAYER_HAND; idx++ )
  {
    if ( this->cursor.getY() <= this->cursor_coords_map[idx].getY() )
      return this->cursor_coords_map[idx].getX();
    else // catch all safety switch
      // assume we are at the last position in the index when all else fails
      pos = MAX_PLAYER_HAND;
  }

  return pos;
}

void TTcards::moveCursorLeft ( void )
{
  if ( this->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->cursor.getX() > BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) )
      this->cursor.updateXY ( -( CARD_WIDTH ), 0 );
  }
}

void TTcards::moveCursorRight ( void )
{
  if ( this->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->cursor.getX() < BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) )
      this->cursor.updateXY ( ( CARD_WIDTH ), 0 );
  }
}

void TTcards::moveCursorUp ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->cursor.getState() == 0 )
  {
    if ( this->cursor.getY() > PLAYER1_CURSOR_ORIGIN_Y )
    {
      this->cursor.updateXY ( 0, -( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->hand[player_turn].selectCard ( this->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->cursor.getY() > BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) )
      this->cursor.updateXY ( 0, -( CARD_HEIGHT ) );
  }
}

void TTcards::moveCursorDown ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->cursor.getState() == 0 )
  {
    if ( this->cursor.getY() < ( CARD_HEIGHT / 2 ) * ( this->hand[player_turn].size() ) )
    {
      this->cursor.updateXY ( 0, ( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->hand[player_turn].selectCard ( this->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->cursor.getY() < BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) )
      this->cursor.updateXY ( 0, ( CARD_HEIGHT ) );
  }
}

void TTcards::updateCursor ( void )
{
  if ( this->get_turn() == 0 ) // player1
    this->cursor.setSheetID ( INTERFACE_CURSOR_RIGHT );
  else // player2
    this->cursor.setSheetID ( INTERFACE_CURSOR_LEFT );
}

void TTcards::drawCursor ( void* video_buffer )
{
  this->cursor.Draw ( video_buffer );
}

// Scoring: board_card_count + player_card_count
void TTcards::updateScore ( void )
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

void TTcards::drawScore ( void *video_buffer )
{
  this->score_text.setText ( std::to_string ( player[0].getScore() ) );
  this->score_text.setXY ( PLAYER1_SCORE_ORIGIN_X, PLAYER1_SCORE_ORIGIN_Y );
  this->score_text.Draw ( video_buffer );

  this->score_text.setText ( std::to_string ( player[1].getScore() ) );
  this->score_text.setXY ( PLAYER2_SCORE_ORIGIN_X, PLAYER2_SCORE_ORIGIN_Y );
  this->score_text.Draw ( video_buffer );
}

void TTcards::Update ( void )
{
  this->updateCursor();

  if ( this->get_turn() == 0 ) // player1
    rect.setPosition ( Coords ( 320, 0, 16, 16 ), Color ( 188, 203, 236 ) );
  else // player2
    rect.setPosition ( Coords ( 40, 0, 16, 16 ), Color ( 222, 196, 205 ) );

  this->updateScore();

  if ( this->update.getTicks() > 1100 )
  {
    this->update.Start(); // restart
  }

  //Gfx::updateSurface ( video_buffer ); // FIXME
}

void TTcards::Draw ( void *video_buffer )
{
  Gfx::DrawSurface ( this->background, video_buffer, nom::Coords ( 0, 0 ), nom::Coords ( 0, 0, 384, 224 ) );

  this->board.Draw ( video_buffer );

  this->player[0].Draw ( video_buffer );
  this->player[1].Draw ( video_buffer );

  rect.Draw ( video_buffer );

  this->drawCursor ( video_buffer );

  this->showCardInfoBox ( video_buffer );

  this->drawScore ( video_buffer );

  //if ( this->update.getTicks() > 1000 )
  //{
    //Gfx::drawRect ( 0, 0, 384, 224, 255, 255, 255 );
  //}

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
      this->gameOver_text.setXY ( ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
      this->gameOver_text.Draw ( video_buffer );

      for ( unsigned int i = 0; i < this->hand[PLAYER2].size(); i++ )
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

      Gfx::updateSurface ( video_buffer ); // FIXME
      SDL_Delay ( 2500 );

      nom::GameStates::PushState ( std::unique_ptr<GameOver>( new GameOver( this->engine, winning_cards, 1 ) ) );
    }
    else if ( this->player[PLAYER2].getScore() > this->player[PLAYER1].getScore() ) // player 2 wins
    {
      this->gameOver_text.setText ( "Player 2 wins!" );
      signed int width = this->gameOver_text.getTextWidth ();
      this->gameOver_text.setXY ( ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
      this->gameOver_text.Draw ( video_buffer );

      for ( unsigned int i = 0; i < this->hand[PLAYER1].size(); i++ )
        winning_cards.push_back ( this->hand[PLAYER1].cards[i] );

      for ( int y = 0; y < BOARD_GRID_HEIGHT; y++ )
      {
        for ( int x = 0; x < BOARD_GRID_WIDTH; x++ )
        {
          Card card = this->board.getCard ( x, y );

          if ( card.getPlayerOwner() == Card::PLAYER1 )
            winning_cards.push_back ( card );
        }
      }

      Gfx::updateSurface ( video_buffer ); // FIXME
      SDL_Delay ( 2500 );

      nom::GameStates::PushState ( std::unique_ptr<GameOver>( new GameOver( this->engine, winning_cards, 2 ) ) );
    }
    else if ( this->player[PLAYER1].getScore() == this->player[PLAYER2].getScore() )  // player tie
    {
      this->gameOver_text.setText ( "Tie!" );
      signed int width = this->gameOver_text.getTextWidth ();
      this->gameOver_text.setXY ( ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
      this->gameOver_text.Draw ( video_buffer );

      winning_cards.clear();

      Gfx::updateSurface ( video_buffer ); // FIXME
      SDL_Delay ( 2500 );

      nom::GameStates::PushState ( std::unique_ptr<GameOver>( new GameOver( this->engine, winning_cards, 3 ) ) );
    }
    else // Undefined
    {
      std::cout << "ERR in isGameOver()" << "\n";
      exit(1);
    }
  }
  Gfx::updateSurface ( video_buffer );
}
