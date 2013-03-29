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
  SDL_FreeSurface ( card_buffer );
  SDL_FreeSurface ( background );
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
            //card[0].x-=CARD_WIDTH;
            break;
          case SDLK_RIGHT:
            //card.x+=CARD_WIDTH;
            break;
          case SDLK_UP:
            //card.y-=CARD_HEIGHT;
            break;
          case SDLK_DOWN:
            //card.y+=CARD_HEIGHT;
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
  Board board;

  background = board.LoadBackground ( BOARD_BACKGROUND );

  Deck cards ( CARDS_DB );

  mixer.LoadMusicTrack ( MUSIC_TRACK );

  txt.LoadTTF ( CARD_FONTFACE, 12 );
  score.LoadTTF ( SCORE_FONTFACE, 32 );

  Sprite card0 ( 64, 64 );
  Sprite card0g ( 64, 64 );
  Sprite card0e ( 16, 16 );

  // Player 2
/*
  Sprite card1 ( 64, 64 );
  Sprite card1g ( 64, 64 );
  Sprite card1e ( 16, 16 );
*/

  card0.LoadImage ( "./data/images/faces/89.bmp" );

  card0.x = 0; //card0.x = PLAYER1_ORIGIN_X;
  card0.y = 0; //card0.y = PLAYER1_ORIGIN_Y;
  card0.z = 0; //card0.z = 0;

  card0g.LoadImage ( PLAYER1_CARDFACE );

  card0g.x = card0.x;
  card0g.y = card0.y;
  card0g.z = card0.z;

  card0e.LoadImage ( ELEMENT_WATER );

  card0e.x = card0.x + 46;
  card0e.y = card0.y + 4;
  card0e.z = 0;

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

  Pile player1 (&cards);
  //Pile player2 (&cards);

  //cards.Shuffle();

  //cards.Draw();

  for ( int i = 0; i < 5; i++ ) // for ( int i = 0; i < MAX_CARDSET; i++ )
  {
    std::cout << player1.cards[i].id << " " << player1.cards[i].name << endl;
  }

  mixer.PlayMusicTrack ();

  card_buffer = SDL_CreateRGBSurface ( 0, 64, 64,
                                      screen->format->BitsPerPixel,
                                      screen->format->Rmask,
                                      screen->format->Gmask,
                                      screen->format->Bmask,
                                      screen->format->Amask);

  SDL_Rect offsets = { 96, 18, 64, 64 };

  while( TTcards::IsRunning() ) // main loop
  {
    TTcards::Input();

    SDL_BlitSurface ( background, NULL, screen, NULL );

    // Player 1
    score.DrawText ( screen, "5", 32, 176, WHITE );

    card0g.Draw ( card_buffer );
    card0.Draw ( card_buffer );
    card0e.Draw ( card_buffer );

    // Player 2
/*
    //card1g.Draw ( card_buffer );
    //card1.Draw ( card_buffer );
    //card1e.Draw ( card_buffer );
*/

    // Player 1

    txt.DrawText ( card_buffer, "9", 8, 0, WHITE ); //txt.DrawText ( card_buffer, "9", 26, 0, WHITE );
    txt.DrawText ( card_buffer, "6", 12, 8, WHITE ); //txt.DrawText ( card_buffer, "6", 30, 8, WHITE );
    txt.DrawText ( card_buffer, "A", 8, 16, WHITE ); //txt.DrawText ( card_buffer, "A", 26, 16, WHITE );
    txt.DrawText ( card_buffer, "2", 4, 8, WHITE ); //txt.DrawText ( card_buffer, "2", 22, 8, WHITE );

    // Player 2
/*
    txt.DrawText ( screen, "9", 104, 18, WHITE );
    txt.DrawText ( screen, "6", 112, 24, WHITE );
    txt.DrawText ( screen, "A", 104, 26, WHITE );
    txt.DrawText ( screen, "2", 100, 24, WHITE );
*/

    SDL_BlitSurface ( card_buffer, NULL, screen, &offsets );

    if (SDL_Flip(screen) !=0)
    {
      printf("ERROR: Failed to swap video buffers.\n");
      return false;
    }

  } // while TTcards::IsRunning()

  return true;
}
