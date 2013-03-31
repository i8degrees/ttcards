/******************************************************************************
    audio.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "audio.h"

using namespace std;

Audio::Audio ( void )
{
  Audio::audio_rate = 22050;
  Audio::audio_format = AUDIO_S16;
  Audio::audio_channels = 2; // ??
  Audio::audio_buffers = 4096;

  Audio::aTrack_loops = 0;
  Audio::mTrack_loops = 0;
  Audio::milliseconds = 2500; // time in ms for fade in/out

  Audio::aTrack = NULL;
  Audio::mTrack = NULL;
}

Audio::~Audio ( void )
{
  Mix_FreeChunk ( aTrack );

  Mix_HaltMusic (); // ???
  Mix_FreeMusic ( mTrack );
  Mix_CloseAudio ();
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

bool Audio::LoadSoundTrack ( std::string filename )
{
  aTrack = Mix_LoadWAV ( filename.c_str() );

  return true;
}

bool Audio::PlaySoundTrack ( void ) // 0 = no looping (1 = one loops) .. -1 for infinity
{
  Mix_PlayChannel ( -1, aTrack, Audio::aTrack_loops );

  return true;
}

void Audio::SetSoundLooping ( signed int loops )
{
  Audio::aTrack_loops = loops;
}

bool Audio::LoadMusicTrack ( std::string filename )
{
  mTrack = Mix_LoadMUS ( filename.c_str() );

  return true;
}

bool Audio::PlayMusicTrack ( void ) // 0 = never .. -1 for infinity
{
  Mix_PlayMusic ( mTrack, Audio::mTrack_loops );

  return true;
}

void Audio::SetMusicLooping ( signed int loops )
{
  Audio::mTrack_loops = loops;
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
    //Mix_FadeOutMusic ( milliseconds );
    //Mix_FadeOutMusic ( mTrack, Audio::mTrack_loops, milliseconds );
    Mix_PauseMusic ();
  }

  return true;
}
