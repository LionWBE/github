//version 0.10 date 01/11/2022
#include "lib_Tags.h"
//-----------------(методы класса MyClass_MQTT_Tag)------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT_Tag::setup(MyClass_Config *my_config){
  settings = my_config;
  settings->data.Tags = (int*)this;
  my_MQTT = (MyClass_MQTT*)settings->data.MQTT;
  timer_general_interogation.time_delay_const = 60*1000;
  timer_general_interogation.start();

  col_tag = 0;
  cur_tagLink = 0;
  MaxCountTag = 250;  // править для увеличения - уменьшения количество доступных тегов (сделано статикой, чтобы не тормозить с обработкой динамических массивов или стека)
  MaxSizeStack = 250;
  TagNames = new char*[MaxCountTag];
  TagUseTimeStamps = new bool[MaxCountTag];

  TagValsOld = new String[MaxCountTag];
  TagVals = new String[MaxSizeStack];
  TagLinks = new uint16_t[MaxSizeStack];
  
  settings->status.MQTT_Tag.version_lib = "0.11";
  settings->status.MQTT_Tag.date_lib    = "01.11.2022";   
  Serial.println("MyClass_MQTT_Tag setup done");
}
//-----------------(методы класса MyClass_MQTT_Tag)------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT_Tag::start(){
  uint32_t t[2];
  t[0] = micros();  

  PublicAllMessange();

  t[1] = micros();
  settings->data.dt[8] = t[1] - t[0];   
}
//-----------------(методы класса MyClass_MQTT_Tag)------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t MyClass_MQTT_Tag::CreateNewTag(String *TagName){
  return CreateNewTag(TagName, false);
}  
//-----------------(методы класса MyClass_MQTT_Tag)------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t MyClass_MQTT_Tag::CreateNewTag(String *TagName, bool UseTimeStamp){
  uint16_t rez;
  uint8_t size = TagName->length();

  TagNames[col_tag] = new char[size + 1];
  TagValsOld[col_tag] = "";                             //test
  memcpy(TagNames[col_tag], TagName->c_str(), size + 1);
  rez = col_tag;
  col_tag++;
  TagUseTimeStamps[rez] = UseTimeStamp;

  if(UseTimeStamp){
    String TagNameT = *TagName + ".t";
    size = TagNameT.length();
    TagNames[col_tag] = new char[size + 1];
    TagValsOld[col_tag] = "";                             //test
    memcpy(TagNames[col_tag], TagNameT.c_str(), size + 1);
    col_tag++;    
  }
  settings->data.Tags_count = col_tag;
  return rez;
}
//-----------------(методы класса MyClass_MQTT_Tag)------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT_Tag::SetTagVal(uint16_t TagLink, String *TagVal){
  if (cur_tagLink < MaxSizeStack){                // первая проверка переполнения буфера исх. сообщений
    TagVals[cur_tagLink] = *TagVal;
    TagLinks[cur_tagLink] = TagLink;
    cur_tagLink++;
  }
}
//-----------------(методы класса MyClass_MQTT_Tag)------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT_Tag::SetTagVal(uint16_t TagLink, float *TagVal){
  String val = String(*TagVal);
  SetTagVal (TagLink, &val);
}
//-----------------(методы класса MyClass_MQTT_Tag)------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT_Tag::SetTagVal(uint16_t TagLink, bool TagVal){
  String val = String(TagVal);
  SetTagVal (TagLink, &val);
}
//-----------------(методы класса MyClass_MQTT_Tag)------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT_Tag::SetTagVal(uint16_t TagLink, byte TagVal){
  String val = String(TagVal);
  SetTagVal (TagLink, &val);
}
//-----------------(методы класса MyClass_MQTT_Tag)------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT_Tag::SetTagVal(uint16_t TagLink, uint16_t TagVal){
  String val = String(TagVal);
  SetTagVal (TagLink, &val);
}
//-----------------(методы класса MyClass_MQTT_Tag)------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT_Tag::SetTagVal(uint16_t TagLink, uint32_t TagVal){
  String val = String(TagVal);
  SetTagVal (TagLink, &val);
}
//-----------------(методы класса MyClass_MQTT_Tag)------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT_Tag::PublicAllMessange(){
  bool test_print = false;

  if (test_print) Serial.println("test_01");
  if(cur_tagLink > 0 and my_MQTT->is_connect){
    if (test_print) Serial.println("test_02");
    bool GI = timer_general_interogation.is_done();
    if (test_print) Serial.println("test_03");
    settings->data.Tags_queue_count = cur_tagLink;          // число тегов в стеке ---------------------------------------------------------------------------------
    if (test_print) Serial.println("test_04");
    for (byte i = 0; i < cur_tagLink; i++){
      uint16_t TagLink = TagLinks[i];
      String TagVal = TagVals[i];
      bool logic = (TagValsOld[TagLink] != TagVal) or GI;    // сравниваем предыдущее значение с текущим + Общий опрос по времени
      if (logic){
        char* TagName = TagNames[TagLink];
        bool TagUseTimeStamp = TagUseTimeStamps[TagLink];
        my_MQTT->publish(TagName, TagVal.c_str());
        if (TagUseTimeStamp){
          TagName = TagNames[TagLink + 1];
          my_MQTT->publish(TagName, settings->data.time_stamp.c_str());
        }
      }
      TagValsOld[TagLink] = TagVal;   // переносим новое значение в старое
    }
    cur_tagLink = 0;
    if (test_print) Serial.println("test_05");
  }
  if (test_print) Serial.println("test_06");
}