#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "ttcards.h"
//#include "board.h"
#include "card.h"
//#include "input.h"
#include "sprite.h"

enum {
  MENU,
  PICK_CARDS
};

struct Player {
    unsigned int state;
    struct Card cards[MAX_CARDSET];
    //struct Sprite *sprite_ptr;
};

struct Player player1;
struct Player player2;
struct Deck deck;
struct Sprite card[MAX_CARDSET];

int input_poll(void)
{
  SDL_Event event;

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

/*
int load_background(SDL_Surface *screen_ptr, char filename[255])
{
  return 0;
}
*/

int main(int argc, char* argv[])
{
  player1.state = PICK_CARDS;

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

  SDL_WM_SetCaption( "TTcards", NULL );

  if ( ( background = SDL_LoadBMP ( "board.bmp" ) ) == NULL )
  {
#ifdef DEBUG
    printf("ERROR: Could not load %s: %s\n", background, SDL_GetError() );
#endif
    return -1;
  }

  //init_sprite(&card, 96, 18, CARD_WIDTH, CARD_HEIGHT, 0);
  //load_sprite(&card, "cards.bmp");

  //init_sprite(&card[0], CARD_WIDTH, CARD_HEIGHT, &card[0].id);
  //draw_sprite(&card[0]);

  /*
  for ( int i = 0; i < MAX_CARDSET; i++)
  {
    //init_card(&cards[i]);
    load_card(&cards[i], "cards.txt");
    //load_card(&cards[i], "cards.txt");
    printf("%d %s\n", cards[i].id, cards[i].name);
  }
  */

  load_deck(&deck,"cards.txt");

  for ( int i = 0; i < MAX_DECKSET; i++ )
  {
      //printf("%d %s %d %d %d %d\n", deck.cards[i].id, deck.cards[i].name, deck.cards[i].power[0], deck.cards[i].power[1], deck.cards[i].power[2], deck.cards[i].power[3]);
  }


  int r = NULL;
  srand ( time (NULL) );
  //r = init_card(player1.cards, &deck);

  init_card (player1.cards, &deck);

  for ( int i = 0; i < MAX_CARDSET; i++ )
  {
    printf("%d %s\n", player1.cards[i].id, player1.cards[i].name);
  }


  init_sprite(&card[0], 16, 18, CARD_WIDTH, CARD_HEIGHT, r);
  load_sprite(&card[0], "cards.bmp");

  /*
  r = init_card(&deck);
  init_sprite(&card[1], 16, 50, CARD_WIDTH, CARD_HEIGHT, r);
  load_sprite(&card[1], "cards.bmp");

  r = init_card(&deck);
  init_sprite(&card[2], 16, 82, CARD_WIDTH, CARD_HEIGHT, r);
  load_sprite(&card[2], "cards.bmp");

  r = init_card(&deck);
  init_sprite(&card[3], 16, 114, CARD_WIDTH, CARD_HEIGHT, r);
  load_sprite(&card[3], "cards.bmp");

  r = init_card(&deck);
  init_sprite(&card[4], 16, 146, CARD_WIDTH, CARD_HEIGHT, r);
  load_sprite(&card[4], "cards.bmp");
  */

  while(game_running) // main loop
  {

    SDL_BlitSurface(background, NULL, screen, NULL);

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

    draw_sprite(&card[0], screen);
    draw_sprite(&card[1], screen);
    draw_sprite(&card[2], screen);
    draw_sprite(&card[3], screen);
    draw_sprite(&card[4], screen);

    //draw_image();

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

  //destroy_sprite(&card);
  SDL_FreeSurface(background);
  SDL_FreeSurface(screen);

  return 0;
}
