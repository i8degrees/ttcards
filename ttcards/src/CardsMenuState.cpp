/******************************************************************************
    CardsMenuState.cpp

  Cards Menu state

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "CardsMenuState.h"

using namespace nom;

CardsMenu::CardsMenu ( void )
{
  unsigned int pid = 0; // temp var for for loop iteration

  #ifdef DEBUG_CARDS_MENU_OBJ
    std::cout << "CardsMenu::CardsMenu (): Hello, world!" << "\n" << std::endl;
  #endif

  this->collection.clear();
  this->hand.clear ();

  this->collection.LoadJSON ( CARDS_DB );

  // Borrowed from Player class; this is perhaps a hack-(ish) workaround
    /* FIXME: */
  // As we presently expect / depend on this state to always be player1 and we
  // know the only required data structure being passed onwards is the player1
  // hand, we can fudge all the cards / objects leading up to said point without
  // anybody knowing the differnece.
  //
  for ( pid = 0; pid < this->collection.cards.size(); pid++ )
    this->collection.cards[pid].setPlayerID ( PLAYER1_ID );

  this->msgbox.push_back ( nom::Color ( 41, 41, 41 ) ); // top1
  this->msgbox.push_back ( nom::Color ( 133, 133, 133 ) ); // top2

  this->msgbox.push_back ( nom::Color ( 41, 41, 41 ) ); // left1
  this->msgbox.push_back ( nom::Color ( 133, 133, 133 ) ); // left2

  this->msgbox.push_back ( nom::Color ( 57, 57, 57 ) ); // bottom1
  this->msgbox.push_back ( nom::Color ( 41, 41, 41 ) ); // bottom2

  this->msgbox.push_back ( nom::Color ( 57, 57, 57 ) ); // right1
  this->msgbox.push_back ( nom::Color ( 41, 41, 41 ) ); // right2

  linear.setEndColor ( nom::Color ( 99, 99, 99, 255 ) );
  linear.setStartColor ( nom::Color ( 67, 67, 67, 255 ) );

  this->menu_box.Init ( PICK_CARDS_MENU_ORIGIN_X, PICK_CARDS_MENU_ORIGIN_Y, PICK_CARDS_MENU_WIDTH, PICK_CARDS_MENU_HEIGHT, msgbox, linear );

  this->per_page = 11; // number of cards to display per menu page
  this->total_pages = this->collection.cards.size() / per_page;
  this->current_index = 0; // current card position

  this->selectedCard = this->collection.cards.front();
}

CardsMenu::~CardsMenu ( void )
{
  #ifdef DEBUG_CARDS_MENU_OBJ
    std::cout << "CardsMenu::~CardsMenu (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  this->selectedCard = Card();
  this->hand.clear ();

  this->collection.clear();
}

void CardsMenu::onInit ( void )
{
  unsigned int idx = 0; // cursor_coords_map

  this->background.loadFromImage ( BOARD_BACKGROUND, nom::Color ( nom::Color::Black ), 0 );

  this->info_text.Load ( INFO_FONTFACE, nom::Color ( 110, 144, 190 ), 16, 16 );
  this->info_small_text.Load ( INFO_SMALL_FONTFACE, nom::Color ( 110, 144, 190 ), 16, 16 );
  this->info_text_gray.Load ( INFO_FONTFACE, nom::Color ( 110, 144, 190 ), 16, 16 );

  // Initialize card name text so that we can obtain height info early on
  this->info_text.setText ( this->collection.cards[0].getName() );
  this->info_text_height = this->info_text.getTextHeight();

  this->menu_element = nom::Sprite ( MENU_ELEMENT_WIDTH, MENU_ELEMENT_HEIGHT );
  this->menu_element.Load ( MENU_ELEMENTS, nom::Color ( 0, 0, 0 ) );
  this->menu_element.setSheetDimensions ( 58, 16, 0, 0 );

  this->cursor = nom::SDL_Cursor ( MENU_CARDS_CURSOR_ORIGIN_X, MENU_CARDS_CURSOR_ORIGIN_Y, CURSOR_WIDTH, CURSOR_HEIGHT );
  this->cursor.Load ( INTERFACE_CURSOR, nom::Color ( 0, 0, 0 ) );
  this->cursor.setSheetDimensions ( 78, 16, 0, 0 );
  this->cursor.setSheetID ( INTERFACE_CURSOR_RIGHT ); // default cursor image
  this->cursor.setState ( 0 ); // default state for navigating card menu

  for ( idx = 0; idx < per_page; idx++ )
  {
    this->cursor_coords_map[idx] = std::make_pair ( MENU_CARDS_CURSOR_ORIGIN_Y + ( this->info_text_height * idx ), idx );

    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << "\nidx:" << std::get<1>(this->cursor_coords_map[idx]) << " " << "y:" << std::get<0>(this->cursor_coords_map[idx]) << "\n";
    #endif
  }

  //this->info_text.setPosition ( 20, 20 );
  //nom::Coords glyph = this->info_text.findGlyph ( "Geezard!" );
  //std::cout << "\n" << glyph.x << " " << glyph.y << " " << glyph.width << " " << glyph.height << "\n";
}

void CardsMenu::onExit ( void )
{
  std::cout << "\n" << "CardsMenu state onExit" << "\n";
}

void CardsMenu::Pause ( void )
{
  std::cout << "\n" << "CardsMenu state Paused" << "\n";
}

void CardsMenu::Resume ( void )
{
  std::cout << "\n" << "CardsMenu state Resumed" << "\n";
}

void CardsMenu::onKeyDown ( int32_t key, int32_t mod )
{
  switch ( key )
  {
    // Reset / New Game State
    //case SDLK_r: if ( mod == KMOD_LSHIFT ) reloadDebugFile(); else this->engine->PopStateThenChangeState ( std::unique_ptr<Game>( new Game ( this->engine ) ) ); break;
     // Pause State
    case SDLK_p: /*this->engine->PopState ()*/; break;

    // Debug helpers
    case SDLK_LEFTBRACKET: this->debug.ListCards ( this->hand.cards ); break;

    case SDLK_LEFT: this->moveCursorLeft(); break;
    case SDLK_RIGHT: this->moveCursorRight(); break;
    case SDLK_UP: this->moveCursorUp(); break;
    case SDLK_DOWN: this->moveCursorDown(); break;

    case SDLK_d: this->hand.removeCard ( this->selectedCard ); break;
    case SDLK_SPACE: this->hand.addCard ( this->selectedCard ); break;
    case SDLK_RETURN: nom::GameStates::ChangeState ( std::unique_ptr<Game>( new Game ( this->hand ) ) ); break;

    default: break;
  }
}

void CardsMenu::onJoyButtonDown ( int32_t which, int32_t button )
{
  switch ( button )
  {
    // Debug helpers
    case nom::PSXBUTTON::L1: this->debug.ListCards ( this->hand.cards ); break;

    case nom::PSXBUTTON::UP: this->moveCursorUp(); break;
    case nom::PSXBUTTON::RIGHT: this->moveCursorRight(); break;
    case nom::PSXBUTTON::DOWN: this->moveCursorDown(); break;
    case nom::PSXBUTTON::LEFT: this->moveCursorLeft(); break;

    case nom::PSXBUTTON::TRIANGLE: /* TODO */ break;
    case nom::PSXBUTTON::CIRCLE: this->hand.removeCard ( this->selectedCard ); break;
    case nom::PSXBUTTON::CROSS: this->hand.addCard ( this->selectedCard ); break;
    case nom::PSXBUTTON::START: nom::GameStates::PushState ( std::unique_ptr<Game>( new Game ( this->hand ) ) ); break;

    default: break;
  }
}

void CardsMenu::onMouseLeftButtonDown ( int32_t x, int32_t y )
{
  // TODO
}

void CardsMenu::onMouseRightButtonDown ( int32_t x, int32_t y )
{
  // TODO
}

void CardsMenu::onMouseWheel ( bool up, bool down )
{
  if ( this->cursor.getState() == 0 )
  {
    if ( up )
      this->moveCursorUp();
    else if ( down )
      this->moveCursorDown();
  }
}

void CardsMenu::Update ( float delta_time )
{
  this->updateCursor();

  this->menu_box.Update();
  //this->menu_element.Update();

  this->context.Update();
}

void CardsMenu::Draw ( void* video_buffer )
{
  unsigned int y_offset = MENU_CARDS_FIELD_ORIGIN_Y; // card text, helper elements, card numbers
  unsigned int hand_index = 0; // "dummy" card index var

  this->background.Draw ( video_buffer );

  // static player2 hand background
  for ( hand_index = 0; hand_index < MAX_PLAYER_HAND; hand_index++ ) // TODO: std::get<1>(player_coords)
    this->card.drawFaceDown ( video_buffer, PLAYER2_ORIGIN_X, PLAYER2_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index );

  // Active player's card selection(s)
  for ( hand_index = 0; hand_index < this->hand.size(); hand_index++ ) // TODO: std::get<1>(player_coords)
  {
    if ( this->hand.isValid ( this->hand.cards.at ( hand_index) ) == true )
      this->card.DrawCard ( video_buffer, this->hand.cards.at ( hand_index ), PLAYER1_ORIGIN_X, PLAYER1_ORIGIN_Y + ( CARD_HEIGHT / 2 ) * hand_index );
  }

  this->menu_box.Draw ( video_buffer );

  for ( int i = current_index; i < total_pages + current_index + 1; i++ ) // padded + 1 since page starts at zero, not one
  {
    // Draw the top-left box title
    this->info_small_text.setText ( "CARDS" );
    this->info_small_text.setPosition ( MENU_CARDS_TITLE_ORIGIN_X, MENU_CARDS_TITLE_ORIGIN_Y );
    this->info_small_text.Update();
    this->info_small_text.Draw ( video_buffer );

    // Draw page number if we have more than one page to display
    if ( total_pages > 0 )
    {
      this->info_small_text.setText ( "P. " + std::to_string ( current_index / per_page + 1 ) ); // padded + 1 since page starts at zero, not one
      this->info_small_text.setPosition ( MENU_CARDS_TITLE_PAGE_ORIGIN_X, MENU_CARDS_TITLE_PAGE_ORIGIN_Y );
      this->info_small_text.Update();
      this->info_small_text.Draw ( video_buffer );
    }

    // Draw the top-right box title (number of cards)
    this->info_small_text.setText ( "NUM." );
    this->info_small_text.setPosition ( MENU_CARDS_TITLE_NUM_ORIGIN_X, MENU_CARDS_TITLE_NUM_ORIGIN_Y );
    this->info_small_text.Update();
    this->info_small_text.Draw ( video_buffer );

    // Draw the card selection helper element
    this->menu_element.setPosition ( MENU_CARDS_HELPER_ORIGIN_X, y_offset );

    if ( this->hand.isValid ( this->collection.cards[i] ) )
      this->menu_element.setSheetID ( INTERFACE_MENU_ELEMENT_USED );
    else
      this->menu_element.setSheetID ( INTERFACE_MENU_ELEMENT );

    this->menu_element.Update();
    this->menu_element.Draw ( video_buffer );

    // Draw the card's name onto our menu box
    // FIXME ( this->info_text_gray )
    if ( this->hand.isValid ( this->collection.cards[i] ) )
    {
      this->info_text_gray.setText ( this->collection.cards[i].getName() );
      this->info_text_gray.setStyle ( Style::Faded, 150 );
      this->info_text_gray.setPosition ( MENU_CARDS_NAME_ORIGIN_X, y_offset );
      this->info_text_gray.Update();
      this->info_text_gray.Draw ( video_buffer );
    }
    else
    {
      this->info_text.setText ( this->collection.cards[i].getName() );
      this->info_text.setPosition ( MENU_CARDS_NAME_ORIGIN_X, y_offset );
      this->info_text.Update();
      this->info_text.Draw ( video_buffer );
    }

    // Draw the number of cards in player's possession
    // TODO: Stub
    if ( this->hand.isValid ( this->collection.cards[i] ) )
    {
      this->info_text_gray.setText ( "0" );
      this->info_text_gray.setPosition ( MENU_CARDS_NUM_ORIGIN_X, y_offset );
      this->info_text_gray.Update();
      this->info_text_gray.Draw ( video_buffer );
    }
    else
    {
      this->info_text.setText ( "1" );
      this->info_text.setPosition ( MENU_CARDS_NUM_ORIGIN_X, y_offset );
      this->info_text.Update();
      this->info_text.Draw ( video_buffer );
    }

    // Lastly, check to see which page indicators we need to draw
    if ( current_index >= per_page )
    {
      this->menu_element.setSheetID ( INTERFACE_MENU_ELEMENT_PAGE_LEFT );
      this->menu_element.setPosition ( MENU_CARDS_PAGE_LEFT_ORIGIN_X, MENU_CARDS_PAGE_LEFT_ORIGIN_Y );
      this->menu_element.Update();
      this->menu_element.Draw ( video_buffer );
    }

    if ( current_index / per_page < total_pages - 1 ) // calculate current page minus padding of one
    {
      this->menu_element.setSheetID ( INTERFACE_MENU_ELEMENT_PAGE_RIGHT );
      this->menu_element.setPosition ( MENU_CARDS_PAGE_RIGHT_ORIGIN_X, MENU_CARDS_PAGE_RIGHT_ORIGIN_Y );
      this->menu_element.Update();
      this->menu_element.Draw ( video_buffer );
    }

    // Move on to the next card in stack to draw
     // We calculate height after setting the text buffer for each card name
    y_offset += this->info_text_height;
  }

  this->drawCursor ( video_buffer );

  this->card.DrawCard ( video_buffer, this->selectedCard, BOARD_ORIGIN_X + ( CARD_WIDTH * 2 ), BOARD_ORIGIN_Y + ( ( CARD_HEIGHT / 2 ) + CARD_HEIGHT * 1 ) - 8 );
}

void CardsMenu::updateCursor ( void )
{
  unsigned int pos = 0;

  if ( this->cursor.getState() == 0 )
  {
    this->cursor.setSheetID ( INTERFACE_CURSOR_RIGHT );

    pos = this->getCursorPos() + this->current_index;
    this->selectedCard = this->collection.cards[pos];

    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << "\npos: " << pos << "\n";
      std::cout << "\nselectedCard: " << this->collection.cards[pos].getName() << "\n";
    #endif
  }

  this->cursor.Update();
}

void CardsMenu::drawCursor ( void* video_buffer )
{
  this->cursor.Draw ( video_buffer );
}

// Helper method for obtaining card hand index position based off given origin
// coords definitions, creating us an ID map, initialized early on within the
// encapsulating class
//
// cursor_coords_map
//   [ index, y coordinate value ]
//
unsigned int CardsMenu::getCursorPos ( void )
{
  unsigned int pos = 0;
  unsigned int idx = 0;

  for ( idx = 0; idx < per_page; idx++ )
  {
    if ( this->cursor.getY() <= std::get<0>(cursor_coords_map[idx]) )
      return std::get<1>(cursor_coords_map[idx]);
    else // catch all safety switch
    // assume we are at the last position in the index when all else fails
      pos = per_page;
  }

  return pos;
}

// Helper method for paging menu backwards
 // TODO: rename method call
void CardsMenu::moveCursorLeft ( void )
{
  if ( this->cursor.getState() == 0 )
  {
    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << current_index << " " << per_page << "\n";
    #endif

    if ( current_index > 0 )
      current_index -= per_page;
  }
}

// Helper method for paging menu forwards
// TODO: rename method call
void CardsMenu::moveCursorRight ( void )
{
  if ( this->cursor.getState() == 0 )
  {
    #ifdef DEBUG_CARDS_MENU_CURSOR
      std::cout << current_index << " " << per_page << "\n";
    #endif

    if ( current_index < MAX_COLLECTION - per_page )
        current_index += per_page;
  }
}

void CardsMenu::moveCursorUp ( void )
{
  unsigned int pos = 0;

  if ( this->cursor.getState() == 0 )
  {
    if ( this->cursor.getY() > MENU_CARDS_CURSOR_ORIGIN_Y )
    {
      this->cursor.move ( 0, -( this->info_text_height ) );

      // FIXME: onMouseWheel breaks without the below support code:
      pos = this->current_index + this->getCursorPos();
      this->selectedCard = this->collection.cards[pos];

      #ifdef DEBUG_CARDS_MENU_CURSOR
        std::cout << "\npos: " << pos << "\n";
        std::cout << "\nselectedCard: " << this->collection.getCards (pos).getName() << "\n";
      #endif
    }
  }
}

void CardsMenu::moveCursorDown ( void )
{
  unsigned int pos = 0;

  if ( this->cursor.getState() == 0 )
  {
    if ( this->cursor.getY() < PICK_CARDS_MENU_HEIGHT )
    {
      this->cursor.move ( 0, this->info_text_height );

      // FIXME: onMouseWheel breaks without the below support code:
      pos = current_index + this->getCursorPos();
      this->selectedCard = this->collection.cards[pos];

      #ifdef DEBUG_CARDS_MENU_CURSOR
        std::cout << "\npos: " << pos << "\n";
        std::cout << "\nselectedCard: " << this->collection.getCards (pos).getName() << "\n";
      #endif
    }
  }
}
