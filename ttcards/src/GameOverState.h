/******************************************************************************

    Game Over state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_GAMEOVER_HEADERS
#define GAMEAPP_GAMEOVER_HEADERS

#include <iostream>
#include <string>

#include <gfx/SDL_Display.hpp>
#include <sys/SDL_Timer.hpp>
#include <nomlib_types.hpp>

#include "Game.hpp"
#include "card.h"
#include "card_debug.h"
#include "cfg.h"

class GameOver: public nom::IState
{
  public:
    GameOver ( std::vector<Card> cards_, unsigned int state );
    ~GameOver ( void );

    void onInit ( void );
    void onExit ( void );

    void Pause ( void );
    void Resume ( void );

    void Update ( float delta_time );
    void Draw ( void* video_buffer );

  private:
    void onKeyDown ( int32_t key, int32_t mod );

    /// our public / visible display context handle
    nom::SDL_Display context;
    /// card rendering
    CardView card;
    /// Board background image
    nom::SDL_Canvas background;
    // SDL_TFont gameOver_text; // dialog text
    std::vector<Card> cards;

    unsigned int state;
};

#endif // GAMEAPP_GAMEOVER_HEADERS defined
