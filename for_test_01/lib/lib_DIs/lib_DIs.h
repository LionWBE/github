//version 0.01 date 04/07/2022
#ifndef lib_DIs_h
#define lib_DIs_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Arduino.h>
#include <lib_Config.h>
#include <lib_timer.h>
#include <lib_Mini.h>
#include <lib_Tags.h>

class MyClass_DIs {
  public:
    void setup(MyClass_Config*);
    void start();
  private:
    MyClass_Config *settings;              // указатель на настройки
    MyClass_MQTT_Tag *my_Tags;             // указатель на теги для MQTT
    uint16_t *MQTT_link_val;               // ссылка на теги MQTT со значениями
    byte *Link_to_DO;                      // ссылка на DO который управляется по конфигурации указанным DI (для записи в него значения)     
};
#endif