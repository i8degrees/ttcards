# TTcards

## Known Bugs

* Consistent crashes upon PopStateThenChangeState method call *only* when using
while loop within SDLInput::Input with SDL_PollEvent

* First (probable) related crash bug with onKeyUp method call within SDLInput::Input
consistently crashes with and without while loop implementation.

* Second (probable) related bug to the above that is inconsistently reproduced
consistently involving onMouseMotion call within SDLInput::Input when using
PopStateThenChangeState method call

### Footer

Copyright (c) 2013 Jeffrey Carpenter <jeffrey.carp@gmail.com>
