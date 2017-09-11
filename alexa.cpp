#include "alexa.h"

std::string getApplicationId(JSONObject req) {
  return req.get<JSONObject>("session").get<JSONObject>("application").
    get<std::string>("applicationId");
}

std::string getRequestType(JSONObject req) {
  return req.get<JSONObject>("session").get<JSONObject>("application").
    get<JSONObject>("user").get<JSONObject>("request").
    get<std::string>("type");
}

std::string getIntentName(JSONObject req) {
  return req.get<JSONObject>("session").get<JSONObject>("application").
    get<JSONObject>("user").get<JSONObject>("request").
    get<JSONObject>("intent").get<std::string>("name");
}

JSONObject createPlainText(std::string text) {
  JSONObject obj;
  obj.add<std::string>("type", "PlainText");
  obj.add<std::string>("text", text);
  return obj;
}

JSONObject createResponse (std::string text) {
  JSONObject response_obj;
  response_obj.add<JSONObject>("outputSpeech", createPlainText(text));
  response_obj.add<bool>("shouldEndSession", true);
  
  JSONObject response;
  response.add<std::string>("verison", "1.0");
  response.add<JSONObject>("response", response_obj);
  return response;
}

JSONObject invokeSkill(JSONObject req) {
  std::string app_id = getApplicationId(req);
  // TODO: make sure that your web service only responds to your skill
  //if (app_id != "")
  
  std::string req_type = getRequestType(req);
  if (req_type == "LaunchRequest")
    return createResponse("You invoked this skill with a launch request.");
  else if (req_type == "IntentRequest")
    return createResponse("You invoked this skill with " + getIntentName(req) + ".");
  
  return createResponse("You invoked this skill with an unknown request.");
}