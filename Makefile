# Makefile per il simulatore di circuiti quantistici in un contesto multithread.
#
# Target:
#   make         compila il programma 'qsim'
#   make clean   rimuove eseguibile e file oggetto

CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -O2 -pthread
LDFLAGS = -pthread
LDLIBS  = -lm

TARGET  = qsim

OBJS = \
    main.o \
    parser.o \
    matrix.o \
    threads.o \
    circuit.o \
    measure.o \
    complex.o

$(TARGET): $(OBJS)
    $(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

# Dipendenze dagli header .
main.o:      main.c parser.h circuit.h matrix.h complex.h measure.h threads.h
parser.o:    parser.c parser.h matrix.h complex.h circuit.h
matrix.o:    matrix.c matrix.h complex.h
threads.o:   threads.c threads.h matrix.h complex.h
circuit.o:   circuit.c circuit.h matrix.h complex.h threads.h
measure.o:   measure.c measure.h complex.h
complex.o:   complex.c complex.h

%.o: %.c
    $(CC) $(CFLAGS) -c -o $@ $<

clean:
    rm -f $(TARGET) $(OBJS)

.PHONY: clean