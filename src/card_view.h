/******************************************************************************
    card_view.h

    Card graphics / drawing

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_VIEW_HEADERS
#define GAMEAPP_CARD_VIEW_HEADERS

#include <iostream>

#include "gamelib/gamelib.h"

#include "cfg.h"
#include "card.h"

class CardView
{
  public:
    CardView ( void );
    ~CardView ( void );

    bool EraseCard ( Gfx *engine, unsigned int x, unsigned int y );
    bool DrawCard ( Gfx *engine, Card &card, unsigned int x, unsigned int y );
    // bool flipCard ();
  private:
    Sprite *card_face;
    Sprite *card_background;
    Sprite *card_element;
    Font text_buffer;
    //unsigned int x;
    //unsigned int y;
    //unsigned int width = CARD_WIDTH;
    //unsigned int height = CARD_HEIGHT;
};

#endif // CARD_VIEW_HEADERS defined
