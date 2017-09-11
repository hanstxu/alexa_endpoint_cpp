CXX=g++
CXXOPTIMIZE= -O2
CXXDEBUG= -g
CXXFLAGS= -Wall -std=c++0x $(CXXOPTIMIZE)
CLASSES=HttpRequest.h HttpRequest.cpp alexa.h alexa.cpp HttpResponse.h \
  HttpResponse.cpp
LDLIBS= -lssl -lcrypto
JSON=JSONObject.cpp JSONArray.cpp

all: libjson server

libjson: $(JSON)
	$(CXX) -c $(CXXFLAGS) $^
	ar -cvq $@.a JSONArray.o JSONObject.o
	rm -rf JSONArray.o JSONObject.o

server: $(CLASSES)
		$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp $(LDLIBS) libjson.a

clnserv:
	rm -rf server

clean:
	rm -rf *.o server libjson.a *.h.gch