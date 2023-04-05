CC=gcc
CFLAGS=-Wall
PROG=exec_fun

all:
	$(CC) $(CFLAGS) -o $(PROG) $(PROG).c

test:
	mkdir new_dir
	./$(PROG) "."
	./$(PROG) "/"
	./$(PROG) "new_dir"

clean:
	rm -rf $(PROG) new_dir