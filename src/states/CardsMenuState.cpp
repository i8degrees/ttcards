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
#include "CardsMenuState.hpp"

// Private headers
#include "helpers.hpp"
#include "CardRenderer.hpp"

// Forward declarations
#include "Game.hpp"

using namespace nom;

namespace tt {

CardsMenuState::CardsMenuState(nom::SDLApp* object) :
  nom::IState( Game::State::CardsMenu ),
  game( NOM_SCAST(Game*, object) )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

CardsMenuState::~CardsMenuState()
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

void CardsMenuState::on_init(nom::void_ptr data)
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->cursor_state_ = 0;

  auto& p1_hand = this->game->hand[PlayerIndex::PLAYER_1];
  auto& p2_hand = this->game->hand[PlayerIndex::PLAYER_2];

  p1_hand.clear();
  p2_hand.clear();

  // ...Initialize game rules...

  auto& rules = this->game->rules_;
  auto cfg = this->game->config_.get();
  this->game->init_game_rules(cfg, rules);

  this->game->hand[PlayerIndex::PLAYER_1].init(this->game->card_res_.get() );
  this->game->hand[PlayerIndex::PLAYER_2].init(this->game->card_res_.get() );

  auto p1_db = this->game->cards_db_[PlayerIndex::PLAYER_1].get();
  auto p2_db = this->game->cards_db_[PlayerIndex::PLAYER_2].get();
  NOM_ASSERT(p1_db != nullptr);
  NOM_ASSERT(p2_db != nullptr);

  for( auto itr = p2_db->begin(); itr != p2_db->end(); ++itr ) {

    if( this->game->debug_game_ == false ) {
      itr->face_down = true;
    } else {
      itr->face_down = false;
    }
  }

  while( p2_hand.size() < MAX_PLAYER_HAND ) {
    // p2_hand.shuffle(1, 10, this->game->collection);
    p2_hand.shuffle(1, 1, *p2_db);
  }

  // ...Initialize opponent's rendering position -- to the left of player 1...
  nom::size_type hand_idx = 0;
  Point2i p2_pos(Point2i::zero);
  for( auto card = p2_hand.begin(); card != p2_hand.end(); ++card ) {

    p2_pos.x = PLAYER2_ORIGIN_X;
    p2_pos.y = PLAYER2_ORIGIN_Y + (CARD_HEIGHT / 2) * hand_idx;
    ++hand_idx;

    auto card_renderer =
      card->card_renderer;
    if( card_renderer != nullptr && card_renderer->valid() == true ) {
      card_renderer->set_position(p2_pos);
    }
  }

  // ...Initialize the player's selected card rendering position -- to the
  // right of player 2...
  this->p1_selected_card_pos_.x =
    BOARD_ORIGIN_X + ( CARD_WIDTH * 2 );

  this->p1_selected_card_pos_.y =
    BOARD_ORIGIN_Y + ( (CARD_HEIGHT / 2) + CARD_HEIGHT * 1) - 8;

  // Reset the state of the cards model to ensure no persistent state; this
  // can lead to subtle bugs or worse if we don't do this
  this->game->cards_page_model_.reset( new CardsPageDataSource("cards_db") );

  if( this->game->cards_menu_.set_context(&this->game->gui_window_) == false ) {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could set GUI desktop." );
    exit(NOM_EXIT_FAILURE);
    // return false;
  }

  const auto GUI_CARDS_MENU =
    this->game->res_cfg_->get_string("GUI_CARDS_MENU");
  if( this->game->cards_menu_.load_document_file(GUI_CARDS_MENU) == false ) {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION,
                  "Could not load resource from file:",
                  GUI_CARDS_MENU );
    exit(NOM_EXIT_FAILURE);
    // return false;
  }

  // Create the card pages with the active player's card deck
  Cards deck;
  for( auto itr = p1_db->begin(); itr != p1_db->end(); ++itr ) {

    Card& pcard = *itr;

    // FIXME: This breaks card lookup, rendering, etc. due to the fact that up
    // to this point, we've been depending on the ID of the card matching the
    // array index.
    // Options I've thought of: experiment with the idea of modeling the data
    // with std::map<int,Card>, keeping a copy of the position index with the
    // card or model (yuck!) ...
    // if( (*itr).num() > 0 ) {
      deck.push_back(pcard);
    // }
  }

  // Card pages initializing...
  NOM_ASSERT( this->game->cards_page_model_ != nullptr );
  if( this->game->cards_page_model_ != nullptr ) {
    this->game->cards_page_model_->insert_cards(0, deck);
  }

  // Build card row data
  this->game->gui_window_.update();

  NOM_LOG_DEBUG( TTCARDS_LOG_CATEGORY_CARDS_MENU_STATE,
                  "---Cards page model---",
                  this->game->cards_page_model_->dump() );

  // Event listener for mouse button clicks

  // TODO: This is currently a leaked resource -- no body is freeing it, and it
  // is our responsibility! I'm not sure whose responsibility it should be,
  // though -- should it be nom::UIEventListener, nom::UIWidget or
  // application-level..?
  this->game->cards_menu_.register_event_listener(
    this->game->cards_menu_.document(), "mouseup",
    new nom::UIEventListener( [&] ( Rocket::Core::Event& ev ) {
      this->on_mouse_button_up(ev);
    })
  );

  // Build offset coordinate map for the game cursor; this is necessary for
  // syncing key, mouse wheel and joystick input.
  Rocket::Core::ElementList tags;
  Rocket::Core::Element* top =
    this->game->cards_menu_.document()->GetElementById("content");

  NOM_ASSERT(top != nullptr);
  if( top != nullptr ) {
    Rocket::Core::ElementUtilities::GetElementsByTagName(tags, top, "datagridrow");

    // NOM_DUMP( tags.size() );
    Rocket::Core::Vector2f position;
    Rocket::Core::Vector2f size;
    IntRect row;
    for(auto itr = tags.begin(); itr != tags.end(); ++itr)
    {
      // NOM_DUMP( (*itr)->GetTagName().CString() );

      position = (*itr)->GetAbsoluteOffset(Rocket::Core::Box::PADDING);
      size = (*itr)->GetBox().GetSize(Rocket::Core::Box::PADDING);

      row.x = position.x - this->game->cursor_->size().w;
      row.y = position.y + this->game->cursor_->size().h / 2;
      row.w = size.x;

      // This value correlates with the line-height RCSS property value in
      // gui/dataview.rml and must **not** be fractional, or else we accumulate
      // rounding err with each cursor offset row that results in mis-placement
      // of the cursor. This can ultimately break its intended Y-axis bounds.
      row.h = size.y;

      this->cursor_coords_map_.push_back(row);

      NOM_DUMP_VAR(TTCARDS_LOG_CATEGORY_TEST, "cursor_coords_map_:", row);

      // Check the value of row.h to ensure non-fractional
      NOM_DUMP_VAR(TTCARDS_LOG_CATEGORY_TEST, "datagridrow:", position.x, ",", position.y, ",", size.x, ",", size.y );
    }
  }

  // Starting origin for game cursor
  this->game->cursor_->set_position( this->cursor_coords_map_[0].position() );
  this->game->cursor_->set_sprite_sheet(this->game->right_cursor_frames_);
  this->game->cursor_->set_frame(INTERFACE_CURSOR_SHOWN);

  nom::InputActionMapper state;

  auto cursor_prev( [=](const nom::Event& evt) {
    this->cursor_prev();
  });

  auto cursor_next( [=](const nom::Event& evt) {
    this->cursor_next();
  });

  auto prev_page( [=](const nom::Event& evt) {
    this->prev_page();
  });

  auto next_page( [=](const nom::Event& evt) {
    this->next_page();
  });

  auto delete_card( [=](const nom::Event& evt) {
    this->remove_card(this->p1_selected_card_);
  });

  auto select_card( [=](const nom::Event& evt) {
    this->add_card(this->p1_selected_card_);
  });

  auto start_game( [=](const nom::Event& evt) {
    this->game->set_state(Game::State::ConfirmationDialog);
  });

  // ...Keyboard input mappings...

  state.insert("cursor_prev", nom::KeyboardAction(SDLK_UP), cursor_prev);
  state.insert("cursor_next", nom::KeyboardAction(SDLK_DOWN), cursor_next);
  state.insert("prev_page", nom::KeyboardAction(SDLK_LEFT), prev_page);
  state.insert("next_page", nom::KeyboardAction(SDLK_RIGHT), next_page);
  state.insert("delete_card", nom::KeyboardAction(SDLK_d), delete_card);
  state.insert("select_card", nom::KeyboardAction(SDLK_SPACE), select_card);
  state.insert("start_game", nom::KeyboardAction(SDLK_RETURN), start_game);

  // ...Mouse wheel input mappings...

  state.insert( "cursor_prev",
                nom::MouseWheelAction(nom::MOUSE_WHEEL_UP), cursor_prev );
  state.insert( "cursor_next",
                nom::MouseWheelAction(nom::MOUSE_WHEEL_DOWN), cursor_next );
  state.insert( "prev_page",
                nom::MouseWheelAction(nom::MOUSE_WHEEL_LEFT), prev_page );
  state.insert( "next_page",
                nom::MouseWheelAction(nom::MOUSE_WHEEL_RIGHT), next_page );

  // ...Joystick input mappings...
  auto& joystick_id = this->game->joystick_id_;

  state.insert( "cursor_prev",
                nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_DPAD_UP), cursor_prev );

  state.insert( "cursor_next", nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_DPAD_DOWN), cursor_next );

  state.insert( "prev_page", nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_DPAD_LEFT), prev_page );

  state.insert( "next_page", nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_DPAD_RIGHT), next_page );

  state.insert( "delete_card", nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_B), delete_card );

  state.insert( "select_card", nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_A), select_card );

  state.insert( "start_game", nom::GameControllerButtonAction(joystick_id,
                nom::GameController::BUTTON_START), start_game );

  this->game->input_mapper.erase( "CardsMenuState" );
  this->game->input_mapper.insert( "CardsMenuState", state, true );
  this->game->input_mapper.activate_only( "CardsMenuState" );
  this->game->input_mapper.activate( "Game" );

  this->game->cards_menu_.show();

  // Yeah buddy!
  this->game->theme_track_->Play();

  // Set the default card selection for the player
  this->p1_selected_card_ =
    this->game->cards_db_[PlayerIndex::PLAYER_1]->front();

  this->p1_selected_card_sprite_ =
    this->set_player_selected_card_position(  this->p1_selected_card_,
                                              this->p1_selected_card_pos_ );
}

void CardsMenuState::on_exit(nom::void_ptr data)
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->game->cards_menu_.close();
  Rocket::Core::Factory::ClearStyleSheetCache();
  Rocket::Core::Factory::ClearTemplateCache();

  this->game->actions_.cancel_actions();
}

void CardsMenuState::on_pause(nom::void_ptr data)
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  // Hide the cursor so that it doesn't show up during undesirable states such
  // as during the ConfirmationDialogState or Pause states.
  this->game->cursor_->set_frame(INTERFACE_CURSOR_HIDDEN);
}

void CardsMenuState::on_resume(nom::void_ptr data)
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  // Restore the rendering of the game cursor
  this->game->cursor_->set_frame(INTERFACE_CURSOR_SHOWN);

  // Response from the previous game state
  nom::int32_ptr response = static_cast<nom::int32_ptr>(data);

  // User is happy with their cards -- let's play! Response from
  // ConfirmationDialogState was 'yes'
  if( response != nullptr && *response == 1 ) {
    NOM_DELETE_PTR(response);
    this->game->set_state(Game::State::Play);
  }
  else {
    // Continue running this state; response from ConfirmationDialogState was 'no'
    this->game->input_mapper.activate_only("CardsMenuState");
    this->game->input_mapper.activate("Game");
  }
}

void CardsMenuState::on_update(nom::real32 delta_time)
{
  this->update_cursor();

  this->game->window.update();
  this->game->gui_window_.update();

  this->update_page_indicators();
}

void CardsMenuState::on_draw(nom::RenderWindow& target)
{
  this->game->background.draw(target);

  // Player 2 is to the left
  tt::render_player_hand(target, &this->game->hand[PlayerIndex::PLAYER_2]);

  // Player 1 is to the right
  tt::render_player_hand(target, &this->game->hand[PlayerIndex::PLAYER_1]);

  this->game->gui_window_.draw();

  TT_RENDER_SPRITE(this->game->cursor_);
  TT_RENDER_SPRITE(this->p1_selected_card_sprite_);
}

// Private scope

bool CardsMenuState::on_event(const nom::Event& ev)
{
  return false;
}

void CardsMenuState::on_mouse_button_up(Rocket::Core::Event& ev)
{
  using namespace Rocket::Core;

  Element* target = ev.GetTargetElement();

  NOM_ASSERT( target != nullptr );
  if( target == nullptr ) return;

  if( ev == "mouseup" ) {

    Input::KeyIdentifier button =
      (Input::KeyIdentifier) ev.GetParameter<int>("button", 3);

    NOM_ASSERT( this->game->cards_page_model_ != nullptr );
    if( this->game->cards_page_model_ == nullptr ) return;

    Card card = this->game->cards_page_model_->lookup_by_name( target->GetInnerRML().CString() );

    if( target->GetTagName() == "card" && button == 0 ) // Left click
    {
      this->add_card(card);
    }
    else if( target->GetTagName() == "card" && button == 1 )  // Right click
    {
      this->remove_card(card);
    }

    NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_TEST, target->GetInnerRML().CString() );
  } // end if mouseup event
}

// void CardsMenuState::on_left_mouse_button_up(const nom::Event& ev)
// {
//   using namespace Rocket::Core;
//   Vector2f mouse_coords(ev.mouse.x, ev.mouse.y);

//   Element* target =
//     this->game->cards_menu_.document()->GetElementById("datagridrow");
//   NOM_ASSERT( target != nullptr );
//   if( target == nullptr ) return;

//   NOM_ASSERT( this->game->cards_page_model_ != nullptr );
//   if( this->game->cards_page_model_ == nullptr ) return;

//   Card card =
//     this->game->cards_page_model_->lookup_by_name( target->GetInnerRML().CString() );

//   if( target->IsPointWithinElement(mouse_coords) ) {
//       this->add_card(card);
//   } // end if target is within mouse hit test

//   NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_TEST, target->GetInnerRML().CString() );
// }

// void CardsMenuState::on_right_mouse_button_up(const nom::Event& ev)
// {
//   using namespace Rocket::Core;
//   Vector2f mouse_coords(ev.mouse.x, ev.mouse.y);

//   Element* target =
//     this->game->cards_menu_.document()->GetElementById("datagridrow");
//   NOM_ASSERT( target != nullptr );
//   if( target == nullptr ) return;

//   NOM_ASSERT( this->game->cards_page_model_ != nullptr );
//   if( this->game->cards_page_model_ == nullptr ) return;

//   Card card =
//     this->game->cards_page_model_->lookup_by_name( target->GetInnerRML().CString() );

//   if( target->IsPointWithinElement(mouse_coords) ) {
//       this->remove_card(card);
//   } // end if target is within mouse hit test

//   NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_TEST, target->GetInnerRML().CString() );
// }

void CardsMenuState::update_cursor()
{
  int row_index = 0;      // Card position index as per cards model
  int pos = 0;            // The card's position index (as per CardCollection)

  if( this->cursor_state_ == 0 ) {
    this->game->cursor_->set_frame(INTERFACE_CURSOR_SHOWN);

    // Update selected card
    row_index = this->cursor_position();
    pos = this->game->cards_page_model_->map_row(row_index);

    this->p1_selected_card_ =
      this->game->cards_page_model_->lookup_by_id(pos);
  }
}

void CardsMenuState::update_page_indicators()
{
  using namespace Rocket::Core;

  int page = 0;         // The current card page
  int num_pages = 0;  // Total card pages
  Element* page_left = nullptr;
  Element* page_right = nullptr;

  NOM_ASSERT( this->game->cards_page_model_ != nullptr );
  if( this->game->cards_page_model_ == nullptr ) {
    return;
  }

  page = this->game->cards_page_model_->page();
  num_pages = this->game->cards_page_model_->total_pages();

  // Update left page indicator
  page_left =
    this->game->cards_menu_.document()->GetElementById("page-left");
  NOM_ASSERT( page_left != nullptr );

  if( page_left ) {
    page_left->SetClassNames("");

    if( page > 0 ) {
      page_left->SetClassNames("page-left");
    }
  } // end if page_left

  // Update right page indicator
  page_right =
    this->game->cards_menu_.document()->GetElementById("page-right");
  NOM_ASSERT( page_right != nullptr );

  if( page_right ) {
    page_right->SetClassNames("");

    if( page >= 0 && (page < num_pages - 1) ) {
      page_right->SetClassNames("page-right");
    }
  } // end if page_right
}

int CardsMenuState::cursor_position()
{
  NOM_ASSERT( this->game->cards_page_model_ != nullptr );

  int pos = 0;

  if( this->game->cards_page_model_ != nullptr ) {

    for(auto idx = 0; idx < this->game->cards_page_model_->per_page(); ++idx)
    {
      if( this->game->cursor_->position().y <= this->cursor_coords_map_.at(idx).y ) {
        return idx;
      }
      else { // catch all safety switch

        // assume we are at the last position in the index when all else fails
        pos = this->game->cards_page_model_->per_page();
      }
    }
  }

  return pos;
}

void CardsMenuState::set_cursor_position(int pos)
{
  this->game->cursor_->set_position( this->cursor_coords_map_.at(pos).position() );
  NOM_DUMP_VAR(TTCARDS_LOG_CATEGORY_TEST, "cursor_pos:", this->game->cursor_->position() );
}

void CardsMenuState::prev_page()
{
  using namespace Rocket::Core;

  int page = 0;

  if( this->cursor_state_ == 0 ) {

    NOM_ASSERT( this->game->cards_page_model_ != nullptr );
    if( this->game->cards_page_model_ == nullptr ) {
      return;
    }

    page = this->game->cards_page_model_->page();

    // Update cards model, play audio & set column text for previous page
    if( page > 0 ) {

      --page;
      this->game->cards_page_model_->set_page(page);

      auto row_index = this->cursor_position();
      auto pos = this->game->cards_page_model_->map_row(row_index);

      Card card_sp =
        this->game->cards_page_model_->lookup_by_id(pos);

      this->p1_selected_card_sprite_ =
        this->set_player_selected_card_position(  card_sp,
                                                  this->p1_selected_card_pos_ );

      this->game->cursor_move->Play();

      // Update column with page number
      this->game->cards_menu_.set_column_title( 1, "CARDS P. " + std::to_string(page+1) );
    }
  } // end if cursor state == 0
}

void CardsMenuState::next_page()
{
  using namespace Rocket::Core;

  int page = 0;
  int num_pages = 0;

  if( this->cursor_state_ == 0 ) {

    NOM_ASSERT( this->game->cards_page_model_ != nullptr );
    if( this->game->cards_page_model_ == nullptr ) {
      return;
    }

    page = this->game->cards_page_model_->page();
    num_pages = this->game->cards_page_model_->total_pages();

    // Update cards model, play audio & set column text for next page
    if( page < num_pages - 1 ) {

      ++page;
      this->game->cards_page_model_->set_page(page);

      auto row_index = this->cursor_position();
      auto pos = this->game->cards_page_model_->map_row(row_index);

      Card card_sp =
        this->game->cards_page_model_->lookup_by_id(pos);

      this->p1_selected_card_sprite_ =
        this->set_player_selected_card_position(  card_sp,
                                                  this->p1_selected_card_pos_ );

      this->game->cursor_move->Play();

      // Update column with page number
      this->game->cards_menu_.set_column_title( 1, "CARDS P. " + std::to_string(page+1) );
    }

  } // end if cursor state == 0
}

void CardsMenuState::cursor_prev()
{
  int row_index = 0;      // Card position index as per cards model
  int pos = 0;            // The card's position index (as per CardCollection)
  Point2i move_to_offset(Point2i::zero);

  if( this->cursor_state_ == 0 ) {

    // Move up if the game cursor is before the first card entry of the page
    if( this->game->cursor_->position().y > this->cursor_coords_map_.at(0).y ) {

      move_to_offset.x = 0;
      move_to_offset.y = -(this->cursor_coords_map_.at(pos).h);
      this->game->cursor_->translate(move_to_offset);

      NOM_DUMP_VAR( TTCARDS_LOG_CATEGORY_TEST, "cursor.y:",
                    this->game->cursor_->position().y );

      row_index = this->cursor_position();
      pos = this->game->cards_page_model_->map_row(row_index);

      Card card_sp =
        this->game->cards_page_model_->lookup_by_id(pos);

      this->p1_selected_card_sprite_ =
        this->set_player_selected_card_position(  card_sp,
                                                  this->p1_selected_card_pos_ );

      this->game->cursor_move->Play();
    }
  } // end if cursor state == 0
}

void CardsMenuState::cursor_next()
{
  int row_index = 0;      // Card position index as per cards model
  int pos = 0;            // The card's position index (as per CardCollection)
  int max_per_page = 0;   // maximum number of cards per one page
  Point2i move_to_offset(Point2i::zero);

  NOM_ASSERT( this->game->cards_page_model_ != nullptr );
  if( this->game->cards_page_model_ != nullptr ) {
    // Next to last card
    max_per_page = this->game->cards_page_model_->per_page() - 1;
    NOM_DUMP_VAR(TTCARDS_LOG_CATEGORY_TEST, "cursor.at(max_per_page).y:", this->cursor_coords_map_.at(max_per_page).y );
  }

  if( this->cursor_state_ == 0 ) {

    // Move down if the game cursor is not at the last card entry of the page
    if( this->game->cursor_->position().y >= this->cursor_coords_map_.at(0).y &&
        this->game->cursor_->position().y < this->cursor_coords_map_.at(max_per_page).y )
    {
      move_to_offset.x = 0;
      move_to_offset.y = this->cursor_coords_map_.at(pos).h;
      this->game->cursor_->translate(move_to_offset);

      NOM_DUMP_VAR(TTCARDS_LOG_CATEGORY_TEST, "cursor.y:", this->game->cursor_->position().y );

      row_index = this->cursor_position();
      pos = this->game->cards_page_model_->map_row(row_index);

      Card card_sp =
        this->game->cards_page_model_->lookup_by_id(pos);

      this->p1_selected_card_sprite_ =
        this->set_player_selected_card_position(  card_sp,
                                                  this->p1_selected_card_pos_ );

      this->game->cursor_move->Play();
    }
  } // end if cursor state == 0
}

void CardsMenuState::add_card(const Card& card)
{
  using namespace Rocket::Core;

  int pos = 0;            // The card's position index (as per CardCollection)
  int page = 0;           // Current page of model
  int row_index = 0;      // Element's position of the card (relative to page)

  auto& p1_hand = this->game->hand[PlayerIndex::PLAYER_1];
  Point2i p1_card_pos(Point2i::zero);

  NOM_ASSERT( this->game->cards_page_model_ != nullptr );
  if( this->game->cards_page_model_ == nullptr ) {
    return;
  }

  page = this->game->cards_page_model_->page();
  Card c = card;
  pos = c.id;

  // Card logic for adding a card to the player's hand
  //
  // 1. Decrease available card count by one
  // 2. Sync the cards model to reflect modified card count (-1)
  // 3. Update player hand w/ modified card.
  // 4. Update game cursor position.
  // 5. Queue audio clip
  if( c.num > 0 ) {

    c.num = (c.num - 1);

    if( p1_hand.push_back(c) == true ) {

      this->game->cards_page_model_->insert_card(pos, c);

      tt::update_card_layout(&p1_hand, PLAYER1_ORIGIN);

      // Get the position, relative to current page, from model for
      // updating game cursor position
      row_index =
        this->game->cards_page_model_->map_page_row(pos, page);
      this->set_cursor_position(row_index);

      this->game->card_place->Play();
    } // end if successful add
  }
}

void CardsMenuState::remove_card(const Card& card)
{
  using namespace Rocket::Core;

  int pos = 0;            // The card's position index (as per CardCollection)
  int page = 0;           // Current page of model
  int row_index = 0;      // Element's position of the card (relative to page)

  auto& p1_hand = this->game->hand[PlayerIndex::PLAYER_1];
  Point2i p1_card_pos(Point2i::zero);

  NOM_ASSERT( this->game->cards_page_model_ != nullptr );
  if( this->game->cards_page_model_ == nullptr ) {
    return;
  }

  page = this->game->cards_page_model_->page();
  Card c = card;
  pos = c.id;

  // Use the opponent's card deck to compare total number of cards (to return)
  //
  // TODO: This is stub code and needs to be replaced with an actual deck
  // query that is independent of the opponent's total number of cards count
  Card ref_card =
    this->game->cards_db_[PlayerIndex::PLAYER_2]->find(pos);

  // Card logic for removing a card from the player's hand
  //
  // 1. Increase available card count by one
  // 2. Sync the cards model to reflect modified card count (+1)
  // 3. Remove the card from the player's hand.
  // 4. Update game cursor position.
  // 5. Queue audio clip
  if( c.num < ref_card.num ) {

    c.num = (c.num + 1);

    if( p1_hand.erase(c) == true ) {

      this->game->cards_page_model_->insert_card(pos, c);

      tt::update_card_layout(&p1_hand, PLAYER1_ORIGIN);

      // Get the position, relative to current page, from model for
      // updating game cursor position
      row_index =
        this->game->cards_page_model_->map_page_row(pos, page);
      this->set_cursor_position(row_index);

      this->game->cursor_cancel->Play();
    } // end if successful removal
  }
}

std::unique_ptr<nom::Sprite>
CardsMenuState::set_player_selected_card_position(  const Card& card,
                                                    const nom::Point2i& pos )
{
  auto p1_card_renderer =
    tt::create_card_renderer(this->game->card_res_.get(), card);

  NOM_ASSERT(p1_card_renderer != nullptr);
  NOM_ASSERT(p1_card_renderer->valid() == true);

  if( p1_card_renderer != nullptr && p1_card_renderer->valid() == true ) {
    auto card_sp =
      p1_card_renderer->rendered_card();

    card_sp->set_position(pos);

    // Success!
    return card_sp;
  }

  // Err
  return nullptr;
}

} // namespace tt
