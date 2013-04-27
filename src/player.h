/******************************************************************************
    player.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_PLAYER_HEADERS
#define GAMEAPP_PLAYER_HEADERS

#include <iostream>
#include <string>

#include "gamelib/gamelib.h"

#include "cfg.h"
#include "board.h"
#include "card_debug.h"
#include "card_hand.h"
#include "card_view.h"

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
  void Draw ( Gfx *engine );
  void DrawScore ( Gfx *engine, unsigned int x, unsigned int y );

private:
  Font text_score;
  Sprite left_cursor; // player1 cursor
  Sprite right_cursor; // player2 cursor
  CardDebug debug;
  CardView card;
  CardHand *hand;
  Board *board;
  unsigned int x;
  unsigned int y;
  unsigned int id; // unique identifier
  unsigned int state; // ...is it my turn yet?
  unsigned int score;
  signed int card_pos;

  Audio mixer1, mixer2; // Two audio mixing channels for playing sound effects
};

#endif // PLAYERS_HEADERS defined
