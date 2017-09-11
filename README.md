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
an Alexa Skill. You should just build a web service yourself (I might start
another project to show how to do that with Node.js).

## How to Run

This tutorial assumes that you have not coded an Alexa skill before and that
you are going to use a linux machine to host your web server.

### Registering an Alexa Skill with an Amazon Developer Account

1. Go to the [amazon developers homepage](https://developer.amazon.com/) and
login or make a new account if you have don't have one.

2. Follow the instructions from **Step 2** of [this tutorial](https://developer.amazon.com/alexa-skills-kit/alexa-skill-quick-start-tutorial)
from Amazon. Note that we are bypassing **Step 1** since we are creating our
web server to handle Alexa requests. Also, stop when your reach the last
instruction of **Step 2**. We are going to make our own endpoint instead of
using AWS Lambda.

3. For the **Service Endpoint Type**, select **HTTPS** and populate the
**Default** text box with the ip address of a publicly accessible web server.
In my example screenshot below, I'm using a temporary IP address of an AWS
EC2 instance.

![Endpoint configuration](https://raw.githubusercontent.com/hanstxu/alexa_endpoint_cpp/master/screenshots/endpoint_conf.png)

4. Select **No** for the rest of the settings, leave the rest of the checkboxes
blank, and select **Next** to continue the registration.

5. Follow the instructions [here] to create a **self-signed certificate**.

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
```
make
```

5. If you haven't done so already, make your own **self-signed certificates**.
Instructions are [here]. I have some example certificates in my repository to
show you what they should look like but you should really use your own.

6. Run the server.
```bash
sudo ./server <port> <cert-file> <key-file>
```

```bash
sudo ./server 443 certificates/certificate.pem certificates/private-key.pem
```

### Test your Alexa Skill

1. While the server is running, you can test your skill using your own Alexa
device, the **Service Simulator** of your Alexa skill in the **Amazon developer
portal**, or [echosim.io](https://echosim.io/). (source: **Step 3** from the
[previous tutorial](https://developer.amazon.com/alexa-skills-kit/alexa-skill-quick-start-tutorial)).

### Understanding the Alexa Skill Flow

### Notes

Finished the code and it works and reorganized everything

TODO: write a tutorial on how to make an Alexa skill

TODO: reimplement this with Node.js (should be pretty quick)