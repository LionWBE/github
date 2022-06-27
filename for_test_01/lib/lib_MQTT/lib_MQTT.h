//version 0.12
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

class MyClass_MQTT{
  public:
    void setup(MyClass_Config*);
    void start();
    boolean publish(const char* topic, const char* payload);
    bool is_connect;
  private:
    void callback(char*, byte*, unsigned int);
    void reconnect();
    
    unsigned long lastMsg = 0;
    #define MSG_BUFFER_SIZE	(50)
    char msg[MSG_BUFFER_SIZE];
    int value = 0;
    WiFiClient espClient;
    PubSubClient client;

    MyClass_Config *settings;  
    MyClass_timer timer_reconect;
};
#endif
