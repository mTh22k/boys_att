# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -std=c99 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = theboys.o libconjunto.o  liblef.o libfila.o 
     
theboys: theboys.o libconjunto.o  liblef.o libfila.o
	$(CC) -o theboys theboys.o libconjunto.o  liblef.o libfila.o $(LDFLAGS)

libconjunto.o: libconjunto.c
	$(CC) -c $(CFLAGS) libconjunto.c

liblef.o: liblef.c
	$(CC) -c $(CFLAGS) liblef.c

libfila.o: libfila.c
	$(CC) -c $(CFLAGS) libfila.c

theboys.o: theboys.c
	$(CC) -c $(CFLAGS) theboys.c

clean:
	rm -f $(objects) theboys


