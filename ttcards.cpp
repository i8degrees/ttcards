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
  SDL_FreeSurface ( this->background );
  this->background = NULL;
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

void TTcards::InterfaceInput ( SDL_Event &event )
{
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
      switch ( event.key.keysym.sym )
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
        case SDLK_EQUALS:
          if ( this->show_fps == true )
          {
            this->show_fps = false;
          }
          else
          {
            this->show_fps = true;
          }
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
    this->player2.Input ( this->input );
  }
}

bool TTcards::LoadBackground ( std::string filename )
{
  this->background = this->engine.LoadImage ( filename );

  return true;
}

bool TTcards::DrawBackground ( void )
{
  this->engine.DrawSurface ( this->background, 0, 0 );

  return true;
}

bool TTcards::Run ( void )
{
  this->LoadBackground ( BOARD_BACKGROUND );

  this->music.LoadMusicTrack ( MUSIC_TRACK );

  this->music.PlayMusicTrack ( -1 );
  this->music.PauseMusic ();

  this->collection.Load ( CARDS_DB );
  //this->debug.ListCards ( this->collection.cards );

  this->player1.AddCard ( this->collection.cards[89] ); // Diablos
  this->player1.AddCard ( this->collection.cards[109] ); // Squall
  this->player1.AddCard ( this->collection.cards[99] ); // Ward
  this->player1.AddCard ( this->collection.cards[20] ); // Jelleye [pos 3]
  this->player1.AddCard ( this->collection.cards[16] ); // Thrustaevis

  // These two cards should be discarded ( MAX_HAND = 5 )
  this->player1.AddCard ( this->collection.cards[88] ); // Carbuncle
  this->player1.AddCard ( this->collection.cards[24] ); // TriFace

  // This card should be removed
  this->player1.RemoveCard ( this->player1.cards[3] ); // Jelleye

  this->player1.AddCard ( this->collection.cards[88] ); // Carbuncle

  this->player2.AddCard ( this->collection.cards[20] ); // Jelleye
  this->player2.AddCard ( this->collection.cards[88] ); // Carbuncle
  this->player2.AddCard ( this->collection.cards[24] ); // TriFace
  this->player2.AddCard ( this->collection.cards[66] ); // Propagator
  this->player2.AddCard ( this->collection.cards[50] ); // Malboro

  // This card should be discarded ( MAX_HAND = 5 )
  this->player2.AddCard ( this->collection.cards[88] ); // Carbuncle

  this->timer_text.LoadTTF ( CARD_FONTFACE, 12 );
  this->timer_text.SetTextColor ( 170, 17, 17 ); // color: red

  this->fps.Start();

  this->player1.SetID ( 0 );
  this->player2.SetID ( 1 );

  while( this->IsRunning() ) // main loop
  {
    this->DrawBackground ();

    this->Input ();

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

    player1.Draw ( this->engine );
    player2.Draw ( this->engine );

    player1.DrawScore ( this->engine, 32, 176 ); // SCREEN_HEIGHT - 48
    player2.DrawScore ( this->engine, 320, 176 ); // 64 * 5

    this->engine.UpdateScreen ();

    this->fps.Update();

  } // while this->IsRunning()

  return true;
}
