#include <SDL.h>

int init_sprite(struct Sprite *sprite_ptr);
int load_sprite(void);
int clear_sprite(void);
int draw_sprite(struct Sprite *sprite_ptr);
int update_sprite(void);

struct Sprite {

    SDL_Surface *sprite; // Memory buffer alloc
    SDL_Rect srect, drect;

    unsigned int width; // Sprite width
    unsigned int height; // Sprite height
};
