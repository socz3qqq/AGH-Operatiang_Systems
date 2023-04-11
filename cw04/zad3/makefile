CC=gcc
CFLAGS=-Wall
SENDER=sender
CATCHER=catcher

all:
	make sender
	make catcher

sender:
	$(CC) $(CFLAGS) -o $(SENDER) $(SENDER).c

catcher:
	$(CC) $(CFLAGS) -o $(CATCHER) $(CATCHER).c

clean:
	rm -rf $(SENDER) $(CATCHER)