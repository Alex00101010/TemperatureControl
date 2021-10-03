#include <DS_raw.h>
#include <microDS18B20.h>
#include <microOneWire.h>
#include "myPID.h"

#define PIN_LED 13
#define PIN_PMP 12
#define PIN_FAN 11
#define PIN_POT A0
MicroDS18B20<2> DS;

bool isDebugEnabled = true;
const float minPotTemp = 20;
const float maxPotTemp = 40;

float TARG_TEMP = 20;
float CURR_TEMP = 36;

myPID FAN_PID(1, 0, 0, TARG_TEMP);
//myPID PMP_PID(0, 0, 0, TARG_TEMP);

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_PMP, OUTPUT);
  pinMode(PIN_FAN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  uint32_t delTimer = millis();
  while(millis() - delTimer < 100) {
    changeTargTempHandler();
  }
  debug();
  //DS.requestTemp();
  //CURR_TEMP = DS.getTemp();
}

void changeTargTempHandler() {
  float potTemp = ((analogRead(PIN_POT) * (maxPotTemp - minPotTemp)) / 1024) + minPotTemp;
  if(abs(potTemp - TARG_TEMP) > 0.5) {
    TARG_TEMP = potTemp;
    FAN_PID.setTarget(TARG_TEMP);
  }
}

void debug() {
  if(isDebugEnabled) {
    Serial.println("current:" + String(CURR_TEMP) + ",target:" + String(TARG_TEMP));
  }
}
