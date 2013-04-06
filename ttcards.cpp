/******************************************************************************
    ttcards.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"

using namespace std;

TTcards::TTcards ( void )
{
  SDL_Surface *screen = NULL;
  game_state = true;
}

TTcards::~TTcards ( void )
{
  SDL_FreeSurface ( screen );
}

bool TTcards::Init ( void )
{

  if ( gfx.Init ( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == false )
  {
    exit ( EXIT_FAILURE );
  }

  screen = gfx.SetMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                  SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RLEACCEL | SDL_RESIZABLE);

  if ( screen == 0 )
  {
    exit ( EXIT_FAILURE );
  }

  gfx.SetWindowTitle ( APP_NAME );
  gfx.SetWindowIcon ( APP_ICON );

  mixer.Init ();

  txt.Init ();

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

void TTcards::Input ( void )
{
  SDL_Event event;

  while ( SDL_PollEvent (&event) )
  {
    switch ( event.type )
    {
      case SDL_QUIT:
        this->SetGameState ( false );
        break;
      case SDL_VIDEORESIZE:
        break;
      default:
        break;

      case SDL_KEYDOWN:
        switch ( event.key.keysym.sym )
        {
          case SDLK_ESCAPE:
            this->SetGameState ( false );
            break;
          case SDLK_LEFT:
            mixer.PlaySoundTrack ();
            break;
          case SDLK_RIGHT:
            mixer.PlaySoundTrack ();
            break;
          case SDLK_UP:
            break;
          case SDLK_DOWN:
            break;
          case SDLK_q:
            this->SetGameState ( false );
            break;
          case SDLK_PLUS:
            break;
          case SDLK_MINUS:
            break;
          case SDLK_p:
            mixer.isSongPlaying ();
            break;
          default:
            break;
        }
        break;
      }
    }
}

bool TTcards::Run ( void )
{
  Player player1, player2;
  Board board;

  board.Init ();
  board.LoadBackground ();

  mixer.LoadMusicTrack ( MUSIC_TRACK );
  mixer.LoadSoundTrack ( CURSOR_MOVE );

  mixer.PlayMusicTrack ();

  SDL_Rect offsets;
  offsets.x = 96;
  offsets.y = 18;

  while( this->IsRunning() ) // main loop
  {
    this->Input();

    board.DrawBackground ( screen );

    player1.DrawScore ( screen, 32, 176 );

    // Player 1
    player1.Draw ( screen, offsets.x, offsets.y );

    if (SDL_Flip(screen) !=0)
    {
      printf("ERR: Failed to swap video buffers.\n");
      return false;
    }

  } // while this->IsRunning()

  return true;
}
