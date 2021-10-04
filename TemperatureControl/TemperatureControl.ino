#include <DS_raw.h>
#include <microDS18B20.h>
#include <microOneWire.h>
#include "PID.h"

#define PIN_FAN 10
#define PIN_PMP 11
#define PIN_POT A0
#define PIN_BTN A1
MicroDS18B20<2> DS18;

float minPot = 20;
float maxPot = 40;

float targTemp = 0;
float currTemp = 0;

//bool isDebugEnabled = false;

PID FAN_PID(0, 0, 0, targTemp);

void setup() {
  //if(isDebugEnabled)
  Serial.begin(9600);
  
  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_PMP, OUTPUT);

}

void loop() {
  DS18.requestTemp();
  uint32_t delTimer = millis();
  while(millis() - delTimer < 100) {}
  currTemp = DS18.getTemp();
  Serial.println(FAN_PID.out(currTemp));
}
