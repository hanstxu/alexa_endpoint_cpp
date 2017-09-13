# alexa_endpoint_cpp

Starter code for making a C++ web server that hosts an Amazon Alexa custom
skill.

[Background Information](https://developer.amazon.com/public/solutions/alexa/alexa-skills-kit/docs/developing-an-alexa-skill-as-a-web-service)

## Motivation

All the tutorials I've looked at for creating custom Alexa Skills use AWS
Lambda. However, AWS Lambda does not support C++.

![AWS Lambda languages](https://raw.githubusercontent.com/hanstxu/alexa_endpoint_cpp/master/screenshots/lambda_languages.png)

I think it would be cool to write a custom Alexa Skill in C++.

Also, I think that you are handicapping yourself if using AWS Lambda to write
an Alexa Skill. You should just build a web service yourself (Starter code for
Node.js: [alexa_endpoint_js](https://github.com/hanstxu/alexa_endpoint_js)).

## How to Run

This tutorial assumes that you have not coded an Alexa skill before and that
you are going to use a linux machine to host your web server.

### Registering an Alexa Skill with an Amazon Developer Account

1. Go to the [amazon developers homepage](https://developer.amazon.com/) and
login or make a new account if you have don't have one.

2. Follow the instructions from **Step 2** of [this tutorial](https://developer.amazon.com/alexa-skills-kit/alexa-skill-quick-start-tutorial)
from Amazon. Note that we are bypassing **Step 1** since we are creating our
web server to handle Alexa requests. Also, stop when your reach the last
instruction of **Step 2**. We are going to use our own endpoint instead of AWS
Lambda.

3. For the **Service Endpoint Type**, select **HTTPS** and populate the
**Default** text box with the ip address of a publicly accessible web server.
In my example screenshot below, I'm using a temporary IP address of an AWS
EC2 instance.

![Endpoint configuration](https://raw.githubusercontent.com/hanstxu/alexa_endpoint_cpp/master/screenshots/endpoint_conf.png)

4. Select **No** for the rest of the settings, leave the rest of the checkboxes
blank, and select **Next** to continue the registration.

5. Follow the instructions [here](https://github.com/hanstxu/alexa_endpoint_cpp/tree/master/certificates)
to create a **self-signed certificate**.

6. You should now be ready to test your Alexa skill.

### Running the Web Server

1. Make sure that whichever linux machine that you're going to use to run the
web server is publicly accessible on the internet (whether through port
forwarding or through an IaaS). In my example, I used a **t2.micro** [AWS EC2
instance](https://aws.amazon.com/ec2/) and configured the **security group**
so that my instance would accept **inbound** connections on **port 443**.

2. Open up a **terminal** session and make sure that **git**, **openssl**,
and **libssl-dev** are installed. Otherwise, you can install them with the
following commands.
```bash
apt-get install git
apt-get install openssl
apt-get install libssl-dev
```

3. Clone the repository.
```bash
git clone https://github.com/hanstxu/alexa_endpoint_cpp.git
```

4. Build the project.
```bash
cd alexa_endpoint_cpp
make
```

5. If you haven't done so already, make your own **self-signed certificates**.
Instructions are [here](https://github.com/hanstxu/alexa_endpoint_cpp/tree/master/certificates).
I have some example certificates in my repository to show you what they should
look like but you should really use your own.

6. Run the server.
```bash
sudo ./server <port> <cert-file> <key-file>
```

```bash
sudo ./server 443 certificates/certificate.pem certificates/private-key.pem
```

### Test your Alexa Skill

While the server is running, you can test your skill using your own Alexa
device, the **Service Simulator** of your Alexa skill in the **Amazon developer
portal**, or [echosim.io](https://echosim.io/). (source: **Step 3** from the
[previous tutorial](https://developer.amazon.com/alexa-skills-kit/alexa-skill-quick-start-tutorial)).

Below is an example screenshot of what to expect when using the **Service
Simulator**.

![test result](https://raw.githubusercontent.com/hanstxu/alexa_endpoint_cpp/master/screenshots/test_result.png)

### Understanding the Alexa Skill Flow

**tl;dr** Amazon will send a JSON object to your web server and expects
a JSON object back. Both these objects are wrapped by HTTP with a header
field that specifies that the **Content-Type** is **application/json**.

![Alexa flow](https://raw.githubusercontent.com/hanstxu/alexa_endpoint_cpp/master/screenshots/alexa_flow.png)

Here is an example of a JSON Request object

```javascript
{
  "session": {
    "new": true,
    "sessionId": "SessionId.4a78423d-e483-44de-bb44-9656768d801b",
    "application": {
      "applicationId": "amzn1.ask.skill.08456690-5174-4506-8696-40dc5a046dec"
    },
    "attributes": {},
    "user": {
      "userId": "amzn1.ask.account.AFBTHJWS2A3YSSBVSPD35HCI47QCBTBPYNDT2JUBITCO3UP3K6LLM52MY23M52SLRXPICFFH7SYI4O2TS5PBMP4DA5TMDAY3LUA2MGGRRMXWLIKO4UH7CDVMWBF6SR2HGDSCUW3OXYRYPE3UBGAXD3FYFNEA4LJGS77HJ372QDZDE7B6YVO5V3SYEI5HQRVFM6WIXVHUBCNFE3I"
    }
  },
  "request": {
    "type": "IntentRequest",
    "requestId": "EdwRequestId.3d2072d2-27c5-4a2a-976f-6ce74b89b18b",
    "intent": {
      "name": "testIntent",
      "slots": {}
    },
    "locale": "en-US",
    "timestamp": "2017-09-11T21:34:50Z"
  },
  "context": {
    "AudioPlayer": {
      "playerActivity": "IDLE"
    },
    "System": {
      "application": {
        "applicationId": "amzn1.ask.skill.08456690-5174-4506-8696-40dc5a046dec"
      },
      "user": {
        "userId": "amzn1.ask.account.AFBTHJWS2A3YSSBVSPD35HCI47QCBTBPYNDT2JUBITCO3UP3K6LLM52MY23M52SLRXPICFFH7SYI4O2TS5PBMP4DA5TMDAY3LUA2MGGRRMXWLIKO4UH7CDVMWBF6SR2HGDSCUW3OXYRYPE3UBGAXD3FYFNEA4LJGS77HJ372QDZDE7B6YVO5V3SYEI5HQRVFM6WIXVHUBCNFE3I"
      },
      "device": {
        "supportedInterfaces": {}
      }
    }
  },
  "version": "1.0"
}
```

* Important parameters to note here are **applicationId**, **type**,and
**intent.name**. These are basic variables that you can build on to decide
on how you want to respond.

Here is an example of a valid JSON Response object

```javascript
{
  "version": "1.0",
  "response": {
    "shouldEndSession": true,
    "outputSpeech": {
      "text": "You invoked this skill with WhatsMyColorIntent",
      "type": "PlainText"
    }
  }
}
```

* Note that with this response, an Alexa device would respond with "You invoked
this skill with WhatsMyColorIntent" and end the conversation.

More detailed information about these two JSON objects can be found
[here](https://developer.amazon.com/public/solutions/alexa/alexa-skills-kit/docs/alexa-skills-kit-interface-reference).

With regards to the starter C++ code, alexa.cpp is the place you want to look
for writing logic to respond to different Alexa requests. It contains an
**invokeSkill** function which is analogous to the **exports.handler** function
that AWS Lambda provides for Node.js. It also makes use of the
[simple_json_cpp](https://github.com/hanstxu/simple_json_cpp) library to handle
the json, but you're welcome to use other json libraries and rewrite
alexa.h/alexa.cpp. Fundamentally, all you're doing when creating an Alexa skill
is reading a JSON object and writing a JSON object to send back.

```c++
JSONObject invokeSkill(JSONObject req) {
  std::string app_id = getApplicationId(req);
  
  // Populate this with your own application id
  if (app_id != "TODO: fill in with your Skill application id")
    return JSONObject();
	  
  std::string req_type = getRequestType(req);
  if (req_type == "LaunchRequest")
    return createResponse("You invoked this skill with a launch request.");
  else if (req_type == "IntentRequest")
    return createResponse("You invoked this skill with " + getIntentName(req) + ".");
  
  return createResponse("You invoked this skill with an unknown request.");
}
```

*Your skill* **_Application Id_** *found on the Amazon developer portal as
shown in the screenshot below.*

![Application Id](https://raw.githubusercontent.com/hanstxu/alexa_endpoint_cpp/master/screenshots/application_id.png)