/******************************************************************************
    ttcards.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/

#include <SDL/SDL.h>
#include <cstdlib> // std::srand, std::rand

#include "board.h"
#include "card.h"
//#include "input.h"
#include "sprite.h"

using namespace std;

// defaults
#define SCREEN_WIDTH 384
#define SCREEN_HEIGHT 224
#define SCREEN_COLORBIT 24

class TTcards
{
public:
    TTcards();
    bool Run ( void );
    bool IsRunning ( void );
    void SetGameState ( bool state );

    bool Init ( void );
    void Input ( void );
    bool CleanUp ( void );

private:
    bool game_state; // global app state

    SDL_Surface *screen = NULL; // primary video buffer
    SDL_Surface *background = NULL;
};
