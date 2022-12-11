//version 0.18 date 11/12/2022
#ifndef lib_MQTT_h
#define lib_MQTT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <lib_timer.h>
#include <lib_Config.h>
#include <lib_Mini.h>
#include <lib_Ethernet.h>

class MyClass_MQTT{
  public:
    void setup(MyClass_Config*);
    void start();
    boolean publish(const char* topic, const char* payload);
    bool is_connect;
  private:
    void callback(char*, byte*, unsigned int);
    void reconnect();
    void subscribe();         // подписаться на все входящие сообщения
    void setup_DI();          // для настройки входных сообщений и привязки их к DI
    byte col_input_messange;  // количество входных сообщений
    byte *link_to_DI;         // массив с номерами DI для входных сообщений
    unsigned long lastMsg = 0;
    #define MSG_BUFFER_SIZE	(50)
    char msg[MSG_BUFFER_SIZE];
    int value = 0;
    WiFiClient espClient;
    PubSubClient client;
    // volatile MyClass_Config *settings;  
    MyClass_Config *settings;  
    MyClass_timer timer_reconect;
    uint16_t *MQTT_link_val;               // ссылка на теги MQTT со значениями
};
#endif
