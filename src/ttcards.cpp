/******************************************************************************
    ttcards.cpp

  Final Fantasy VIII Triple Triad Remake

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"

TTcards::TTcards ( Gfx *engine )
{
  #ifdef DEBUG_TTCARDS_OBJ
    std::cout << "TTcards::TTcards (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->engine = engine; // initialize rendering interface

  this->turn = 0;
  this->cursor_locked = false;
  this->show_fps = true;
  this->fullscreen = false;

  this->collection.cards.clear();

  this->Init();
}

TTcards::~TTcards ( void )
{
  #ifdef DEBUG_TTCARDS_OBJ
    std::cout << "TTcards::~TTcards (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->engine != NULL )
  {
    this->engine = NULL;
  }
}

bool TTcards::Init ( void )
{
  #ifndef EMSCRIPTEN
    engine->setTitle ( APP_NAME );
  #endif

  this->Load();

  #ifdef DEBUG_TTCARDS
    this->debugCardsNoRuleset();
    //this->debugCardsSameRuleset();
  #endif

  //this->music.PlayMusicTrack ( -1 );
  //this->music.PauseMusic ();

  this->player[0].setID ( 1 ); // player1
  this->player[1].setID ( 2 ); // player2

  this->player_turn ( 0 );

  this->fps.Start();

  //SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

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

bool TTcards::Load ( void )
{
  this->collection.Load ( CARDS_DB );

  this->board.Init ( &this->card, &this->rules );
  this->board.LoadBackground ( BOARD_BACKGROUND );

  score_text.Load ( SCORE_FONTFACE, 32 );
  score_text.setTextColor ( 255, 255, 255 ); // white

  this->info_text.Load ( INFO_FONTFACE, GColor ( 110, 144, 190 ), 16, 16 );
  this->info_small_text.Load ( INFO_SMALL_FONTFACE, GColor ( 110, 144, 190 ), 16, 16 );

  this->cursor = Sprite ( CURSOR_WIDTH, CURSOR_HEIGHT );
  this->cursor.LoadImage ( INTERFACE_CURSOR, GColor ( 0, 0, 0 ) );
  this->cursor.SetSheetDimensions ( 78, 16, 0, 0 );
  this->cursor.SetSheetID ( INTERFACE_CURSOR_NONE );
  this->cursor.SetXY ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y ); //this->cursor.SetXY ( CURSOR_ORIGIN_X, CURSOR_ORIGIN_Y );
  this->cursor.setState ( 0 ); // player hand select

  this->music.LoadMusicTrack ( MUSIC_TRACK );

  this->player[0].Init ( &this->hand[0], &this->card );
  player[0].setXY ( PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y );

  this->player[1].Init ( &this->hand[1], &this->card );
  player[1].setXY ( PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y );

  player_cursor_coords[0] = std::make_pair ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y );
  player_cursor_coords[1] = std::make_pair ( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y );

  // We cannot map std::pair<0, 0>, so we are "missing" the first element here
  for ( int idx = 0; idx < MAX_PLAYER_HAND; idx++ )
    this->cursor_coords_map[idx] = std::make_pair ( std::get<1>(player_cursor_coords[0]) + ( CARD_HEIGHT / 2 ) * idx, idx );

  this->rules.setRules ( 1 );

  this->msgbox[0].setColor ( 41, 41, 41 ); // top1
  this->msgbox[1].setColor ( 133, 133, 133 ); // top2

  this->msgbox[2].setColor ( 41, 41, 41 ); // left1
  this->msgbox[3].setColor ( 133, 133, 133 ); // left2

  this->msgbox[4].setColor ( 57, 57, 57 ); // bottom1
  this->msgbox[5].setColor ( 57, 57, 57 ); // right1

  #ifndef DEBUG_TTCARDS
    this->debug_box.disable ( true );
  #endif

  for ( unsigned int i = 0; i < 6; i++ )
  {
    this->info_box.setBorder ( msgbox[i] );
    this->debug_box.setBorder ( msgbox[i] );
  }

  this->info_box.setBackground ( &linear );
  this->debug_box.setBackground ( &linear );

  return true;
}

// These cards should be discarded from player's hand ( MAX_HAND = 5 )
void TTcards::debugCardsDiscard ( void )
{
  this->hand[0].AddCard ( this->collection.cards[88] ); // Carbuncle
  this->hand[0].AddCard ( this->collection.cards[24] ); // TriFace

  this->hand[1].AddCard ( this->collection.cards[88] ); // Carbuncle
}

// Debug player hand set for no and combo rulesets
void TTcards::debugCardsNoRuleset ( void )
{
  this->hand[0].AddCard ( this->collection.cards[89] ); // Diablos
  this->hand[0].AddCard ( this->collection.cards[109] ); // Squall
  this->hand[0].AddCard ( this->collection.cards[99] ); // Ward
  this->hand[0].AddCard ( this->collection.cards[84] ); // Ifrit [pos 3]
  this->hand[0].AddCard ( this->collection.cards[16] ); // Thrustaevis

  this->hand[1].AddCard ( this->collection.cards[20] ); // Jelleye
  this->hand[1].AddCard ( this->collection.cards[88] ); // Carbuncle
  this->hand[1].AddCard ( this->collection.cards[24] ); // TriFace
  this->hand[1].AddCard ( this->collection.cards[66] ); // Propagator
  this->hand[1].AddCard ( this->collection.cards[50] ); // Malboro
}

// Debug player hand set for same rulesets
void TTcards::debugCardsSameRuleset ( void )
{
  this->hand[0].AddCard ( this->collection.cards[89] ); // Diablos
  this->hand[0].AddCard ( this->collection.cards[109] ); // Squall
  this->hand[0].AddCard ( this->collection.cards[99] ); // Ward
  this->hand[0].AddCard ( this->collection.cards[84] ); // Ifrit [pos 3]
  //this->hand[0].AddCard ( this->collection.cards[16] ); // Thrustaevis
  this->hand[0].AddCard ( this->collection.cards[60] ); // Iguion

  this->hand[1].AddCard ( this->collection.cards[20] ); // Jelleye
  this->hand[1].AddCard ( this->collection.cards[2] ); // Bite Bug
  //this->hand[1].AddCard ( this->collection.cards[88] ); // Carbuncle
  this->hand[1].AddCard ( this->collection.cards[5] ); // Gayla
  this->hand[1].AddCard ( this->collection.cards[63] ); // Oilboyle
  this->hand[1].AddCard ( this->collection.cards[77] ); // Chubby Chocobo
  //this->hand[1].AddCard ( this->collection.cards[50] ); // Malboro
}

// debug helper method
void TTcards::removePlayerCard ( void )
{
  unsigned int player_turn = get_turn();

  hand[player_turn].RemoveCard ( hand[player_turn].getSelectedCard() );
  hand[player_turn].clearSelectedCard();
  hand[player_turn].selectCard ( hand[player_turn].cards.front() );

  cursor.SetXY ( std::get<0>(player_cursor_coords[player_turn]), std::get<1>(player_cursor_coords[player_turn]) );
}

// debug helper method; shows Card's ID number in a message box
void TTcards::showCardID ( void )
{
  Card selected;

  unsigned int player_turn = get_turn();

  selected = this->hand[player_turn].getSelectedCard();

  #ifdef DEBUG_CARD_VIEW
    this->info_text.setTextBuffer ( std::to_string ( selected.getID() ) );
    signed int text_width = this->info_text.getTextWidth ();

    this->debug_box.Draw ( this->engine->screen, 170, 8, 43, 20 ); // 86x20 @ 140, 8
    this->info_text.Draw ( this->engine, ( SCREEN_WIDTH - text_width ) / 2, 10 );
  #endif
}

void TTcards::debugBox ( void )
{
  if ( this->debug_box.isEnabled() == true )
    this->debug_box.disable ( true );
  else
    this->debug_box.enable ( true );
}

void TTcards::debugListCards ( SDLMod mod )
{
  if ( mod == KMOD_LMETA )
    this->debug.ListCards ( this->hand[1].cards );
  else
    this->debug.ListCards ( this->hand[0].cards );
}

void TTcards::debugListCollection ( SDLMod mod )
{
  if ( mod == KMOD_LMETA )
    this->debug.ListCards ( this->collection.cards );
  else
    this->board.List();
}

bool TTcards::IsFullScreen ( void )
{
  if ( this->fullscreen == false )
    return false;
  else
    return true;
}

void TTcards::setFullscreen ( bool fs )
{
  this->fullscreen = fs;
}

bool TTcards::getShowFPS ( void )
{
  return this->show_fps;
}

void TTcards::showFPS ( bool show )
{
  this->show_fps = show;
}

// Helper method; toggles showing FPS counter (or not)
void TTcards::toggleFPS ( void )
{
  if ( this->getShowFPS() == true )
    this->showFPS ( false );
  else
    this->showFPS ( true );
}

void TTcards::drawFPS ( void )
{
  if ( this->getShowFPS() == true )
  {
    this->engine->setTitle ( std::to_string ( fps.GetFPS() / 1.000f ) );
  }
  else
  {
    this->engine->setTitle ( "TTcards" );
  }
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
  this->cursor.SetXY ( std::get<0>(player_cursor_coords[player_turn]), std::get<1>(player_cursor_coords[player_turn]) );
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
  this->cursor.setState ( 1 ); // board select

  if ( this->isCursorLocked() == false )
  {
    if ( get_turn() == 0 )
      this->cursor.SetXY ( CURSOR_ORIGIN_X, CURSOR_ORIGIN_Y ); // FIXME
    else if ( get_turn() == 1 )
      this->cursor.SetXY ( CURSOR_ORIGIN_X-16, CURSOR_ORIGIN_Y ); // FIXME

    this->lockCursor ( true );
  }
  else
  {
    if ( this->cursor.GetX() <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && this->cursor.GetX() >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && this->cursor.GetY() <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) && this->cursor.GetY() >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 0 ) ) )
      moveTo ( 0, 0 );
    else if ( this->cursor.GetX() <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && this->cursor.GetX() >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && this->cursor.GetY() <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) && this->cursor.GetY() >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 0 ) ) )
      moveTo ( 1, 0 );
    else if ( this->cursor.GetX() <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) ) && this->cursor.GetX() >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && this->cursor.GetY() <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) && this->cursor.GetY() >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 0 ) ) )
      moveTo ( 2, 0 );
    else if ( this->cursor.GetX() <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && this->cursor.GetX() >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && this->cursor.GetY() <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) && this->cursor.GetY() >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) )
      moveTo ( 0, 1 );
    else if ( this->cursor.GetX() <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && this->cursor.GetX() >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && this->cursor.GetY() <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) && this->cursor.GetY() >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) )
      moveTo ( 1, 1 );
    else if ( this->cursor.GetX() <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) ) && this->cursor.GetX() >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && this->cursor.GetY() <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) && this->cursor.GetY() >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) )
      moveTo ( 2, 1 );
    else if ( this->cursor.GetX() <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && this->cursor.GetX() >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && this->cursor.GetY() <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) ) && this->cursor.GetY() >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) )
      moveTo ( 0, 2 );
    else if ( this->cursor.GetX() <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && this->cursor.GetX() >= ( BOARD_ORIGIN_X ) && this->cursor.GetY() <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) ) && this->cursor.GetY() >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) )
      moveTo ( 1, 2 );
    else if ( this->cursor.GetX() <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) ) && this->cursor.GetX() >= ( BOARD_ORIGIN_X ) && this->cursor.GetY() <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) ) && this->cursor.GetY() >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) )
      moveTo ( 2, 2 );

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
          this->hand[turn].RemoveCard ( this->hand[turn].getSelectedCard() );

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
  if ( this->IsFullScreen() == false )
  {
    this->engine->SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_FULLSCREEN );
    this->setFullscreen ( true );
  }
  else
  {
    this->engine->SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_RESIZABLE );
    this->setFullscreen ( false );
  }
}

void TTcards::HandleInput ( void )
{
  SDLInput::Input ();
}

void TTcards::onKeyDown ( SDLKey key, SDLMod mod )
{
  switch ( key )
  {
    case SDLK_ESCAPE:
    case SDLK_q: this->onExit(); break;
    case SDLK_f: this->onResize ( 0, 0 ); break;

    case SDLK_p: /* Pause State ... */ break;
    case SDLK_m: this->music.togglePlayingMusic(); break;
    case SDLK_EQUALS: this->toggleFPS(); break;

    case SDLK_e: this->endTurn(); break;
    case SDLK_LEFTBRACKET: debugListCards ( mod ); break;
    case SDLK_RIGHTBRACKET: debugListCollection( mod ); break;
    case SDLK_d: if ( mod == KMOD_LMETA ) this->removePlayerCard(); break;

    case SDLK_i: debugBox(); break;
    case SDLK_r: this->engine->PushState ( std::unique_ptr<TTcards>( new TTcards ( this->engine ) ) ); break;
    case SDLK_SLASH: this->engine->PushState ( std::unique_ptr<CardsMenu> ( new CardsMenu ( this->engine, &this->collection ) ) ); break;

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
  unsigned int turn = 0;

  for ( turn = 0; turn < TOTAL_PLAYERS; turn++ )
  {
    if ( get_turn() == turn ) // locks the input to the current player
    {
      std::pair<int, int> player_coords = player[turn].getXY(); // PLAYER ORIGIN XY

      // player hand selection checks

      if ( x <= ( std::get<0>(player_coords) + CARD_WIDTH ) && x >= ( std::get<0>(player_coords) ) && y <= ( std::get<1>(player_coords) + ( CARD_HEIGHT / 2 ) * 1 ) && y >= ( std::get<1>(player_coords) ) )
      {
        //std::cout << hand[turn].cards[0].name << "\n";
        hand[turn].selectCard ( hand[turn].cards[0] );

        // Updates Cursor Position
        if ( hand[turn].cards[0].getID() != 0 )
          cursor.SetXY ( std::get<0>(player_cursor_coords[turn]), std::get<1>(player_cursor_coords[turn]) + ( CARD_HEIGHT / 2 ) * 0 );
      }

      else if ( x <= ( std::get<0>(player_coords) + CARD_WIDTH ) && x >= ( std::get<0>(player_coords) ) && y <= ( std::get<1>(player_coords) + ( CARD_HEIGHT / 2 ) * 2 ) && y >= ( std::get<1>(player_coords) ) )
      {
        //std::cout << hand[turn].cards[1].name << "\n";
        hand[turn].selectCard ( hand[turn].cards[1] );

        // Updates Cursor Position
        if ( hand[turn].cards[1].getID() != 0 )
          cursor.SetXY ( std::get<0>(player_cursor_coords[turn]), std::get<1>(player_cursor_coords[turn]) + ( CARD_HEIGHT / 2 ) * 1 );
      }

      else if ( x <= ( std::get<0>(player_coords) + CARD_WIDTH ) && x >= ( std::get<0>(player_coords) ) && y <= ( std::get<1>(player_coords) + ( CARD_HEIGHT / 2 ) * 3 ) && y >= ( std::get<1>(player_coords) ) )
      {
        //std::cout << hand[turn].cards[2].name << "\n";
        hand[turn].selectCard ( hand[turn].cards[2] );

        // Updates Cursor Position
        if ( hand[turn].cards[2].getID() != 0 )
          cursor.SetXY ( std::get<0>(player_cursor_coords[turn]), std::get<1>(player_cursor_coords[turn]) + ( CARD_HEIGHT / 2 ) * 2 );
      }

      else if ( x <= ( std::get<0>(player_coords) + CARD_WIDTH ) && x >= ( std::get<0>(player_coords) ) && y <= ( std::get<1>(player_coords) + ( CARD_HEIGHT / 2 ) * 4 ) && y >= ( std::get<1>(player_coords) ) )
      {
        //std::cout << hand[turn].cards[3].name << "\n";
        hand[turn].selectCard ( hand[turn].cards[3] );

        // Updates Cursor Position
        if ( hand[turn].cards[3].getID() != 0 )
          cursor.SetXY ( std::get<0>(player_cursor_coords[turn]), std::get<1>(player_cursor_coords[turn]) + ( CARD_HEIGHT / 2 ) * 3 );
      }

      else if ( x <= ( std::get<0>(player_coords) + CARD_WIDTH ) && x >= ( std::get<0>(player_coords) ) && y <= ( std::get<1>(player_coords) + ( CARD_HEIGHT / 2 ) * 5 ) && y >= ( std::get<1>(player_coords) ) )
      {
        //std::cout << hand[turn].cards[4].name << "\n";
        hand[turn].selectCard ( hand[turn].cards[4] );

        // Updates Cursor Position
        if ( hand[turn].cards[4].getID() != 0 )
          cursor.SetXY ( std::get<0>(player_cursor_coords[turn]), std::get<1>(player_cursor_coords[turn]) + ( CARD_HEIGHT / 2 ) * 4 );
      }

      // board grid checks of players

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 0 ) ) )
        moveTo ( 0, 0 );

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 0 ) ) )
        moveTo ( 1, 0 );

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 0 ) ) )
        moveTo ( 2, 0 );

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) )
        moveTo ( 0, 1 );

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) )
        moveTo ( 1, 1 );

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) )
        moveTo ( 2, 1 );

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) )
        moveTo ( 0, 2 );

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && x >= ( BOARD_ORIGIN_X ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) )
        moveTo ( 1, 2 );

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) ) && x >= ( BOARD_ORIGIN_X ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) )
        moveTo ( 2, 2 );
    } // end get_turn() == turn
  } // end player turns
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
    case 4: this->moveCursorUp(); break;
    case 5: this->moveCursorRight(); break;
    case 6: this->moveCursorDown(); break;
    case 7: this->moveCursorLeft(); break;

    case 10: this->endTurn(); break;

    case 12: // triangle
      // TODO
    break;

    // circle
    case 13: this->unlockSelectedCard(); break;

    // cross
    case 14: this->lockSelectedCard(); break;
  }
}

// Helper method for obtaining card hand index position based off coords ID map
unsigned int TTcards::getCursorPos ( void )
{
  unsigned int pos = 0;

  if ( this->cursor.GetY() <= std::get<0>(cursor_coords_map[0]) )
    pos = 0;
  else if ( this->cursor.GetY() <= std::get<0>(cursor_coords_map[1]) )
    pos = std::get<1>(cursor_coords_map[1]);
  else if ( this->cursor.GetY() <= std::get<0>(cursor_coords_map[2]) )
    pos = std::get<1>(cursor_coords_map[2]);
  else if ( this->cursor.GetY() <= std::get<0>(cursor_coords_map[3]) )
    pos = std::get<1>(cursor_coords_map[3]);
  else
    pos = 4;

  return pos;
}

void TTcards::moveCursorLeft ( void )
{
  if ( this->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->cursor.GetX() > BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) )
      this->cursor.UpdateXY ( -( CARD_WIDTH ), 0 );
  }
}

void TTcards::moveCursorRight ( void )
{
  if ( this->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->cursor.GetX() < BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) )
      this->cursor.UpdateXY ( ( CARD_WIDTH ), 0 );
  }
}

void TTcards::moveCursorUp ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->cursor.getState() == 0 )
  {
    if ( this->cursor.GetY() > PLAYER1_CURSOR_ORIGIN_Y )
    {
      this->cursor.UpdateXY ( 0, -( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->hand[player_turn].selectCard ( this->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->cursor.GetY() > BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) )
      this->cursor.UpdateXY ( 0, -( CARD_HEIGHT ) );
  }
}

void TTcards::moveCursorDown ( void )
{
  unsigned int pos = 0;
  unsigned int player_turn = get_turn();

  if ( this->cursor.getState() == 0 )
  {
    if ( this->cursor.GetY() < ( CARD_HEIGHT / 2 ) * ( this->hand[player_turn].getCount() ) )
    {
      this->cursor.UpdateXY ( 0, ( CARD_HEIGHT / 2 ) );

      pos = this->getCursorPos();
      this->hand[player_turn].selectCard ( this->hand[player_turn].cards[pos] );
    }
  }
  else if ( this->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->cursor.GetY() < BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) )
      this->cursor.UpdateXY ( 0, ( CARD_HEIGHT ) );
  }
}

void TTcards::updateCursor ( void )
{
  if ( this->get_turn() == 0 ) // player1
    this->cursor.SetSheetID ( INTERFACE_CURSOR_LEFT );
  else // player2
    this->cursor.SetSheetID ( INTERFACE_CURSOR_RIGHT );

  if ( this->debug_box.isEnabled() == true )
    this->showCardID();
}

void TTcards::drawCursor ( void )
{
  this->cursor.Draw ( this->engine );
}

void TTcards::interface_playingCards ( void )
{
  unsigned int player_turn = get_turn();

  Card active;

  active = this->hand[player_turn].getSelectedCard();

  if ( active.getName() != "\0" )
  {
    this->info_text.setTextBuffer ( active.getName() );
    unsigned int text_width = this->info_text.getTextWidth();
    this->info_small_text.setTextBuffer ( "INFO" );

    this->info_box.Draw ( this->engine->screen, 104, 194, 176, 24 );
    this->info_text.Draw ( this->engine, ( SCREEN_WIDTH - text_width ) / 2, 196 );
    this->info_small_text.Draw ( this->engine, 108, 194 );
  }
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

    hand_count = this->hand[turn].getCount();

    this->player[turn].setScore ( board_count + hand_count );
  }
}

void TTcards::drawScore ( void )
{
  this->score_text.setTextBuffer ( std::to_string ( player[0].getScore() ) );
  this->score_text.Draw ( engine, PLAYER1_SCORE_ORIGIN_X, PLAYER1_SCORE_ORIGIN_Y );

  this->score_text.setTextBuffer ( std::to_string ( player[1].getScore() ) );
  this->score_text.Draw ( engine, PLAYER2_SCORE_ORIGIN_X, PLAYER2_SCORE_ORIGIN_Y );
}

void TTcards::Update ( void )
{
  this->fps.Update();

  // game / round is over when board card count >= 9
  if ( this->board.getCount () >= 9 /* || this->hand[0].getCount() == 0 || this->hand[1].getCount() == 0 */)
  {
    // Game Over State
    if ( this->player[0].getScore() > this->player[1].getScore() ) // player 1 wins
      engine->PushState ( std::unique_ptr<GameOver>( new GameOver( this->engine, 1 ) ) );
    else if ( this->player[1].getScore() > this->player[0].getScore() ) // player 2 wins
      engine->PushState ( std::unique_ptr<GameOver>( new GameOver( this->engine, 2 ) ) );
    else if ( this->player[0].getScore() == this->player[1].getScore() )  // player tie
      engine->PushState ( std::unique_ptr<GameOver>( new GameOver( this->engine, 3 ) ) );
    else
      engine->PushState ( std::unique_ptr<GameOver>( new GameOver( this->engine, 0 ) ) );
  }

  this->updateCursor();
  this->updateScore();

  this->engine->UpdateScreen ();
}

void TTcards::Draw ( void )
{
  this->board.Draw ( this->engine );

  this->player[0].Draw ( this->engine );
  this->player[1].Draw ( this->engine );

  if ( this->isCursorLocked() == false )
    this->interface_playingCards();

  if ( this->get_turn() == 0 ) // player1
    this->engine->DrawRectangle ( 48, 0, 16, 16, 188, 203, 236 ); // FIXME: placeholder for player select sprite animation
  else // player2
    this->engine->DrawRectangle ( 320, 0, 16, 16, 222, 196, 205 ); // // FIXME: placeholder for player select sprite animation

  this->drawCursor();

  this->drawScore ();

  this->drawFPS();
}
