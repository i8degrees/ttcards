/******************************************************************************
    CardsMenuState.cpp

  Cards Menu state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "CardsMenuState.h"

CardsMenu::CardsMenu ( Gfx *engine )
{
  #ifdef DEBUG_CARDS_MENU_OBJ
    std::cout << "CardsMenu::CardsMenu (): Hello, world!" << "\n" << std::endl;
  #endif

  this->engine = engine;

  this->Load();
}

CardsMenu::~CardsMenu ( void )
{
  #ifdef DEBUG_CARDS_MENU_OBJ
    std::cout << "CardsMenu::~CardsMenu (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  this->engine = NULL;
}

void CardsMenu::Load ( void )
{
  // Stub
}

void CardsMenu::Pause ( void )
{
  std::cout << "\n" << "GameOver state Paused" << "\n";
}

void CardsMenu::Resume ( void )
{
  std::cout << "\n" << "GameOver state Resumed" << "\n";
}

void CardsMenu::HandleInput ( void )
{
  SDLInput::Input();
}

void CardsMenu::onExit ( void )
{
  this->engine->Quit();
}

void CardsMenu::onKeyDown ( SDLKey key, SDLMod mod )
{
  switch ( key )
  {
    case SDLK_ESCAPE:
    case SDLK_q: this->engine->Quit(); break;
    // Reset / New Game State
    case SDLK_r: this->engine->PopStateThenChangeState ( std::unique_ptr<TTcards>( new TTcards ( this->engine ) ) ); break;
     // Pause State
    //case SDLK_p: this->engine->PopState (); break;

    default: break;
  }
}

void CardsMenu::Update ( void )
{
  this->engine->UpdateScreen ();
}

void CardsMenu::Draw ( void )
{
  // Stub
}
