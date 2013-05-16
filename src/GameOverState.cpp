/******************************************************************************
    GameOverState.cpp

  Game Over state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "GameOverState.h"

GameOver::GameOver ( Gfx *engine )
{
  #ifdef DEBUG_GAMEOVER_OBJ
    std::cout << "GameOver::GameOver (): Hello, world!" << "\n" << std::endl;
  #endif

  this->engine = engine;
}

GameOver::~GameOver ( void )
{
  #ifdef DEBUG_GAMEOVER_OBJ
    std::cout << "GameOver::~GameOver (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  this->engine = NULL;
}

void GameOver::Pause ( void )
{
  std::cout << "\n" << "GameOver state Paused" << "\n";
}

void GameOver::Resume ( void )
{
  std::cout << "\n" << "GameOver state Resumed" << "\n";
}

void GameOver::HandleInput ( void )
{
  SDLInput::Input();
}

void GameOver::onExit ( void )
{
  this->engine->Quit();
}

void GameOver::onKeyDown ( SDLKey key, SDLMod mod )
{
  switch ( key )
  {
    case SDLK_ESCAPE:
    case SDLK_q: this->engine->Quit(); break;
    // Reset / New Game State
    case SDLK_r: this->engine->PopStateThenChangeState ( std::unique_ptr<TTcards>( new TTcards ( this->engine) ) ); break;
     // Pause State
    case SDLK_p: this->engine->PopState (); break;

    default: break;
  }
}

void GameOver::Update ( void )
{
  this->engine->UpdateScreen ();
}

void GameOver::Draw ( void )
{
  // Stub
}
