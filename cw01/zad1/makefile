CC=gcc
CFLAGS=-c -Wall
LIB = wordcount

all:
	make make_static
	make make_shared

make_static: $(LIB).c
	$(CC) $(CFLAGS) $(LIB).c
	ar cr lib$(LIB).a $(LIB).o

make_shared: $(LIB).c
	$(CC) $(CFLAGS) -fPIC $(LIB).c
	$(CC) -shared -Wl,-soname,lib$(LIB).so.1 -o lib$(LIB).so.1.0.1 $(LIB).o
	ln -s lib$(LIB).so.1.0.1 lib$(LIB).so.1
	ln -s lib$(LIB).so.1 lib$(LIB).so

clean:
	rm *.o *.a *.so*