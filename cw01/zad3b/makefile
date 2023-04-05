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

o0:
	make all OFLAGS=-O0
o1:
	make all OFLAGS=-O1
o2:
	make all OFLAGS=-O2
o3:
	make all OFLAGS=-O3
os:
	make all OFLAGS=-Os

test:
	sudo cp libwordcount.so /usr/lib
	make gen_test
	./repl_static < test.in 1> results_static_$(OFLAGS).txt
	./repl_shared < test.in 1> results_shared_$(OFLAGS).txt

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

zad3b:
	make o0
	make test OFLAGS="O0"

	make o1
	make test OFLAGS=O1

	make o2
	make test OFLAGS=O2

	make o3
	make test OFLAGS=O3

	make os
	make test OFLAGS=Os


static: $(LIB).c
	gcc $(CFLAGS) $(OFLAGS) -c $(LIB).c
	ar cr lib$(LIB).a $(LIB).o

shared: $(LIB).c
	$(CC) $(CFLAGS) $(OFLAGS) -c -fPIC $(LIB).c
	$(CC) $(CFLAGS) $(OFLAGS) -shared -o lib$(LIB).so $(LIB).o

prog_static: $(LIB).c $(REPL)
	$(CC) $(CFLAGS) $(OFLAGS) $(REPL) -o repl_static -L . lib$(LIB).a

prog_shared: $(LIB).c $(REPL)
	$(CC) $(CFLAGS) $(OFLAGS) $(REPL) -o repl_shared -L . lib$(LIB).so

clean:
	rm -rf *.o *.so *.a repl_* results_*