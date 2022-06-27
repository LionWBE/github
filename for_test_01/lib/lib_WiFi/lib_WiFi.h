//version 0.13
#ifndef lib_WiFi_h
#define lib_WiFi_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <lib_Config.h>
#include <lib_timer.h>
#include <lib_Mini.h>

class MyClass_WiFi {
  public:
    void setup(MyClass_Config*);
    void start();
    String IP;  
    String MAC;
  private:
    MyClass_Config *settings;
    MyClass_timer timer_reconect;
    bool first_start;
    bool reconect;
    char *wifi_ssid;
    char *wifi_password;      
};
#endif
