/******************************************************************************
    ttcards.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"

using namespace std;

TTcards::TTcards ( void )
{
  TTcards::game_state = true;
}

TTcards::~TTcards ( void )
{
  SDL_FreeSurface ( screen );
}

bool TTcards::Init ( void )
{
  if ( SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == -1 )
  {
    std::cout << "ERR: Could not initialize SDL: " << SDL_GetError() << "\n";
    exit(-1);
  }

  atexit ( SDL_Quit );

  screen = SDL_SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLORBIT, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RLEACCEL | SDL_RESIZABLE );

  if(screen == NULL)
  {
    std::cout << "ERR: Could not set video mode: " << SDL_GetError() << "\n";
    exit(-1);
  }

  mixer.Init ();

  txt.Init ();

  SDL_WM_SetCaption( "TTcards", NULL );

  return true;
}

bool TTcards::IsRunning ( void )
{
  if (TTcards::game_state == false)
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
        // TODO
        break;
      default:
        // TODO
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
            // TODO
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

  // Player 2
/*
  card1.LoadImage ( "./data/images/faces/1.bmp" );

  card1.x = 96;
  card1.y = 18;
  card1.z = 0;

  card1g.LoadImage ( PLAYER2_CARDFACE );

  card1g.x = card1.x;
  card1g.y = card1.y;
  card1g.z = 0;

  card1e.LoadImage ( ELEMENT_THUNDER );

  card1e.x = card1.x + 46;
  card1e.y = card1.y + 4;
  card1e.z = 0;
*/

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
    //SDL_BlitSurface ( background, NULL, screen, NULL );

    player1.DrawScore ( screen, 32, 176 );

    //player2.DrawScore ( screen );

    // Player 2
/*
    //card1g.Draw ( card_buffer );
    //card1.Draw ( card_buffer );
    //card1e.Draw ( card_buffer );
*/

    // Player 1
    player1.Draw ( screen, offsets.x, offsets.y );
    //player2.Draw ( screen );

/*
    txt.DrawText ( card_buffer, "9", 8, 0, WHITE ); //txt.DrawText ( card_buffer, "9", 26, 0, WHITE );
    txt.DrawText ( card_buffer, "6", 12, 8, WHITE ); //txt.DrawText ( card_buffer, "6", 30, 8, WHITE );
    txt.DrawText ( card_buffer, "A", 8, 16, WHITE ); //txt.DrawText ( card_buffer, "A", 26, 16, WHITE );
    txt.DrawText ( card_buffer, "2", 4, 8, WHITE ); //txt.DrawText ( card_buffer, "2", 22, 8, WHITE );
*/

    // Player 2
/*
    txt.DrawText ( screen, "9", 104, 18, WHITE );
    txt.DrawText ( screen, "6", 112, 24, WHITE );
    txt.DrawText ( screen, "A", 104, 26, WHITE );
    txt.DrawText ( screen, "2", 100, 24, WHITE );
*/

    //SDL_BlitSurface ( card_buffer, NULL, screen, &offsets );

    if (SDL_Flip(screen) !=0)
    {
      printf("ERROR: Failed to swap video buffers.\n");
      return false;
    }

  } // while TTcards::IsRunning()

  return true;
}
