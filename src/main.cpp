/******************************************************************************
    main.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "ttcards.h"

int main(int argc, char *argv[])
{
  TTcards app;

  if ( app.Init() == true )
  {
    app.Run();
  }

  return EXIT_SUCCESS;
}
