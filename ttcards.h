#include <SDL.h>

// defaults
#define SCREEN_WIDTH 384
#define SCREEN_HEIGHT 224
#define SCREEN_COLORBIT 24

// DEBUG
#define VERBOSE
//#define DEBUG

/*
#ifdef DEBUG
  #define DEBUG_QUEUE 8
#endif
*/

int input_poll (void);
int init_game (void);
int load_image (void);
int init_image (void);
int draw_image (void);

SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;

int screen_width = SCREEN_WIDTH;
int screen_height = SCREEN_HEIGHT;

int screen_lock = 0;
int screen_unlock = 0;
//int resize_window = 0;
//int video_resize = 0;
//int fs_toggled = 0;
static int game_running = 1; // global app state
