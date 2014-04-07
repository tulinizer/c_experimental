CC      = gcc
CFLAGS  = -pthread

all : contest main clean

contest: contest.o
	$(CC) $(CFLAGS) -o contest contest.o
contest.o: contest.c
	$(CC) -c $(CFLAGS) contest.c

main: main.o
	$(CC) $(CFLAGS) -o main main.o
main.o: main.c
	$(CC) -c $(CFLAGS) main.c

.PHONY: clean realclean 
clean: 
	$(RM) *.o
realclean : clean 
	$(RM) contest main

