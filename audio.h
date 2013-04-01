/******************************************************************************
    audio.h

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef AUDIO_HEADERS
#define AUDIO_HEADERS

#include <iostream>

#include "SDL.h"
#include "SDL_mixer.h"

class Audio {
public:
  Audio ( void );
  ~Audio ( void );

  bool Init ( void );

  bool LoadSoundTrack (std::string filename );
  bool PlaySoundTrack ( void );
  void SetSoundLooping ( signed int loops );

  bool LoadMusicTrack (std::string filename );
  bool PlayMusicTrack ( void );
  void SetMusicLooping ( signed int loops );

  bool toggleMusic ( void );

private:
  // Defaults for audio hardware setup
  unsigned int audio_rate;
  Uint16 audio_format;
  unsigned int audio_channels;
  unsigned int audio_buffers;

  // -1 loops for infinity and beyond..!
  signed int sound_loops;
  signed int music_loops;

  // Memory allocations for holding audio samples
  Mix_Chunk *sound;
  Mix_Music *music;

/*
  unsigned int milliseconds;
*/
};

#endif // AUDIO_HEADERS defined
