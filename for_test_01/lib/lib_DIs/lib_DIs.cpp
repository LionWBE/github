//version 0.02 date 01/11/2022
#include "lib_DIs.h"
//-----------------(методы класса MyClass_DIs)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_DIs::setup(MyClass_Config *my_config){
  String adr;
  byte col;
  settings = my_config;
  col = settings->config.DIs.col;
  my_Tags = (MyClass_MQTT_Tag*)settings->data.Tags;
  MQTT_link_val = new uint16_t[col];

  for (byte i = 0; i < col; i++) {
    adr = settings->config.MQTT.Home_topic + "DIs/DI_ " + String(i) + ".val"; 
    MQTT_link_val[i] = my_Tags->CreateNewTag(&adr);     
  }
  settings->status.DIs.version_lib = "0.02";
  settings->status.DIs.date_lib    = "01.11.2022"; 
  Serial.println("MyClass_DIs setup done");
}
//-----------------(методы класса MyClass_DIs)---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_DIs::start(){
  uint32_t t[2];
  t[0] = micros();  
  byte col = settings->config.DIs.col;
  bool val;
  for (byte i = 0; i < col; i++) {
    val = settings->config.DIs.DI[i].Val;
    my_Tags->SetTagVal(MQTT_link_val[i], val);
  }
  t[1] = micros();
  settings->data.dt[9] = t[1] - t[0];    
}