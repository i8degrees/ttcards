/******************************************************************************
    card_face.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef CARD_FACE_HEADERS
#define CARD_FACE_HEADERS

#include <iostream>

#include "SDL.h"

#include "cfg.h"

class CardFace
{
  public:
    CardFace ( void );
    ~CardFace ( void );
  // CardFace ( Card *card );
  // draw ( int x, int y, SDL_Surface *video_buffer );
  // bool flipCard ();
  private:
    int x;
    int y;
    int w = CARD_WIDTH;
    int h = CARD_HEIGHT;
    //Card *card;
    //Sprite card;
};

#endif // CARD_FACE_HEADERS defined
