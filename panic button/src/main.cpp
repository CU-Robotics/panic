#include <Arduino.h>
#include <TM1637Display.h>
#include <Encoder.h>

//display pins
#define CLK 27
#define DIO 14

//encoder pins
#define OUTPUT_A 4 //CLK
#define OUTPUT_B 2 //D

#define BIG_BUTTON 34

#define SELECT_PIN 15 //button on encoder

//create display object of type TM1637
TM1637Display display = TM1637Display(CLK, DIO); //not quite sure about this line

Encoder myEnc(5, 6);
long oldPosition  = -999;

int counter = 0;

//array that turns all segments on
const uint8_t allOn[] = {0xff, 0xff, 0xff, 0xff};

//array that turns all segments off
const uint8_t blank[] = {0x00, 0x00, 0x00, 0x00};

void setup() {
    display.clear();

    display.setBrightness(7);
    display.showNumberDec(counter);

    pinMode(BIG_BUTTON, INPUT_PULLDOWN);
    pinMode(SELECT_PIN, INPUT_PULLUP);

    Serial.begin(9600);
    Serial.println("Big red button");   
}

void loop() {
    long newPosition = myEnc.read();
    if (newPosition != oldPosition) {
        oldPosition = newPosition;
        Serial.println(newPosition);
    }
}