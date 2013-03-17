CC=$(shell which g++-4.2)
CFLAGS=ttcards.cpp sprite.cpp card.cpp board.cpp
LDFLAGS=ttcards
SDL_CFLAGS := $(shell pkg-config --cflags sdl)
SDL_LDFLAGS := $(shell pkg-config --libs sdl)

all:
	$(CC) -o $(LDFLAGS) $(CFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS)
