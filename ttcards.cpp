#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

// defaults
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360
#define SCREEN_COLORBIT 24
#define IMAGE_WIDTH 80
#define IMAGE_HEIGHT 100

// DEBUG
#define VERBOSE
//#define DEBUG

/*
#ifdef DEBUG
  #define DEBUG_QUEUE 8
#endif
*/

int init_game (void);

int init_board (void);

int load_image (void);
int init_image (void);
int draw_image (void);

int init_cards (void);
int load_cards (void);

int input_poll (void);

SDL_Surface *screen;
SDL_Surface *image;
SDL_Rect rect; // TODO: use rect.w & rect.h as well
Uint16 black, blue, red;

struct Cards {

  //int magic;
  //int ver;

  unsigned int id;
  unsigned int level; // 1..10
  unsigned int type;
  unsigned int element;
  unsigned int power[4]; // clockwise; top, right, down, left
  char name[255];
};

struct Cards card;

/*
  enum {
    MONSTER, // type 1; lvl 1..5
    BOSS, // type 2; lvl 6..7
    GF, // type 3; lvl 8..9
    PLAYER // type 4; lvl 10
  } class;
*/

/*
  enum {
    EARTH, // type 1
    FIRE, // type 2
    HOLY, // type 3
    POISON, // type 4
    THUNDER, // type 5
    WATER, // type 6
    WIND // type 7
  } elemental;
*/

int board[3][3];

int screen_width = SCREEN_WIDTH;
int screen_height = SCREEN_HEIGHT;
int image_x = 0;
int image_y = 0;

int screen_lock = 0;
int screen_unlock = 0;
//int resize_window = 0;
//int video_resize = 0;
//int fs_toggled = 0;
int game_running = 1; // global app state

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

int init_cards(void)
{
  char data_file[255] = "cards.txt";
  FILE *fp = NULL;

  fp = fopen ( data_file, "r" );

  if ( fp == NULL)
  {
    printf("Error opening file: %s\n", data_file);
    return -1;
  }

  for ( int y = 0; ! feof(fp); y++ )
  {
    // 0 1 1 0 1 4 5 1 Geezard
    fscanf ( fp, "%i %i %i %i %i %i %i %i %[A-Za-z0-9- ]",
                  &card.id, &card.level, &card.type, &card.element,
                  &card.power[0], &card.power[1], &card.power[2], &card.power[3],
                  card.name);
    fscanf ( fp, "\n");

    printf("%i %i %i %i %i %i %i %i %s\n",
            card.id, card.level, card.type, card.element,
            card.power[0], card.power[1], card.power[2], card.power[3],
            card.name);
  }

  fclose(fp);

  return 0;
}

int init_board(void)
{
  board[3][3] = ( 0, 0, 0,
                  0, 0, 0,
                  0, 0, 0
                );
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

  init_cards();
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
