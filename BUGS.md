# BUGS #

- [x] Text alignment issues with info_text object; this is likely due to the shared
nature of the object between states and its potential leftover values with the
font size and or text buffer (the calculations require the current text string).

- [x] Consistent crashes upon PopStateThenChangeState method call *only* when using
while loop within SDLInput::Input with SDL_PollEvent

- [x] First (probable) related crash bug with onKeyUp method call within SDLInput::Input
consistently crashes with and without while loop implementation.

- [x] Second (probable) related bug to the above that is inconsistently reproduced
consistently involving onMouseMotion call within SDLInput::Input when using
PopStateThenChangeState method call
