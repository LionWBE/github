//version 0.12
#ifndef lib_JSON_h
#define lib_JSON_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ArduinoJson.h>
#include <SPI.h>
#include <FS.h>
#include <lib_Config.h>

class MyClass_JSON {
  public:
    void loadConfiguration(const char *, MyClass_Config *);
    bool error_load_config;
  private:
};
#endif
