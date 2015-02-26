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
#include "CardResourceLoader.hpp"

// Forward declarations
#include "Card.hpp"
#include "GameConfig.hpp"

// Private headers
#include "helpers.hpp"

using namespace nom;
using namespace ttcards;

static std::unique_ptr<nom::Gradient>
init_card_background(const Color4iColors& colors)
{
  auto card_background =
    nom::make_unique<nom::Gradient>();
  if( card_background == nullptr ) {
    return nullptr;
  }

  card_background->set_colors(colors);

#if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
  card_background->set_size( Size2i(CARD_WIDTH-2, CARD_HEIGHT-2) );
  card_background->set_margins( Point2i(1,1) );
#else
  card_background->set_size( Size2i(CARD_WIDTH-6, CARD_HEIGHT-6) );
  card_background->set_margins( Point2i(3,2) );
#endif

  nom::Gradient::FillDirection fill_dir =
    nom::Gradient::FillDirection::Top;
  card_background->set_fill_direction(fill_dir);

  card_background->set_position(Point2i::zero);

  return std::move(card_background);
}

static std::unique_ptr<nom::SpriteBatch>
init_card_elements(const GameConfig* config)
{
  SpriteSheet frames;

  if( config == nullptr ) {
    return nullptr;
  }

  auto tex = std::make_shared<Texture>();
  if( tex == nullptr ) {
    return nullptr;
  }

  auto card_elements =
    nom::make_unique<nom::SpriteBatch>();
  if( card_elements == nullptr ) {
    return nullptr;
  }

#if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
  if( frames.load_file( config->get_string("CARD_ELEMENTS_ATLAS") ) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load sprite sheet:",
                  config->get_string("CARD_ELEMENTS_ATLAS") );
    return nullptr;
  }

  if( tex->load( config->get_string("CARD_ELEMENTS") ) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load sprite sheet:",
                  config->get_string("CARD_ELEMENTS") );
    return nullptr;
  }
#else
  if( frames.load_file( config->get_string("CARD_ELEMENTS_ATLAS_SCALE2X") ) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load sprite sheet:",
                  config->get_string("CARD_ELEMENTS_ATLAS_SCALE2X") );
    return nullptr;
  }

  if( tex->load( config->get_string("CARD_ELEMENTS_SCALE2X") ) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load sprite sheet:",
                  config->get_string("CARD_ELEMENTS_SCALE2X") );
    return nullptr;
  }
#endif

  card_elements->set_texture(tex);
  card_elements->set_sprite_sheet(frames);

  return std::move(card_elements);
}

static std::unique_ptr<nom::SpriteBatch>
init_card_faces(const GameConfig* config)
{
  SpriteSheet frames;

  if( config == nullptr ) {
    return nullptr;
  }

  auto tex = std::make_shared<Texture>();
  if( tex == nullptr ) {
    return nullptr;
  }

  auto card_faces =
    nom::make_unique<nom::SpriteBatch>();
  if( card_faces == nullptr ) {
    return nullptr;
  }

#if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
  if( frames.load_file( config->get_string("CARD_FACES_ATLAS") ) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load sprite sheet:",
                  config->get_string("CARD_FACES_ATLAS") );
    return nullptr;
  }

  if( tex->load( config->get_string("CARD_FACES") ) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,  "Could not load texture:",
                  config->get_string("CARD_FACES") );
    return nullptr;
  }
#else
  if( frames.load_file( config->get_string("CARD_FACES_ATLAS_SCALE2X") ) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load sprite sheet:",
                  config->get_string("CARD_FACES_ATLAS_SCALE2X") );
    return nullptr;
  }

  if( tex->load( config->get_string("CARD_FACES_SCALE2X") ) == false ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,  "Could not load texture:",
                  config->get_string("CARD_FACES_SCALE2X") );
    return nullptr;
  }
#endif

  card_faces->set_texture(tex);
  card_faces->set_sprite_sheet(frames);

  return std::move(card_faces);
}

static std::unique_ptr<nom::Text>
init_card_text(const Font& font)
{
  auto card_text =
    nom::make_unique<nom::Text>();
  if( card_text == nullptr ) {
    return nullptr;
  }

  card_text->set_font(font);

  return std::move(card_text);
}

bool CardResourceLoader::
load_file(const GameConfig* config, const nom::Font& card_font)
{
  if( config == nullptr ) {
    return false;
  }

  // Initialize rendering of the card backgrounds for each player; we cache
  // these individually to gain some performance in load time
  this->card_backgrounds_[Card::NO_PLAYER] =
    init_card_background(CARD_BG_NO_PLAYER);
  if( this->card_backgrounds_[Card::NO_PLAYER] == nullptr ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not initialize cards: failed to allocate memory for"
                  "a card background" );
    return false;
  }

  this->card_backgrounds_[Card::PLAYER1] =
    init_card_background(CARD_BG_PLAYER1);
  if( this->card_backgrounds_[Card::PLAYER1] == nullptr ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not initialize cards: failed to allocate memory for"
                  "a card background" );
    return false;
  }

  this->card_backgrounds_[Card::PLAYER2] =
    init_card_background(CARD_BG_PLAYER2);
  if( this->card_backgrounds_[Card::PLAYER2] == nullptr ) {
    NOM_LOG_ERR(  TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not initialize cards: failed to allocate memory for"
                  "a card background" );
    return false;
  }

  this->card_faces_ = init_card_faces(config);
  NOM_ASSERT(this->card_faces_ != nullptr);
  if( this->card_faces_ == nullptr ) {
    // TODO: logging
  }

  this->card_elements_ = init_card_elements(config);
  NOM_ASSERT(this->card_elements_ != nullptr);
  if( this->card_elements_ == nullptr ) {
    // TODO: logging
  }

  // Initialize text renderer for the cards
  this->card_text_ = init_card_text(card_font);
  NOM_ASSERT(this->card_text_ != nullptr);
  if( this->card_text_ == nullptr ) {
    // TODO: logging
  }

  return true;
}
