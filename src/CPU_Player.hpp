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
#ifndef GAMEAPP_CPU_PLAYER_HPP
#define GAMEAPP_CPU_PLAYER_HPP

#include <string>
#include <functional>

#include <nomlib/math.hpp>

#include "config.hpp"
#include "IPlayer.hpp"

// Forward declarations
class CardHand;
class Board;
class BoardTile;
class CardView;

class CPU_Player: public IPlayer
{
  public:
    enum Difficulty
    {
      Training = 0,
      Easy,
      Hard
    };

    typedef std::function<void(BoardTile&)> action_callback;

    virtual ~CPU_Player();

    CPU_Player( Difficulty difficulty, Board* board, CardHand* hand,
                CardView* view, const action_callback& action);

    virtual nom::uint32 player_id() const override;
    virtual void set_player_id(nom::uint32 id) override;

    /// \brief Calculate the best possible move for the CPU player followed by
    /// execution of said move with the specified action callback.
    ///
    /// \remarks The CPU player plays the following strategy, in the order of
    /// success (non-NULL result); 1. best_move; 2. edge_move; 3. random_move.
    virtual void update() override;

    virtual void draw(nom::IDrawable::RenderTarget& target) override;

  protected:
    /// \brief Random play.
    ///
    /// \returns BoardTile::null on failure to pick an un-used tile.
    virtual BoardTile random_move();

    /// \brief Defensive play.
    ///
    /// \returns BoardTile::null on failure to find an empty board edge.
    virtual BoardTile edge_move();

    /// \brief Calculate a winning play for the CPU player.
    ///
    /// \returns BoardTile::null on failure to find a winning move.
    ///
    /// \remarks Iterates through each board position with every card in its
    /// hand until a winning play is found (if any). This is entirely
    /// brute-force (has no true hindsight / strategy).
    virtual BoardTile best_move();

  private:
    /// \brief Not implemented; how challenging the CPU player is.
    Difficulty difficulty_;

    /// \brief The board to base calculations off of.
    ///
    /// \remarks This object pointer is **not** owned by us; do not free.
    Board* board_;

    /// \brief The player hand to base calculations off of.
    ///
    /// \remarks This object pointer is **not** owned by us; do not free.
    CardHand* hand_;

    /// \remarks This object pointer is **not** owned by us; do not free.
    CardView* card_renderer_;

    /// \brief The assigned function to perform when the CPU player is ready to
    /// execute its turn (place a card down).
    action_callback action_;

    /// \brief The unique identifier for the player.
    nom::uint32 player_id_;
};

#endif // CPU_PLAYERS_HEADERS defined
