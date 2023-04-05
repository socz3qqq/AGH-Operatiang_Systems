CC=gcc
CFLAGS=-Wall
LIB=wordcount
REPL=repl.c

all:
	make static
	make shared
	make prog_static
	make prog_shared


static: $(LIB).c
	gcc $(CFLAGS) -c $(LIB).c
	ar cr lib$(LIB).a $(LIB).o

shared: $(LIB).c
	$(CC) $(CFLAGS) -c -fPIC $(LIB).c
	$(CC) $(CFLAGS) -shared -o lib$(LIB).so $(LIB).o

prog_static: $(LIB).c $(REPL)
	$(CC) $(CFLAGS) $(REPL) -o repl_static -L . lib$(LIB).a

prog_shared: $(LIB).c $(REPL)
	$(CC) $(CFLAGS) $(REPL) -o repl_static -L . lib$(LIB).so

clean:
	rm -rf *.o *.so *.a *.exe