/******************************************************************************
    GameOverState.h

  Game Over state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_GAMEOVER_HEADERS
#define GAMEAPP_GAMEOVER_HEADERS

#include <iostream>
#include <string>

#include "SDL_Timer.hpp"

#include "Game.hpp"
#include "SDL_Display.hpp"

class GameOver: public nom::IState
{
  public:
    GameOver ( std::vector<Card> cards_, unsigned int state );
    ~GameOver ( void );

    void onClose ( void );
    void Load ( void );

    void Pause ( void );
    void Resume ( void );

    void Update ( float elapsed_time );
    void Draw ( void* video_buffer );

  private:
    void onKeyDown ( int32_t key, int32_t mod );

    nom::SDL_Display context;   // our public / visible display context handle
    CardView card; /// card rendering
    nom::SDL_Canvas background;
    //SDL_TFont gameOver_text; // dialog text
    std::vector<Card> cards;

    unsigned int state;
};

#endif // GAMEAPP_GAMEOVER_HEADERS defined
