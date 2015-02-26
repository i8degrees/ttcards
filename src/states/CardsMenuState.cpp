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
#include "CardRenderer.hpp"

// Forward declarations
#include "Game.hpp"

using namespace nom;

CardsMenuState::CardsMenuState(nom::SDLApp* object) :
  nom::IState( Game::State::CardsMenu ),
  game( NOM_SCAST(Game*, object) )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );
}

CardsMenuState::~CardsMenuState()
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->selected_card_ = Card();
}

void CardsMenuState::on_init( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->cursor_state_ = 0;

  this->selected_card_pos_.x =
    BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ),

  this->selected_card_pos_.y =
    BOARD_ORIGIN_Y + ( (CARD_HEIGHT / 2) + CARD_HEIGHT * 1) - 8;

  this->game->hand[PLAYER1].clear();
  this->game->hand[PLAYER2].clear();

  // Default rule set
  this->game->rules.setRules(0);

  // TODO: Finish implementing CardRules interface -- bitwise flags!
  bool open_ruleset = false;
  bool combo_ruleset = false;
  bool same_ruleset = false;
  nom::StringList ruleset = this->game->config.string_array("REGION_RULESET");
  for( auto itr = ruleset.begin(); itr != ruleset.end(); ++itr ) {

    if( (*itr) == "Open" ) {
      open_ruleset = true;
    }

    if( (*itr) == "Combo" ) {
      combo_ruleset = true;
    }

    if( (*itr) == "Same" ) {
      same_ruleset = true;
    }
  }

  if( combo_ruleset == true ) {
    this->game->rules.setRules(CardRules::Combo);
  }

  if( same_ruleset == true ) {
    this->game->rules.setRules(CardRules::Same);
  }

  this->game->hand[PLAYER1].init(this->game->card_res_.get() );
  this->game->hand[PLAYER2].init(this->game->card_res_.get() );

  auto p1_db = this->game->cards_db_[PLAYER1].get();
  auto p2_db = this->game->cards_db_[PLAYER2].get();
  NOM_ASSERT(p1_db != nullptr);
  NOM_ASSERT(p2_db != nullptr);

  for( auto itr = p2_db->begin(); itr != p2_db->end(); ++itr ) {

    if( this->game->debug_game_ == false ) {
      itr->set_face_down(true);
    } else {
      itr->set_face_down(false);
    }
  }

  while( this->game->hand[PLAYER2].size() < MAX_PLAYER_HAND ) {
    // this->game->hand[PlAYER2].shuffle(1, 10, this->game->collection);
    this->game->hand[PLAYER2].shuffle(1, 1, *p2_db);
  }

  // Player 2 positioning (to the left)
  nom::size_type hand_idx = 0;
  Point2i p2_pos(Point2i::zero);
  auto p2_hand = this->game->hand[PLAYER2];
  for( auto itr = p2_hand.begin(); itr != p2_hand.end(); ++itr ) {

    p2_pos.x = PLAYER2_ORIGIN_X;
    p2_pos.y = PLAYER2_ORIGIN_Y + (CARD_HEIGHT / 2) * hand_idx;
    ++hand_idx;

    auto card_renderer =
      itr->card_renderer();
    if( card_renderer != nullptr && card_renderer->valid() == true ) {
      card_renderer->set_position(p2_pos);
    }
  }

  // Reset the state of the cards model to ensure no persistent state; this
  // can lead to subtle bugs or worse if we don't do this
  this->game->cards_page_model_.reset( new CardsPageDataSource("cards_db") );

  if( this->game->cards_menu_.set_context(&this->game->gui_window_) == false )
  {
    NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could set GUI desktop." );
    // return false;
  }

  #if defined(SCALE_FACTOR) && SCALE_FACTOR == 1
    if( this->game->cards_menu_.load_document_file( this->game->config.get_string("GUI_CARDS_MENU") ) == false )
    {
      NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could not load file:",
                    this->game->config.get_string("GUI_CARDS_MENU") );
      // return false;
    }
  #else
    if( this->game->cards_menu_.load_document_file( this->game->config.get_string("GUI_CARDS_MENU_SCALE2X") ) == false )
    {
      NOM_LOG_CRIT( TTCARDS_LOG_CATEGORY_APPLICATION, "Could not load file:",
                    this->game->config.get_string("GUI_CARDS_MENU_SCALE2X") );
      // return false;
    }
  #endif

  NOM_LOG_INFO( TTCARDS_LOG_CATEGORY_CARDS_MENU_STATE,
                "---Deck from collection---" );

  // Create the card pages with the active player's card deck
  Cards deck;
  for( auto itr = p1_db->begin(); itr != p1_db->end(); ++itr ) {

    // While we are at it, we can go ahead and initialize the card renderer for
    // each card in the database, so the player can immediately start browsing
    // once the pages are constructed
    Card& pcard = *itr;
    pcard.set_card_renderer( create_card_renderer(this->game->card_res_.get(), pcard) );

    auto card_renderer =
      pcard.card_renderer();

    NOM_ASSERT(card_renderer != nullptr);
    NOM_ASSERT(card_renderer->valid() == true);
    if( card_renderer == nullptr ) {
      // TODO: logging && handle err
      // return false;
      std::exit(-1);
    }

    if( card_renderer->valid() == false ) {
      // TODO: logging && handle err
      // return false;
      std::exit(-1);
    }

    card_renderer->set_position(this->selected_card_pos_);

    // FIXME: This breaks card lookup, rendering, etc. due to the fact that up
    // to this point, we've been depending on the ID of the card matching the
    // array index.
    // Options I've thought of: experiment with the idea of modeling the data
    // with std::map<int,Card>, keeping a copy of the position index with the
    // card or model (yuck!) ...
    // if( (*itr).num() > 0 ) {
      deck.push_back(pcard);
    // }

    NOM_LOG_DEBUG( TTCARDS_LOG_CATEGORY_CARDS_MENU_STATE,
                  (*itr).getID(), ":", (*itr).getName(),
                  "[", (*itr).num(), "]" );
  }

  // This card will be updated from the input subsystem
  this->selected_card_ = this->game->cards_db_[PLAYER1]->front();

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

  this->cursor_state_ = 0;  // default state for navigating card menu

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
    this->remove_card(this->selected_card_);
  });

  auto select_card( [=](const nom::Event& evt) {
    this->add_card(this->selected_card_);
  });

  auto start_game( [=](const nom::Event& evt) {
    this->game->set_state(Game::State::ConfirmationDialog);
  });

  state.insert( "cursor_prev", nom::KeyboardAction( SDL_KEYDOWN, SDLK_UP ), cursor_prev );
  state.insert( "cursor_prev", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_Y, nom::MouseWheelAction::UP ), cursor_prev );
  state.insert( "cursor_prev", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::UP ), cursor_prev );

  state.insert( "cursor_next", nom::KeyboardAction( SDL_KEYDOWN, SDLK_DOWN ), cursor_next );
  state.insert( "cursor_next", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_Y, nom::MouseWheelAction::DOWN ), cursor_next );
  state.insert( "cursor_next", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::DOWN ), cursor_next );

  state.insert( "prev_page", nom::KeyboardAction( SDL_KEYDOWN, SDLK_LEFT ), prev_page );
  state.insert( "prev_page", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_X, nom::MouseWheelAction::LEFT ), prev_page );
  state.insert( "prev_page", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::LEFT ), prev_page );

  state.insert( "next_page", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RIGHT ), next_page );
  state.insert( "next_page", nom::MouseWheelAction( SDL_MOUSEWHEEL, nom::MouseWheelAction::AXIS_X, nom::MouseWheelAction::RIGHT ), next_page );
  state.insert( "next_page", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::RIGHT ), next_page );

  state.insert( "delete_card", nom::KeyboardAction( SDL_KEYDOWN, SDLK_d ), delete_card );

  state.insert( "delete_card", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::CIRCLE ), delete_card );

  state.insert( "select_card", nom::KeyboardAction( SDL_KEYDOWN, SDLK_SPACE ), select_card );
  state.insert( "select_card", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::CROSS ), select_card );

  state.insert( "start_game", nom::KeyboardAction( SDL_KEYDOWN, SDLK_RETURN ), start_game );
  state.insert( "start_game", nom::JoystickButtonAction( 0, SDL_JOYBUTTONDOWN, nom::PSXBUTTON::START ), start_game );

  this->game->input_mapper.erase( "CardsMenuState" );
  this->game->input_mapper.insert( "CardsMenuState", state, true );
  this->game->input_mapper.activate_only( "CardsMenuState" );
  this->game->input_mapper.activate( "Game" );

  this->game->cards_menu_.show();

  // Yeah buddy!
  this->game->theme_track_->Play();
}

void CardsMenuState::on_exit( nom::void_ptr data )
{
  NOM_LOG_TRACE( TTCARDS_LOG_CATEGORY_TRACE_STATES );

  this->game->cards_menu_.close();
  Rocket::Core::Factory::ClearStyleSheetCache();
  Rocket::Core::Factory::ClearTemplateCache();
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

void CardsMenuState::on_update( float delta_time )
{
  this->update_cursor();

  this->game->window.update();
  this->game->gui_window_.update();

  this->update_page_indicators();
}

void CardsMenuState::on_draw( nom::RenderWindow& target )
{
  this->game->background.draw(target);

  // Player 2 is to the left
  auto p2_hand = this->game->hand[PLAYER2];
  for( auto itr = p2_hand.begin(); itr != p2_hand.end(); ++itr ) {

    auto card_renderer =
      itr->card_renderer();
    if( card_renderer != nullptr && card_renderer->valid() == true ) {
      card_renderer->render(target);
    }
  }

  // Player 1 is to the right
  auto hand_idx = 0;
  Point2i p1_pos(Point2i::zero);
  auto p1_hand = this->game->hand[PLAYER1];
  for( auto itr = p1_hand.begin(); itr != p1_hand.end(); ++itr ) {

    p1_pos.x = PLAYER1_ORIGIN_X;
    p1_pos.y = PLAYER1_ORIGIN_Y + (CARD_HEIGHT / 2) * hand_idx;
    ++hand_idx;

    auto card_renderer =
      itr->card_renderer();
    if( card_renderer != nullptr && card_renderer->valid() == true ) {
      // TODO: Update the positions here at state initialization -- this will
      // help ease the integration of animation into the state; we just need to
      // keep the coordinates of the selected card separate from the other
      // inside ::add_card ...
      card_renderer->set_position(p1_pos);
      card_renderer->render(target);
    }
  }

  this->game->gui_window_.draw();

  if( this->game->cursor_ != nullptr &&
      this->game->cursor_->valid() == true )
  {
    this->game->cursor_->draw(target);
  }

  // Player's current card selection
  auto card_renderer =
    this->selected_card_.card_renderer();
  if( card_renderer != nullptr && card_renderer->valid() == true ) {
    card_renderer->render(target);
  }
}

// Private scope

bool CardsMenuState::on_event( const nom::Event& ev )
{
  this->game->gui_window_.process_event(ev);

  return true;
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

  if( this->cursor_state_ == 0 )
  {
    this->game->cursor_->set_frame(INTERFACE_CURSOR_SHOWN);

    row_index = this->cursor_position();

    // Update selected card
    pos = this->game->cards_page_model_->map_row(row_index);
    this->selected_card_ = this->game->cards_page_model_->lookup_by_id(pos);
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
    if( this->game->cards_page_model_ == nullptr ) return;

    page = this->game->cards_page_model_->page();

    // Update cards model, play audio & set column text for previous page
    if( page > 0 )
    {
      --page;
      this->game->cards_page_model_->set_page(page);

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
    if( this->game->cards_page_model_ == nullptr ) return;

    page = this->game->cards_page_model_->page();
    num_pages = this->game->cards_page_model_->total_pages();

    // Update cards model, play audio & set column text for next page
    if( page < num_pages - 1 )
    {
      ++page;
      this->game->cards_page_model_->set_page(page);

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
      this->selected_card_ = this->game->cards_page_model_->lookup_by_id(pos);

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
      this->selected_card_ = this->game->cards_page_model_->lookup_by_id(pos);

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

  NOM_ASSERT( this->game->cards_page_model_ != nullptr );
  if( this->game->cards_page_model_ == nullptr ) return;

  page = this->game->cards_page_model_->page();
  Card c = card;
  pos = c.getID();

  // Card logic for adding a card to the player's hand
  //
  // 1. Decrease available card count by one
  // 2. Sync the cards model to reflect modified card count (-1)
  // 3. Update player hand w/ modified card.
  // 4. Update game cursor position.
  // 5. Queue audio clip
  if( c.num() > 0 )
  {
    c.set_num( c.num() - 1 );

    if( this->game->hand[PLAYER1].push_back(c) == true ) {

      this->game->cards_page_model_->insert_card(pos, c);
      this->selected_card_ = c;

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

  NOM_ASSERT( this->game->cards_page_model_ != nullptr );
  if( this->game->cards_page_model_ == nullptr ) return;

  page = this->game->cards_page_model_->page();
  Card c = card;
  pos = c.getID();

  // Used to compare the selected card from the current model with the game
  // cards collection (number of cards to return)
  Card ref_card = this->game->cards_db_[PLAYER2]->find(pos);

  // Card logic for removing a card from the player's hand
  //
  // 1. Increase available card count by one
  // 2. Sync the cards model to reflect modified card count (+1)
  // 3. Remove the card from the player's hand.
  // 4. Update game cursor position.
  // 5. Queue audio clip
  if( c.num() < ref_card.num() )
  {
    c.set_num( c.num() + 1 );

    if( this->game->hand[0].erase(c) == true ) {

      this->game->cards_page_model_->insert_card(pos, c);
      this->selected_card_ = card;

      // Get the position, relative to current page, from model for
      // updating game cursor position
      row_index =
        this->game->cards_page_model_->map_page_row(pos, page);
      this->set_cursor_position(row_index);

      this->game->cursor_cancel->Play();
    } // end if successful removal
  }
}
