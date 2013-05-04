CC := /usr/bin/clang++
EMCC := ~/local/src/emscripten/emcc
EMCC_CFLAGS = --jcache -DEMSCRIPTEN
EMCC_PRELOAD_FILES = data/

CFLAGS = -std=c++11 -stdlib=libc++
DEBUG_CFLAGS := $(CFLAGS) -gfull -O0 -Wall
RELEASE_CFLAGS := $(CFLAGS) -O2

GAMELIB_CFLAGS = -I/usr/local/include/gamelib
SDL_CFLAGS := $(shell pkg-config --cflags sdl)

LDFLAGS = -L/usr/local/lib -lgamelib
SDL_LDFLAGS := $(shell pkg-config --libs sdl)

SRC = src/board.cpp src/card.cpp src/card_debug.cpp src/card_view.cpp \
src/card_collection.cpp src/card_hand.cpp src/card_rules.cpp src/cfg.cpp \
src/main.cpp src/player.cpp src/ttcards.cpp

OBJ = build/board.o build/card.o build/card_debug.o build/card_collection.o \
build/card_view.o build/card_hand.o build/card_rules.o build/cfg.o build/main.o \
build/player.o build/ttcards.o

GAMELIB_OBJ = ~/Projects/hax/gamelib.git/libgamelib.bc

TARGET_BIN = ttcards

all: bin

bin: $(OBJ)
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) $(LDFLAGS) $(SDL_CFLAGS) $(GAMELIB_CFLAGS) $(SDL_LDFLAGS) $(OBJ) -o $(TARGET_BIN)

web: $(OBJ)
	$(EMCC) $(CFLAGS) $(EMCC_CFLAGS) $(RELEASE_CFLAGS) $(GAMELIB_CFLAGS) $(GAMELIB_OBJ) $(SRC) -o public/index.html --preload-file $(EMCC_PRELOAD_FILES)

build/board.o: src/board.cpp src/board.h src/cfg.cpp src/cfg.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) $(GAMELIB_CFLAGS) src/board.cpp -o build/board.o

build/card.o: src/card.cpp src/card.h
	$(CC) -c $(CFLAGS) $(GAMELIB_CFLAGS) src/card.cpp -o build/card.o

build/card_debug.o: src/card_debug.cpp src/card_debug.h src/cfg.cpp src/cfg.h
	$(CC) -c $(CFLAGS) $(GAMELIB_CFLAGS) src/card_debug.cpp -o build/card_debug.o

build/card_view.o: src/card_view.cpp src/card_view.h src/cfg.cpp src/cfg.h
	$(CC) -c $(CFLAGS) $(GAMELIB_CFLAGS) src/card_view.cpp -o build/card_view.o

build/card_collection.o: src/card_collection.cpp src/card_collection.h src/cfg.cpp src/cfg.h
	$(CC) -c $(CFLAGS) $(GAMELIB_CFLAGS) src/card_collection.cpp -o build/card_collection.o

build/card_hand.o: src/card_hand.cpp src/card_hand.h src/cfg.cpp src/cfg.h
	$(CC) -c $(CFLAGS) $(GAMELIB_CFLAGS) src/card_hand.cpp -o build/card_hand.o

build/card_rules.o: src/card_rules.cpp src/card_rules.h src/cfg.cpp src/cfg.h
	$(CC) -c $(CFLAGS) $(GAMELIB_CFLAGS) src/card_rules.cpp -o build/card_rules.o

build/cfg.o: src/cfg.cpp src/cfg.h
	$(CC) -c $(CFLAGS) $(GAMELIB_CFLAGS) src/cfg.cpp -o build/cfg.o

build/main.o: src/main.cpp
	$(CC) -c $(CFLAGS) $(GAMELIB_CFLAGS) src/main.cpp -o build/main.o

build/player.o: src/player.cpp src/player.h src/cfg.cpp src/cfg.h
	$(CC) -c $(CFLAGS) $(GAMELIB_CFLAGS) src/player.cpp -o build/player.o

#build/cpu_player.o: src/cpu_player.cpp src/cpu_player.h src/cfg.cpp src/cfg.h
#	$(CC) -c $(CFLAGS) $(GAMELIB_CFLAGS) src/cpu_player.cpp -o build/cpu_player.o

build/ttcards.o: src/ttcards.cpp src/ttcards.h src/cfg.cpp src/cfg.h
	$(CC) -c $(CFLAGS) $(GAMELIB_CFLAGS) src/ttcards.cpp -o build/ttcards.o

clean:
	/bin/rm -rf *.o build/*.o $(TARGET_BIN) public/index.html public/index.data

analyze:
	scan-build -k --use-c++=/usr/bin/clang++ /usr/bin/clang++ -c $(CFLAGS) $(SRC)

app:
	./ttcards

sublime-cpp:
	subl src/cpu_player.cpp src/player.cpp src/card_hand.cpp src/board.cpp src/ttcards.cpp src/card_view.cpp src/card_collection.cpp src/card_debug.cpp src/card.cpp src/card_rules.cpp src/cfg.cpp
sublime-h:
	subl src/cpu_player.h src/player.h src/card_hand.h src/board.h src/ttcards.h src/card_view.h src/card_collection.h src/card_debug.h src/card.h src/card_rules.h src/cfg.h

# DO NOT DELETE
