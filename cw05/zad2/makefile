CC=gcc
CFLAGS=-Wall
PROG=integral


all:
	$(CC) $(CFLAGS) -o $(PROG) $(PROG).c

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
	rm -rf $(PROG)