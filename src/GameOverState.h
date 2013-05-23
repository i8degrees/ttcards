/******************************************************************************
    GameOverState.h

  Game Over state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_GAMEOVER_HEADERS
#define GAMEAPP_GAMEOVER_HEADERS

#include <iostream>
#include <string>

#include "ttcards.h"

class GameOver: public GameState
{
  public:
    GameOver ( Gfx *engine, unsigned int state = 0 );
    ~GameOver ( void );

    void Pause ( void );
    void Resume ( void );

    void HandleInput ( void );
    void Update ( void );
    void Draw ( void );

  private:
    void onExit ( void );
    void onKeyDown ( SDLKey key, SDLMod mod );
    void Load ( void );

    Gfx *engine; // Pointer reference to our rendering interface
    SDL_TFont gameOver_text; // dialog text

    unsigned int state;
};

#endif // GAMEAPP_GAMEOVER_HEADERS defined
