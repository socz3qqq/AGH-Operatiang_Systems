CC=gcc
CFLAGS=-Wall
SERVER=server
CLIENT=client

all:
	make server
	make client

server:
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER).c

client:
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT).c

clean:
	rm -rf $(SERVER) $(CLIENT)