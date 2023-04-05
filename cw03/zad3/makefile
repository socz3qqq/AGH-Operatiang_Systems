CC=gcc
CFLAGS=-Wall
PROG=search

all:
	$(CC) $(CFLAGS) -o $(PROG) $(PROG).c

test:
	echo "TEST 1: (.)"
	./$(PROG) "." "#include"
	echo "TEST 2: (..)"
	./$(PROG) ".." "#include"
	echo "TEST 3: (../)"
	./$(PROG) "../" "#include"
	echo "TEST 4: (/home)"
	./$(PROG) "/home/$(USER)/Desktop" "#include"
	echo "TEST 5: (/home)"
	./$(PROG) "/home/$(USER)/Documents" "#include"

clean:
	rm -rf $(PROG)