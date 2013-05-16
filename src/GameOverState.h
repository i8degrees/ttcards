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
    GameOver ( Gfx *engine );
    ~GameOver ( void );

    void Pause();
    void Resume();

    void HandleInput();
    void onExit();
    void onKeyDown ( SDLKey key, SDLMod mod );

    void Update();
    void Draw();

  private:
    Gfx *engine;
};

#endif // GAMEAPP_GAMEOVER_HEADERS defined
