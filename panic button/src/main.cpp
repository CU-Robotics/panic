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

int counter = 0;

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

void IRAM_ATTR encoder_ISR()
{
  if (digitalRead(OUTPUT_B))
  {
    counter++;
  } else {
    if (counter > 0)
    {
      counter--;
    }
    
  }
  
}

void setup(){
  /* rotary encoder to seven segment */
  //clear display
  display.clear();

  pinMode(OUTPUT_A, INPUT);
  pinMode(OUTPUT_B, INPUT);

  pinMode(BIG_BUTTON, INPUT_PULLDOWN);

  attachInterrupt(OUTPUT_A, encoder_ISR, RISING);

  Serial.begin(9600);

  //Serial.begin(115200);
  Serial.println();
}

void loop(){
  //set brightness
  display.setBrightness(7);

  //https://www.best-microcontroller-projects.com/rotary-encoder.html

  static uint16_t state = 0, counter = 0;

}