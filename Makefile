CC = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)


csim: main.o Cache.o cacheHelpers.o
		$(CC) -o csim main.o Cache.o cacheHelpers.o

Cache.o: Cache.cpp Cache.h
		$(CC) -c Cache.cpp Cache.h $(CFLAGS)

cacheHelpers.o: cacheHelpers.cpp cacheHelpers.h
		$(CC) -c cacheHelpers.cpp cacheHelpers.h $(CFLAGS)

main.o: main.cpp Cache.h cacheHelpers.h
		$(CC) -c main.cpp Cache.h cacheHelpers.h $(CFLAGS)

.PHONY: clean all
clean:
		rm -f *.o *.gch csim