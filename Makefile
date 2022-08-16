CC      = gcc

CPPC    = g++
ARGS    = -std=c++17 -Wall --pedantic-errors

PROGRAM = main

NET		= ./network/
NETOBJ1 = network

OTHERS  = ./others/

URL     = http://192.168.56.103:8080/CHaserOnline003/user/
SKRURL	= http://www7019ug.sakura.ne.jp:80/CHaserOnline003/user/
USERNM  = cool91
PASSWD  = cool
ROOM    = 4008

all: $(PROGRAM)

$(NETOBJ1).o: $(NET)$(NETOBJ1).c
	$(CC) -c $(NET)$(NETOBJ1).c

main.o: main.c
	$(CC) -c main.c

$(PROGRAM): $(NETOBJ1).o main.o
	$(CC) $(NETOBJ1).o main.o -o $(PROGRAM)
	rm -f *.o

run: $(PROGRAM)
	./$(PROGRAM) $(URL) -u $(USERNM) -p $(PASSWD) -r $(ROOM)

skrun: $(PROGRAM)
	./$(PROGRAM) $(SKRURL) -u $(USERNM) -p $(PASSWD) -r $(ROOM)

clean:
	rm -f *.o
	rm -f main
