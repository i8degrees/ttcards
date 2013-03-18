#include <SDL.h>

int init_sprite(struct Sprite *sprite_ptr, int x, int y, int width, int height, int id);
int load_sprite(struct Sprite *sprite_ptr, char filename[255]);

int draw_sprite(struct Sprite *sprite_ptr, SDL_Surface *screen);
int destroy_sprite(struct Sprite *sprite_ptr);

struct Sprite {

    SDL_Surface *addr; // Memory buffer alloc
    //SDL_Rect srect, drect;

    unsigned int id;
    unsigned int x;
    unsigned int y;
    unsigned int width; // Sprite width
    unsigned int height; // Sprite height
    unsigned int state;
    //char filename[255];
};
