#include "board.h"

int init_board (struct Board *board_ptr)
{
  board_ptr->grid[3][3] = ( 0, 0, 0,
                            0, 0, 0,
                            0, 0, 0
                            );
  return 0;
}
