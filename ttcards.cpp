/******************************************************************************
    ttcards.cpp

  Final Fantasy VIII Triple Triad Remake

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"

TTcards::TTcards ( void )
{
  this->game_state = true;
}

TTcards::~TTcards ( void )
{
  // Stub
}

bool TTcards::Init ( void )
{
  unsigned int flags = SDL_HWSURFACE | SDL_RLEACCEL | SDL_RESIZABLE | SDL_DOUBLEBUF;

  std::srand ( ( unsigned ) time ( 0 ) );

  if ( this->engine.Init ( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == false )
  {
    std::cout << "ERR in TTcards::Init (): " << SDL_GetError() << std::endl;
    return false;
  }

  if ( this->engine.SetVideoMode (  SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, flags ) == false )
  {
    std::cout << "ERR in TTcards::Init (): " << SDL_GetError() << std::endl;
    return false;
  }

  this->engine.SetWindowTitle ( APP_NAME );
  this->engine.SetWindowIcon ( APP_ICON );
  this->show_fps = true;

  //SDL_EnableKeyRepeat( SDL_DEFAULT_REPEAT_DELAY / 12, SDL_DEFAULT_REPEAT_INTERVAL / 12 );

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

    else if ( key == SDLK_MINUS && mod == KMOD_LSHIFT ) // DEBUG
    {
      this->debug.ListCards ( this->collection.cards );
    }
  }
}

void TTcards::Input ( void )
{
  while ( SDL_PollEvent ( &input ) )
  {
    this->InterfaceInput ( this->input.type, this->input.key.keysym.sym, this->input.key.keysym.mod );
    this->player1.Input ( this->input.type, this->input.key.keysym.sym, this->input.key.keysym.mod, this->board );
    this->player2.Input ( this->input.type, this->input.key.keysym.sym, this->input.key.keysym.mod, this->board );
  }
}

bool TTcards::LoadGameData ( void )
{
  this->board.LoadBackground ( BOARD_BACKGROUND );

  this->timer_text.LoadTTF ( CARD_FONTFACE, 12 );
  this->timer_text.SetTextColor ( 170, 17, 17 ); // color: red

  this->collection.Load ( CARDS_DB );

  this->music.LoadMusicTrack ( MUSIC_TRACK );

  return true;
}

bool TTcards::Run ( void )
{
  this->LoadGameData();

  this->player1.Init ( this->board, this->player1_hand );
  this->player2.Init ( this->board, this->player2_hand );
  this->board.Init ( this->player1_hand, this->player2_hand );

  this->player1_hand.AddCard ( this->collection.cards[89] ); // Diablos
  this->player1_hand.AddCard ( this->collection.cards[109] ); // Squall
  this->player1_hand.AddCard ( this->collection.cards[99] ); // Ward
  this->player1_hand.AddCard ( this->collection.cards[20] ); // Jelleye [pos 3]
  this->player1_hand.AddCard ( this->collection.cards[16] ); // Thrustaevis

  // These two cards should be discarded ( MAX_HAND = 5 )
  this->player1_hand.AddCard ( this->collection.cards[88] ); // Carbuncle
  this->player1_hand.AddCard ( this->collection.cards[24] ); // TriFace

  // This card should be removed
  this->player1_hand.RemoveCard ( this->player1_hand.cards[3] ); // Jelleye

  this->player1_hand.AddCard ( this->collection.cards[88] ); // Carbuncle

  this->player2_hand.AddCard ( this->collection.cards[20] ); // Jelleye
  this->player2_hand.AddCard ( this->collection.cards[88] ); // Carbuncle
  this->player2_hand.AddCard ( this->collection.cards[24] ); // TriFace
  this->player2_hand.AddCard ( this->collection.cards[66] ); // Propagator
  this->player2_hand.AddCard ( this->collection.cards[50] ); // Malboro

  // This card should be discarded ( MAX_HAND = 5 )
  this->player2_hand.AddCard ( this->collection.cards[88] ); // Carbuncle

  this->music.PlayMusicTrack ( -1 );
  this->music.PauseMusic ();

  this->player1.SetState ( 0 );

  this->fps.Start();

  while( this->IsRunning() ) // main loop
  {
    this->Input ();

    this->board.DrawBackground ( this->engine );

    this->player1.SetID ( 0 );
    this->player2.SetID ( 1 );

    this->player1.Draw ( this->engine, this->board );
    this->player2.Draw ( this->engine, this->board );

    this->player1.DrawScore ( this->engine, 32, 176 ); // SCREEN_HEIGHT - 48
    this->player2.DrawScore ( this->engine, 320, 176 ); // 64 * 5

    this->ShowFPS();

    this->engine.UpdateScreen ();

    this->fps.Update();

  } // while this->game_state == true

  return true;
}
