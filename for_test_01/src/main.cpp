#include <Arduino.h>
//************************************************************************************************************************
#include <lib_Ethernet.h>
#include <lib_JSON.h>
#include <lib_Config.h>
#include <lib_Mini.h>
#include <Wire.h>
#include <lib_SysInfo.h>
#include <lib_Tags.h>
#include <lib_pcf8575_NEW.h>
#include <lib_DIs.h>
#include <lib_NTP.h>
#include <lib_WiFi.h>
//************************************************************************************************************************
MyClass_timer t1;
MyClass_JSON my_MyClass_JSON;
MyClass_Config my_config;
MyClass_Ethernet my_ethernet;
MyClass_MQTT_Tag my_Tags;
MyClass_all_PCF8575 my_PCF8575;
MyClass_MQTT my_MQTT;
MyClass_SysInfo my_SysInfo;
MyClass_DIs my_DIs;
MyClass_NTP my_NTP;
MyClass_NTP my_WiFi;
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial.println();
  my_MyClass_JSON.loadConfiguration("/settings.json", &my_config); //загрузка конфигурации  
  my_config.setup();
  my_WiFi.setup(&my_config);
  my_ethernet.setup(&my_config);
  my_MQTT.setup(&my_config);
  my_Tags.setup(&my_config);
  my_SysInfo.setup(&my_config);  
  my_PCF8575.setup(&my_config);
  my_DIs.setup(&my_config);
  my_NTP.setup(&my_config);
  lib_Mini_setup(&my_config);
  // my_config.print();
  // delay(10000);  

  t1.time_delay_const = 1000;
  t1.start();
  
  pinMode(D0, INPUT);   // пин задействован под програмное прерывание INT от плат расширения
  pinMode(D4, OUTPUT);  // пин задействован для индикации LED процессорной платы
  digitalWrite(D4, 1);  // погасить светодиод после загрузки
}
//************************************************************************************************************************
void loop() {
  my_ethernet.start();
  my_WiFi.start();
  my_MQTT.start();  
  my_NTP.start(); 
  my_PCF8575.start();
  my_SysInfo.start();
  my_DIs.start();
  my_Tags.start();

  int interupt = digitalRead(D0);
  if (interupt == LOW)my_PCF8575.interrupt();
  if (t1.is_done())my_ethernet.DEBUG("1 sec");
  
  // CODE for PCF8575 - OUTPUT
  // Wire.beginTransmission(Address);
  // Wire.write(0xFF);
  // Wire.write(0xFF);
  // Wire.endTransmission(); 
  // Serial.println("1");
  // delay(3000);
  // Wire.beginTransmission(Address);
  // Wire.write(0x00);
  // Wire.write(0x00);
  // Wire.endTransmission();   
  // Serial.println("0");
  // delay(3000);
}