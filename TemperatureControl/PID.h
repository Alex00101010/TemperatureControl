#ifndef PID_h
#define PID_h
#include <Arduino.h>

class PID {
public:
  PID(float kP = 0, float kI = 0, float kD = 0, float targVal = 0);
  void set(float kP = 0, float kI = 0, float kD = 0);
  void setTargetValue(float targVal = 0);
  void setP(float kP = 0);
  void setI(float kI = 0);
  void setD(float kD = 0);
  float getP();
  float getI();
  float getD();
  float out(float currVal);
private:
  float _kP;
  float _kI;
  float _kD;
  float _targVal;

  float P = 0;
  float I = 0;
  float D = 0;
  
  float prevErr = 0;
  float currErr = 0;

  uint32_t prevMillis = 0;
};

PID::PID(float kP = 0, float kI = 0, float kD = 0, float targVal = 0) {
  _kP = kP;
  _kI = kI;
  _kD = kD;
  _targVal = targVal;
}
void PID::set(float kP = 0, float kI = 0, float kD = 0) {
  _kP = kP;
  _kI = kI;
  _kD = kD;
}

float PID::getP(){ return(_kP); }
float PID::getI(){ return(_kI); }
float PID::getD(){ return(_kD); }

void PID::setP(float kP = 0){ _kP = kP; }
void PID::setI(float kI = 0){ _kI = kI; }
void PID::setD(float kD = 0){ _kD = kD; }

float PID::out(float currVal) {
  float dt = 1 / float(millis() - prevMillis);
  prevMillis = millis();

  currErr = _targVal - currVal;
  
  P = currErr;
  I += currErr * dt;
  D = (currErr - prevErr) / dt;

  prevErr = currErr;

  float U = (_kP * P) + (_kI * I) + (_kD * D);
  return(U);
}
#endif
