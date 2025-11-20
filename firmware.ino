#include <DelibWlan.h> // include WLAN module

#define RELAY_PIN 14 // some pin to work with

byte mac[] = { 0x15, 0x15, 0x15, 0x15, 0x15, 0x15 }; // some mac device to assign

DelibWlan delib = DelibWlan(); // delib instance

/*
So below the most interesting part are handlers on() and off().
They contains `delib.auth_wlan_request()` which listen to client's request and determine,
whenever a request is valid/authorized. You can set secret using `delib.set_secret("YOUR_AUTH_SECRET")`.

Average request to this device should look like:

HTTP 1.1
Method: POST,
Body: YOUR_AUTH_SECRET,
...

So you just send a POST request with a secret in body.
Yea yea, this is vulnerable, childish, etc., etc., so don't do this without ssl or aes.
My lib is not about to solve this issue at this stage.
*/

void on() { // some handler when received request to http://ip_of_device:80/on
  if (delib.auth_wlan_request()) { // check if request body has a pass phrase...
    digitalWrite(RELAY_PIN, HIGH); // ...and do something if an auth is valid
  }
}

void off() { // some handler when received request to http://ip_of_device:80/off
  if (delib.auth_wlan_request()) { // check if request body has a pass phrase...
    digitalWrite(RELAY_PIN, LOW);  // ...and do something if an auth is valid
  }
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT); // pin setup

  delib.server.on("/on", on);   // add /on handler
  delib.server.on("/off", off); // add /off handler

  delib.set_wifi_credentials("your_wifi_name", "your_wifi_password"); // wifi credentials
  delib.set_secret("YOUR_AUTH_SECRET"); // request secret
  delib.set_device_name("Wawa"); // device name

  delib.init(mac); // init delib
}

void loop() {
  delib.update(); // update delib server stuff
}