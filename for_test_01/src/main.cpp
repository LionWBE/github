#include <Arduino.h>
//************************************************************************************************************************
#include <lib_Ethernet.h>
#include <lib_JSON.h>
#include <lib_Config.h>
#include <lib_Mini.h>


#include <Wire.h>
#define Address 0x20
//************************************************************************************************************************
MyClass_timer t1;
MyClass_JSON my_MyClass_JSON;
MyClass_Config my_config;
MyClass_Ethernet my_ethernet;
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
void setup() {
  // CODE for Ethernet
  Serial.begin(115200);
  Serial.println();
  // my_FTP.setup();
  my_MyClass_JSON.loadConfiguration("/settings.json", &my_config); //загрузка конфигурации  
  my_config.setup();
  lib_Mini_setup(&my_config);
  my_ethernet.setup(&my_config);
  // my_config.print();
  // delay(10000);  
  

  // pinMode(BUILTIN_LED, OUTPUT);
  t1.time_delay_const = 1000;
  t1.start();
  // Serial.println("VER01");
  
  
  
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

}
//************************************************************************************************************************
void loop() {
  // CODE for Ethernet
  my_ethernet.start(); 
  if (t1.is_done()){
    my_ethernet.DEBAG("1 sec");
  }

//  // CODE for PCF8575 - INPUT
//   byte read_value;
//   if (Wire.requestFrom(Address,2) == 2)
//    {
//     read_value=(Wire.read());
//     Serial.print(" Byte 1:");
//     Serial.print(read_value, HEX);    
//     Serial.println();
//     read_value=(Wire.read());
//     Serial.print(" Byte 2:");
//     Serial.print(read_value, HEX);    
//     Serial.println();
//    }
//   delay(1000);

  // CODE for PCF8575 - OUTPUT
  // Wire.beginTransmission(Address);
  // byte error = Wire.endTransmission();
  // if (error == 0) {
  //   Serial.println("online");
  // }else{
  //   Serial.println("offline");
  // }

  // Wire.beginTransmission(Address);
  // // Wire.write(0xFF);
  // // Wire.write(0xFF);
  // Wire.endTransmission(); 
  // Serial.println("1");
  // delay(5000);
  // Wire.beginTransmission(Address);
  // // Wire.write(0x00);
  // // Wire.write(0x00);
  // Wire.endTransmission();   
  // Serial.println("0");
  // delay(5000);
}