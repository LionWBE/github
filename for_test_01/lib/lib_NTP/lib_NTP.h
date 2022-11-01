//version 0.13
#ifndef lib_NTP_h
#define lib_NTP_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <lib_NTPClient.h>
#include <WiFiUdp.h>
#include <lib_Config.h>
#include <lib_Mini.h>

class MyClass_NTP {
  public:
    void setup(MyClass_Config*);
    void start();
    void SetLocalTime(String remote_date_time);
  private:
	  WiFiUDP ntpUDP;
	  NTPClient timeClient;
    MyClass_Config *settings;
};
#endif
