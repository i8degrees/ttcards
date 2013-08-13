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
#include "CardsMenuState.hpp"

CardsMenuState::CardsMenuState ( std::shared_ptr<GameObject> object )
{
  nom::Gradient linear;

  unsigned int pid = 0; // temp var for for loop iteration

TTCARDS_LOG_CLASSINFO;

  this->game = object;

  this->game->hand[0].clear ();
  // this->game->hand[1] is initialized for us in the Game state

  // this->game->collection is initialized for us in the main app loop

  // Borrowed from Player class; this is perhaps a hack-(ish) workaround
    /* FIXME: */
  // As we presently expect / depend on this state to always be player1 and we
  // know the only required data structure being passed onwards is the player1
  // hand, we can fudge all the cards / objects leading up to said point without
  // anybody knowing the difference.
  //
  for ( pid = 0; pid < this->game->collection.cards.size(); pid++ )
    this->game->collection.cards[pid].setPlayerID ( Card::PLAYER1 );

  linear.setEndColor ( nom::Color ( 99, 99, 99, 255 ) );
  linear.setStartColor ( nom::Color ( 67, 67, 67, 255 ) );

  this->menu_box = nom::ui::MessageBox  ( PICK_CARDS_MENU_ORIGIN_X,
                                          PICK_CARDS_MENU_ORIGIN_Y,
                                          PICK_CARDS_MENU_WIDTH,
                                          PICK_CARDS_MENU_HEIGHT,
                                          nom::ui::FrameStyle::Gray, linear
                                        );

  this->per_page = 11; // number of cards to display per menu page
  this->total_pages = this->game->collection.cards.size() / per_page;
  this->current_index = 0; // current card position

  this->selectedCard = this->game->collection.cards.front();
}

CardsMenuState::~CardsMenuState ( void )
{
TTCARDS_LOG_CLASSINFO;

  this->selectedCard = Card();
}

void CardsMenuState::onInit ( void )
{
  unsigned int idx = 0; // iterator for cursor_coords_map

  // Initialize card name text so that we can obtain height info early on
  this->game->info_text.setText ( this->game->collection.cards[0].getName() );
  this->info_text_height = this->game->info_text.getFontHeight();

  this->menu_element = nom::Sprite ( MENU_ELEMENT_WIDTH, MENU_ELEMENT_HEIGHT );
  this->menu_element.load ( MENU_ELEMENTS, nom::Color ( 0, 0, 0 ), true );
  this->menu_element.setSheetDimensions ( 58, 16, 0, 0 );

  this->game->cursor.setPosition ( MENU_CARDS_CURSOR_ORIGIN_X, MENU_CARDS_CURSOR_ORIGIN_Y );
  this->game->cursor.setSheetID ( INTERFACE_CURSOR_RIGHT ); // default cursor image
  this->game->cursor.setState ( 0 ); // default state for navigating card menu

  for ( idx = 0; idx < per_page; idx++ )
  {
    this->cursor_coords_map[idx] = std::make_pair ( MENU_CARDS_CURSOR_ORIGIN_Y + ( this->info_text_height * idx ), idx );

    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << "\nidx:" << std::get<1>(this->cursor_coords_map[idx]) << " " << "y:" << std::get<0>(this->cursor_coords_map[idx]) << "\n";
    #endif
  }
}

void CardsMenuState::onExit ( void )
{
  std::cout << "\n" << "CardsMenu state onExit" << "\n";
}

void CardsMenuState::Pause ( void )
{
  std::cout << "\n" << "CardsMenu state Paused" << "\n";
}

void CardsMenuState::Resume ( void )
{
  std::cout << "\n" << "CardsMenu state Resumed" << "\n";
}

void CardsMenuState::onKeyDown ( int32_t key, int32_t mod )
{
  switch ( key )
  {
    default: break;

    // Reset game
    case SDLK_r:
    {
      nom::GameStates::ChangeState ( CardsMenuStatePtr ( new CardsMenuState ( this->game ) ) );
    }
    break;

    // Debug helpers
    case SDLK_LEFTBRACKET: this->debug.ListCards ( this->game->hand[0].cards ); break;

    case SDLK_LEFT: this->moveCursorLeft(); break;
    case SDLK_RIGHT: this->moveCursorRight(); break;
    case SDLK_UP: this->moveCursorUp(); break;
    case SDLK_DOWN: this->moveCursorDown(); break;

    case SDLK_d: if ( this->game->hand[0].erase( this->selectedCard ) ) this->game->cursor_cancel.Play(); break;
    case SDLK_SPACE:
    {
      if ( this->game->hand[0].push_back ( this->selectedCard ) )
        this->game->card_place.Play();
    }
    break;
    case SDLK_RETURN:
    {
#ifdef TTCARDS_DEBUG
      if ( this->game->hand[0].size() < MAX_PLAYER_HAND )
      {
        this->game->hand[0].randomize ( 8, 10, this->game->collection );
      }
#endif
      nom::GameStates::ChangeState( PlayStatePtr( new PlayState ( this->game ) ) );
    }
    break;
  }
}

void CardsMenuState::onJoyButtonDown ( int32_t which, int32_t button )
{
  switch ( button )
  {
    default: break;

    // Debug helpers
    case nom::PSXBUTTON::L1: this->debug.ListCards ( this->game->hand[0].cards ); break;

    case nom::PSXBUTTON::UP: this->moveCursorUp(); break;
    case nom::PSXBUTTON::RIGHT: this->moveCursorRight(); break;
    case nom::PSXBUTTON::DOWN: this->moveCursorDown(); break;
    case nom::PSXBUTTON::LEFT: this->moveCursorLeft(); break;

    case nom::PSXBUTTON::TRIANGLE: /* TODO */ break;
    case nom::PSXBUTTON::CIRCLE: if ( this->game->hand[0].erase ( this->selectedCard ) ) this->game->cursor_cancel.Play(); break;
    case nom::PSXBUTTON::CROSS: if ( this->game->hand[0].push_back ( this->selectedCard ) ) this->game->card_place.Play(); break;
    case nom::PSXBUTTON::START:
    {
      nom::GameStates::ChangeState( PlayStatePtr( new PlayState ( this->game ) ) );
    }
    break;
  }
}

void CardsMenuState::onMouseLeftButtonDown ( int32_t x, int32_t y )
{
  // TODO
}

void CardsMenuState::onMouseRightButtonDown ( nom::int32 x, nom::int32 y )
{
  if ( this->game->hand[0].push_back ( this->selectedCard ) )
    this->game->card_place.Play();
}

void CardsMenuState::onMouseWheel ( bool up, bool down )
{
  if ( this->game->cursor.getState() == 0 )
  {
    if ( up )
      this->moveCursorUp();
    else if ( down )
      this->moveCursorDown();

    this->game->cursor_move.Play();
  }
}

void CardsMenuState::Update ( nom::uint32 delta_time )
{
  this->updateCursor();

  this->menu_box.Update();
  //this->menu_element.Update();

  this->game->context.Update();
}

void CardsMenuState::Draw ( void* video_buffer )
{
  unsigned int y_offset = MENU_CARDS_FIELD_ORIGIN_Y; // card text, helper elements, card numbers
  nom::int32 hand_index = 0; // "dummy" card index var

  this->game->background.Draw ( video_buffer );

  // static player2 hand background
  for ( hand_index = 0; hand_index < MAX_PLAYER_HAND; hand_index++ ) // TODO: std::get<1>(player_coords)
    this->game->card.drawFaceDown ( video_buffer, PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index );

  // Active player's card selection(s)
  for ( hand_index = 0; hand_index < this->game->hand[0].size(); hand_index++ ) // TODO: std::get<1>(player_coords)
  {
    this->game->card.DrawCard ( video_buffer, this->game->hand[0].cards.at ( hand_index ), PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index );
  }

  this->menu_box.Draw ( video_buffer );

  for ( nom::uint32 i = current_index; i < total_pages + current_index + 1; i++ ) // padded + 1 since page starts at zero, not one
  {
    // Draw the top-left box title
    this->game->info_small_text.setText ( "CARDS" );
    this->game->info_small_text.setPosition ( nom::Coords( MENU_CARDS_TITLE_ORIGIN_X, MENU_CARDS_TITLE_ORIGIN_Y ) );
    this->game->info_small_text.Update();
    this->game->info_small_text.Draw ( video_buffer );

    // Draw page number if we have more than one page to display
    if ( total_pages > 0 )
    {
      this->game->info_small_text.setText ( "P. " + std::to_string ( current_index / per_page + 1 ) ); // padded + 1 since page starts at zero, not one
      this->game->info_small_text.setPosition ( nom::Coords( MENU_CARDS_TITLE_PAGE_ORIGIN_X, MENU_CARDS_TITLE_PAGE_ORIGIN_Y ) );
      this->game->info_small_text.Update();
      this->game->info_small_text.Draw ( video_buffer );
    }

    // Draw the top-right box title (number of cards)
    this->game->info_small_text.setText ( "NUM." );
    this->game->info_small_text.setPosition ( nom::Coords( MENU_CARDS_TITLE_NUM_ORIGIN_X, MENU_CARDS_TITLE_NUM_ORIGIN_Y ) );
    this->game->info_small_text.Update();
    this->game->info_small_text.Draw ( video_buffer );

    // Draw the card selection helper element
    this->menu_element.setPosition ( nom::Coords( MENU_CARDS_HELPER_ORIGIN_X, y_offset ) );

    if ( this->game->hand[0].exists ( this->game->collection.cards[i] ) )
      this->menu_element.setSheetID ( INTERFACE_MENU_ELEMENT_USED );
    else
      this->menu_element.setSheetID ( INTERFACE_MENU_ELEMENT );

    this->menu_element.Update();
    this->menu_element.Draw ( video_buffer );

    // Draw the card's name onto our menu box
    // FIXME ( this->game->info_text_gray )
    if ( this->game->hand[0].exists ( this->game->collection.cards[i] ) )
    {
      this->game->info_text_gray.setText ( this->game->collection.cards[i].getName() );
      this->game->info_text_gray.setFontStyle ( nom::FontStyle::Faded, 150 );
      this->game->info_text_gray.setPosition ( nom::Coords( MENU_CARDS_NAME_ORIGIN_X, y_offset ) );
      this->game->info_text_gray.Update();
      this->game->info_text_gray.Draw ( video_buffer );
    }
    else
    {
      this->game->info_text.setText ( this->game->collection.cards[i].getName() );
      this->game->info_text.setPosition ( nom::Coords( MENU_CARDS_NAME_ORIGIN_X, y_offset ) );
      this->game->info_text.Update();
      this->game->info_text.Draw ( video_buffer );
    }

    // Draw the number of cards in player's possession
    // TODO: Stub
    if ( this->game->hand[0].exists ( this->game->collection.cards[i] ) )
    {
      this->game->info_text_gray.setText ( "0" );
      this->game->info_text_gray.setPosition ( nom::Coords( MENU_CARDS_NUM_ORIGIN_X, y_offset ) );
      this->game->info_text_gray.Update();
      this->game->info_text_gray.Draw ( video_buffer );
    }
    else
    {
      this->game->info_text.setText ( "1" );
      this->game->info_text.setPosition ( nom::Coords( MENU_CARDS_NUM_ORIGIN_X, y_offset ) );
      this->game->info_text.Update();
      this->game->info_text.Draw ( video_buffer );
    }

    // Lastly, check to see which page indicators we need to draw
    if ( current_index >= per_page )
    {
      this->menu_element.setSheetID ( INTERFACE_MENU_ELEMENT_PAGE_LEFT );
      this->menu_element.setPosition ( nom::Coords( MENU_CARDS_PAGE_LEFT_ORIGIN_X, MENU_CARDS_PAGE_LEFT_ORIGIN_Y ) );
      this->menu_element.Update();
      this->menu_element.Draw ( video_buffer );
    }

    if ( current_index / per_page < total_pages - 1 ) // calculate current page minus padding of one
    {
      this->menu_element.setSheetID ( INTERFACE_MENU_ELEMENT_PAGE_RIGHT );
      this->menu_element.setPosition ( nom::Coords( MENU_CARDS_PAGE_RIGHT_ORIGIN_X, MENU_CARDS_PAGE_RIGHT_ORIGIN_Y ) );
      this->menu_element.Update();
      this->menu_element.Draw ( video_buffer );
    }

    // Move on to the next card in stack to draw
    // We calculate height after setting the text buffer for each card name
    y_offset += this->info_text_height;
  }

  this->drawCursor ( video_buffer );

  this->game->card.DrawCard ( video_buffer, this->selectedCard, BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ), BOARD_ORIGIN_Y + ( ( CARD_HEIGHT / 2 ) + CARD_HEIGHT * 1 ) - 8 );
}

void CardsMenuState::updateCursor ( void )
{
  unsigned int pos = 0;

  if ( this->game->cursor.getState() == 0 )
  {
    this->game->cursor.setSheetID ( INTERFACE_CURSOR_RIGHT );

    pos = this->getCursorPos() + this->current_index;
    this->selectedCard = this->game->collection.cards[pos];

    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << "\npos: " << pos << "\n";
      std::cout << "\nselectedCard: " << this->game->collection.cards[pos].getName() << "\n";
    #endif
  }

  this->game->cursor.Update();
}

void CardsMenuState::drawCursor ( void* video_buffer )
{
  this->game->cursor.Draw ( video_buffer );
}

// Helper method for obtaining card hand index position based off given origin
// coords definitions, creating us an ID map, initialized early on within the
// encapsulating class
//
// cursor_coords_map
//   [ index, y coordinate value ]
//
unsigned int CardsMenuState::getCursorPos ( void )
{
  unsigned int pos = 0;
  unsigned int idx = 0;

  for ( idx = 0; idx < per_page; idx++ )
  {
    if ( this->game->cursor.getY() <= std::get<0>(cursor_coords_map[idx]) )
      return std::get<1>(cursor_coords_map[idx]);
    else // catch all safety switch
    // assume we are at the last position in the index when all else fails
      pos = per_page;
  }

  return pos;
}

// Helper method for paging menu backwards
 // TODO: rename method call
void CardsMenuState::moveCursorLeft ( void )
{
  if ( this->game->cursor.getState() == 0 )
  {
    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << current_index << " " << per_page << "\n";
    #endif

    if ( current_index > 0 )
    {
      current_index -= per_page;
      this->game->cursor_move.Play();
    }
  }
}

// Helper method for paging menu forwards
// TODO: rename method call
void CardsMenuState::moveCursorRight ( void )
{
  if ( this->game->cursor.getState() == 0 )
  {
    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << current_index << " " << per_page << "\n";
    #endif

    if ( current_index < MAX_COLLECTION - per_page )
    {
      current_index += per_page;
      this->game->cursor_move.Play();
    }
  }
}

void CardsMenuState::moveCursorUp ( void )
{
  unsigned int pos = 0;

  if ( this->game->cursor.getState() == 0 )
  {
    if ( this->game->cursor.getY() > MENU_CARDS_CURSOR_ORIGIN_Y )
    {
      this->game->cursor.move ( 0, -( this->info_text_height ) );

      // FIXME: onMouseWheel breaks without the below support code:
      pos = this->current_index + this->getCursorPos();
      this->selectedCard = this->game->collection.cards[pos];

      this->game->cursor_move.Play();

      #ifdef DEBUG_CARDS_MENU_CURSOR
        std::cout << "\npos: " << pos << "\n";
        std::cout << "\nselectedCard: " << this->game->collection.getCards (pos).getName() << "\n";
      #endif
    }
  }
}

void CardsMenuState::moveCursorDown ( void )
{
  unsigned int pos = 0;

  if ( this->game->cursor.getState() == 0 )
  {
    if ( this->game->cursor.getY() < PICK_CARDS_MENU_HEIGHT )
    {
      this->game->cursor.move ( 0, this->info_text_height );

      // FIXME: onMouseWheel breaks without the below support code:
      pos = current_index + this->getCursorPos();
      this->selectedCard = this->game->collection.cards[pos];

      this->game->cursor_move.Play();

      #ifdef DEBUG_CARDS_MENU_CURSOR
        std::cout << "\npos: " << pos << "\n";
        std::cout << "\nselectedCard: " << this->game->collection.getCards (pos).getName() << "\n";
      #endif
    }
  }
}
