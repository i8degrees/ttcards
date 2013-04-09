/******************************************************************************
    card_face.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef CARD_FACE_HEADERS
#define CARD_FACE_HEADERS

#include <iostream>

#include "SDL.h"

#include "cfg.h"
#include "card.h"
#include "card_hand.h"
#include "sprite.h"

//#define DEBUG_CARDFACE

class CardFace
{
  public:
    CardFace ( void );
    ~CardFace ( void );
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

#endif // CARD_FACE_HEADERS defined
