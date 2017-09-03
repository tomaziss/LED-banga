#include "Adafruit_TLC59711.h"
#include <SPI.h>
#include <math.h>

// How many boards do you have chained?
#define NUM_TLC59711 1

#define data   D7
#define clock  D5


const int steps = 500; //setps for each LED strip
const int overlay = 400;
const int ledStrips = 5; //No of led stips to control
int stepyStep[ledStrips];
int nextAction;
unsigned long previousMillis = 0;
int wait = 15;
byte action;
float x;
float x1 = 4;
float x2 = 7.8;
float inc;

float stepArray[steps];


Adafruit_TLC59711 tlc = Adafruit_TLC59711(NUM_TLC59711, clock, data);

void setup() {
  Serial.begin(115200);
  tlc.begin();

  inc = (x2 - x1) / (float)steps * 2;

  for (int i = 0; i <= steps / 2 ; i++) {
    stepArray[i] = (1 / (1 + exp (x1 - 4))) + 0.02;
    x1 = x1 + inc;
  }
  for (int i = steps / 2; i <= steps ; i++) {
    stepArray[i] = (1 / (1 + exp (x1 - 4))) + 0.02;
    x1 = x1 - inc;
  }


  for (int j = 0; j < ledStrips; j++) {
    stepyStep[j] = 0;
    tlc.setPWM (j, 0xFFFF);
  }
  tlc.write();

  stepyStep[0] = 1;
}


void loop() {

  //printArray();
  //Serial.println();

//  while (Serial.available() > 0) {
//    int fadeInFrom = Serial.parseInt();
//    if (Serial.read() == '\n') {
//      tlc.setPWM (0, fadeInFrom);
//      tlc.write();
//    }
//  }

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= wait) {


      previousMillis = currentMillis;
      for (action = 0; action < ledStrips; action++) {

        if (stepyStep[action] > 0) {

          singleWave(action);

          if (action + 1 == ledStrips) {
            nextAction = 0;
          } else {
            nextAction = action + 1;
          }


          if (stepyStep[action] > (steps - overlay) and stepyStep[nextAction] == 0) {
            singleWave(nextAction);

          }
        }
      }

    } 
}

void singleWave(int ledNumber) {

  if (stepyStep[ledNumber] < steps) {
    tlc.setPWM (ledNumber, stepArray[stepyStep[ledNumber]] * 0xFFFF);
    if (ledNumber == 0) {
      //    Serial.print("Action: "); Serial.println(ledNumber);
      Serial.print("StepyStep: "); Serial.println(stepyStep[ledNumber]);
      Serial.print("PWM: "); Serial.println(stepArray[stepyStep[ledNumber]] * 0xFFFF);
      Serial.println();
    }
    tlc.write();
    stepyStep[ledNumber]++;
  } else {
    stepyStep[ledNumber] = 0;

  }

}

void printArray() {
  for (int a = 0; a <= steps; a++) {
    Serial.println(stepArray[a], 6);
  }
}

