CXX=g++
CXXOPTIMIZE= -O2
CXXDEBUG= -g
CXXFLAGS= -Wall -std=c++0x $(CXXOPTIMIZE)
CLASSES=http/HttpRequest.h http/HttpRequest.cpp http/HttpResponse.h \
  http/HttpResponse.cpp alexa.h alexa.cpp
LDLIBS= -lssl -lcrypto
JSON=libjson/JSONObject.cpp libjson/JSONArray.cpp

all: json server

json: $(JSON)
	$(CXX) -c $(CXXFLAGS) $^
	ar -cvq libjson.a JSONArray.o JSONObject.o
	rm -rf JSONArray.o JSONObject.o

server: $(CLASSES)
		$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp $(LDLIBS) libjson.a

clnserv:
	rm -rf server

clean:
	rm -rf *.o server libjson.a *.h.gch