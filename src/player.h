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

class Player
{
public:
  Player ( void );
  ~Player ( void );

  void Init ( Board &board, CardHand &player_cards );

  SDL_Rect GetXY ( void );
  void SetXY ( unsigned int x, unsigned int y );

  unsigned int GetID ( void );
  void SetID ( unsigned int id );

  unsigned int GetState ( void );
  void SetState ( unsigned int state );

  // TODO: Consider branching this into Score class
  unsigned int GetScore ( void );
  void SetScore ( unsigned int score );

  void Input ( unsigned int type, SDLKey key, SDLMod mod );
  void Draw ( Gfx &engine );
  void DrawScore ( Gfx &engine, unsigned int x, unsigned int y );

private:
  Sprite left_cursor; // player1 cursor
  Sprite right_cursor; // player2 cursor
  CardDebug debug;
  CardView card;
  Font text_score;
  Board *board;
  CardHand *hand;
  unsigned int x;
  unsigned int y;
  unsigned int id; // unique identifier
  unsigned int state; // ...is it my turn yet?
  unsigned int score;
  signed int card_pos;

  Audio mixer1, mixer2; // Two audio mixing channels for playing sound effects
};

#endif // PLAYERS_HEADERS defined
