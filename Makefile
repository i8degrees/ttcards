CC := $(shell which clang++)
CFLAGS = -gfull -O0 -std=c++11 -stdlib=libc++ -Wall
SDL_CFLAGS := $(shell pkg-config --cflags sdl SDL_ttf SDL_mixer)
SDL_LDFLAGS := $(shell pkg-config --libs sdl SDL_ttf SDL_mixer)

OBJ = audio.o board.o card.o card_debug.o card_deck.o card_view.o card_hand.o cfg.o font.o gfx.o main.o player.o sprite.o ttcards.o
BIN = ttcards

all: bin

bin: $(OBJ)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS) $(OBJ) -o ttcards

audio.o: audio.cpp audio.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) audio.cpp

board.o: board.cpp board.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) board.cpp

card.o: card.cpp card.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) card.cpp

card_debug.o: card_debug.cpp card_debug.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) card_debug.cpp

card_view.o: card_view.cpp card_view.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) card_view.cpp

card_deck.o: card_deck.cpp card_deck.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) card_deck.cpp

card_hand.o: card_hand.cpp card_hand.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) card_hand.cpp

cfg.o: cfg.cpp cfg.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) cfg.cpp

font.o: font.cpp font.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) font.cpp

gfx.o: gfx.cpp gfx.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) gfx.cpp

main.o: main.cpp
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) main.cpp

player.o: player.cpp player.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) player.cpp

sprite.o: sprite.cpp sprite.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) sprite.cpp

ttcards.o: ttcards.cpp ttcards.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) ttcards.cpp

clean:
	/bin/rm -rf *.o ttcards

analyze:
	scan-build -k --use-c++=/usr/bin/clang++ /usr/bin/clang++ -c $(CFLAGS) $(SDL_CFLAGS) audio.cpp board.cpp card.cpp cfg.cpp font.cpp gfx.cpp main.cpp player.cpp sprite.cpp ttcards.cpp

app:
	./ttcards
# DO NOT DELETE
