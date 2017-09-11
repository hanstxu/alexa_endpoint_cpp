# alexa_endpoint_cpp

Starter code for making a C++ web server that hosts an Amazon Alexa custom
skill.

[Background Information](https://developer.amazon.com/public/solutions/alexa/alexa-skills-kit/docs/developing-an-alexa-skill-as-a-web-service)

### Motivation

All the tutorials I've looked at for creating custom Alexa Skills use AWS Lambda.
However, AWS Lambda does not support C++.

![AWS Lambda languages](https://raw.githubusercontent.com/hanstxu/alexa_endpoint_cpp/master/screenshots/lambda_languages.png)

I think it would be cool to write a custom Alexa Skill in C++.

Also, I think that you are handicapping yourself if using AWS Lambda to write
an Alexa Skill. You should just build a web service yourself (I might start
another project to show how to do that with Node.js).

### Note

apt-get install libssl-dev to use openssl libraries

Finished the code and it works and reorganized everything

TODO: write a tutorial on how to make an Alexa skill

TODO: reimplement this with Node.js (should be pretty quick)