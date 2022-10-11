#include <Arduino.h>
//************************************************************************************************************************
#include <lib_Ethernet.h>
#include <lib_JSON.h>
#include <lib_Config.h>
#include <lib_Mini.h>


#include <Wire.h>
#define Address 0x20

#include <lib_Tags.h>
#include <lib_pcf8575_NEW.h>
//************************************************************************************************************************
MyClass_timer t1;
MyClass_JSON my_MyClass_JSON;
MyClass_Config my_config;
MyClass_Ethernet my_ethernet;
// MyClass_MQTT_Tag my_Tags;
MyClass_all_PCF8575 my_PCF8575;
// bool interupt;
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
// void test_int() {
//   interupt = true;
// }
//************************************************************************************************************************
void setup() {
  // CODE for Ethernet
  Serial.begin(115200);

  Serial.println();
  // my_FTP.setup(); - удалить бибилиотеку - не нужна
  my_MyClass_JSON.loadConfiguration("/settings.json", &my_config); //загрузка конфигурации  
  my_config.setup();
  lib_Mini_setup(&my_config);
  my_ethernet.setup(&my_config);
  // my_Tags.setup(&my_config);
  my_PCF8575.setup(&my_config);
  // my_config.print();
  // delay(10000);  

  t1.time_delay_const = 1000;
  t1.start();
  
    // // CODE for PCF8575 - INPUT
  // Serial.begin(115200);
  // Serial.println();
  // Wire.begin();
  // Wire.beginTransmission(Address);
  // Wire.write(0xFF);
  // Wire.write(0xFF);
  // Wire.endTransmission();  

  // CODE for PCF8575 - OUTPUT
  // Serial.begin(115200);
  // Serial.println();
  // Wire.begin();
  // Wire.beginTransmission(Address);
  // Wire.write(0xFF);
  // Wire.write(0xFF);
  // Wire.endTransmission(); 

  pinMode(D0, INPUT);   // пин задействован под програмное прерывание INT от плат расширения
  pinMode(D4, OUTPUT);  // пин задействован для индикации LED процессорной платы
}
//************************************************************************************************************************
void loop() {
  // CODE for Ethernet

  my_ethernet.start(); 
  if (t1.is_done()){
    my_ethernet.DEBAG("1 sec");
  }

  int interupt = digitalRead(D0);
  if (interupt == LOW){
    my_PCF8575.interrupt();
  }
  my_PCF8575.start();

  // my_ethernet.DEBAG("interupt " + String(interupt));

  // my_PCF8575.start();
  // my_Tags.start();

//  // CODE for PCF8575 - INPUT
  // int interupt = digitalRead(D0);
  // if (interupt == LOW){
  //   my_ethernet.DEBAG("interupt detect");
  //   delay(50);
  //   byte read_value;
  //   if (Wire.requestFrom(Address,2) == 2){
  //     read_value=(Wire.read());
  //     Serial.print(" Byte 1:");
  //     Serial.print(read_value, HEX);    
  //     Serial.println();
  //     read_value=(Wire.read());
  //     Serial.print(" Byte 2:");
  //     Serial.print(read_value, HEX);    
  //     Serial.println();
  //   }
  // } 

  // CODE for PCF8575 - online/offline
  // Wire.beginTransmission(Address);
  // byte error = Wire.endTransmission();
  // if (error == 0) {
  //   Serial.println("online");
  // }else{
  //   Serial.println("offline");
  // }

  // delay(500);
  
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