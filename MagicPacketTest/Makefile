CC=g++
CFLAGS=-std=c++11 -I. -Wall -Wextra -ggdb -O0 -Wunreachable-code

%.o: %.cpp $(DEPS)
		$(CC) -c -g -o $@ $< $(CFLAGS)

Test: Test.o fakeEEPROM.o 
		$(CC) $(CFLAGS) -o MagicPacketTest Test.o fakeEEPROM.o

make clean:
	  rm *.o MagicPacketTest

