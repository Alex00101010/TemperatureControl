#ifndef SerialHandler_h
#define SerialHandler_h
#include <Arduino.h>

class SerialHandler {
};

void serialHandler() {
  static String command;
  if(Serial.available() > 0) {
    char inChar = Serial.read();
    command += inChar;
    if(inChar = "\n") {
      commandHandler(command);
      command = "";
    }
  }
}

void commandHandler(String command) {
  if(command == "debug") isDebugEnabled = !isDebugEnabled;
}

#endif
