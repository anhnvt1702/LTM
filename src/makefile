CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LDFLAGS = -lssl -lcrypto `pkg-config --libs gtk+-3.0`

all: server client

server: server.c online.c online.h
	$(CC) server.c online.c -o server -lpthread -lssl -lcrypto

client: client.c signin.c signin.h chessgame.c chessgame.h online.c online.h createwindow.h createwindow.c cchess.c cchess.h callback.c callback.h  network.c network.h
	$(CC) $(CFLAGS) client.c signin.c chessgame.c online.c createwindow.c cchess.c callback.c network.c -o client $(LDFLAGS)

.PHONY: clean

runs:
	./server 5501
runc:
	./client 5501
clean:
	rm -f server client

