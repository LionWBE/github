//version 0.13
#ifndef lib_Tags_h
#define lib_Tags_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <lib_Config.h>
#include <lib_MQTT.h>
#include <lib_timer.h>
#include <lib_Mini.h>
#include <StackArray.h>  

class MyClass_MQTT_Tag {
  public:
    void setup(MyClass_Config*);
    void start();
    uint16_t CreateNewTag(String *TagName, bool UseTimeStamp);           //функция создания нового тега (добавляет в массив имя тега и возвращает его номер - далее по нему будет отправка)
    uint16_t CreateNewTag(String *TagName);                              //функция создания нового тега (добавляет в массив имя тега и возвращает его номер - далее по нему будет отправка)
    void SetTagVal(uint16_t TagLink, String *TagVal);
    void SetTagVal(uint16_t TagLink, float *TagVal);
    void SetTagVal(uint16_t TagLink, bool  TagVal);
    void SetTagVal(uint16_t TagLink, byte TagVal);
    void SetTagVal(uint16_t TagLink, uint16_t TagVal);
    void SetTagVal(uint16_t TagLink, uint32_t TagVal);
    void PublicAllMessange();
  private:
    uint16_t MaxCountTag;                      // Максимальное количество доступных тегов
    uint16_t MaxSizeStack;                     // Максимальный размер стека 
    uint16_t col_tag;                          // количество тегов
    uint16_t cur_tagLink;                      // номер для записи значения тега
    bool *TagUseTimeStamps;                    // параметр для определения использования метки времени
    char **TagNames;                           // имена тегов
    char **TagValsOld;                         // значение тегов старое
    StackArray <char*> TagVals;                // значение тегов старое
    StackArray <uint16_t> TagLinks;            // ссылка на имя тега (номер в массиве TagNames и TagValsOld)
    MyClass_timer timer_general_interogation;
    MyClass_Config *settings; 
    MyClass_MQTT *my_MQTT;
};
#endif