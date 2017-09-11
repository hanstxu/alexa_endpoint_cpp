#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>

class HttpResponse {
public:
  HttpResponse(std::string version, std::string status, std::string reason);
  void addHeader(std::string key, std::string value);
  void addBody(std::string body);
  std::string toString() const;
private:
  std::string version;
  std::string status;
  std::string reason;
  std::map<std::string, std::string> headers;
  std::string body;
};

#endif