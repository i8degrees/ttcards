#include "sprite.h"

int init_sprite(struct Sprite *sprite_ptr)
{
    sprite_ptr->width = 16;
    sprite_ptr->height = 16;

    return 0;
}

int draw_sprite(struct Sprite *sprite_ptr)
{
    printf("%d %d\n", sprite_ptr->width, sprite_ptr->height);

    return 0;
}
