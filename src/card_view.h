/******************************************************************************
    card_view.h

    Card graphics / drawing

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_VIEW_HEADERS
#define GAMEAPP_CARD_VIEW_HEADERS

#include <iostream>

#include "gamelib.h"

#include "cfg.h"
#include "card.h"

class CardView
{
  public:
    CardView ( void );
    ~CardView ( void );

    void DrawName ( Gfx *engine, Card &card, unsigned int y );
    bool DrawCard ( Gfx *engine, Card &card, unsigned int x, unsigned int y );
    // bool flipCard ();
  private:
    Sprite *card_face;
    Sprite *card_background;
    Sprite *card_element;
    Font text_buffer;
};

#endif // CARD_VIEW_HEADERS defined
