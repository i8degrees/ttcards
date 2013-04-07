CC := $(shell which clang++)
CFLAGS = -gfull -O0 -std=c++11 -stdlib=libc++
SDL_CFLAGS := $(shell pkg-config --cflags sdl SDL_ttf SDL_mixer)
SDL_LDFLAGS := $(shell pkg-config --libs sdl SDL_ttf SDL_mixer)

BIN = ttcards

all: bin

bin: audio.o board.o card.o cfg.o font.o gfx.o main.o player.o sprite.o ttcards.o
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS) audio.o board.o card.o cfg.o font.o gfx.o main.o player.o sprite.o ttcards.o -o ttcards

audio.o: audio.cpp
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) audio.cpp

board.o: board.cpp
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) board.cpp

card.o: card.cpp
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) card.cpp

cfg.o: cfg.cpp
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) cfg.cpp

font.o: font.cpp
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) font.cpp

gfx.o: gfx.cpp
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) gfx.cpp

main.o: main.cpp
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) main.cpp

player.o: player.cpp
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) player.cpp

sprite.o: sprite.cpp
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) sprite.cpp

ttcards.o: ttcards.cpp
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) ttcards.cpp

clean:
	/bin/rm -rf *.o ttcards

app:
	./ttcards
