/******************************************************************************
    audio.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef AUDIO_HEADERS
#define AUDIO_HEADERS

#include <iostream>
#include <cmath> // ceil, floor, etc.

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

//#define DEBUG_AUDIO

class Audio {
public:
  Audio ( unsigned int rate = 44100, Uint16 format = MIX_DEFAULT_FORMAT,
          unsigned int channels = MIX_DEFAULT_CHANNELS, unsigned int buffers = 4096 );

  ~Audio ( void );

  //bool LoadSoundTrack ( std::string filename );
  bool PlaySoundTrack ( std::string filename, signed int mix_channel = 0, signed int loops = 0 );
  //bool PlaySoundTrack ( signed int mix_channel = 1, signed int loops = 0 );

  void setMusicVolume ( int volAsPercent );  // set the music volume by percent
  signed int getMusicVolume ( void ); // return the current music volume as a percent

  void setChannelVolume ( signed int mix_channel, int volAsPercent );  // set the sound volume by percent
  int getChannelVolume ( signed int mix_channel ); // return the current sound volume as a percent

  bool LoadMusicTrack ( std::string filename );
  bool PlayMusicTrack ( signed int loops = 0 );
  bool PauseMusic ( void );
  bool togglePlayingMusic ( void );

  static void soundFinished ( signed int mix_channel );

private:
  // Defaults for audio hardware setup

  /* 22050 kHz */
  //unsigned int rate;

  /* AUDIO_S16LSB if little endian, AUDIO_S16MSB if big endian */
  //Uint16 format;

  /* Hardware Channels (1 = Mono, 2 = Stereo, ...) */
  //unsigned int channels;

  /* 4096 bytes */
  //unsigned int buffers;

  // Memory allocation buffers for audio
  Mix_Chunk *sound;
  Mix_Music *music;

/*
  unsigned int milliseconds;
*/
};

#endif // AUDIO_HEADERS defined
