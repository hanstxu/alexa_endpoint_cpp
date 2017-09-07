#include "HttpRequest.h"

HttpRequest::HttpRequest(std::string req) {
  int i = 0;
  parseReqLine(req, i);
  parseHeaders(req, i);
  body = req.substr(i, req.size() - i);
}

std::string HttpRequest::toString() const {
  std::string output = method + " " + uri + " " + version + "\r\n";
  std::map<std::string,std::string>::const_iterator it;
  for (it = headers.begin(); it != headers.end(); ++it)
    output += it->first + ":" + it->second + "\r\n";
  output += "\r\n" + body;
  return output;
}

void HttpRequest::parseReqLine(std::string req, int& i) {
  method = req[i++];
  while (req[i] != '\x20')
    method += req[i++];
  while (req[i] == ' ')
    i++;
  while (req[i] != '\x20')
    uri += req[i++];
  while (req[i] == ' ')
    i++;
  while (req[i] != '\r')
    version += req[i++];
  // Go to the index past /r/n
  i += 2;
}

void HttpRequest::parseHeader(std::string req, int& i) {
  std::string key, value;
  while (req[i] != ':')
    key += req[i++];
  // go past the colon
  i++;
  while (req[i] != '\r')
    value += req[i++];
  headers[key] = value;
  // Go to the index past /r/n
  i += 2;
}

void HttpRequest::parseHeaders(std::string req, int& i) {
  while (req[i] != '\r') {
    parseHeader(req, i);
  }
  // Go to the index past /r/n
  i += 2;
}