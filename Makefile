CC=g++
CFLAGS=-I -Wall
DEPS = list.h error.h
OBJ = main.o list.o error.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

list: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean :
	rm *.o
