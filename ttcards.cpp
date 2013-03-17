#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "ttcards.h"
//#include "board.h"
#include "card.h"
//#include "keyboard.h"
#include "sprite.h"

int input_poll(void)
{
  SDL_Event event;
  //Uint8 *keys;

  while ( SDL_PollEvent (&event) )
  {
    switch ( event.type )
    {
      case SDL_QUIT:
        game_running = 0;
        break;
      case SDL_VIDEORESIZE:
        // TODO
        break;
      default:
        return -1;
        break;

      case SDL_KEYDOWN:
        switch ( event.key.keysym.sym )
        {
          case SDLK_ESCAPE:
            game_running = 0;
            break;
          case SDLK_LEFT:
            image_x-=80;
            break;
          case SDLK_RIGHT:
            image_x+=80;
            break;
          case SDLK_UP:
            image_y-=100;
            break;
          case SDLK_DOWN:
            image_y+=100;
            break;
          case SDLK_q:
            game_running = 0;
            break;
          default:
            return -1;
            break;
        }
        break;
      }
    }

  return 0;
}

int init_game(void)
{
  struct Sprite card[110];

  init_sprite(&card[0]);
  draw_sprite(&card[0]);

  return 0;
}

int load_image(void) // ADD: SDL_Surface *surface
{

  char csprite_file[255] = "cards.bmp";

  if ( ( image = SDL_LoadBMP ( csprite_file ) ) == NULL )
  {
#ifdef DEBUG
    printf("ERROR: Could not load %s: %s\n", csprite_file, SDL_GetError() );
    return -1;
#endif
  }

  return 0;
}

int init_image(void) // ADD: SDL_Surface *surface
{
  image_x = 120;
  image_y = 30;
  //image_y = 130;
  //image_x = (screen_width / 2) - (IMAGE_WIDTH / 2);
  //image_y = (screen_height / 2) - (IMAGE_HEIGHT / 2);

  rect.x = image_x;
  rect.y = image_y;
  //rect.w = IMAGE_WIDTH;
  //rect.h = IMAGE_HEIGHT;

  SDL_BlitSurface(image, 0, screen, &rect);

  return 0;
}

int draw_image(void) // ADD: SDL_Surface *surface
{
  //image_x = (screen_width / 2) - (IMAGE_WIDTH / 2);
  //image_y = (screen_height / 2) - (IMAGE_HEIGHT / 2);

  rect.x = image_x;
  rect.y = image_y;
  //rect.w = IMAGE_WIDTH;
  //rect.h = IMAGE_HEIGHT;

  SDL_BlitSurface(image, 0, screen, &rect);

  return 0;
}

int main(int argc, char* argv[])
{
  if ( SDL_Init (SDL_INIT_VIDEO) == -1 )
  {
    fprintf (stderr, "Could not initialize SDL: %s\n", SDL_GetError() );
    exit(1);
  }

  atexit(SDL_Quit);

  screen = SDL_SetVideoMode(screen_width, screen_height, SCREEN_COLORBIT, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);

  if(screen == NULL)
  {
    fprintf (stderr, "Could not set video mode: %s\n", SDL_GetError() );
  }

  init_game();
  init_card();
  init_image();
  load_image();

  black = SDL_MapRGB(screen->format, 0, 0, 0);
  blue = SDL_MapRGB(screen->format, 0, 0, 255);
  red = SDL_MapRGB(screen->format, 255, 0, 0);

  while(game_running) // main loop
  {
    SDL_FillRect(screen, 0, black);

    input_poll();

    if(SDL_MUSTLOCK(screen))
    {
      if (SDL_LockSurface(screen) !=0)
      {
        printf("ERROR: Could not lock surface.\n");
        return -1;
      }
    }
    if (SDL_MUSTLOCK(screen) == 0)
    {
      if (screen_lock == 0)
      {
#ifdef DEBUG
        printf("DEBUG: Surface appears to not require locking.\n");
        screen_lock = 1;
#endif
      }
    }

    draw_image();

    if(SDL_MUSTLOCK(screen))
    {
      SDL_UnlockSurface(screen);
    }
    if (SDL_MUSTLOCK(screen) == 0)
    {
      if (screen_unlock == 0)
      {
#ifdef DEBUG
        printf("DEBUG: Surface appears to not require unlocking.\n");
        screen_unlock = 1;
#endif
      }
    }

    if (SDL_Flip(screen) !=0)
    {
      printf("ERROR: Failed to swap video buffers.\n");
      return -1;
    }
  }

  return 0;
}
