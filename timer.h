/******************************************************************************
    timer.cpp

    SDL-based Timer

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_TIMER_HEADERS
#define GAMELIB_TIMER_HEADERS

#include <iostream>

#include "SDL/SDL.h"

#define DEBUG_TIMER
#define DEBUG_TIMER_OBJ

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

#define DEBUG_TIMER_FPS
#define DEBUG_TIMER_FPS_OBJ

class FPS: public Timer
{
  public:
    FPS ( void );
    ~FPS ( void );

    void Start ( void );
    void Stop ( void );

    unsigned int GetFrames ( void );
    unsigned int GetFPS ( void );
    void Update ( void );

  private:
    unsigned int total_frames;
    Timer fps, fps_update;
};

#endif // GAMELIB_TIMER_HEADERS defined
