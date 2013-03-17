#include "keyboard.h"
#include "ttcards.h"

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
