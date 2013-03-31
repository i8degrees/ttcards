/******************************************************************************
    ttcards.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"

using namespace std;

TTcards::TTcards ( void )
{
  SDL_Surface *screen = NULL;
  TTcards::game_state = true;
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
  if ( TTcards::game_state == false )
    return false;
  else
    return true;
}

void TTcards::SetGameState ( bool state )
{
  TTcards::game_state = state;
}

void TTcards::Input ( void )
{
  SDL_Event event;

  while ( SDL_PollEvent (&event) )
  {
    switch ( event.type )
    {
      case SDL_QUIT:
        TTcards::SetGameState ( false );
        break;
      case SDL_VIDEORESIZE:
        break;
      default:
        break;

      case SDL_KEYDOWN:
        switch ( event.key.keysym.sym )
        {
          case SDLK_ESCAPE:
            TTcards::SetGameState ( false );
            break;
          case SDLK_LEFT:
            break;
          case SDLK_RIGHT:
            break;
          case SDLK_UP:
            mixer.PlaySoundTrack ();
            break;
          case SDLK_DOWN:
            break;
          case SDLK_q:
            TTcards::SetGameState ( false );
            break;
          case SDLK_PLUS:
            break;
          case SDLK_MINUS:
            break;
          case SDLK_p:
            mixer.toggleMusic ();
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

  Deck cards ( CARDS_DB );

  mixer.LoadMusicTrack ( MUSIC_TRACK );
  mixer.LoadSoundTrack ( CURSOR_MOVE );

  //Pile player1 (&cards);
  //Pile player2 (&cards);

  //cards.Shuffle();

/*
  for ( int i = 0; i < 5; i++ ) // for ( int i = 0; i < MAX_CARDSET; i++ )
  {
    std::cout << player1.cards[i].id << " " << player1.cards[i].name << endl;
  }
*/

  mixer.PlayMusicTrack ();

  SDL_Rect offsets;
  offsets.x = 96;
  offsets.y = 18;

  while( TTcards::IsRunning() ) // main loop
  {
    TTcards::Input();

    board.DrawBackground ( screen );

    player1.DrawScore ( screen, 32, 176 );

    // Player 1
    player1.Draw ( screen, offsets.x, offsets.y );

    if (SDL_Flip(screen) !=0)
    {
      printf("ERR: Failed to swap video buffers.\n");
      return false;
    }

  } // while TTcards::IsRunning()

  return true;
}
