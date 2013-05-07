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

bool TTcards::LoadGameData ( void )
{
  this->collection.Load ( CARDS_DB );

  this->board.Init ( &this->card, &this->rules );
  this->board.LoadBackground ( BOARD_BACKGROUND );

  this->timer_text.LoadTTF ( CARD_FONTFACE, 12 );
  this->timer_text.SetTextColor ( 170, 17, 17 ); // color: red

  this->message_text.LoadTTF ( CARD_FONTFACE, 36 );
  this->message_text.SetTextColor ( 255, 255, 255 ); // color: red

  this->cursor = Sprite ( CURSOR_WIDTH, CURSOR_HEIGHT );
  this->cursor.LoadImage ( INTERFACE_CURSOR );
  this->cursor.SetSheetDimensions ( 78, 16, 0, 0 );
  this->cursor.SetSheetID ( INTERFACE_CURSOR_NONE );
  this->cursor.SetXY ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y ); //this->cursor.SetXY ( CURSOR_ORIGIN_X, CURSOR_ORIGIN_Y );

  this->music.LoadMusicTrack ( MUSIC_TRACK );

  this->player[0].Init ( &this->hand[0], &this->card );
  player[0].setXY ( PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y );

  this->player[1].Init ( &this->hand[1], &this->card );
  player[1].setXY ( PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y );

  this->rules.SetRules ( 0 );

  AI.Init ( &this->board, &this->hand[1] );

  return true;
}

bool TTcards::Init ( Gfx *engine )
{
  this->engine = engine; // initialize rendering interface
  this->show_fps = true;

#ifndef EMSCRIPTEN
  engine->setTitle ( APP_NAME );
#endif

  this->LoadGameData();

  this->hand[0].AddCard ( this->collection.cards[89] ); // Diablos
  this->hand[0].AddCard ( this->collection.cards[109] ); // Squallppp
  this->hand[0].AddCard ( this->collection.cards[99] ); // Ward
  this->hand[0].AddCard ( this->collection.cards[84] ); // Ifrit [pos 3]
  this->hand[0].AddCard ( this->collection.cards[16] ); // Thrustaevis

  // These two cards should be discarded ( MAX_HAND = 5 )
  //this->hand[0].AddCard ( this->collection.cards[88] ); // Carbuncle
  //this->hand[0].AddCard ( this->collection.cards[24] ); // TriFace

  // This card should be removed
  //this->hand[0].RemoveCard ( this->hand[0].cards[3] ); // Ifrit

  this->hand[1].AddCard ( this->collection.cards[20] ); // Jelleye
  this->hand[1].AddCard ( this->collection.cards[88] ); // Carbuncle
  this->hand[1].AddCard ( this->collection.cards[24] ); // TriFace
  this->hand[1].AddCard ( this->collection.cards[66] ); // Propagator
  this->hand[1].AddCard ( this->collection.cards[50] ); // Malboro

  // This card should be discarded ( MAX_HAND = 5 )
  //this->hand[1].AddCard ( this->collection.cards[88] ); // Carbuncle

  //this->music.PlayMusicTrack ( -1 );
  //this->music.PauseMusic ();

  this->player[0].SetID ( 1 );
  this->player[1].SetID ( 2 );

  this->player_turn ( 0 );

  this->fps.Start();

  SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

  return true;
}

bool TTcards::IsFullScreen ( void )
{
  if ( this->fullscreen == false )
    this->fullscreen = true;
  else
    this->fullscreen = false;

  return this->fullscreen;
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

void TTcards::ShowFPS ( void )
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

  for ( int turn = 0; turn < TOTAL_PLAYERS; turn++ )
  {
    if ( this->get_turn() == turn )
    {
      this->hand[turn].SelectCard ( this->hand[turn].cards.front() );
    }

    if ( this->get_turn() == 0 ) // player1 cursor
    {
      this->cursor.SetXY ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y );
    }
    else if ( this->get_turn() == 1 ) // player2 cursor
    {
      this->cursor.SetXY ( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y );
    }
  }
}

// Helper method for updating board with player's selected card
void TTcards::moveTo ( unsigned int x, unsigned int y )
{
  Card selected;

  for ( int turn = 0; turn < TOTAL_PLAYERS; turn++ )
  {
    selected = this->hand[turn].GetSelectedCard();

    if ( selected.id != 0 )
    {
      if ( this->board.GetStatus ( x, y ) == false )
      {
        if ( this->get_turn() == turn )
        {
          this->board.UpdateBoard ( x, y, this->hand[turn].GetSelectedCard() );
          this->hand[turn].RemoveCard ( this->hand[turn].GetSelectedCard() );

          std::pair<int, int> coords = board.checkBoard ( x, y, this->hand[turn].GetSelectedCard() );

          if ( std::get<0>(coords) != -1 && std::get<1>(coords) != -1 )
          {
            board.UpdatePlayerID ( std::get<0>(coords), std::get<1>(coords), this->hand[turn].GetSelectedCard().player_id );
          }

          if ( this->get_turn() == 0 )
          {
            this->player_turn ( 1 );
          }
          else if ( this->get_turn() == 1 )
          {
            this->player_turn ( 0 );
          }
        }
      }
    }
  }
}

void TTcards::cursor_input ( SDL_Event *input )
{
  SDLKey key = input->key.keysym.sym;

  if ( input->type == SDL_KEYDOWN )
  {
    if ( key == SDLK_SPACE )
    {
      if ( this->get_turn() == 0 ) // player1
      {
        signed int pos = this->hand[0].getCardIndex();
        this->hand[0].SelectCard ( this->hand[0].cards[pos] );
      }
      else if ( this->get_turn() == 1 ) // player2
      {
        signed int pos = this->hand[1].getCardIndex();
        this->hand[1].SelectCard ( this->hand[1].cards[pos] );
      }
    }

    else if ( key == SDLK_LEFT )
    {
      if ( this->get_turn() == 0 ) // player1
      {
        if ( this->cursor.GetX() > 96 )
        {
          this->cursor.UpdateXY ( -32, 0 );
        }
      }
    }

    else if ( key == SDLK_RIGHT )
    {
      if ( this->get_turn() == 0 ) // player1
      {
        if ( this->cursor.GetX() < 288 )
        {
          this->cursor.UpdateXY ( 32, 0 );
        }
      }
    }

    else if ( key == SDLK_UP )
    {
      if ( this->get_turn() == 0 ) // player1
      {
        if ( this->cursor.GetY() > PLAYER1_CURSOR_ORIGIN_Y && this->cursor.GetX() == PLAYER1_CURSOR_ORIGIN_X )
        {
          this->cursor.UpdateXY ( 0, -32 );

          if ( this->hand[0].getCardIndex() > 0 && this->hand[0].getCardIndex() < this->hand[0].cards.size() )
          {
            signed int pos = this->hand[0].getPrevCardIndex();
            this->hand[0].SelectCard ( this->hand[0].cards[pos] );
          }
        }
        else if ( this->cursor.GetX() > 96 && this->cursor.GetY() > 16 )
        {
          this->cursor.SetXY ( 96, this->cursor.GetY() ); // FIXME
        }
      }
    }

    else if ( key == SDLK_DOWN )
    {
      if ( this->get_turn() == 0 ) // player1
      {
        if ( this->cursor.GetY() < ( CARD_HEIGHT / 2 ) * ( this->hand[0].cards.size() ) && this->cursor.GetX() == PLAYER1_CURSOR_ORIGIN_X )
        {
          this->cursor.UpdateXY ( 0, ( CARD_HEIGHT / 2 ) );

          if ( this->hand[0].getCardIndex() < this->hand[0].cards.size() )
          {
            signed int pos = this->hand[0].getNextCardIndex();
            this->hand[0].SelectCard ( this->hand[0].cards[pos] );
          }
        }
        else if ( this->cursor.GetX() > PLAYER1_CURSOR_ORIGIN_X && this->cursor.GetY() < 128 )
        {
          this->cursor.UpdateXY ( 0, 32 );
        }
      }
    }
  }
}

void TTcards::Input ( void )
{
  SDLInput::Input (); //this->cursor_input ( &this->input );
}

void TTcards::onKeyDown ( SDLKey key, SDLMod mod )
{
  switch ( key )
  {
    case SDLK_ESCAPE:
    case SDLK_q:
      onExit();
    break;

    case SDLK_f:
      onResize(0,0);
    break;

    case SDLK_p:
      music.togglePlayingMusic();
    break;

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
      for ( int turn = 0; turn < TOTAL_PLAYERS; turn++ )
      {
        hand[turn].ClearSelected();
      }

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
        board.ListContents();
    }
    break;

    case SDLK_1: // move selected card to grid[0][0] if possible
    {
      if ( mod == KMOD_LMETA )
      {
        for ( int turn = 0; turn < TOTAL_PLAYERS; turn++ )
        {
          hand[turn].RemoveCard ( hand[turn].GetSelectedCard() );
          hand[turn].ClearSelected();
          hand[turn].SelectCard ( hand[turn].cards.front() );
        }

        if ( get_turn() == 0 ) // player1
          cursor.SetXY ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y );
        else // player2
          cursor.SetXY ( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y );
      }
      else
        moveTo ( 0, 0 );
    }
    break;

    case SDLK_2: // move selected card to grid[1][0] if possible
      moveTo ( 1, 0 );
    break;

    case SDLK_3: // move selected card to grid[2][0] if possible
      moveTo ( 2, 0 );
    break;

    case SDLK_4: // move selected card to grid[0][1] if possible
      moveTo ( 0, 1 );
    break;

    case SDLK_5: // move selected card to grid[1][1] if possible
      moveTo ( 1, 1 );
    break;

    case SDLK_6: // move selected card to grid[2][1] if possible
      moveTo ( 2, 1 );
    break;

    case SDLK_7: // move selected card to grid[0][2] if possible
      moveTo ( 0, 2 );
    break;

    case SDLK_8: // move selected card to grid[1][2] if possible
      moveTo ( 1, 2 );
    break;

    case SDLK_9: // move selected card to grid[2][2] if possible
      moveTo ( 2, 2 );
    break;

    default:
    break;
  }
}

void TTcards::onExit ( void )
{
  SetGameState ( false );
}

void TTcards::onResize ( unsigned int width, unsigned int height )
{
  if ( this->IsFullScreen() == false )
    this->engine->SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_FULLSCREEN );
  else
    this->engine->SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_RESIZABLE );
}

void TTcards::onMouseLeftButtonUp ( unsigned int x, unsigned int y )
{
  //std::cout << "Goodbye!\n\n";
}

void TTcards::onMouseLeftButtonDown ( unsigned int x, unsigned int y )
{
  unsigned int turn = 0;

  for ( turn = 0; turn < TOTAL_PLAYERS; turn++ )
  {
    if ( get_turn() == turn )
    {
      coords = player[turn].getXY(); // PLAYER ORIGIN XY

      // player hand selection checks

      if ( x <= ( std::get<0>(coords) + CARD_WIDTH ) && x >= ( std::get<0>(coords) ) && y <= ( std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * 1 ) && y >= ( std::get<1>(coords) ) )
      {
        std::cout << hand[turn].cards[0].name << "\n";
        hand[turn].SelectCard ( hand[turn].cards[0] );

        // Updates Cursor Position
        if ( hand[turn].cards[0].id != 0 )
          cursor.SetXY ( std::get<0>(coords), std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * 0 );
      }

      else if ( x <= ( std::get<0>(coords) + CARD_WIDTH ) && x >= ( std::get<0>(coords) ) && y <= ( std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * 2 ) && y >= ( std::get<1>(coords) ) )
      {
        std::cout << hand[turn].cards[1].name << "\n";
        hand[turn].SelectCard ( hand[turn].cards[1] );

        // Updates Cursor Position
        if ( hand[turn].cards[1].id != 0 )
          cursor.SetXY ( std::get<0>(coords), std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * 1 );
      }

      else if ( x <= ( std::get<0>(coords) + CARD_WIDTH ) && x >= ( std::get<0>(coords) ) && y <= ( std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * 3 ) && y >= ( std::get<1>(coords) ) )
      {
        std::cout << hand[turn].cards[2].name << "\n";
        hand[turn].SelectCard ( hand[turn].cards[2] );

        // Updates Cursor Position
        if ( hand[turn].cards[2].id != 0 )
          cursor.SetXY ( std::get<0>(coords), std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * 2 );
      }

      else if ( x <= ( std::get<0>(coords) + CARD_WIDTH ) && x >= ( std::get<0>(coords) ) && y <= ( std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * 4 ) && y >= ( std::get<1>(coords) ) )
      {
        std::cout << hand[turn].cards[3].name << "\n";
        hand[turn].SelectCard ( hand[turn].cards[3] );

        // Updates Cursor Position
        if ( hand[turn].cards[3].id != 0 )
          cursor.SetXY ( std::get<0>(coords), std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * 3 );
      }

      else if ( x <= ( std::get<0>(coords) + CARD_WIDTH ) && x >= ( std::get<0>(coords) ) && y <= ( std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * 5 ) && y >= ( std::get<1>(coords) ) )
      {
        std::cout << hand[turn].cards[4].name << "\n";
        hand[turn].SelectCard ( hand[turn].cards[4] );

        // Updates Cursor Position
        if ( hand[turn].cards[4].id != 0 )
          cursor.SetXY ( std::get<0>(coords), std::get<1>(coords) + ( CARD_HEIGHT / 2 ) * 4 );
      }

      // board grid checks of players

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 0 ) ) )
      {
        moveTo ( 0, 0 );
      }

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 0 ) ) )
      {
        moveTo ( 1, 0 );
      }

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 0 ) ) )
      {
        moveTo ( 2, 0 );
      }

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) )
      {
        moveTo ( 0, 1 );
      }

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) )
      {
        moveTo ( 1, 1 );
      }

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) )
      {
        moveTo ( 2, 1 );
      }

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) )
      {
        moveTo ( 0, 2 );
      }

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && x >= ( BOARD_ORIGIN_X ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) )
      {
        moveTo ( 1, 2 );
      }

      else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) ) && x >= ( BOARD_ORIGIN_X ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) )
      {
        moveTo ( 2, 2 );
      }
    } // end get_turn() == turn
  } // end player turns
}

void TTcards::onMouseWheel ( bool up, bool down )
{
  if ( up )
    std::cout << "Mouse Wheel [Up]" << "\n\n";
  else if ( down )
    std::cout << "Mouse Wheel [Down]" << "\n\n";
}

void TTcards::check_cursor_movement ( void )
{
  //
}

void TTcards::draw_cursor ( void )
{
  if ( this->get_turn() == 0 ) // player1
  {
    this->cursor.SetSheetID ( INTERFACE_CURSOR_LEFT );
  }
  else // player2
  {
    this->cursor.SetSheetID ( INTERFACE_CURSOR_RIGHT );
  }

  this->cursor.Draw ( this->engine );
}

void TTcards::update_cursor ( void )
{
  if ( this->get_turn() == 0 ) // player1
  {
    //this->card.DrawCard ( engine, this->hand->GetSelectedCard (), PLAYER1_ORIGIN_X + 16, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * this->hand[0].getCardIndex() );
  }
  else if ( this->get_turn() == 1 ) // player2
  {
    //this->card.DrawCard ( engine, this->hand->GetSelectedCard (), PLAYER2_ORIGIN_X - 16, PLAYER2_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * card_pos );
  }
}

void TTcards::interface_GameOver ( void )
{
  this->message_text.SetTextBuffer ( "Game Over" );
  signed int width = this->message_text.GetTextWidth ();
  this->message_text.DrawText ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT - 128 ) / 2 );

  if ( this->player[0].GetScore() > this->player[1].GetScore() ) // player 1 wins
  {
    this->message_text.SetTextBuffer ( "Player 1 wins!" );
    signed int width = this->message_text.GetTextWidth ();
    this->message_text.DrawText ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
  else if ( this->player[1].GetScore() > this->player[0].GetScore() ) // player 2 wins
  {
    this->message_text.SetTextBuffer ( "Player 2 wins!" );
    signed int width = this->message_text.GetTextWidth ();
    this->message_text.DrawText ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
  else if ( this->player[0].GetScore() == this->player[1].GetScore() )  // player tie
  {
    this->message_text.SetTextBuffer ( "Tie!" );
    signed int width = this->message_text.GetTextWidth ();
    this->message_text.DrawText ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
}

void TTcards::Run ( void )
{
  if ( this->get_turn() == 1 && this->hand[1].cards.size() > 0 )
  {
    unsigned int moveX = std::rand() % 3;
    unsigned int moveY = std::rand() % 3;
    unsigned int rID = std::rand() % 4;

    this->hand[1].SelectCard ( this->hand[1].cards[rID] );

    if ( this->board.checkBoard ( moveX, moveY, this->hand[1].GetSelectedCard() ) )
    {
      this->moveTo ( moveX, moveY );
      std::cout << "CPU:" << " " << "[random]" << std::endl;
    }
  }

  this->check_cursor_movement();

  this->Input ();

  this->board.DrawBackground ( this->engine );
  this->board.DrawBoard ( this->engine );

  this->player[0].Draw ( this->engine );
  this->player[1].Draw ( this->engine );

  this->draw_cursor();
  this->update_cursor();

  this->player[0].DrawScore ( this->engine, &this->board, 32, 176 ); // SCREEN_HEIGHT - 48
  this->player[1].DrawScore ( this->engine, &this->board, 320, 176 ); // 64 * 5

  if ( this->get_turn() == 0 )
  {
    this->card.DrawName ( this->engine, this->hand[0].GetSelectedCard(), 208 );
    this->engine->DrawRectangle ( 48, 0, 16, 16, 188, 203, 236 ); // FIXME: placeholder for player select sprite animation
  }
  else if ( this->get_turn() == 1 )
  {
    this->card.DrawName ( this->engine, this->hand[1].GetSelectedCard(), 208 );
    this->engine->DrawRectangle ( 320, 0, 16, 16, 222, 196, 205 ); // // FIXME: placeholder for player select sprite animation
  }

  if ( this->board.GetTotalCount () >= 9 ) // game / round is over
  {
    interface_GameOver();
  }

  this->ShowFPS();

  this->engine->UpdateScreen ();

  this->fps.Update();

  //SDL_Delay ( 250 );
}
