CC=gcc
CFLAGS=-Wall
SALON=hair_salon
CLIENT=client
HAIRDRESSER=hairdresser
UTIL=sem.c haircut_queue.c shared_memory.c

all: 
	make salon
	make client
	make hairdresser
salon:
	$(CC) $(CFLAGS) -o $(SALON) $(SALON).c  $(UTIL)
client:
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT).c $(UTIL)
hairdresser:
	$(CC) $(CFLAGS) -o $(HAIRDRESSER) $(HAIRDRESSER).c $(UTIL)
clean:
	rm -rf $(CLIENT) $(SALON) $(HAIRDRESSER) *.o


