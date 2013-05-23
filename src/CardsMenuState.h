/******************************************************************************
    CardsMenuState.h

  Cards Menu state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARDS_MENU_HEADERS
#define GAMEAPP_CARDS_MENU_HEADERS

#include <iostream>
#include <string>

#include "ttcards.h"

#define DEBUG_CARDS_MENU
#define DEBUG_CARDS_MENU_OBJ

class CardsMenu: public GameState
{
  public:
    CardsMenu ( Gfx *engine );
    ~CardsMenu ( void );

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

};

#endif // GAMEAPP_CARDS_MENU_HEADERS defined
