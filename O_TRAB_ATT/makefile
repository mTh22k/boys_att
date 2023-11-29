# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -std=c99 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = theboys.o libconjunto.o  liblef.o libfila.o entidades.o eventos.o
     
theboys: theboys.o libconjunto.o  liblef.o libfila.o entidades.o eventos.o
	$(CC) -o theboys theboys.o libconjunto.o  liblef.o libfila.o entidades.o eventos.o $(LDFLAGS)

libconjunto.o: libconjunto.c
	$(CC) -c $(CFLAGS) libconjunto.c

liblef.o: liblef.c
	$(CC) -c $(CFLAGS) liblef.c

libfila.o: libfila.c
	$(CC) -c $(CFLAGS) libfila.c

entidades.o: entidades.c
	$(CC) -c $(CFLAGS) entidades.c

eventos.o: eventos.c
	$(CC) -c $(CFLAGS) eventos.c

theboys.o: theboys.c
	$(CC) -c $(CFLAGS) theboys.c

clean:
	rm -f $(objects) theboys


