#include <SDL.h>

int init_board (struct Board *board_ptr);

struct Board {

    unsigned int grid[3][3];
};
