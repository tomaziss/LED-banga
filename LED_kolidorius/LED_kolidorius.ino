#include "Adafruit_TLC59711.h"
#include <SPI.h>
// How many boards do you have chained?
#define NUM_TLC59711 1
#define data   D7
#define clock  D5

const int steps = 500; //setps for each LED strip
const int overlay = 420;
const float fadeOutTo = 0.02; // %
const float fadeInFrom = 0.3; // %
float stepArray[steps];
float singleStep;
//float x = 3.141;
float x = 0;
const int ledStrips = 4; //No of led stips to control
int stepyStep[ledStrips];
int nextAction;
unsigned long previousMillis = 0;
const long interval = 10;
byte action;

Adafruit_TLC59711 tlc = Adafruit_TLC59711(NUM_TLC59711, clock, data);
void setup() {
  Serial.begin(115200);
  tlc.begin();

  singleStep = 6.283185 / steps;
  for (int i = 0; i <= steps; i++) {
    //stepArray[i] = 1 - ((cos(x) + 1) / 2 * (1 - fadeOutTo)) ; //fadeout to...
    stepArray[i] = ((cos(x) + 1) / 2 * fadeInFrom);
    x = x + singleStep;
  }
  for (int j = 0; j < ledStrips; j++) {
    stepyStep[j] = 0;
    tlc.setPWM (j, stepArray[0]);
  }
  tlc.write();
  stepyStep[0] = 1;
}
void loop() {

  //  ArduinoOTA.handle();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
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
          //          printCos();
          //          Serial.println();
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
void printCos() {
  for (int a = 0; a <= steps; a++) {
    Serial.println(stepArray[a]);
  }
}
