//version 0.10
//date 05.05.2022
#ifndef lib_temper
#define lib_temper

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <microDS18B20.h>
// #include <lib_Tags.h>
#include <lib_Config.h>
#include <lib_Mini.h>

class MyClass_temper {
  public:
    void printTemperatures();
    void setup(MyClass_Config*);
    void start();
    uint8_t deviceCount;
    MicroDS18B20 *sensors; 
  private:
    float *SensorTemp;                   // текущее значение температуры
    
    MyClass_Config *settings;            // указатель на настройки
    // MyClass_MQTT_Tag *my_Tags;              // указатель на теги для MQTT
    uint16_t *MQTT_link_temp_val;
    uint16_t *MQTT_link_sensor_addres;
};
#endif
