CC := $(shell which clang++)
CFLAGS = -gfull -O0 -std=c++11 -stdlib=libc++ -Wall
SDL_CFLAGS := $(shell pkg-config --cflags sdl SDL_image SDL_ttf SDL_mixer)
SDL_LDFLAGS := $(shell pkg-config --libs sdl SDL_image SDL_ttf SDL_mixer)

SRC = src/audio.cpp src/board.cpp src/card.cpp src/card_debug.cpp src/card_view.cpp src/card_collection.cpp src/card_hand.cpp src/cfg.cpp src/font.cpp src/gfx.cpp src/main.cpp src/player.cpp src/sprite.cpp src/timer.cpp src/fps.cpp src/ttcards.cpp
OBJ = audio.o board.o card.o card_debug.o card_collection.o card_view.o card_hand.o cfg.o font.o gfx.o main.o player.o sprite.o timer.o fps.o ttcards.o
BIN = ttcards

all: bin

bin: $(OBJ)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS) $(OBJ) -o ttcards

audio.o: src/audio.cpp src/audio.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/audio.cpp

board.o: src/board.cpp src/board.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/board.cpp

card.o: src/card.cpp src/card.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/card.cpp

card_debug.o: src/card_debug.cpp src/card_debug.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/card_debug.cpp

card_view.o: src/card_view.cpp src/card_view.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/card_view.cpp

card_collection.o: src/card_collection.cpp src/card_collection.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/card_collection.cpp

card_hand.o: src/card_hand.cpp src/card_hand.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/card_hand.cpp

cfg.o: src/cfg.cpp src/cfg.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/cfg.cpp

font.o: src/font.cpp src/font.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/font.cpp

gfx.o: src/gfx.cpp src/gfx.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/gfx.cpp

main.o: src/main.cpp
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/main.cpp

player.o: src/player.cpp src/player.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/player.cpp

sprite.o: src/sprite.cpp src/sprite.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/sprite.cpp

timer.o: src/timer.cpp src/timer.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/timer.cpp

fps.o: src/timer.cpp src/timer.h src/fps.cpp src/fps.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/fps.cpp

ttcards.o: src/ttcards.cpp src/ttcards.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/ttcards.cpp

clean:
	/bin/rm -rf *.o ttcards

analyze:
	scan-build -k --use-c++=/usr/bin/clang++ /usr/bin/clang++ -c $(CFLAGS) $(SDL_CFLAGS) $(SRC)

app:
	./ttcards

sublime-cpp:
	subl src/player.cpp src/card_hand.cpp src/board.cpp src/ttcards.cpp src/card_view.cpp src/card_collection.cpp src/card_debug.cpp src/card.cpp src/sprite.cpp src/gfx.cpp src/font.cpp src/timer.cpp src/fps.cpp
sublime-h:
	subl src/player.h src/card_hand.h src/board.h src/ttcards.h src/card_view.h src/card_collection.h src/card_debug.h src/card.h src/sprite.h src/gfx.h font.h timer.h fps.h

# DO NOT DELETE
