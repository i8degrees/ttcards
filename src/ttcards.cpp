/******************************************************************************
    ttcards.cpp

  Final Fantasy VIII Triple Triad Remake

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"

TTcards::TTcards ( void )
{
  #ifdef DEBUG_TTCARDS_OBJ
    std::cout << "TTcards::TTcards (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->turn = 0;
  this->cursor_locked = false;
  this->game_state = true;
  this->fullscreen = false;
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

bool TTcards::Init ( Gfx *engine )
{
  this->engine = engine; // initialize rendering interface
  this->show_fps = true;

  #ifndef EMSCRIPTEN
    engine->setTitle ( APP_NAME );
  #endif

  this->LoadGameData();

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

bool TTcards::LoadGameData ( void )
{
  this->collection.Load ( CARDS_DB );

  this->board.Init ( &this->card, &this->rules );
  this->board.LoadBackground ( BOARD_BACKGROUND );

  this->timer_text.LoadTTF ( CARD_FONTFACE, 12 );
  this->timer_text.SetTextColor ( 170, 17, 17 ); // color: red

  this->message_text.LoadTTF ( CARD_FONTFACE, 36 );
  this->message_text.SetTextColor ( 255, 255, 255 ); // color: red

  score_text.LoadTTF ( SCORE_FONTFACE, 32 );
  score_text.SetTextColor ( 255, 255, 255 ); // white

  this->cursor = Sprite ( CURSOR_WIDTH, CURSOR_HEIGHT );
  this->cursor.LoadImage ( INTERFACE_CURSOR );
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

  this->rules.setRules ( 1 );

  //AI.Init ( &this->board, &this->hand[1] );

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

bool TTcards::IsFullScreen ( void )
{
  if ( this->fullscreen == false )
    return false;
  else
    return true;
}

bool TTcards::IsRunning ( void )
{
  if ( this->game_state == false )
    return false;
  else
    return true;
}

void TTcards::SetGameState ( bool state )
{
  this->game_state = state;
}

void TTcards::drawFPS ( void )
{
  if ( this->show_fps == true )
  {
    this->timer_text.SetTextBuffer ( std::to_string ( this->fps.GetFPS() ) );
    signed int w = this->timer_text.GetTextWidth ();
    this->timer_text.DrawText ( this->engine, (SCREEN_WIDTH - w) / 2, 4 );
  }
  else // false
  {
    this->timer_text.SetTextBuffer ( " " );
    signed int w = this->timer_text.GetTextWidth ();
    this->timer_text.DrawText ( this->engine, (SCREEN_WIDTH - w) / 2, 4 );
  }
}

unsigned int TTcards::get_turn ( void )
{
  return this->turn;
}

void TTcards::player_turn ( unsigned int player )
{
  this->turn = player;

  resetCursor();
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
  //unsigned int player_turn = get_turn();

  this->cursor.setState ( 0 ); // player card select

  this->resetCursor();

  this->cursor_locked = false;
}

// helper method for cursor input selection
void TTcards::lockSelectedCard ( void )
{
  this->cursor.setState ( 1 ); // board select

  if ( this->cursor_locked == false )
  {
    if ( get_turn() == 0 )
      this->cursor.SetXY ( CURSOR_ORIGIN_X, CURSOR_ORIGIN_Y ); // FIXME
    else if ( get_turn() == 1 )
      this->cursor.SetXY ( CURSOR_ORIGIN_X-16, CURSOR_ORIGIN_Y ); // FIXME

    this->cursor_locked = true;
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

          if ( get_turn() == 0 )
          {
            player_turn ( 1 );
          }
          else if ( get_turn() == 1 )
          {
            player_turn ( 0 );
          }
        }
      }
    }
  }
}

void TTcards::Input ( void )
{
  SDLInput::Input ();
}


void TTcards::onJoyButtonDown ( unsigned int which, unsigned int button )
{
  switch ( button )
  {
    case 4: this->moveCursorUp(); break;
    case 5: this->moveCursorRight(); break;
    case 6: this->moveCursorDown(); break;
    case 7: this->moveCursorLeft(); break;

    case 10:
    {
      this->hand[turn].clearSelectedCard ();
      if ( get_turn() == 0 )
        player_turn ( 1 );
      else
        player_turn ( 0 );
    }
    break;

    case 12: // triangle
      // TODO
    break;

    // circle
    case 13: this->unlockSelectedCard(); break;

    // cross
    case 14: this->lockSelectedCard(); break;


  }
  std::cout << "onJoyButton(): " << button << std::endl;
}

void TTcards::onKeyDown ( SDLKey key, SDLMod mod )
{
  switch ( key )
  {
    case SDLK_ESCAPE:
    case SDLK_q: onExit(); break;

    case SDLK_f: onResize(0,0); break;

    case SDLK_p: music.togglePlayingMusic(); break;

    case SDLK_EQUALS:
    {
      if ( show_fps == true )
        show_fps = false;
      else
        show_fps = true;
    }
    break;

    case SDLK_u:
    case SDLK_e:
    {
      this->hand[turn].clearSelectedCard ();

      if ( get_turn() == 0 )
        player_turn ( 1 );
      else
        player_turn ( 0 );
    }
    break;

    case SDLK_LEFTBRACKET:
    {
      if ( mod == KMOD_LMETA )
        debug.ListCards ( hand[1].cards );
      else
        debug.ListCards ( hand[0].cards );
    }
    break;

    case SDLK_RIGHTBRACKET:
    {
      if ( mod == KMOD_LMETA )
        debug.ListCards ( collection.cards );
      else
        board.List();
    }
    break;

    case SDLK_d: if ( mod == KMOD_LMETA ) this->removePlayerCard(); break;

    case SDLK_LEFT: this->moveCursorLeft(); break;
    case SDLK_RIGHT: this->moveCursorRight(); break;
    case SDLK_UP: this->moveCursorUp(); break;
    case SDLK_DOWN: this->moveCursorDown(); break;

    case SDLK_x: this->unlockSelectedCard(); break;
    case SDLK_SPACE: this->lockSelectedCard(); break;

    case SDLK_1: moveTo ( 0, 0 ); break;
    case SDLK_2: moveTo ( 1, 0 ); break;
    case SDLK_3: moveTo ( 2, 0 ); break;
    case SDLK_4: moveTo ( 0, 1 ); break;
    case SDLK_5: moveTo ( 1, 1 ); break;
    case SDLK_6: moveTo ( 2, 1 ); break;
    case SDLK_7: moveTo ( 0, 2 ); break;
    case SDLK_8: moveTo ( 1, 2 ); break;
    case SDLK_9: moveTo ( 2, 2 ); break;

    default: break;
  }
}

void TTcards::onExit ( void )
{
  SetGameState ( false );
}

void TTcards::onResize ( unsigned int width, unsigned int height )
{
  if ( this->IsFullScreen() == false )
  {
    this->engine->SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_FULLSCREEN );
    this->fullscreen = true;
  }
  else
  {
    this->engine->SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_RESIZABLE );
    this->fullscreen = false;
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
  unsigned int player_turn = get_turn();

  if ( this->cursor.getState() == 0 )
  {
    if ( this->cursor.GetY() > PLAYER1_CURSOR_ORIGIN_Y )
    {
      this->cursor.UpdateXY ( 0, -( CARD_HEIGHT / 2 ) );

      if ( this->hand[player_turn].getCardIndex() > 0 && this->hand[player_turn].getCardIndex() < this->hand[player_turn].getCount() )
      {
        signed int pos = this->hand[player_turn].getPrevCardIndex();
        this->hand[player_turn].selectCard ( this->hand[player_turn].cards[pos] );
      }
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
  unsigned int player_turn = get_turn();

  if ( this->cursor.getState() == 0 )
  {
    if ( this->cursor.GetY() < ( CARD_HEIGHT / 2 ) * ( this->hand[player_turn].getCount() ) )
    {
      this->cursor.UpdateXY ( 0, ( CARD_HEIGHT / 2 ) );

      if ( this->hand[player_turn].getCardIndex() < this->hand[player_turn].getCount() )
      {
        signed int pos = this->hand[player_turn].getNextCardIndex();
        this->hand[player_turn].selectCard ( this->hand[player_turn].cards[pos] );
      }
    }
  }
  else if ( this->cursor.getState() == 1 ) // locked cursor to board select mode
  {
    if ( this->cursor.GetY() < BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) )
      this->cursor.UpdateXY ( 0, ( CARD_HEIGHT ) );
  }
}

void TTcards::update_cursor ( void )
{
  if ( this->get_turn() == 0 ) // player1
  {
    this->cursor.SetSheetID ( INTERFACE_CURSOR_LEFT );
  }
  else // player2
  {
    this->cursor.SetSheetID ( INTERFACE_CURSOR_RIGHT );
  }
}

void TTcards::draw_cursor ( void )
{
  this->cursor.Draw ( this->engine );
}

void TTcards::interface_GameOver ( void )
{
  this->message_text.SetTextBuffer ( "Game Over" );
  signed int width = this->message_text.GetTextWidth ();
  this->message_text.DrawText ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT - 128 ) / 2 );

  if ( this->player[0].getScore() > this->player[1].getScore() ) // player 1 wins
  {
    this->message_text.SetTextBuffer ( "Player 1 wins!" );
    signed int width = this->message_text.GetTextWidth ();
    this->message_text.DrawText ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
  else if ( this->player[1].getScore() > this->player[0].getScore() ) // player 2 wins
  {
    this->message_text.SetTextBuffer ( "Player 2 wins!" );
    signed int width = this->message_text.GetTextWidth ();
    this->message_text.DrawText ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
  else if ( this->player[0].getScore() == this->player[1].getScore() )  // player tie
  {
    this->message_text.SetTextBuffer ( "Tie!" );
    signed int width = this->message_text.GetTextWidth ();
    this->message_text.DrawText ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
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
    board_count = board.getPlayerCount ( turn + 1 );

    hand_count = hand[turn].cards.size();

    player[turn].setScore ( board_count + hand_count );
  }
}

void TTcards::drawScore ( void )
{
  score_text.SetTextBuffer ( std::to_string ( player[0].getScore() ) );
  score_text.DrawText ( engine, PLAYER1_SCORE_ORIGIN_X, PLAYER1_SCORE_ORIGIN_Y );

  score_text.SetTextBuffer ( std::to_string ( player[1].getScore() ) );
  score_text.DrawText ( engine, PLAYER2_SCORE_ORIGIN_X, PLAYER2_SCORE_ORIGIN_Y );
}


void TTcards::Update ( void )
{
  fps.Update();
  update_cursor();

  updateScore();

  // game / round is over when board card count >= 9
  if ( this->board.getCount () >= 9 /* || this->hand[0].getCount() == 0 || this->hand[1].getCount() == 0 */)
  {
    interface_GameOver();
  }

  engine->UpdateScreen ();
}

void TTcards::Draw ( void )
{
  board.Draw ( this->engine );

  player[0].Draw ( this->engine );
  player[1].Draw ( this->engine );

  draw_cursor();

  drawScore ();

  if ( get_turn() == 0 ) // player1
  {
    card.DrawName ( engine, hand[0].getSelectedCard(), 208 );
    engine->DrawRectangle ( 48, 0, 16, 16, 188, 203, 236 ); // FIXME: placeholder for player select sprite animation
  }
  else // player2
  {
    card.DrawName ( engine, hand[1].getSelectedCard(), 208 );
    engine->DrawRectangle ( 320, 0, 16, 16, 222, 196, 205 ); // // FIXME: placeholder for player select sprite animation
  }

  drawFPS();
}

void TTcards::Run ( void )
{
  Input ();
  Update ();
  Draw ();
}
