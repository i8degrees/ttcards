/******************************************************************************
    ttcards.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"

TTcards::TTcards ( void )
{
  this->screen = NULL;
  this->game_state = true;
}

TTcards::~TTcards ( void )
{
  SDL_FreeSurface ( this->screen );
  this->screen = NULL;
}

bool TTcards::Init ( void )
{
  std::srand ( ( unsigned ) time ( 0 ) );

  if ( this->gfx.Init ( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == false )
  {
    exit ( EXIT_FAILURE );
  }

  this->screen = this->gfx.SetMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                  SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RLEACCEL | SDL_RESIZABLE);

  if ( this->screen == 0 )
  {
    exit ( EXIT_FAILURE );
  }

  this->gfx.SetWindowTitle ( APP_NAME );
  this->gfx.SetWindowIcon ( APP_ICON );

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
            this->mixer1.PlaySoundTrack ( CURSOR_MOVE, 1, 0);
            break;
          case SDLK_RIGHT:
            this->mixer2.PlaySoundTrack ( CURSOR_CANCEL, 2, 0 );
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
            this->music.togglePlayingMusic ( );
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
  Player player1; // player2
  Board board;

  SDL_Rect offsets;
  offsets.x = 96;
  offsets.y = 18;

  board.LoadBackground ();

  this->music.LoadMusicTrack ( MUSIC_TRACK );

  this->music.PlayMusicTrack( -1 );
  this->music.PauseMusic();

  while( this->IsRunning() ) // main loop
  {
    this->Input();

    board.DrawBackground ( this->screen );

    player1.DrawScore ( this->screen, 32, 176 ); // SCREEN_HEIGHT - 48
    player2.DrawScore ( this->screen, 320, 176 ); // 64 * 5

    //player1.Draw ( this->screen, offsets.x, offsets.y );

    if (SDL_Flip ( this->screen ) !=0)
    {
      printf("ERR: Failed to swap video buffers.\n");
      return false;
    }

  } // while this->IsRunning()

  return true;
}
