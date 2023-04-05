CC=gcc
CFLAGS=-Wall
LIB=wordcount
REPL=repl.c
count := $(shell ls | wc -l)


all:
	make static
	make shared
	make prog_static
	make prog_shared

test:
	sudo cp libwordcount.so /usr/lib
	make gen_test
	./repl_static < test.in 1> results_static.txt
	./repl_shared < test.in 1> results_shared.txt

gen_test:
	echo "init 100" > test.in
	@for file in `ls`; do \
		`echo "count $$file" >> test.in`; \
	done
	@for i in $$(seq 1 $(count)); do \
		`echo "show $$i" >> test.in`; \
	done

	@for i in $$(seq 1 $(count)); do \
		`echo "delete $$i" >> test.in`; \
	done

	echo "destroy" >> test.in
	echo "^D" >> test.in

static: $(LIB).c
	gcc $(CFLAGS) -c $(LIB).c
	ar cr lib$(LIB).a $(LIB).o

shared: $(LIB).c
	$(CC) $(CFLAGS) -c -fPIC $(LIB).c
	$(CC) $(CFLAGS) -shared -o lib$(LIB).so $(LIB).o

prog_static: $(LIB).c $(REPL)
	$(CC) $(CFLAGS) $(REPL) -o repl_static -L . lib$(LIB).a

prog_shared: $(LIB).c $(REPL)
	$(CC) $(CFLAGS) $(REPL) -o repl_shared -L . lib$(LIB).so

clean:
	rm -rf *.o *.so *.a *.exe