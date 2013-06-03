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

#include "timer.h"

class GameOver: public GameState
{
  public:
    GameOver ( Gfx *engine, std::vector<Card> cards_, unsigned int state );
    ~GameOver ( void );

    void Pause ( void );
    void Resume ( void );

    void HandleInput ( void );
    void Update ( void );
    void Draw ( SDL_Surface *video_buffer );

  private:
    void onExit ( void );
    void onKeyDown ( SDLKey key, SDLMod mod );
    void Load ( void );

    Gfx *engine; // Pointer reference to our rendering interface
    CardView card; // card rendering
    SDL_Surface *background;
    //SDL_TFont gameOver_text; // dialog text
    std::vector<Card> cards;

    unsigned int state;
};

#endif // GAMEAPP_GAMEOVER_HEADERS defined
