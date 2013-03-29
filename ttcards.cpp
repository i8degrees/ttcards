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

bool TTcards::CleanUp ( void )
{
  TTF_CloseFont ( font );


  SDL_FreeSurface ( screen );

  return true;
}

bool TTcards::Init ( void )
{
  if ( SDL_Init ( SDL_INIT_VIDEO ) == -1 )
  {
    std::cout << "ERR: Could not initialize SDL: " << SDL_GetError() << "\n";
    return false;
  }

  atexit ( SDL_Quit );

  screen = SDL_SetVideoMode ( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLORBIT, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RLEACCEL | SDL_RESIZABLE );

  if(screen == NULL)
  {
    std::cout << "ERR: Could not set video mode: " << SDL_GetError() << "\n";
    return false;
  }

  SDL_WM_SetCaption( "TTcards", NULL );



  TTF_Init();

  atexit ( TTF_Quit );

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
  SDL_Color white = { 255, 255, 255, 0 };
  SDL_Rect p1_north = { 26, 0, 0, 0 };
  SDL_Rect p1_east = { 30, 8, 0, 0 };
  SDL_Rect p1_south = { 26, 16, 0, 0 };
  SDL_Rect p1_west = { 22, 8, 0, 0 };

  SDL_Rect p2_north = { 104, 18, 0, 0 };
  SDL_Rect p2_east = { 112, 24, 0, 0 };
  SDL_Rect p2_south = { 104, 26, 0, 0 };
  SDL_Rect p2_west = { 100, 24, 0, 0 };

  background = board.LoadBackground ( BOARD_BACKGROUND );

  Deck cards ( CARDS_DB );

  TTF_Font *font = TTF_OpenFont ( CARD_FONTFACE, 12 );
  SDL_Surface *p1_txtNorth = TTF_RenderText_Solid ( font, "9", white );
  SDL_Surface *p1_txtEast = TTF_RenderText_Solid ( font, "6", white );
  SDL_Surface *p1_txtSouth = TTF_RenderText_Solid ( font, "A", white );
  SDL_Surface *p1_txtWest = TTF_RenderText_Solid ( font, "2", white );

  SDL_Surface *p2_txtNorth = TTF_RenderText_Solid ( font, "9", white );
  SDL_Surface *p2_txtEast = TTF_RenderText_Solid ( font, "6", white );
  SDL_Surface *p2_txtSouth = TTF_RenderText_Solid ( font, "A", white );
  SDL_Surface *p2_txtWest = TTF_RenderText_Solid ( font, "2", white );

  Sprite card0 ( 64, 64 );
  Sprite card0g ( 64, 64 );
  Sprite card0e ( 16, 16 );
  Sprite card1 ( 64, 64 );
  Sprite card1g ( 64, 64 );
  Sprite card1e ( 16, 16 );

  // p1.cards [16, 18] n+32

  card0.LoadImage ( "./data/images/faces/89.bmp" );

  card0.x = 18;
  card0.y = 0;
  card0.z = 0;

  card0g.LoadImage ( PLAYER1_CARDFACE );

  card0g.x = card0.x;
  card0g.y = card0.y;
  card0g.z = card0.z;

  card0e.LoadImage ( ELEMENT_WATER );

  card0e.x = card0.x + 46;
  card0e.y = card0.y + 4;
  card0e.z = 0;

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

  Pile player1 (&cards);
  //Pile player2 (&cards);

  //cards.Shuffle();

  //cards.Draw();

  for ( int i = 0; i < 5; i++ ) // for ( int i = 0; i < MAX_CARDSET; i++ )
  {
    std::cout << player1.cards[i].id << " " << player1.cards[i].name << endl;
  }

  while( TTcards::IsRunning() ) // main loop
  {

    SDL_BlitSurface(background, NULL, screen, NULL);

    card0g.Draw ( screen );
    card0.Draw ( screen );
    card0e.Draw ( screen );

    card1g.Draw ( screen );
    card1.Draw ( screen );
    card1e.Draw ( screen );

    SDL_BlitSurface ( p1_txtNorth, NULL, screen, &p1_north );
    SDL_BlitSurface ( p1_txtEast, NULL, screen, &p1_east );
    SDL_BlitSurface ( p1_txtSouth, NULL, screen, &p1_south );
    SDL_BlitSurface ( p1_txtWest, NULL, screen, &p1_west );

    SDL_BlitSurface ( p2_txtNorth, NULL, screen, &p2_north );
    SDL_BlitSurface ( p2_txtEast, NULL, screen, &p2_east );
    SDL_BlitSurface ( p2_txtSouth, NULL, screen, &p2_south );
    SDL_BlitSurface ( p2_txtWest, NULL, screen, &p2_west );

    TTcards::Input();

    if (SDL_Flip(screen) !=0)
    {
      printf("ERROR: Failed to swap video buffers.\n");
      return false;
    }
  }

  card0.Destroy ();
  card1.Destroy ();

  return true;
}
