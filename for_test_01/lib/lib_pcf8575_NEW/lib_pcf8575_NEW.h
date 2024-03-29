//version 0.15
#ifndef lib_PCF8575_NEW_h
#define lib_PCF8575_NEW_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Arduino.h>
#include <Wire.h>
#include <lib_Config.h>
#include <lib_timer.h>
#include <lib_Mini.h>
#include <lib_Tags.h>
#include <lib_Ethernet.h>

class MyClass_PCF8575_DI {
  public:
    void debounce(bool);            // устранить дребезг
    void setup(MyClass_Config*);
    bool is_enable;
    bool val_curr;                  // текущее состояние
    bool is_set_val_curr;           // бит = 1 когда состояние установившееся
  private:
    bool val_prev;                  // предыдущее состояние
    bool is_set_val_prev;           // бит = 1 когда состояние установившееся
    bool is_first_start;            // бит = 1 при первом запуске
    bool is_bounce;                 // идет процесс устранения дребезга 
    MyClass_timer timer_bounce;     // таймер для устранения дребезга
    MyClass_Config *settings;       // указатель на настройки
};

class MyClass_PCF8575_DO {
  public:
    void debounce(bool);            // устранить дребезг
    void setup(MyClass_Config*);
    bool is_enable;
    bool val_curr;                  // текущее состояние
  private:
    bool val_prev;                  // предыдущее состояние
    bool is_set_val_curr;           // бит = 1 когда состояние установившееся
    bool is_set_val_prev;           // бит = 1 когда состояние установившееся
    bool is_first_start;            // бит = 1 при первом запуске
    bool is_bounce;                 // идет процесс устранения дребезга 
    MyClass_timer timer_bounce;     // таймер для устранения дребезга
    MyClass_Config *settings;       // указатель на настройки
};

class MyClass_PCF8575 {
  public:
    void setup(MyClass_Config*, byte num_board);
    void start();
    void interrupt();
    bool is_online;
  private:
    byte I2C_adr;                          // адрес I2C
    byte board_type;                       // тип платы 1 - DI, 2 - DO
    const uint8_t pin_interrupt = D0;      // пин с программным прерыванием от плат DI / DO

    MyClass_PCF8575_DI DI[16];
    bool All_DI_is_set;    
    MyClass_PCF8575_DI DO[16];
    byte link_to_DI_in_config[16];
    byte link_to_DO_in_config[16];
    void read_state_DI();  
    void write_state_DO();
    void setup_DI_one(byte i, byte num_board);
    void setup_DO_one(byte i, byte num_board);
    MyClass_Config *settings;              // указатель на глобальные настройки
    MyClass_MQTT_Tag *my_Tags;             // указатель на теги для MQTT
    MyClass_Ethernet *my_ethernet;         // указатель на Ethernet
    uint16_t *MQTT_link_val;               // ссылка на теги MQTT со значениями
    uint16_t MQTT_link_adr[3];             // ссылки на общие параметры (адрес I2C, наличие связи)
    MyClass_timer timer_link;              // таймер для определения наличия связи
    uint16_t all_state_do;                 // обощенное состояние выодов для записи
    MyClass_timer timer_write_do;          // таймер для записи значений в выхода
    MyClass_timer timer_read_di;           // таймер для чтения значений входов
};

class MyClass_all_PCF8575 {
  public:
    byte col_board;
    bool *is_online;
    void setup(MyClass_Config*);
    void start();  
    void interrupt();
  private:
    MyClass_PCF8575 *board;
    MyClass_Config *settings;              // указатель на глобальные настройки
    MyClass_Ethernet *my_ethernet;          // указатель на Ethernet
};
#endif