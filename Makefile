CXX=g++
CXXOPTIMIZE= -O2
CXXDEBUG= -g
CXXFLAGS= -Wall -std=c++0x $(CXXOPTIMIZE)
CLASSES=HttpRequest.h HttpRequest.cpp
LDLIBS= -lssl -lcrypto

all: server

server: $(CLASSES)
		$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp $(LDLIBS)

clean:
	rm -rf *.o server