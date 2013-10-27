# TTcards v0.9.0 #

## First Binary Release (OS X) ##

[TTcards_v0.9.0.tar.gz](https://github.com/i8degrees/ttcards/releases/download/v0.9.0/TTcards_v0.9.0.tar.gz)

I provide this release straight from my own development build, and has *never*been tested beyond the confines of this computer system (unless you count virtual machines ;-P), so the usual disclaimers apply, and so with that aside..!

At this time, the binary is for OS X users only, and I strongly suspect that you will need a minimum OS X version of v10.7 (libc++ incompatibility issue comes to mind), but other than that ... you might be OK. All necessary libraries are hopefully self-contained within the bundle. After executing one simple installation step (outlined in the following paragraph) you should be able to mount the DMG file and execute TTcards.app directly from the volume
mounted.

The one (and only!) installation task I am aware of that must be done before-hand is copying the config.json file you will find in the root directory of this DMG volume to a folder named "ttcards" under your home directory's Documents folder -- "$HOME/Documents/ttcards". This installation task is normally taken care of, but as you are running a developer's build.. you were suppose to already know that ;-)

If you so happen to figure out how to take screenshots, save or load games -- or perhaps more likely hit the right keys without realizing it -- you will notice that this same folder is where these actions store and access information at.

Lastly, do bare in mind that since this is a debug binary release, you will see a few things that won't ever be seen normally (most people would never bother to compile the sources). Not that any of that should be a concern to you, per se, but alas! You might enjoy hacking the JSON scripts and modifying cards or something, I know I have ;>o.

### Progress Ramblings ###

All in all, I've accomplished way more than I had anticipated for!

* I occasionally like to dabble with pixel manipulation code in order to
 challenge myself on how well I understand this and that. It is nice now to
 finally say it has paid off with the wisdom of how to flip, rotate & scale bitmaps, pixel by pixel..! :D Unfortunately, it's dog-slow for anything serious, and I don't think it's worth the time investment to see what more I can optimize ... but certainly was worthwhile to learn.

* Most of the UI / interface work is near-finished, and I've even got some rudimentary text alignment -- ugh, what a silly headache that can be -- for the informational boxes. It has been very rewarding to me to see this result, as it provides a polish that I finally feel at ease with. (Sadly, I don't have a readily accessible screenshot of long ago to show the comparison, but oh well).

* Almost have a fully functioning run-time configuration system built on JSON programmed in. (This is a "side project" of interest that I occasionally hack on).

![TTcards_v0.9.0](https://f.cloud.github.com/assets/1329364/1204974/f301ef6c-2572-11e3-8825-7df7daf6d307.png)

![TTcards_v0.9.0-1](https://f.cloud.github.com/assets/1329364/1230855/e6ce504c-280f-11e3-8b33-c7879ebf9ae6.png)

![TTcards_v0.9.0-2](https://f.cloud.github.com/assets/1329364/1230856/f7f51d42-280f-11e3-85a3-46fc91772745.png)

#### Upcoming v1.0.0 Wish List ####

(In no particular order)

- [ ] Animation effects -- my current task/interest at hand, and by far the most time consuming of all these tasks -- is well upon its way on implementing this most exciting section. I now understand well how the card transformation logic works, and to finish this task up: nomlib is undergoing the necessary refactoring work to support the SDL2 frameworks (w00t!!!), with the base infrastructure already existing for the support infrastructure for my first ever real attempt at animations (much less a serious object-oriented design!). It has taken me roughly ~3..6 months of learning, if not a whole lifetime -- depending on how you look at things, anyhow -- for me to realize this, so I'm quite excited beyond words indeed.

- [ ] Improved AI bot (right now, only a dumb, easy "random" pick bot exists). A considerable infrastructure for finishing this up is already in place and mostly just needs some logic connections to become alive!

- [ ] Replace player rectangle placeholders with an image

- [ ] User interfacing for card rules & states (most of the underlying programming logic for this is already there and functional; I just need to finish design & implementation of a few GUI-related classes).

- [x] AboutState
- [x] PauseState
- [x] Find a nice looking scoreboard font

- [ ] Add music streaming support (this will knock off ~60..80MB on memory usage, yay!)

- [x] Finish sound buffers implementation, NULL sound support, etc.
- [x] Fix path issues with working dir
- [x] Finish OSX App Bundling (this release binary is hopefully proof of it!)
