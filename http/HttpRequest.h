#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <map>

class HttpRequest {
public:
  HttpRequest(std::string req);
  std::string toString() const;
  std::string getBody() const;
private:
  void parseReqLine(std::string req, int& i);
  void parseHeader(std::string req, int& i);
  void parseHeaders(std::string req, int& i);
  std::string method;
  std::string uri;
  std::string version;
  std::map<std::string, std::string> headers;
  std::string body;
};

#endif