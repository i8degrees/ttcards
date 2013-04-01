CC=$(shell which clang++) -std=c++11 -stdlib=libc++
CFLAGS=main.cpp cfg.cpp ttcards.cpp gfx.cpp sprite.cpp font.cpp audio.cpp card.cpp board.cpp player.cpp
LDFLAGS=ttcards
SDL_CFLAGS := $(shell pkg-config --cflags sdl SDL_ttf SDL_mixer)
SDL_LDFLAGS := $(shell pkg-config --libs sdl SDL_ttf SDL_mixer)

all:
	$(CC) -o $(LDFLAGS) $(CFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS)

app:
	./ttcards
