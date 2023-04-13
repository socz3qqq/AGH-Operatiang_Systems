CC=gcc
CFLAGS=-Wall
PROG=integral2
SUBPROG=sub_integral


integral:
	$(CC) $(CFLAGS) -o $(PROG) $(PROG).c

sub:
	$(CC) $(CFLAGS) -o $(SUBPROG) $(SUBPROG).c

all:
	make integral
	make sub

test:
	./$(PROG) 0.000000001 2 >> report.txt
	./$(PROG) 0.000000001 10 >> report.txt
	./$(PROG) 0.000000001 21 >> report.txt
	./$(PROG) 0.000000001 55 >> report.txt
	./$(PROG) 0.0000000005 2 >> report.txt
	./$(PROG) 0.0000000005 10 >> report.txt
	./$(PROG) 0.0000000005 21 >> report.txt
	./$(PROG) 0.0000000005 55 >> report.txt
	./$(PROG) 0.0000000001 2 >> report.txt
	./$(PROG) 0.0000000001 10 >> report.txt
	./$(PROG) 0.0000000001 21 >> report.txt
	./$(PROG) 0.0000000001 55 >> report.txt
	
clean:
	rm -rf $(PROG) $(SUBPROG)