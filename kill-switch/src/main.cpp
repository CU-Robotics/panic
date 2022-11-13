#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#define IN1 14
#define IN2 27

/* ESP32 client-server wifi communication between two boards */
//set access point network credentials
const char* ssid = "robot1";
const char* password = "123456789";

//create AsyncWebServer object on port 80
AsyncWebServer server(80);
// WiFiServer server(80);

unsigned long lastReceived = INT_MAX;
unsigned long inDistress = 0;

void setup(){
  delay(1000);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);

  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  
  //Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/clear", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "all good");
    lastReceived = millis();
    Serial.println("connected");
  });

  server.on("/panic", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "panic");
    inDistress = millis();
    Serial.println("distress");
  });

  server.begin();
}

void loop(){
  if((millis() - lastReceived > 500) || (millis() - inDistress < 500)){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  if((millis() - lastReceived < 500) && (millis() - inDistress > 500)){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
  }
}