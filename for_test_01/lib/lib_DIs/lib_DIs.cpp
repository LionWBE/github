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
    Link_to_DO[i] = 255;  // показываем что по умолчанию нет связанного DO
  }

  for (byte j = 0; j < settings->config.DOs.col; j++) {
    // Serial.print("j =");
    // Serial.println(j);         
    // Serial.print("settings->config.DOs.DO[j].LinkTo.Enable =");
    // Serial.println(settings->config.DOs.DO[j].LinkTo.Enable); 
    // Serial.print("settings->config.DOs.DO[j].LinkTo.type =");
    // Serial.println(settings->config.DOs.DO[j].LinkTo.type); 
    // Serial.print("settings->config.DOs.DO[j].LinkTo.link =");
    // Serial.println(settings->config.DOs.DO[j].LinkTo.link);             
    if(settings->config.DOs.DO[j].LinkTo.Enable){           // если включена ссылка
      if(settings->config.DOs.DO[j].LinkTo.type == 2){      // если тип ссылки на DI
        byte link = settings->config.DOs.DO[j].LinkTo.link; // номер ссылки на номер DI
        Link_to_DO[link] = j;
      }
    }
  }

  // delay(5000);
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
    if(link != 255){ //если есть связанный DO
      settings->config.DOs.DO[link].Cmd = bool_to_byte(val);
      // Serial.print("settings->config.DOs.DO[link].Cmd =");
      // Serial.println(settings->config.DOs.DO[link].Cmd);
    }

    // Serial.print("i =");
    // Serial.println(i);
    // Serial.print("bool_to_byte(val) =");
    // Serial.println(bool_to_byte(val));
    // Serial.print("link =");
    // Serial.println(link);
    // delay(100);
  }
  t[1] = micros();
  settings->data.dt[9] = t[1] - t[0];    
}