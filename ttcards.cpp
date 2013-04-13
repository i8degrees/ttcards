/******************************************************************************
    ttcards.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"

TTcards::TTcards ( void )
{
  this->game_state = true;
}

TTcards::~TTcards ( void )
{
  // ...
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

void TTcards::InterfaceInput ( SDL_Event &event )
{
  unsigned char key = event.key.keysym.sym;

  switch ( event.type )
  {
    default:
      break;
    case SDL_QUIT:
      this->SetGameState ( false );
      break;
    case SDL_VIDEORESIZE:
      break;
    case SDL_KEYDOWN:
      switch ( key )
      {
        default:
          break;
        case SDLK_ESCAPE:
          this->SetGameState ( false );
          break;
        case SDLK_q:
          this->SetGameState ( false );
          break;
        case SDLK_p:
          this->music.togglePlayingMusic ();
          break;
      }
      break;
  }
}

void TTcards::Input ( void )
{
  while ( SDL_PollEvent ( &input ) )
  {
    this->InterfaceInput ( this->input );
    this->player1.Input ( this->input );
  }
}

bool TTcards::Run ( void )
{
  this->board.LoadBackground ( BOARD_BACKGROUND );

  this->music.LoadMusicTrack ( MUSIC_TRACK );

  this->music.PlayMusicTrack ( -1 );
  this->music.PauseMusic ();

  this->collection.Load ( CARDS_DB );
  //this->debug.ListCards ( this->collection.cards );

  this->player1.AddCard ( this->collection.cards[89] ); // Diablos

  this->player1.AddCard ( this->collection.cards[109] ); // Squall
/*
  this->player1.AddCard ( this->collection.cards[0] ); // Geezard
  this->player1.AddCard ( this->collection.cards[99] ); // Ward
*/
  this->player1.AddCard ( this->collection.cards[50] ); // Malboro

  this->player1.AddCard ( this->collection.cards[20] ); // Jelleye
  this->player1.AddCard ( this->collection.cards[16] ); // Thrustaevis

  // These two cards should be discarded
  this->player1.AddCard ( this->collection.cards[88] ); // Carbuncle
  this->player1.AddCard ( this->collection.cards[24] ); // TriFace

  this->debug.ListCards ( this->player1.cards );

  // This card should be removed
  this->player1.RemoveCard ( this->player1.cards[3] ); // Jelleye
  this->debug.ListCards ( this->player1.cards );

  this->player2.AddCard ( this->collection.cards[20] ); // Jelleye
  this->player2.AddCard ( this->collection.cards[16] ); // Thrustaevis
  this->player2.AddCard ( this->collection.cards[24] ); // TriFace
  this->player2.AddCard ( this->collection.cards[66] ); // Propagator
  this->player2.AddCard ( this->collection.cards[88] ); // Carbuncle
  this->player2.AddCard ( this->collection.cards[88] ); // Carbuncle

  this->debug.ListCards ( this->player2.cards );

  while( this->IsRunning() ) // main loop
  {
    this->Input ();

    board.DrawBackground ( this->engine );

    for ( int idx = 0; idx < this->player1.cards.size(); idx++ )
    {
      if ( this->player1.isValid( this->player1.cards[idx] ) )
      {
        this->cards.DrawCard ( engine, this->player1.cards[idx], 16, 16, 0 );
      }
      idx+=1;
      if ( this->player1.isValid( this->player1.cards[idx] ) == true )
      {
        this->cards.DrawCard ( engine, this->player1.cards[idx], 16, 40, 0 );
      }
      idx+=1;
      if ( this->player1.isValid( this->player1.cards[idx] ) == true )
      {
        this->cards.DrawCard ( engine, this->player1.cards[idx], 16, 64, 0 );
      }
      idx+=1;
      if ( this->player1.isValid( this->player1.cards[idx] ) == true )
      {
        this->cards.DrawCard ( engine, this->player1.cards[idx], 16, 88, 0 );
      }
      idx+=1;
      if ( this->player1.isValid( this->player1.cards[idx] ) == true )
      {
        this->cards.DrawCard ( engine, this->player1.cards[idx], 16, 112, 0 );
      }
      idx+=1;
    }

    for ( int idx = 0; idx < this->player2.cards.size(); idx++ )
    {
      if ( this->player2.isValid( this->player2.cards[idx] ) )
      {
        this->cards.DrawCard ( engine, this->player2.cards[idx], 304, 16, 1 );
      }
      idx+=1;
      if ( this->player2.isValid( this->player2.cards[idx] ) == true )
      {
        this->cards.DrawCard ( engine, this->player2.cards[idx], 304, 40, 1 );
      }
      idx+=1;
      if ( this->player2.isValid( this->player2.cards[idx] ) == true )
      {
        this->cards.DrawCard ( engine, this->player2.cards[idx], 304, 64, 1 );
      }
      idx+=1;
      if ( this->player2.isValid( this->player2.cards[idx] ) == true )
      {
        this->cards.DrawCard ( engine, this->player2.cards[idx], 304, 88, 1 );
      }
      idx+=1;
      if ( this->player2.isValid( this->player2.cards[idx] ) == true )
      {
        this->cards.DrawCard ( engine, this->player2.cards[idx], 304, 112, 1 );
      }
      idx+=1;
    }

    player1.DrawScore ( this->engine, 32, 176 ); // SCREEN_HEIGHT - 48
    player2.DrawScore ( this->engine, 320, 176 ); // 64 * 5

    this->engine.UpdateScreen ();

  } // while this->IsRunning()

  return true;
}
