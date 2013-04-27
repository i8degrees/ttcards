CC := $(shell which clang++)
CFLAGS = -gfull -O0 -std=c++11 -stdlib=libc++ -Wall -I/usr/local/include/gamelib
LDFLAGS = -L/usr/local/lib -lgamelib
SDL_CFLAGS := $(shell pkg-config --cflags sdl)
SDL_LDFLAGS := $(shell pkg-config --libs sdl)

SRC = src/board.cpp src/card.cpp src/card_debug.cpp src/card_view.cpp src/card_collection.cpp src/card_hand.cpp src/cfg.cpp src/main.cpp src/player.cpp src/ttcards.cpp
OBJ = board.o card.o card_debug.o card_collection.o card_view.o card_hand.o cfg.o main.o player.o ttcards.o
TARGET = ttcards

all: bin

bin: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SDL_CFLAGS) $(SDL_LDFLAGS) $(OBJ) -o $(TARGET)

board.o: src/board.cpp src/board.h
	$(CC) -c $(CFLAGS) $(SDL_CFLAGS) src/board.cpp

card.o: src/card.cpp src/card.h
	$(CC) -c $(CFLAGS) src/card.cpp

card_debug.o: src/card_debug.cpp src/card_debug.h
	$(CC) -c $(CFLAGS) src/card_debug.cpp

card_view.o: src/card_view.cpp src/card_view.h
	$(CC) -c $(CFLAGS) src/card_view.cpp

card_collection.o: src/card_collection.cpp src/card_collection.h
	$(CC) -c $(CFLAGS) src/card_collection.cpp

card_hand.o: src/card_hand.cpp src/card_hand.h
	$(CC) -c $(CFLAGS) src/card_hand.cpp

cfg.o: src/cfg.cpp src/cfg.h
	$(CC) -c $(CFLAGS) src/cfg.cpp

main.o: src/main.cpp
	$(CC) -c $(CFLAGS) src/main.cpp

player.o: src/player.cpp src/player.h
	$(CC) -c $(CFLAGS) src/player.cpp

ttcards.o: src/ttcards.cpp src/ttcards.h
	$(CC) -c $(CFLAGS) src/ttcards.cpp

clean:
	/bin/rm -rf *.o $(TARGET)

analyze:
	scan-build -k --use-c++=/usr/bin/clang++ /usr/bin/clang++ -c $(CFLAGS) $(SRC)

app:
	./ttcards

sublime-cpp:
	subl src/player.cpp src/card_hand.cpp src/board.cpp src/ttcards.cpp src/card_view.cpp src/card_collection.cpp src/card_debug.cpp src/card.cpp src/cfg.cpp
sublime-h:
	subl src/player.h src/card_hand.h src/board.h src/ttcards.h src/card_view.h src/card_collection.h src/card_debug.h src/card.h src/cfg.h

# DO NOT DELETE
