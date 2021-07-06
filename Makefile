TARGET=client server 
CC=gcc
CFLAGS= -Wall -Wextra -g
normal: $(TARGET)
client: client.c
	$(CC) $(CFLAGS) client.c -o client
server: server.c
	$(CC) $(CFLAGS) server.c -o server
clean:
	$(RM) $(TARGET)


#all: libcalc test client server



#servermain.o: servermain.cpp
#	$(CXX) -Wall -c servermain.cpp -I.

#clientmain.o: clientmain.cpp
#	$(CXX) -Wall -c clientmain.cpp -I.

#main.o: main.cpp
#	$(CXX) -Wall -c main.cpp -I.


#test: main.o calcLib.o
#	$(CXX) -L./ -Wall -o test main.o -lcalc

#client: clientmain.o calcLib.o
#	$(CXX) -L./ -Wall -o client clientmain.o -lcalc

#server: servermain.o calcLib.o
#	$(CXX) -L./ -Wall -o server servermain.o -lcalc




#calcLib.o: calcLib.c calcLib.h
#	gcc -Wall -fPIC -c calcLib.c

#libcalc: calcLib.o
#	ar -rc libcalc.a -o calcLib.o

#clean:
#	rm *.o *.a test server client
