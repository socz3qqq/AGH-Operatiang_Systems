CC = gcc
CFLAGS = -Wall

all:
	$(CC) $(CFLAGS) -o replace_sys replace_sys.c
	$(CC) $(CFLAGS) -o replace_lib replace_lib.c

test:
	./replace_lib a 4 test.txt result_lib.txt
	./replace_sys a 4 test.txt result_sys.txt 
	tr a 4 < test.txt > tmp
	diff -s tmp result_sys.txt
	diff -s tmp result_lib.txt
	rm -rf tmp

clean:
	rm -rf result* pomiar* replace_lib replace_sys