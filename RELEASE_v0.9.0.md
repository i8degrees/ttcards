TTcards
=======

All in all, I've accomplished way more than I had anticipated for!

* Almost have a fully functioning run-time configuration system built on JSON
 programmed in.

* I occasionally like to dabble with pixel manipulation code in order to
 challenge myself on how well I understand this and that. It is nice now to
 finally say it has paid off with the wisdom of how to flip, rotate & scale bitmaps, pixel by pixel..! :D Unfortunately, it's dog-slow for anything serious, and I don't think it's worth the time investment to see what more I can optimize ... but certainly was worthwhile to learn.

* Most of the UI / interface work is near-finished, and I've even got some rudimentary text alignment -- ugh, what a headache -- for the informational boxes -- it's very nice to see the text aligned perfectly center for the very first time!

![ttcards_v0 9 0](https://f.cloud.github.com/assets/1329364/1204974/f301ef6c-2572-11e3-8825-7df7daf6d307.png)

Upcoming v1.0.0 Wish List
-------------------------

* AI player
* Replace player rectangle placeholders with image
* Card animations
* <strike>AboutState</strike>
* <strike>PauseState</strike>
* <strike>Find a nice looking scoreboard font</strike>

  - Finish OpenAL implementation
    * Add music streaming support (this will knock off ~60..80MB on memory usage)
    * <strike>Finish sound buffers implementation, NULL sound support</strike>

  - <strike>Fix path issue with working dir</strike>
    * <strike>In short, determine where I ought to store run-time data that can be generated, such as the in-game screenshots that one can do.</strike>

  - <strike>Finish OSX App Bundling</strike>
    * <strike>Determine strategy of binary distribution of dependencies for App Bundle & use install_name_tool fix up the dependencies</strike>
