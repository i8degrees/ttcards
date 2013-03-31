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
  void SetSoundLooping ( signed int loops ); // -1 for infinity

  bool LoadMusicTrack (std::string filename );
  bool PlayMusicTrack ( void );
  void SetMusicLooping ( signed int loops ); // -1 for infinity

  bool toggleMusic ( void );

private:
  unsigned int audio_rate;
  Uint16 audio_format;
  unsigned int audio_channels;
  unsigned int audio_buffers;

  signed int aTrack_loops;
  signed int mTrack_loops;
  int milliseconds;

  Mix_Chunk *aTrack;
  Mix_Music *mTrack;
};

#endif // AUDIO_HEADERS defined
