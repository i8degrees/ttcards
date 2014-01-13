/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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

#include "config.hpp"
#include "resources.hpp"
#include "Card.hpp"
#include "GameObject.hpp"
#include "CardRules.hpp"
#include "GameOverStateCursor.hpp"

class GameOverState: public nom::IState
{
  public:
    GameOverState (
                    const std::shared_ptr<GameObject>& object,
                    enum GameOverType gameover_state
                  );

    ~GameOverState ( void );

  private:
    void onInit ( void );
    void onExit ( void );

    void Resume ( nom::int32 response );

    void onKeyDown ( nom::int32 key, nom::int32 mod, nom::uint32 window_id );
    void onMouseLeftButtonDown ( nom::int32 x, nom::int32 y, nom::uint32 window_id );
    void onMouseWheel ( nom::int32 x, nom::int32 y, nom::uint32 window_id );
    void onJoyButtonDown ( nom::int32 which, nom::int32 button );
    void onUserEvent ( nom::uint32 type, nom::int32 code, void* data1, void* data2 );

    void update ( float delta_time );
    void draw ( nom::IDrawable::RenderTarget target );

    std::shared_ptr<GameObject> game;

    nom::Timer transistion;
    bool show_results;
    enum GameOverType gameover_state;

    /// Interface cursor
    GameOverStateCursor cursor;

    Card selected_card;

    nom::MessageBox info_box;
    nom::MessageBox card_info_box;

    /// Position of player 1 hand
    nom::Coords player1_pos;

    /// Position of player 2 hand
    nom::Coords player2_pos;

    nom::EventDispatcher event;

    nom::Text info_text;
    nom::Text card_text;
};

// Convenience declarations for changing state
typedef std::unique_ptr<GameOverState> GameOverStatePtr;

#endif // GAMEAPP_GAMEOVER_HEADERS defined
