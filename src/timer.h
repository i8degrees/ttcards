/******************************************************************************
    timer.cpp

    SDL-based Timer Implementation Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_TIMER_HEADERS
#define GAMELIB_TIMER_HEADERS

#include <iostream>

#include "SDL/SDL.h"

//#define DEBUG_TIMER
//#define DEBUG_TIMER_OBJ

class Timer
{
  public:
    Timer ( void );
    ~Timer ( void );

    void Start ( void );
    void Stop ( void );
    void Pause ( void );
    void UnPause ( void );

    unsigned int GetTicks ( void );

    bool IsStarted ( void );
    bool IsPaused ( void );

  private:
    unsigned int elapsed_ticks; // milliseconds since timer start
    unsigned int paused_ticks; // holds ticks while paused
    bool paused;
    bool started;
};

#endif // GAMELIB_TIMER_HEADERS defined
