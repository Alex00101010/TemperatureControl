#ifndef myPID_h
#define myPID_h
#include <Arduino.h>

class myPID {
public:
  myPID(float kP = 0, float kI = 0, float kD = 0, float target_value = 0);
  void setK(float kP = 0, float kI = 0, float kD = 0);
  void setTarget(float target_value = 0);
  float getOut(float current_value = 0);
private:
  float _kP;
  float _kI;
  float _kD;
  float TARG_VAL;
  float I = 0;
  float prev_err = 0;
  uint32_t pmillis = 0;
};

myPID::myPID(float kP = 0, float kI = 0, float kD = 0, float target_value = 0) {
  _kP = kP;
  _kI = kI;
  _kD = kD;
  TARG_VAL = target_value;
}
void myPID::setK(float kP = 0, float kI = 0, float kD = 0) {
  _kP = kP;
  _kI = kI;
  _kD = kD;
}
void myPID::setTarget(float target_value = 0) {
  TARG_VAL = target_value;
}

float myPID::getOut(float current_value = 0) {
  float err = current_value - TARG_VAL;
  float dt = 1 / float(millis() - pmillis);
  pmillis = millis();
  
  float P = err;
  I = I + err * dt;
  float D = (err - prev_err) / dt;
  
  prev_err = err;
  float U = _kP * P + _kI * I + _kD * D;
  if(U < 0) U = 0;
  return(U);
}

#endif
