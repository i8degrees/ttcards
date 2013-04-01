/******************************************************************************
    audio.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "audio.h"

using namespace std;

Audio::Audio ( void )
{
  // Memory allocations for audio samples to be held
  this->sound = NULL;
  this->music = NULL;

  this->audio_rate = 22050;
  this->audio_format = AUDIO_S16;
  this->audio_channels = -1; // Use all available mixing audio channels
  this->audio_buffers = 4096;

  // -1 loops infinity
  this->sound_loops = 0;
  this->music_loops = 0;

/*  TODO
  this->milliseconds = 2500;
*/
}

Audio::~Audio ( void )
{
  Mix_FreeChunk ( this->sound );

  Mix_HaltMusic ();
  Mix_FreeMusic ( this->music );

  Mix_CloseAudio ();
}

bool Audio::Init ( void )
{
  if ( Mix_OpenAudio ( this->audio_rate, this->audio_format, this->audio_channels, this->audio_buffers ) == -1 )
  {
    std::cout << "ERR: " << Mix_GetError() << std::endl;
    return false;
  }

  return true;
}

bool Audio::LoadSoundTrack ( std::string filename )
{
  this->sound = Mix_LoadWAV ( filename.c_str() );

  if ( ! this->sound )
  {
    std::cout << "ERR: " << Mix_GetError() << std::endl;
    return false;
  }

  return true;
}

bool Audio::PlaySoundTrack ( void )
{
  Mix_PlayChannel ( -1, this->sound, this->sound_loops );

  return true;
}

void Audio::SetSoundLooping ( signed int loops )
{
  this->sound_loops = loops;
}

bool Audio::LoadMusicTrack ( std::string filename )
{
  this->music = Mix_LoadMUS ( filename.c_str() );

  if ( ! this->music )
  {
    std::cout << "ERR: " << Mix_GetError() << std::endl;
  }

  return true;
}

bool Audio::PlayMusicTrack ( void )
{
  Mix_PlayMusic ( this->music, this->music_loops );

  return true;
}

void Audio::SetMusicLooping ( signed int loops )
{
  this->music_loops = loops;
}

bool Audio::toggleMusic ( void )
{
  if ( Mix_PausedMusic () == 1 )
  {
    // Mix_FadeInMusic ( mTrack, -1, 4000 );
    Mix_ResumeMusic ();
  }
  else
  {
    //Mix_FadeOutMusic ( this->milliseconds );
    //Mix_FadeOutMusic ( this->music, this->music_loops, this->milliseconds );
    Mix_PauseMusic ();
  }

  return true;
}
