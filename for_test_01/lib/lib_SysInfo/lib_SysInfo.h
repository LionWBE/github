//version 0.12
#ifndef lib_SysInfo_h
#define lib_SysInfo_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <lib_timer.h>
#include <lib_Config.h>
#include <lib_Tags.h>

class MyClass_SysInfo{
  public:
    void setup(MyClass_Config*);
    void start();
  private:
    MyClass_Config *settings;  
    MyClass_MQTT_Tag *Tags;
    MyClass_timer timer_publish_status;
    MyClass_time_since_start time_since_start;
    uint16_t counter;
    uint16_t cicle_in_sec;
    uint32_t time_cur;
    uint32_t time_prev;
    uint32_t time_cicle;
};
#endif
