/******************************************************************************
    gfx.cpp

    Graphics Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "gfx.h"

Gfx::Gfx ( void )
{
  #ifdef DEBUG_GFX_OBJ
    std::cout << "Gfx::Gfx (): Hello, world!" << "\n" << std::endl;
  #endif
}

Gfx::~Gfx ( void )
{
  this->screen = NULL;

  #ifdef DEBUG_GFX_OBJ
    std::cout << "Gfx::~Gfx (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  IMG_Quit ();

  SDL_Quit ();
}

bool Gfx::Init ( unsigned int sdl_flags, unsigned int img_flags )
{
  if ( SDL_Init ( sdl_flags ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::Init() at SDL_Init(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  if ( IMG_Init ( img_flags ) != img_flags )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::Init() at IMG_Init(): " << IMG_GetError() << std::endl;
    #endif
  }

  return true;
}

bool Gfx::SetVideoMode (  unsigned int screen_width,
                          unsigned int screen_height,
                          unsigned int screen_bpp, unsigned int flags )
{
  this->screen = NULL;

  this->screen = SDL_SetVideoMode ( screen_width, screen_height, screen_bpp, flags );

  if ( this->screen == 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetVideoMode (): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  return true;
}

bool Gfx::SetSurfaceTransparency (  SDL_Surface *video_buffer,
                                    unsigned int r, unsigned int g, unsigned int b,
                                    unsigned int flags )
{
  unsigned int transparent_color = 0;

  transparent_color = SDL_MapRGB ( video_buffer->format, r, g, b );

  if ( SDL_SetColorKey ( video_buffer, flags, transparent_color ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetSurfaceTransparency (): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

SDL_Surface *Gfx::LoadImage ( std::string filename, SDL_Color colorkey,
                              unsigned int flags )
{
  SDL_Surface *temp_buffer = NULL;
  SDL_Surface *video_buffer = NULL;

  temp_buffer = IMG_Load ( filename.c_str() );

  if ( temp_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      //std::cout << "ERR in Gfx::LoadImage() at IMG_Load(): " << IMG_GetError() << std::endl;
    #endif

    SDL_FreeSurface ( temp_buffer );
    temp_buffer = NULL;

    SDL_FreeSurface ( video_buffer );
    video_buffer = NULL;

    return NULL;
  }

  if ( this->SetSurfaceTransparency ( temp_buffer, colorkey.r, colorkey.g, colorkey.b, flags ) == false )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::LoadImage (): " << SDL_GetError() << std::endl;
    #endif

    SDL_FreeSurface ( temp_buffer );
    temp_buffer = NULL;

    SDL_FreeSurface ( video_buffer );
    video_buffer = NULL;

    return NULL;
  }

  video_buffer = SDL_DisplayFormatAlpha ( temp_buffer );

  if ( video_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::LoadImage (): " << SDL_GetError() << std::endl;
    #endif

    SDL_FreeSurface ( temp_buffer );
    temp_buffer = NULL;

    SDL_FreeSurface ( video_buffer );
    video_buffer = NULL;

    return NULL;
  }

  SDL_FreeSurface ( temp_buffer );
  temp_buffer = NULL;

  return video_buffer;
}

bool Gfx::DrawSurface ( SDL_Surface *video_buffer, unsigned int x, unsigned int y, SDL_Rect *offsets )
{
  SDL_Rect coords;

  coords.x = x;
  coords.y = y;

  if ( SDL_BlitSurface ( video_buffer, offsets, this->screen, &coords ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawSurface (): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

bool Gfx::UpdateScreen ( void )
{
  if ( SDL_Flip ( this->screen ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::UpdateScreen (): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

bool Gfx::DrawRectangle ( unsigned int x, unsigned int y,
                          unsigned int width, unsigned int height,
                          unsigned int r, unsigned int g, unsigned int b )
{
  SDL_Rect rectangle = { 0, 0, 0, 0 };
  unsigned int rectangle_color = 0;

  rectangle.x = x;
  rectangle.y = y;
  rectangle.w = width;
  rectangle.h = height;

  rectangle_color = SDL_MapRGB ( this->screen->format, r, g, b );

  if ( SDL_FillRect ( this->screen, &rectangle, rectangle_color ) != 0 )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::DrawRectangle (): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

void Gfx::SetWindowTitle ( std::string app_name )
{
  SDL_WM_SetCaption ( app_name.c_str(), NULL );
}

// We cannot use Gfx::LoadImage() for loading an application icon due to surface
// conversion -- SDL_DisplayFormatAlpha()
bool Gfx::SetWindowIcon ( std::string app_icon, SDL_Color colorkey, unsigned int flags )

{
  SDL_Surface *icon_buffer = NULL;

  icon_buffer = SDL_LoadBMP ( app_icon.c_str() );

  if ( icon_buffer == NULL )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetWindowIcon (): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  if ( this->SetSurfaceTransparency ( icon_buffer, colorkey.r, colorkey.g, colorkey.b, flags ) == false )
  {
    #ifdef DEBUG_GFX
      std::cout << "ERR in Gfx::SetWindowIcon (): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  SDL_WM_SetIcon ( icon_buffer, NULL );

  SDL_FreeSurface ( icon_buffer );
  icon_buffer = NULL;

  return true;
}
