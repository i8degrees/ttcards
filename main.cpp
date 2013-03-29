/******************************************************************************
    main.cpp

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/

#include <iostream>
#include "ttcards.h"

int main(int argc, char *argv[])
{
  TTcards app;

  if ( app.Init() == true )
  {
    app.Run();
  }
  else
  {
    app.CleanUp();
  }

  return true;
}
