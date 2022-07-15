#include <Arduino.h>
//************************************************************************************************************************
#include <lib_Ethernet.h>
#include <lib_JSON.h>
#include <lib_Config.h>
#include <lib_Mini.h>
#include <lib_FTP.h>
//************************************************************************************************************************
MyClass_timer t1;
MyClass_JSON my_MyClass_JSON;
MyClass_Config my_config;
MyClass_Ethernet my_ethernet;
MyClass_FTP my_FTP;
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
//************************************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial.println();
  my_FTP.setup();
  my_MyClass_JSON.loadConfiguration("/settings.json", &my_config); //загрузка конфигурации
  // my_config.print();
  // delay(10000);  
  
  my_ethernet.setup(&my_config);

  pinMode(BUILTIN_LED, OUTPUT);
  t1.time_delay_const = 1000;
  t1.start();
}
//************************************************************************************************************************
void loop() {
  my_ethernet.start(); 
  my_FTP.start();
  if (t1.is_done()){
    // Serial.println("1 sec");
  }
}