#include "sprite.h"

int init_sprite(struct Sprite *sprite_ptr, int x, int y, int width, int height, int id)
{
  sprite_ptr->addr = NULL;
  sprite_ptr->id = id;
  sprite_ptr->x = x;
  sprite_ptr->y = y;
  sprite_ptr->width = width;
  sprite_ptr->height = height;
  sprite_ptr->state = 0;

    return 0;
}

int load_sprite(struct Sprite *sprite_ptr, char filename[255])
{
  if ( ( sprite_ptr->addr = SDL_LoadBMP ( filename ) ) == NULL )
  {
#ifdef DEBUG
    printf("ERROR: Could not load %s: %s\n", filename, SDL_GetError() );

#endif
    return -1;
  }

  //printf("%d %d %d %d %d %s\n", sprite_ptr->id, sprite_ptr->x, sprite_ptr->y, sprite_ptr->width, sprite_ptr->height, filename);

  return 0;
}

int draw_sprite(struct Sprite *sprite_ptr, struct SDL_Surface *screen_ptr)
{
  SDL_Rect dest;
  SDL_Rect src;

  src.x = (sprite_ptr->id) * sprite_ptr->width;
  src.y = 0;
  src.w = sprite_ptr->width;
  src.h = sprite_ptr->height;

  dest.x = sprite_ptr->x;
  dest.y = sprite_ptr->y;
  dest.w = sprite_ptr->width;
  dest.h = sprite_ptr->height;

  SDL_BlitSurface(sprite_ptr->addr, &src, screen_ptr, &dest);

  return 0;
}

int destroy_sprite(struct Sprite *sprite_ptr)
{
  SDL_FreeSurface(sprite_ptr->addr);

  return 0;
}
