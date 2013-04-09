/******************************************************************************
    card_view.cpp

    Card graphics / drawing

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "card_view.h"

CardView::CardView ( void )
{
  #ifdef DEBUG_CARD_VIEW
    std::cout << "Hello, world! <From CardView::CardView>" << "\n" << std::endl;
  #endif
}

bool CardView::Init ( std::vector<Card> &card )
{
  return true;
}

CardView::~CardView ( void )
{
  #ifdef DEBUG_CARD_VIEW
    std::cout << "Goodbye cruel world! <From CardView::~CardView>" << "\n" << std::endl;
  #endif
}

/*
bool CardView::Draw ( void )
{
  // ...
}
*/
