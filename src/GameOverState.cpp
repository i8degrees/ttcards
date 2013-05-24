/******************************************************************************
    GameOverState.cpp

  Game Over state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "GameOverState.h"

GameOver::GameOver ( Gfx *engine, unsigned int state )
{
  #ifdef DEBUG_GAMEOVER_OBJ
    std::cout << "GameOver::GameOver (): Hello, world!" << "\n" << std::endl;
  #endif

  this->engine = engine;
  this->state = state;

  this->Load();
}

GameOver::~GameOver ( void )
{
  #ifdef DEBUG_GAMEOVER_OBJ
    std::cout << "GameOver::~GameOver (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  if ( this->engine )
    this->engine = NULL;
}

void GameOver::Load ( void )
{
  this->gameOver_text.Load ( SCORE_FONTFACE, 36 ); // temp font
  this->gameOver_text.setTextColor ( 255, 255, 255 ); // color: red
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
  SDL_Event event;

  while ( SDL_PollEvent ( &event ) )
    SDLInput::Input ( &event );
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
    case SDLK_r: /*this->engine->PopState();*/ this->engine->ChangeState ( std::unique_ptr<CardsMenu>( new CardsMenu ( this->engine ) ) ); break;
     // Pause State
    //case SDLK_p: this->engine->PopState (); break;

    default: break;
  }
}

void GameOver::Update ( void )
{
  this->engine->UpdateScreen ();
}

void GameOver::Draw ( void )
{
  this->gameOver_text.setTextBuffer ( "Game Over" );
  signed int width = this->gameOver_text.getTextWidth ();
  this->gameOver_text.Draw ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT - 128 ) / 2 );

  if ( this->state == 1 )
  {
    this->gameOver_text.setTextBuffer ( "Player 1 wins!" );
    signed int width = this->gameOver_text.getTextWidth ();
    this->gameOver_text.Draw ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
  else if ( this->state == 2 )
  {
    this->gameOver_text.setTextBuffer ( "Player 2 wins!" );
    signed int width = this->gameOver_text.getTextWidth ();
    this->gameOver_text.Draw ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
  else if ( this->state == 3 )
  {
    this->gameOver_text.setTextBuffer ( "Tie!" );
    signed int width = this->gameOver_text.getTextWidth ();
    this->gameOver_text.Draw ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
}
