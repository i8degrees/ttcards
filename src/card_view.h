/******************************************************************************
    card_view.h

    Card graphics / drawing

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_CARD_VIEW_HEADERS
#define GAMEAPP_CARD_VIEW_HEADERS

#include <iostream>

#include "cfg.h"
#include "card.h"

#include "SDL_BitmapFont.h"
#include "sprite.h"

class CardView
{
  public:
    CardView ( void );
    ~CardView ( void );

    bool drawFaceDown ( Gfx *engine, unsigned int x, unsigned int y );
    bool DrawCard ( Gfx *engine, Card &card, unsigned int x, unsigned int y );

  private:
    nom::Sprite *card_face;
    nom::Sprite *card_background;
    nom::Sprite *card_element;
    SDLBitmapFont card_text;
};

#endif // CARD_VIEW_HEADERS defined
