/******************************************************************************
    card_view.h

    Card graphics / drawing

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef CARD_VIEW_HEADERS
#define CARD_VIEW_HEADERS

#include <iostream>

#include "SDL.h"

#include "cfg.h"
#include "card.h"

#include "sprite.h"

#define DEBUG_CARD_VIEW

class CardView
{
  public:
    CardView ( void );
    ~CardView ( void );
    bool Init ( std::vector<Card> &card );
    bool Load ( void );
    //bool Draw ( SDL_Surface *video_buffer, int x, int y );
    // bool flipCard ();
  private:
    int x;
    int y;
    int w = CARD_WIDTH;
    int h = CARD_HEIGHT;
    //Sprite card;
};

#endif // CARD_VIEW_HEADERS defined
