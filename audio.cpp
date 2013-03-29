/******************************************************************************
    cfg.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "audio.h"

using namespace std;

Audio::Audio ( void )
{
  Audio::audio_rate = 22050;
  Audio::audio_format = AUDIO_S16;
  Audio::audio_channels = 2;
  Audio::audio_buffers = 4096;

  Audio::mTrack = NULL;
  //Mix_??? *aTrack = NULL;
}

bool Audio::Init ( void )
{
  if ( Mix_OpenAudio ( audio_rate, audio_format, audio_channels, audio_buffers ) == -1 )
  {
    std::cout << "ERR: " << SDL_GetError() << endl;
    return false;
  }

  return true;
}

bool Audio::LoadMusicTrack ( std::string filename )
{
  mTrack = Mix_LoadMUS ( filename.c_str() );

  return true;
}

bool Audio::LoadSoundTrack ( std::string filename )
{
  //aTrack = Mix_LoadWAV ( filename );

  return true;
}

bool Audio::PlayMusicTrack ( void )
{
  Mix_PlayMusic ( mTrack, 0 );

  return true;
}

bool Audio::PlaySoundTrack ( void )
{
  return true;
}

bool Audio::toggleMusic ( void )
{
  if ( Mix_PausedMusic () == 1 )
  {
    Mix_ResumeMusic ();
  }
  else
  {
    Mix_PauseMusic ();
  }

  return true;
}
