/******************************************************************************

    Game Over state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_GAMEOVER_HEADERS
#define GAMEAPP_GAMEOVER_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include "nomlib/graphics.hpp"
#include "nomlib/system.hpp"

#include "card.h"
#include "card_debug.h"
#include "card_view.h"
#include "CardsMenuState.h"
#include "GameObject.hpp"
#include "cfg.h"

class GameOver: public nom::IState
{
  public:
    GameOver ( std::shared_ptr<GameObject> object );
    ~GameOver ( void );

    void onInit ( void );
    void onExit ( void );

    void Pause ( void );
    void Resume ( void );

    void Update ( nom::uint32 delta_time );
    void Draw ( void* video_buffer );

  private:
    void onKeyDown ( int32_t key, int32_t mod );

    std::shared_ptr<GameObject> state;
    /// our public / visible display context handle
    nom::Display context;
    /// card rendering
    CardView card;
    /// Board background image
    nom::Canvas background;
    // TrueTypeFont gameOver_text; // dialog text
};

#endif // GAMEAPP_GAMEOVER_HEADERS defined
