CC=gcc
CFLAGS=-Wall -Wno-unused-value

caltrain: Indian-Rail.c IndianRail.c IndianRail.h
	$(CC) -c IndianRail.c  -o IndianRail.o
	$(CC) -c Indian-Rail.c -o Indian-Rail.o
	$(CC) $(CFLAGS) IndianRail.o Indian-Rail.o -o a.out -lpthread
	

