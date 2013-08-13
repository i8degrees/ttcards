/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef GAMEAPP_GAMEOVER_HEADERS
#define GAMEAPP_GAMEOVER_HEADERS

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

#include "Card.hpp"
#include "CardDebug.hpp"
#include "CardsMenuState.hpp"
#include "GameObject.hpp"
#include "PauseState.hpp"
#include "config.hpp"

class GameOverState: public nom::IState
{
  public:
    GameOverState ( std::shared_ptr<GameObject> object, nom::uint32 gameover_state );
    ~GameOverState ( void );
  private:
    void onInit ( void );
    void onExit ( void );

    void Pause ( void );
    void Resume ( void );

    void Update ( nom::uint32 delta_time );
    void Draw ( void* video_buffer );
    void onKeyDown ( nom::int32 key, nom::int32 mod );

    std::shared_ptr<GameObject> game;
    std::vector<Card> player_cards[2];
    CardDebug debug;
    nom::Timer update;
    bool show_results;
    nom::uint32 gameover_state;
};

// Convenience declarations for changing state
typedef std::unique_ptr<GameOverState> GameOverStatePtr;

#endif // GAMEAPP_GAMEOVER_HEADERS defined
