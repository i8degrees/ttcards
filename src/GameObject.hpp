/******************************************************************************

    GameObject

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef GAMEAPP_GAMEOBJECT_HPP
#define GAMEAPP_GAMEOBJECT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "cfg.h"
#include "board.h"
#include "card.h"
#include "card_debug.h"
#include "card_hand.h"
#include "card_view.h"
#include "card_collection.h"

struct GameObject
{
  Collection collection;
};

#endif // include guard defined
