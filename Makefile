CXX=g++
CXXOPTIMIZE= -O2
CXXDEBUG= -g
CXXFLAGS= -Wall -std=c++0x $(CXXOPTIMIZE)
CLASSES=HttpRequest.h HttpRequest.cpp

all: server

server: $(CLASSES)
		$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp

clean:
	rm -rf *.o server