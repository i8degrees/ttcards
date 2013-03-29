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
  bool LoadMusicTrack (std::string filename );

  bool PlaySoundTrack ( void );
  bool PlayMusicTrack ( void );

  bool toggleMusic ( void );

private:
  Mix_Music *mTrack = NULL;

  int audio_rate;
  Uint16 audio_format;
  int audio_channels;
  int audio_buffers;
};

#endif // AUDIO_HEADERS defined
