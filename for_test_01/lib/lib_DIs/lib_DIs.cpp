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

  // поиск связанного DO, куда надо будет записать свое значение
  Link_to_DO = new byte[col];
  for (byte i = 0; i < col; i++) {
    Link_to_DO[i] = -1;  // показываем что по умолчанию нет связанного DO
    for (byte j = 0; j < settings->config.DOs.col; j++) {
      if(settings->config.DOs.DO[j].LinkTo.Enable){
        if(settings->config.DOs.DO[j].LinkTo.type == 2){
          if(settings->config.DOs.DO[j].LinkTo.link == i){
            Link_to_DO[i] = j;
            break;
          }
        }
      }
    }
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
    
    byte link = Link_to_DO[i];
    if(link > -1){ //если есть связанный DO
      settings->config.DOs.DO[link].Cmd = val;
    }
  }
  t[1] = micros();
  settings->data.dt[9] = t[1] - t[0];    
}