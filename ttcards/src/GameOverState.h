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
    GameOver ( std::vector<Card> cards_, unsigned int state );
    ~GameOver ( void );

    void Pause ( void );
    void Resume ( void );

    void Update ( void );
    void Draw ( void *video_buffer );

  private:
    void onKeyDown ( int32_t key, int32_t mod );
    void Load ( void );

    CardView card; // card rendering
    SDL_Surface *background;
    //SDL_TFont gameOver_text; // dialog text
    std::vector<Card> cards;

    unsigned int state;
};

#endif // GAMEAPP_GAMEOVER_HEADERS defined
