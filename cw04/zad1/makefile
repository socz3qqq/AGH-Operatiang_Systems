CC=gcc
CFLAGS=-Wall
PROG=inherit
PROG2=signal_for_exec

all:
	$(CC) $(CFLAGS) -o $(PROG) $(PROG).c
	$(CC) $(CFLAGS) -o $(PROG2) $(PROG2).c

test:
	./$(PROG) ignore
	./$(PROG) handler
	./$(PROG) mask
	./$(PROG) pending
clean:
	rm -rf $(PROG) $(PROG2)