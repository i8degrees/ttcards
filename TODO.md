# TTcards

## TODO

* ttcards.app bundle (think: SteerMouse)
    1. http://qt-project.org/doc/qt-4.8/deployment-mac.html
    2. http://stackoverflow.com/questions/5131015/creating-an-application-bundle-on-osx-with-eclipse-c
    3. http://stackoverflow.com/questions/1596945/building-osx-app-bundle
    4. http://www.j-schell.de/node/316
    5. https://developer.apple.com/library/mac/#documentation/Cocoa/Conceptual/PropertyLists/CreatePropListProgram/CreatePropListProgram.html
    6. py2app

* GameOverState static background image
* PauseState ( msgbox w/ blinking Pause text )
* rename methods in gamelib/Sprite to lowerCaseCamelStyle
* swap player1, player2 coords (human player AKA blue is on the right side )
* break CardView class up further?
* Re-enable AI control
* Relocate cfg.cpp declarations to their applicable class header files
* Collection::getCards() implementation

* gamelib proposed project name
    > NomNom

## Completed

* Clean up Timer class; 1. match Timer & FPS; <strikethrough> 2. goodbye FPS (!) </strikethrough>
* Relocate Timer & FPS declarations to main.cpp
* show card name info box upon cursor lock selection of placed cards on board
* Remove LoadBackground, DrawBackground from Board Class
* msgbox bottom, right border (needs additional depth)

### Footer

Copyright (c) 2013 Jeffrey Carpenter <jeffrey.carp@gmail.com>
