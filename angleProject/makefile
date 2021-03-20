#File: makefile
#Name: Paul Kummer
#Program: getCapture
#Modified: 03/13/21


EXE = getCapture
CC = g++
REMOVABLE_OBJS = main.o bmp3.o functions.o
PERM_OBJS = 
OBJECTS = $(REMOVABLE_OBJS) $(PERM_OBJS)
LFLAGS = 
CFLAGS =

$(EXE):							$(OBJECTS)
									$(CC) $(CFLAGS) -o $(EXE) $(OBJECTS) $(LFLAGS)

main.o:							main.cpp prototypes.h
									$(CC)  $(CFLAGS) -c main.cpp

bmp3.o:							bmp\bmp3.cpp prototypes.h
									$(CC) $(CFLAGS) -c bmp\bmp3.cpp

functions.o:					functions\functions.cpp prototypes.h
									$(CC) $(CFLAGS) -c functions\functions.cpp

clean:
									rm -rf $(EXE) $(REMOVABLE_OBJS) *~

#escapi.o
#escapi.o:						escapi3\escapi.cpp escapi3\escapi.h
									#$(CC) $(CFLAGS) -c escapi3\escapi.cpp