#include "HttpResponse.h"

HttpResponse::HttpResponse(std::string version, std::string status,
  std::string reason) {
  this->version = version;
  this->status = status;
  this->reason = reason;
}

void HttpResponse::addHeader(std::string key, std::string value) {
  headers[key] = value;
}

void HttpResponse::addBody(std::string body) {
  this->body = body;
}

std::string HttpResponse::toString() const {
  std::string output = version + " " + status + " " + reason + "\r\n";
  std::map<std::string,std::string>::const_iterator it;
  for (it = headers.begin(); it != headers.end(); ++it)
    output += it->first + ": " + it->second + "\r\n";
  output += "\r\n" + body;
  return output;
}