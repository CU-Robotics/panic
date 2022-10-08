#include <Arduino.h>
#include <TM1637Display.h>
#include "WiFi.h"
// #include "ESPAsyncWebServer.h"

/* rotary encoder to seven segment */
//D15 -> SW on encoder
//D2 -> DT on encoder
//D4 -> CLK on encoder

//define connections pins
#define CLK 27
#define DIO 14
#define OUTPUT_A 4 //CLK
#define OUTPUT_B 2 //D

#define BIG_BUTTON 5

#define SELECT_PIN 15 //button on encoder

int oldCount = 0;
int counter = 0;

unsigned long lastEnc = 0;

bool bState = false;

String SSID = "";
// bool connected

//create display object of type TM1637
TM1637Display display = TM1637Display(CLK, DIO); //not quite sure about this line

//array that turns all segments on
const uint8_t data[] = {0xff, 0xff, 0xff, 0xff};

//array that turns all segments off
const uint8_t blank[] = {0x00, 0x00, 0x00, 0x00};

void IRAM_ATTR encoder_ISR() {
  if (micros() - lastEnc > 100000)
  {
    lastEnc = micros();
    // bState = digitalRead(OUTPUT_B);
    if (digitalRead(OUTPUT_B))
    {
      counter++;
    } else {
      if (counter > 0)
      {
        counter--;
      }
      // counter--;
    }
  }
}

void IRAM_ATTR select_ISR() {
  SSID = "robot" + String(counter);
}

void setup(){
  display.clear();
  //set brightness
  display.setBrightness(7);
  display.showNumberDec(counter);

  pinMode(OUTPUT_A, INPUT);
  pinMode(OUTPUT_B, INPUT);

  attachInterrupt(OUTPUT_A, encoder_ISR, RISING);

  pinMode(BIG_BUTTON, INPUT_PULLDOWN);


  pinMode(SELECT_PIN, INPUT_PULLDOWN);
  attachInterrupt(SELECT_PIN, select_ISR, RISING);

  Serial.begin(9600);

}

void loop(){
  if (oldCount != counter)
  {
    display.showNumberDec(counter);
    oldCount = counter;
    Serial.print(counter);
    Serial.print(", ");
    Serial.println(millis());
  }

  if (/* condition */)
  {
    /* code */
  }
  
}