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

bool TTcards::Init ( Gfx *engine )
{
  this->engine = engine; // initialize rendering interface
  this->show_fps = true;

  this->engine->SetWindowTitle ( APP_NAME );
  this->engine->SetWindowIcon ( APP_ICON );

  //SDL_EnableKeyRepeat( SDL_DEFAULT_REPEAT_DELAY / 12, SDL_DEFAULT_REPEAT_INTERVAL / 12 );

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

void TTcards::debug_input ( unsigned int type, SDLKey key, SDLMod mod )
{
  if ( type == SDL_KEYDOWN )
  {
    if ( key == SDLK_u || ( key == SDLK_e && mod == KMOD_LMETA ) )
    {
      this->hand[turn].ClearSelected();

      if ( this->get_turn() == 0 )
        this->player_turn ( 1 );
      else
        this->player_turn ( 0 );
    }

    else if ( key == SDLK_LEFTBRACKET )
    {
      this->debug.ListCards ( this->hand[0].cards );
    }

    else if ( key == SDLK_LEFTBRACKET && mod == KMOD_LMETA )
    {
      this->debug.ListCards ( this->hand[1].cards );
    }

    else if ( key == SDLK_RIGHTBRACKET )
    {
      board.ListContents();
    }

    else if ( key == SDLK_RIGHTBRACKET && mod == KMOD_LMETA )
    {
      this->debug.ListCards ( this->collection.cards );
    }
  }
}

void TTcards::board_input ( unsigned int type, SDLKey key, SDLMod mod )
{
  if ( type == SDL_KEYDOWN )
  {
    if ( key == SDLK_1 && mod == KMOD_LMETA )
    {
      for ( int turn = 0; turn < TOTAL_PLAYERS; turn++ )
      {
        if ( this->get_turn() == turn )
        {
          this->hand[turn].RemoveCard ( this->hand[turn].GetSelectedCard() ); //this->hand[0].RemoveCard ( this->hand[0].cards[this->hand[0].card_pos] );
          this->hand[turn].ClearSelected();
          this->hand[turn].SelectCard ( this->hand[turn].cards.front() );

          if ( this->get_turn() == 0 ) // player1
          {
            this->cursor.SetXY ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y );
          }
          else if ( this->get_turn() == 1 ) // player2
            this->cursor.SetXY ( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y );
        }
      }
    }

    else if ( key == SDLK_1 ) // move selected card to grid[0][0] if possible
    {
      this->moveTo ( 0, 0 );
    }

    else if ( key == SDLK_2 ) // move selected card to grid[1][0] if possible
    {
      this->moveTo ( 1, 0 );
    }

    else if ( key == SDLK_3 ) // move selected card to grid[2][0] if possible
    {
      this->moveTo ( 2, 0 );
    }

    else if ( key == SDLK_4 ) // move selected card to grid[0][1] if possible
    {
      this->moveTo ( 0, 1 );
    }

    else if ( key == SDLK_5 ) // move selected card to grid[1][1] if possible
    {
      this->moveTo ( 1, 1 );
    }

    else if ( key == SDLK_6 ) // move selected card to grid[2][1] if possible
    {
      this->moveTo ( 2, 1 );
    }

    else if ( key == SDLK_7 ) // move selected card to grid[0][2] if possible
    {
      this->moveTo ( 0, 2 );
    }

    else if ( key == SDLK_8 ) // move selected card to grid[1][2] if possible
    {
      this->moveTo ( 1, 2 );
    }

    else if ( key == SDLK_9 ) // move selected card to grid[2][2] if possible
    {
      this->moveTo ( 2, 2 );
    }
  }
}

void TTcards::cursor_input ( unsigned int type, SDLKey key, SDLMod mod )
{
  if ( type == SDL_KEYDOWN )
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

void TTcards::mouse_input ( unsigned int type, SDL_MouseButtonEvent button )
{
  unsigned int x = button.x;
  unsigned int y = button.y;

  if ( type == SDL_MOUSEBUTTONDOWN )
  {
    if ( button.button == SDL_BUTTON_LEFT )
    {
      for ( int turn = 0; turn < TOTAL_PLAYERS; turn++ )
      {
        if ( this->get_turn() == 0 ) // player1 hand checks
        {
          if ( x <= ( PLAYER1_ORIGIN_X + CARD_WIDTH ) && x >= ( PLAYER1_ORIGIN_X ) && y <= ( PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 1 ) && y >= ( PLAYER1_ORIGIN_Y ) )
          {
            std::cout << this->hand[0].cards[0].name << "\n";
            this->hand[0].SelectCard ( this->hand[0].cards[0] );
            if ( this->hand[0].cards[0].id != 0 )
              this->cursor.SetXY ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 0 );
          }

          else if ( x <= ( PLAYER1_ORIGIN_X + CARD_WIDTH ) && x >= ( PLAYER1_ORIGIN_X ) && y <= ( PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 2 ) && y >= ( PLAYER1_ORIGIN_Y ) )
          {
            std::cout << this->hand[0].cards[1].name << "\n";
            this->hand[0].SelectCard ( this->hand[0].cards[1] );
            if ( this->hand[0].cards[1].id != 0 )
              this->cursor.SetXY ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 1 );
          }

          else if ( x <= ( PLAYER1_ORIGIN_X + CARD_WIDTH ) && x >= ( PLAYER1_ORIGIN_X ) && y <= ( PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 3 ) && y >= ( PLAYER1_ORIGIN_Y ) )
          {
            std::cout << this->hand[0].cards[2].name << "\n";
            this->hand[0].SelectCard ( this->hand[0].cards[2] );
            if ( this->hand[0].cards[2].id != 0 )
              this->cursor.SetXY ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 2 );
          }

          else if ( x <= ( PLAYER1_ORIGIN_X + CARD_WIDTH ) && x >= ( PLAYER1_ORIGIN_X ) && y <= ( PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 4 ) && y >= ( PLAYER1_ORIGIN_Y ) )
          {
            std::cout << this->hand[0].cards[3].name << "\n";
            this->hand[0].SelectCard ( this->hand[0].cards[3] );
            if ( this->hand[0].cards[3].id != 0 )
              this->cursor.SetXY ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 3 );
          }

          else if ( x <= ( PLAYER1_ORIGIN_X + CARD_WIDTH ) && x >= ( PLAYER1_ORIGIN_X ) && y <= ( PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 5 ) && y >= ( PLAYER1_ORIGIN_Y ) )
          {
            std::cout << this->hand[0].cards[4].name << "\n";
            this->hand[0].SelectCard ( this->hand[0].cards[4] );
            if ( this->hand[0].cards[4].id != 0 )
              this->cursor.SetXY ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 4 );
          }
        } // player1 hand checks

        else if ( this->get_turn() == 1 ) // player2 hand checks
        {
          if ( x <= ( PLAYER2_ORIGIN_X + CARD_WIDTH ) && x >= ( PLAYER2_ORIGIN_X ) && y <= ( PLAYER2_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 1 ) && y >= ( PLAYER2_ORIGIN_Y ) )
          {
            std::cout << this->hand[1].cards[0].name << "\n";
            this->hand[1].SelectCard ( this->hand[1].cards[0] );
            this->cursor.SetXY ( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 0 );
          }

          else if ( x <= ( PLAYER2_ORIGIN_X + CARD_WIDTH ) && x >= ( PLAYER2_ORIGIN_X ) && y <= ( PLAYER2_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 2 ) && y >= ( PLAYER2_ORIGIN_Y ) )
          {
            std::cout << this->hand[1].cards[1].name << "\n";
            this->hand[1].SelectCard ( this->hand[1].cards[1] );
            this->cursor.SetXY ( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 1 );
          }

          else if ( x <= ( PLAYER2_ORIGIN_X + CARD_WIDTH ) && x >= ( PLAYER2_ORIGIN_X ) && y <= ( PLAYER2_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 3 ) && y >= ( PLAYER2_ORIGIN_Y ) )
          {
            std::cout << this->hand[1].cards[2].name << "\n";
            this->hand[1].SelectCard ( this->hand[1].cards[2] );
            this->cursor.SetXY ( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 2 );
          }

          else if ( x <= ( PLAYER2_ORIGIN_X + CARD_WIDTH ) && x >= ( PLAYER2_ORIGIN_X ) && y <= ( PLAYER2_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 4 ) && y >= ( PLAYER2_ORIGIN_Y ) )
          {
            std::cout << this->hand[1].cards[3].name << "\n";
            this->hand[1].SelectCard ( this->hand[1].cards[3] );
            this->cursor.SetXY ( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 3 );
          }

          else if ( x <= ( PLAYER2_ORIGIN_X + CARD_WIDTH ) && x >= ( PLAYER2_ORIGIN_X ) && y <= ( PLAYER2_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 5 ) && y >= ( PLAYER2_ORIGIN_Y ) )
          {
            std::cout << this->hand[1].cards[4].name << "\n";
            this->hand[1].SelectCard ( this->hand[1].cards[4] );
            this->cursor.SetXY ( PLAYER2_CURSOR_ORIGIN_X, PLAYER2_CURSOR_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * 4 );
          }
        } // player2 hand checks

        if ( this->get_turn() == turn ) // board grid checks of players
        {
          if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 0 ) ) )
          {
            this->moveTo ( 0, 0 );
          }

          else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 0 ) ) )
          {
            this->moveTo ( 1, 0 );
          }

          else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 0 ) ) )
          {
            this->moveTo ( 2, 0 );
          }

          else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) )
          {
            this->moveTo ( 0, 1 );
          }

          else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) )
          {
            this->moveTo ( 1, 1 );
          }

          else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 1 ) ) )
          {
            this->moveTo ( 2, 1 );
          }

          else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 1 ) ) && x >= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 0 ) ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) )
          {
            this->moveTo ( 0, 2 );
          }

          else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ) ) && x >= ( BOARD_ORIGIN_X ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) )
          {
            this->moveTo ( 1, 2 );
          }

          else if ( x <= ( BOARD_ORIGIN_X + ( CARD_WIDTH * 3 ) ) && x >= ( BOARD_ORIGIN_X ) && y <= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 3 ) ) && y >= ( BOARD_ORIGIN_Y + ( CARD_HEIGHT * 2 ) ) )
          {
            this->moveTo ( 2, 2 );
          }
        } // end each player's board checks
      } // for each player's turn loop
    } // if button == SDL_BUTTON_LEFT
  } // if type == SDL_MOUSEBUTTONDOWN
} // TTcards::mouse_input

void TTcards::InterfaceInput ( unsigned int type, SDLKey key, SDLMod mod )
{
  if ( type == SDL_QUIT )
  {
    this->SetGameState ( false );
  }

  else if ( type == SDL_VIDEORESIZE )
  {
    // Stub
  }

  else if ( type == SDL_KEYDOWN )
  {
    if ( key == SDLK_ESCAPE || key == SDLK_q )
    {
      this->SetGameState ( false );
      this->SetGameState ( false );
    }

    else if ( key == SDLK_f )
    {
      if ( this->IsFullScreen() == true )
        this->engine->SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_FULLSCREEN );
      else
        this->engine->SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_RESIZABLE );
    }

    else if ( key == SDLK_p )
    {
      this->music.togglePlayingMusic ();
    }

    else if ( key == SDLK_EQUALS )
    {
      if ( this->show_fps == true )
      {
        this->show_fps = false;
      }
      else // false
      {
        this->show_fps = true;
      }
    }
  }
}

void TTcards::Input ( void )
{
  while ( SDL_PollEvent ( &input ) )
  {
    this->InterfaceInput ( this->input.type, this->input.key.keysym.sym, this->input.key.keysym.mod );
    this->debug_input ( this->input.type, this->input.key.keysym.sym, this->input.key.keysym.mod );
    this->board_input ( this->input.type, this->input.key.keysym.sym, this->input.key.keysym.mod );
    this->cursor_input ( this->input.type, this->input.key.keysym.sym, this->input.key.keysym.mod );
    this->mouse_input ( this->input.type, this->input.button );
  }
}

void TTcards::check_cursor_movement ( void )
{

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

bool TTcards::LoadGameData ( void )
{
  this->collection.Load ( CARDS_DB );

  this->board.Init ( &this->card, &this->rules );
  this->board.LoadBackground ( BOARD_BACKGROUND );

  this->timer_text.LoadTTF ( CARD_FONTFACE, 12 );
  this->timer_text.SetTextColor ( 170, 17, 17 ); // color: red

  this->cursor = Sprite ( CURSOR_WIDTH, CURSOR_HEIGHT );
  this->cursor.LoadImage ( INTERFACE_CURSOR );
  this->cursor.SetSheetDimensions ( 78, 16, 0, 0 );
  this->cursor.SetSheetID ( INTERFACE_CURSOR_NONE );
  this->cursor.SetXY ( PLAYER1_CURSOR_ORIGIN_X, PLAYER1_CURSOR_ORIGIN_Y ); //this->cursor.SetXY ( CURSOR_ORIGIN_X, CURSOR_ORIGIN_Y );

  this->music.LoadMusicTrack ( MUSIC_TRACK );

  this->player[0].Init ( &this->hand[0], &this->card );
  this->player[1].Init ( &this->hand[1], &this->card );
  this->rules.SetRules ( 2 );

  return true;
}

void TTcards::Run ( void )
{
  this->LoadGameData();

  this->hand[0].AddCard ( this->collection.cards[89] ); // Diablos
  this->hand[0].AddCard ( this->collection.cards[109] ); // Squall
  this->hand[0].AddCard ( this->collection.cards[99] ); // Ward
  this->hand[0].AddCard ( this->collection.cards[20] ); // Jelleye [pos 3]
  this->hand[0].AddCard ( this->collection.cards[16] ); // Thrustaevis

  // These two cards should be discarded ( MAX_HAND = 5 )
  //this->hand[0].AddCard ( this->collection.cards[88] ); // Carbuncle
  //this->hand[0].AddCard ( this->collection.cards[24] ); // TriFace

  // This card should be removed
  //this->hand[0].RemoveCard ( this->hand[0].cards[3] ); // Jelleye

  this->hand[1].AddCard ( this->collection.cards[20] ); // Jelleye
  this->hand[1].AddCard ( this->collection.cards[88] ); // Carbuncle
  this->hand[1].AddCard ( this->collection.cards[24] ); // TriFace
  this->hand[1].AddCard ( this->collection.cards[66] ); // Propagator
  this->hand[1].AddCard ( this->collection.cards[50] ); // Malboro

  // This card should be discarded ( MAX_HAND = 5 )
  //this->hand[1].AddCard ( this->collection.cards[88] ); // Carbuncle

  this->music.PlayMusicTrack ( -1 );
  this->music.PauseMusic ();

  this->player[0].SetID ( 1 );
  this->player[1].SetID ( 2 );

  this->player_turn ( 0 );

  this->fps.Start();

  while( this->IsRunning() ) // main loop
  {
    this->check_cursor_movement();

    this->Input ();

    this->board.DrawBackground ( this->engine );
    this->board.DrawBoard ( this->engine );

    this->player[0].Draw ( this->engine ); //this->player[0].Update ( this->engine );
    this->player[1].Draw ( this->engine ); //this->player[1].Update ( this->engine );

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

    this->ShowFPS();

    this->engine->UpdateScreen ();

    this->fps.Update();

  } // while this->IsRunning() == true
}
