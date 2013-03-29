BIN=ttcards
CC=$(shell which clang++) -std=c++11 -stdlib=libc++
CFLAGS=main.cpp cfg.cpp ttcards.cpp player.cpp card.cpp board.cpp sprite.cpp font.cpp audio.cpp
LDFLAGS=ttcards
SDL_CFLAGS := $(shell pkg-config --cflags sdl SDL_ttf SDL_mixer)
SDL_LDFLAGS := $(shell pkg-config --libs sdl SDL_ttf SDL_mixer)

all:
	$(CC) -o $(LDFLAGS) $(CFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS)

app:
	./$(BIN)
