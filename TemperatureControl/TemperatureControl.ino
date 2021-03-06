#include <DS_raw.h>
#include <microDS18B20.h>
#include <microOneWire.h>
#include "PID.h"

#define PIN_N_RESET 8
#define PIN_N_CLOCK 9

#define PIN_FAN 10
#define PIN_PMP 11
#define PIN_POT A0
#define PIN_BTN A1

MicroDS18B20<2> DS18;

byte const minFan = 40;
byte const minPmp = 40;

float minPot = 20;
float maxPot = 40;

float targTemp = 0;
float currTemp = 0;

bool isDebugEnabled = false;
uint32_t targTempTime = 0;

PID FAN_PID(0, 0, 0, targTemp);
PID PMP_PID(0, 0, 0, targTemp);

#define ENC_A 2
#define ENC_B 4
#define ENC_TYPE 1
volatile int encCounter;
volatile boolean state0, lastState, turnFlag;

void setup() {
  Serial.begin(9600);
  attachInterrupt(0, encInt, CHANGE);
  
  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_PMP, OUTPUT);

  pinMode(PIN_N_RESET, OUTPUT);
  pinMode(PIN_N_CLOCK, OUTPUT);
  
  setNumber(0);
}

void loop() {
  DS18.requestTemp();
  uint32_t delTimer = millis();
  while(millis() - delTimer < 100) {
    debug();
    buttonHandler();
    CDTemp();
  }
  Serial.println(encCounter);
  currTemp = DS18.getTemp();
  setNumber(currTemp);
  analogWrite(PIN_FAN, clamp(FAN_PID.out(currTemp), 255, minFan));
  analogWrite(PIN_PMP, clamp(PMP_PID.out(currTemp), 255, minPmp));
}

byte clamp(float input, float maxFloatValue, byte minByteVal) {
  if(input < 0) input = 0;
  if(input > maxFloatValue) input = maxFloatValue;
  
  byte result = (input / maxFloatValue) * 255;
  if(result < minByteVal) result = minByteVal;
  
  return(result);
}

void CDTemp() {
  if(millis() < targTempTime)
    setNumber(targTemp);
  else
    setNumber(currTemp);
}

void debug() {
  if(isDebugEnabled) {
    Serial.println("targetTemp:" + String(targTemp) + ",currentTemp:" + String(currTemp));
  }
}

void buttonHandler() {
  float potTempVal = (analogRead(PIN_POT) / 1024) * (maxPot - minPot) + minPot;
  if(digitalRead(PIN_BTN) && (abs(potTempVal - targTemp) > 0.2)) {
    targTemp = potTempVal;
    targTempTime = millis() + 1500;
  }
}

void setNumber(int n) {
  digitalWrite(PIN_N_RESET, HIGH);
  digitalWrite(PIN_N_RESET, LOW);
  
  while(n--) {
    digitalWrite(PIN_N_CLOCK, HIGH);
    digitalWrite(PIN_N_CLOCK, LOW);
  }
}

void encInt() {
  state0 = digitalRead(ENC_A);
  if (state0 != lastState) {
#if (ENC_TYPE == 1)
    turnFlag = !turnFlag;
    if (turnFlag) {
      encCounter += (digitalRead(ENC_B) != lastState) ? -1 : 1;
    }
#else
    encCounter += (digitalRead(ENC_B) != lastState) ? -1 : 1;
#endif
    lastState = state0;
  }
}
