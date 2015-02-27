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
#ifndef TTCARDS_HELPERS_HPP
#define TTCARDS_HELPERS_HPP

#include <nomlib/config.hpp>
#include <nomlib/math.hpp>
#include <nomlib/graphics.hpp>

#include "config.hpp"

// Forward declarations
class Card;
class CardResourceLoader;
class Board;
class CardHand;

namespace ttcards {

bool save_game(Board* game_board, CardHand* player_hand);
bool load_game(Board* game_board, CardHand* player_hand);

bool
render_card_background( nom::int32 player_id, const nom::Point2i& pos,
                        nom::Gradient* card_background,
                        nom::RenderTarget& target,
                        const nom::Texture* render_target = nullptr );

bool
render_card_face( nom::int32 face_id, const nom::Point2i& pos,
                  nom::SpriteBatch* card_face,
                  nom::RenderTarget& target,
                  const nom::Texture* render_target = nullptr );

bool render_card_element( nom::int32 element_id, const nom::Point2i& pos,
                          nom::SpriteBatch* card_element,
                          nom::RenderTarget& target,
                          const nom::Texture* render_target = nullptr );

bool
render_card_text( nom::int32 rank, const nom::Point2i& pos,
                  nom::Text* card_text, nom::RenderTarget& target,
                  const nom::Texture* render_target = nullptr );

bool
render_card(  const Card& card, const nom::Point2i& pos,
              const CardResourceLoader* res, nom::RenderTarget& target,
              const nom::Texture* render_target = nullptr );

/// \brief Mutate a player's hand (card ranks).
///
/// \fixme This method is not properly handling the position of a player's
/// hand when more than one card of the same ID is present.
///
/// \fixme This method breaks when we use ::set_position or ::selectCard; the
/// modified card's position is incorrect and crashes under Windows OS.
///
/// \todo Implement generic usage of this function (i.e.: board) -- perhaps
/// by breaking things into multiple functions, with the base version taking a
/// typical Card object ...
void
modify_card_rank( CardResourceLoader* card_res, CardHand* player_hand, bool modifier,
                  nom::uint32 direction );

void set_face_down(CardHand* player_hand, bool face_down);

void set_frame_interval(nom::uint32 interval);

std::string version_string();

} // namespace ttcards

#endif // include guard defined
