/******************************************************************************
    ttcards.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"

TTcards::TTcards ( void )
{
  this->game_state = true;
}

TTcards::~TTcards ( void )
{
}

bool TTcards::Init ( void )
{
  unsigned int flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RLEACCEL | SDL_RESIZABLE;

  std::srand ( ( unsigned ) time ( 0 ) );

  if ( this->engine.Init ( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == false )
  {
    std::cout << "ERR in TTcards::Init (): " << SDL_GetError() << std::endl;
    return false;
  }

  if ( this->engine.SetVideoMode (  SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, flags ) == false )
  {
    std::cout << "ERR in TTcards::Init (): " << SDL_GetError() << std::endl;
    return false;
  }

  this->engine.SetWindowTitle ( APP_NAME );
  this->engine.SetWindowIcon ( APP_ICON );

  return true;
}

bool TTcards::IsRunning ( void )
{
  if ( this->game_state == false )
    return false;
  else
    return true;
}

void TTcards::SetGameState ( bool state )
{
  this->game_state = state;
}

void TTcards::Input ( void )
{
  SDL_Event event;

  while ( SDL_PollEvent (&event) )
  {
    switch ( event.type )
    {
      case SDL_QUIT:
        this->SetGameState ( false );
        break;
      case SDL_VIDEORESIZE:
        break;
      default:
        break;

      case SDL_KEYDOWN:
        switch ( event.key.keysym.sym )
        {
          case SDLK_ESCAPE:
            this->SetGameState ( false );
            break;
          case SDLK_LEFT:
            this->mixer1.PlaySoundTrack ( CURSOR_MOVE, 1, 0);
            break;
          case SDLK_RIGHT:
            this->mixer2.PlaySoundTrack ( CURSOR_CANCEL, 2, 0 );
            break;
          case SDLK_UP:
            break;
          case SDLK_DOWN:
            break;
          case SDLK_q:
            this->SetGameState ( false );
            break;
          case SDLK_PLUS:
            break;
          case SDLK_MINUS:
            break;
          case SDLK_p:
            this->music.togglePlayingMusic ( );
            break;
          default:
            break;
        }
        break;
      }
    }
}

bool TTcards::Run ( void )
{
  SDL_Rect offsets;
  offsets.x = 96;
  offsets.y = 18;

  this->board.LoadBackground ( BOARD_BACKGROUND );

  this->music.LoadMusicTrack ( MUSIC_TRACK );

  this->music.PlayMusicTrack ( -1 );
  this->music.PauseMusic ();

  Sprite cardFace ( CARD_WIDTH, CARD_HEIGHT );
  Sprite cardBackground ( CARD_WIDTH, CARD_HEIGHT );
  Sprite cardElement ( ELEMENT_WIDTH, ELEMENT_HEIGHT );

  std::string face0 = "./data/images/faces/89.bmp";
  cardFace.LoadImage ( face0 );

  cardFace.SetX ( 96 ); //card0.x = PLAYER1_ORIGIN_X;
  cardFace.SetY ( 16 ); //cardFace.y = 0; //card0.y = PLAYER1_ORIGIN_Y;

  cardBackground.LoadImage ( PLAYER1_CARDFACE );

  cardBackground.SetX ( 96 );
  cardBackground.SetY ( 16 );

  cardElement.LoadImage ( ELEMENT_WATER );

  cardElement.SetX ( 142 ); //cardElement.x = cardFace.x + 46;
  cardElement.SetY ( 20 ); //cardElement.y = cardFace.y + 4;

  while( this->IsRunning() ) // main loop
  {
    this->Input();

    board.DrawBackground ( this->engine );

    cardBackground.Draw ( this->engine );
    cardFace.Draw ( this->engine );
    cardElement.Draw ( this->engine );

    //player1.DrawScore ( this->engine, 32, 176 ); // SCREEN_HEIGHT - 48
    //player2.DrawScore ( this->engine, 320, 176 ); // 64 * 5

    this->player1.Draw ( this->engine, 96, 16 );
    //player1.Draw ( this->screen, offsets.x, offsets.y );

    this->engine.UpdateScreen ();

  } // while this->IsRunning()

  return true;
}
