#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#define IN1 14
#define IN2 27

/* ESP32 client-server wifi communication between two boards */
//set access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

//create AsyncWebServer object on port 80
AsyncWebServer server(80);

unsigned long lastReceived;
bool inDistress = false;

void setup(){
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);

  /* ESP32 client-server wifi communication between two boards */
  // Serial port for debugging purposes
  //Serial.begin(115200);
  //Serial.println();
  
  //Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/connectedToRobot", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "all good");
    lastReceived = millis();
  });

  server.on("/distress", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "panic");
    inDistress = true;
  });

  server.begin();
}

void loop(){
  // if(millis() >= 2000){ //change condition to when button is pressed
  // digitalWrite(IN1, LOW);
  // digitalWrite(IN2, LOW);
  // }

  if(lastReceived > 500 || inDistress == true){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
}