#include <DS_raw.h>
#include <microDS18B20.h>
#include <microOneWire.h>
#include "PID.h"

#define PIN_FAN 10
#define PIN_PMP 11
#define PIN_POT A0
#define PIN_BTN A1
MicroDS18B20<2> DS18;

byte const minFan = 40;
byte const maxFan = 255;

byte const minPmp = 40;
byte const maxPmp = 255;

float minPot = 20;
float maxPot = 40;

float targTemp = 0;
float currTemp = 0;

bool isDebugEnabled = false;

PID FAN_PID(0, 0, 0, targTemp);
PID PMP_PID(0, 0, 0, targTemp);

void setup() {
  if(isDebugEnabled)
    Serial.begin(9600);
  
  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_PMP, OUTPUT);
}

void loop() {
  DS18.requestTemp();
  uint32_t delTimer = millis();
  while(millis() - delTimer < 100) {
    debug();
    buttonHandler();
  }
  currTemp = DS18.getTemp();
  
  analogWrite(PIN_FAN, clamp(FAN_PID.out(currTemp), 255, maxFan - minFan) + minFan);
  analogWrite(PIN_PMP, clamp(PMP_PID.out(currTemp), 255, maxPmp - minPmp) + minPmp);

  
}

byte clamp(float floatVal, float maxFloatVal, byte maxByteVal) {
  if(floatVal < 0) floatVal = 0;
  byte result;
  if(floatVal > maxFloatVal)
    result = 255;
  else
    result = byte((floatVal / maxFloatVal) * float(maxByteVal));
  return(result);
}

void debug() {
  if(isDebugEnabled) {
    Serial.println("targetTemp:" + String(targTemp) + ",currentTemp:" + String(currTemp));
  }
}

void serialHandler() {
  //empty
}

void buttonHandler() {
  float potTempVal = (analogRead(PIN_POT) / 1024) * (maxPot - minPot) + minPot;
  if(digitalRead(PIN_BTN) && (abs(potTempVal - targTemp) > 0.2))
    targTemp = potTempVal;
}
