//version 0.13 date 29/06/2022
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
    void LoadFileToJSON(const char *filename, byte *q);
    void Config_WiFi(const char *, MyClass_Config *);
    void Config_NTP(const char *, MyClass_Config *);
    void Config_FTP(const char *, MyClass_Config *);
    void Config_MQTT(const char *, MyClass_Config *);
    void Config_PCF8575(const char *, MyClass_Config *);
    void Config_DS18B20(const char *, MyClass_Config *);
    byte Config_DIs(const char *, MyClass_Config *, byte);
    byte Config_DOs(const char *, MyClass_Config *, byte);
};
#endif
