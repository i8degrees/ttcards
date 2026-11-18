/******************************************************************************

  TTcards - Triple Triad remake

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this
   list of conditions and the following disclaimer in the documentation and/or
   other materials provided with the distribution.

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
#ifndef TTCARDS_GAMESTATE_HPP
#define TTCARDS_GAMESTATE_HPP

#include <string>

#include <nomlib/system.hpp>

#include "Game.hpp"

namespace tt {

/// \brief Base class for game states.
///
/// Mirrors the AppState base class from the nomlib game controller example:
/// each concrete state holds a non-owning pointer back to Game and declares an
/// input mapper context key. The existing manual lifecycle management in
/// on_init/on_exit/on_pause/on_resume is preserved because the installed
/// nomlib StateMachine does not yet expose set_input_mapper()/input_context().
class GameState: public nom::IState
{
  public:
    GameState(nom::SDLApp* object, nom::uint32 id,
              const std::string& input_context) :
      nom::IState(id),
      game(NOM_SCAST(Game*, object)),
      input_context_(input_context)
    {
    }

    virtual ~GameState() = default;

    /// \brief Input mapper context key for this state.
    const std::string& input_context() const { return this->input_context_; }

  protected:
    Game* game;
    std::string input_context_;
};

} // namespace tt

#endif // include guard defined
