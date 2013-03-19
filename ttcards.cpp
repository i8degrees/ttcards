/******************************************************************************
    ttcards.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/

#include <iostream>
#include <SDL/SDL.h>

#include "ttcards.h"

TTcards::TTcards ( void )
{
  TTcards::game_state = true;
}

bool TTcards::CleanUp ( void )
{
  SDL_FreeSurface ( background );
  SDL_FreeSurface ( screen );

  return true;
}

bool TTcards::Init ( void )
{
  if ( SDL_Init (SDL_INIT_VIDEO) == -1 )
  {
    std::cout << "ERR: Could not initialize SDL: " << SDL_GetError() << "\n";
    return false;
  }

  atexit(SDL_Quit);

  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLORBIT, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);

  if(screen == NULL)
  {
    std::cout << "ERR: Could not set video mode: " << SDL_GetError() << "\n";
    return false;
  }

  SDL_WM_SetCaption( "TTcards", NULL );

  if ( ( background = SDL_LoadBMP ( "board.bmp" ) ) == NULL )
  {

    std::cout << "ERR: Could not load background: " << SDL_GetError() << std::endl;
  }

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
  Deck deck;
  deck.Load ( "cards.txt" );
  deck.Shuffle();
  deck.Draw();

  while( TTcards::IsRunning() ) // main loop
  {

    SDL_BlitSurface(background, NULL, screen, NULL);

    TTcards::Input();

    if (SDL_Flip(screen) !=0)
    {
      printf("ERROR: Failed to swap video buffers.\n");
      return false;
    }
  }

  return true;
}

int main(int argc, char* argv[])
{
  TTcards app;

  std::srand ( unsigned ( std::time(0) ) );

  if ( app.Init() == true )
  {
    app.Run();
  }
  else
  {
    app.CleanUp();
  }

  return true;
}
