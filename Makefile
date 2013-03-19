CC=$(shell which clang++) -std=c++11 -stdlib=libc++
CFLAGS=ttcards.cpp board.cpp sprite.cpp card.cpp
LDFLAGS=ttcards
SDL_CFLAGS := $(shell pkg-config --cflags sdl)
SDL_LDFLAGS := $(shell pkg-config --libs sdl)

all:
	$(CC) -o $(LDFLAGS) $(CFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS)
