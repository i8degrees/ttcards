/******************************************************************************
    GameOverState.cpp

  Game Over state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "GameOverState.h"

using namespace nom;

GameOver::GameOver ( std::vector<Card> cards_, unsigned int state )
{
  #ifdef DEBUG_GAMEOVER_OBJ
    std::cout << "GameOver::GameOver (): Hello, world!" << "\n" << std::endl;
  #endif

  this->state = state;
  this->cards = cards_; // FIXME?

  for ( int i = 0; i < cards.size(); i++ )
    std::cout << cards[i].getID() << " " << cards[i].getName() << " " << cards[i].getPlayerOwner() << " " << std::endl;
}

GameOver::~GameOver ( void )
{
  #ifdef DEBUG_GAMEOVER_OBJ
    std::cout << "GameOver::~GameOver (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif
}

void GameOver::onInit ( void )
{
  this->background.loadFromImage ( GAMEOVER_BACKGROUND, nom::Color ( nom::Color::Black ), 0 );

  //this->gameOver_text.Load ( SCORE_FONTFACE, 36 ); // temp font
  //this->gameOver_text.setTextColor ( 255, 255, 255 ); // color: red
}

void GameOver::onClose ( void )
{
  std::cout << "\n" << "GameOver state onClose" << "\n";
}

void GameOver::Pause ( void )
{
  std::cout << "\n" << "GameOver state Paused" << "\n";
}

void GameOver::Resume ( void )
{
  std::cout << "\n" << "GameOver state Resumed" << "\n";
}

void GameOver::onKeyDown ( int32_t key, int32_t mod )
{
  switch ( key )
  {
    // Reset / New Game State
    case SDLK_r: nom::GameStates::ChangeState ( std::unique_ptr<CardsMenu>( new CardsMenu() ) ); break;
     // Pause State
    //case SDLK_p: this->engine->PopState (); break;

    default: break;
  }
}

void GameOver::Update ( float delta_time )
{
  this->context.Update();
}

void GameOver::Draw ( void* video_buffer )
{
  this->background.Draw ( video_buffer ); // draw static board background

  //this->gameOver_text.setTextBuffer ( "Game Over" );
  //signed int width = this->gameOver_text.getTextWidth ();
  //this->gameOver_text.Draw ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT - 128 ) / 2 );

  // Active player's card selection(s)
  for ( int cards_index = 0; cards_index < this->cards.size(); cards_index++ ) // TODO: std::get<1>(player_coords)
  {
    if ( this->cards.at ( cards_index ).getPlayerOwner() == Card::PLAYER1 )
      this->card.DrawCard ( video_buffer, this->cards.at ( cards_index ), PLAYER1_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * cards_index, PLAYER1_GAMEOVER_ORIGIN_Y );
    else if ( this->cards.at ( cards_index ).getPlayerOwner() == Card::PLAYER2 )
      this->card.DrawCard ( video_buffer, this->cards.at ( cards_index ), PLAYER2_GAMEOVER_ORIGIN_X + ( CARD_WIDTH ) * cards_index, PLAYER2_GAMEOVER_ORIGIN_Y );
  }

  if ( this->state == 1 )
  {
    //this->gameOver_text.setTextBuffer ( "Player 1 wins!" );
    //signed int width = this->gameOver_text.getTextWidth ();
    //this->gameOver_text.Draw ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
  else if ( this->state == 2 )
  {
    //this->gameOver_text.setTextBuffer ( "Player 2 wins!" );
    //signed int width = this->gameOver_text.getTextWidth ();
    //this->gameOver_text.Draw ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
  else if ( this->state == 3 )
  {
    //this->gameOver_text.setTextBuffer ( "Tie!" );
    //signed int width = this->gameOver_text.getTextWidth ();
    //this->gameOver_text.Draw ( this->engine, ( SCREEN_WIDTH - width ) / 2, ( SCREEN_HEIGHT ) / 2 );
  }
}
