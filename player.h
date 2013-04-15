/******************************************************************************
    player.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef PLAYER_HEADERS
#define PLAYER_HEADERS

#include <iostream>
#include <string>

#include "SDL/SDL.h"

#include "cfg.h"
#include "audio.h"
#include "board.h"
#include "card.h"
#include "card_debug.h"
#include "card_hand.h"
#include "card_view.h"

#include "font.h"
#include "sprite.h"
#include "timer.h"

#define DEBUG_PLAYER
#define DEBUG_PLAYER_OBJ

class Player: public CardHand
{
public:
  Player ( void );
  ~Player ( void );

  void Init ( void );

  SDL_Rect GetXY ( void );
  void SetXY ( unsigned int x, unsigned int y );

  unsigned int GetID ( void );
  void SetID ( unsigned int id );

  unsigned int GetState ( void );
  void SetState ( unsigned int state );

  // TODO: Consider branching this into Score class
  unsigned int GetScore ( void );
  void SetScore ( unsigned int score );

  bool CheckCollisions ( Board &board );

  void Input ( unsigned int type, SDLKey key, SDLMod mod, Board &board );
  void Draw ( Gfx &engine, Board &board );
  void DrawScore ( Gfx &engine, unsigned int x, unsigned int y );

private:
  Sprite left_cursor;
  Sprite right_cursor;
  CardDebug debug;
  CardView card;
  Font text_score;
  unsigned int x;
  unsigned int y;
  unsigned int id; // unique identifier
  unsigned int state; // ...is it my turn turn yet?
  unsigned int score;

  Audio mixer1, mixer2; // Two audio mixing channels for playing sound effects

  //Timer turn;
};

#endif // PLAYERS_HEADERS defined
