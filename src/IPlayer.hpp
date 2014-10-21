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
#ifndef GAMEAPP_IPLAYER_HPP
#define GAMEAPP_IPLAYER_HPP

#include <string>

#include <nomlib/math/Point2.hpp>

#include "config.hpp"

class IPlayer
{
  public:
    IPlayer();
    virtual ~IPlayer();

    const nom::Point2i& position() const;
    void set_position(const nom::Point2i& pos);

    virtual nom::uint32 player_id() const = 0;
    virtual void set_player_id(nom::uint32 id) = 0;

    nom::uint score() const;
    std::string score_string() const;
    void set_score(nom::uint score);

    virtual void update() = 0;
    virtual void draw(nom::IDrawable::RenderTarget& target) /*const*/ = 0;

  private:
    /// \brief Rendering coordinates of the player's hand
    nom::Point2i position_;

    /// \brief The player's score.
    nom::uint score_;
};

#endif // GAMEAPP_PLAYER_HEADERS defined
