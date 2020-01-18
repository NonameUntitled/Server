CPP = g++

CPP_FLAGS = -c -Wall

SRC = serverMain.cpp Server.cpp

TARGETS = myServer

.PHONE: all clean

all: myServer

myServer: serverMain.o Server.o
	$(CPP) $^ -o $@

serverMain.o: serverMain.cpp
	$(CPP) $(CPP_FLAGS) $^ -o $@

Server.o: Server.cpp
	$(CPP) $(CPP_FLAGS) $^ -o $@

clean:
	rm -rf *.o myServer
