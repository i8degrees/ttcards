# TODO #

- [ ] Fix building on Linux/Unix platforms; existing cmake build scripts need updating to reflect the many changes long since the last working version (which was back in June or May, I think...)

- Card class
    - [ ] get/set XY && width/height ?

  - CardView class
    - [x] break up further? ...drawElement, drawBackground, drawRanks, etc.

  - CardCollection class
    - [ ] getCards method implementation

- [ ] Relocate cfg.cpp declarations to their applicable class header files?

- [x] Use new preprocessor debugging code methodology from nomlib
- [x] AboutState
- [x] PauseState
- [x] Find a nice looking scoreboard font
- [x] Finish sound buffers implementation, NULL sound support, etc.
- [x] Fix path issues with working dir
- [x] Finish OSX App Bundling
- [x] Resolve resources paths issue when installed under /usr/local
- [x] Rename header file extensions to .hpp
- [x] PauseState ( msgbox w/ blinking Pause text )
- [x] Fix uninstall script
- [x] Remove file name from header files
- [x] GameOverState static background image
- [x] swap player1, player2 coords (human player AKA blue is on the right side )
- [x] Relocate Timer & FPS declarations to main.cpp
- [x] show card name info box upon cursor lock selection of placed cards on board
- [x] Remove LoadBackground, DrawBackground from Board Class
