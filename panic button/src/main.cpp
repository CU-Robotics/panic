#include <Arduino.h>
#include <TM1637Display.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

/* rotary encoder to seven segment */
//D15 -> SW on encoder
//D2 -> DT on encoder
//D4 -> CLK on encoder

//define connections pins
#define CLK 27
#define DIO 14
#define outputA 4 //CLK
#define outputB 2 //D

//create display object of type TM1637
TM1637Display display = TM1637Display(CLK, DIO); //not quite sure about this line

//array that turns all segments on
const uint8_t data[] = {0xff, 0xff, 0xff, 0xff};

//array that turns all segments off
const uint8_t blank[] = {0x00, 0x00, 0x00, 0x00};

const uint8_t dead[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,   // A
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G            // d
};

const uint8_t beef[] = {
  SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,           // b
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
  SEG_A | SEG_E | SEG_F | SEG_G                    // f
};

const uint8_t one[] = {0x00, 0x00, 0x00, SEG_B | SEG_C}; //1
const uint8_t two[] = {0x00, 0x00, 0x00, SEG_A | SEG_B | SEG_D | SEG_E | SEG_G}; //2
const uint8_t three[] = {0x00, 0x00, 0x00, SEG_A | SEG_B | SEG_C | SEG_D | SEG_G}; //3
const uint8_t four[] = {0x00, 0x00, 0x00, SEG_B | SEG_C | SEG_F | SEG_G}; //4
const uint8_t five[] = {0x00, 0x00, 0x00, SEG_A | SEG_C | SEG_D | SEG_F | SEG_G}; //5
const uint8_t six[] = {0x00, 0x00, 0x00, SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G}; //6
const uint8_t seven[] = {0x00, 0x00, 0x00, SEG_A | SEG_B | SEG_C}; //7
const uint8_t eight[] = {0x00, 0x00, 0x00, SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G}; //8
const uint8_t nine[] = {0x00, 0x00, 0x00, SEG_A | SEG_B | SEG_C | SEG_F | SEG_G}; //9


/* ESP32 client-server wifi communication between two boards */
//set access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

//create AsyncWebServer object on port 80
AsyncWebServer server(80);

//function to get connected robot number
String counterStr = "";
String oneStr = "1";
String twoStr = "2";
String threeStr = "3";
String fourStr = "4";
String readRobotConnected() {
  return counterStr;
}

void setup(){
  /* rotary encoder to seven segment */
  //clear display
  display.clear();

  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);

  Serial.begin(9600);

  //Serial.begin(115200);
  Serial.println();

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

  server.on("/connectedRobot", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readRobotConnected().c_str());
  });

  server.begin();
}

void loop(){
  //set brightness
  display.setBrightness(7);

  // //all segments on
  // display.setSegments(data);
  

  // display.setSegments(dead);
  // delay(1000);
  // display.setSegments(beef);
  // delay(1000);

  //https://www.best-microcontroller-projects.com/rotary-encoder.html

  static uint16_t state = 0, counter = 0;

  state = (state << 1) | digitalRead(outputA) | 0xe000; //digital filter is made up of a single 16 bit integer variable into which you shift the current state of the input pin

  if(state == 0xf000){
      state = 0x0000;
    if(digitalRead(outputB)){
      counter--;
    } else {
      counter++;
    }
    counter = counter % 4;
    //counter = counter + 1;
    Serial.println(counter);

    if(counter == 0){
      display.setSegments(one);
      counterStr = oneStr;
    } else if (counter == 1){
      display.setSegments(two);
      counterStr = twoStr;
    } else if (counter == 2){
      display.setSegments(three);
      counterStr = threeStr;
    } else if (counter == 3){
      display.setSegments(four);
      counterStr = fourStr;
    }
  }
}