# alexa_endpoint_cpp

Starter code for making a C++ web server that hosts an Amazon Alexa custom
skill.

[Background Information](https://developer.amazon.com/public/solutions/alexa/alexa-skills-kit/docs/developing-an-alexa-skill-as-a-web-service)

### Motivation

All the tutorials I've looked at for creating custom Alexa Skills use AWS Lambda.
However, AWS Lambda does not support C++.

![AWS Lambda languages](https://raw.githubusercontent.com/hanstxu/alexa_endpoint_cpp/master/screenshots/lambda_languages.png)

So, I started this project to give people the option of writing a custom Alexa
Skill in C++.

Also, writing code for AWS Lambda is a subpar experience  (e.g. I have to go
to CloudWatch to look at console.log() outputs from AWS Lambda and adding
external libraries/modules is a hassle). I wanted to showcase how you could
create an Alexa custom skill without using AWS Lambda and in the future,
I'll probably reimplement this entire project using Node.js.