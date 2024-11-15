CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++11
LIBS = -lcbor

SERVER_SOURCE = server.cpp
CLIENT_SOURCE = client.cpp
DISTANCE_SOURCE = distance.cpp
BIRD = bird.cpp
HEADERS = distance.hpp



all: server client


server: $(SERVER_SOURCE) $(DISTANCE_SOURCE)	$(BIRD)
	$(CC) $(CFLAGS) -o server $(SERVER_SOURCE) $(DISTANCE_SOURCE) $(BIRD) $(LIBS)	



client: $(CLIENT_SOURCE)
	$(CC) $(CFLAGS) -o client $(CLIENT_SOURCE) $(LIBS)

distance: $(DISTANCE_SOURCE)
	$(CC) $(CFLAGS) -o distance $(DISTANCE_SOURCE) $(LIBS)

bird: $(BIRD)
	$(CC) $(CFLAGS) -o bird $(BIRD) $(LIBS)


clean:
	rm -f server client distance


