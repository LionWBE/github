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
  MaxCountTag = 150;  // править для увеличения - уменьшения количество доступных тегов (сделано статикой, чтобы не тормозить с обработкой динамических массивов или стека)
  MaxSizeStack = 150;
  TagNames = new char*[MaxCountTag];
  TagValsOld = new char*[MaxCountTag];
  TagUseTimeStamps = new bool[MaxCountTag];
  Serial.println("MyClass_MQTT_Tag setup done");
}
//-----------------(методы класса MyClass_MQTT_Tag)------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT_Tag::start(){
  PublicAllMessange();
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
  TagValsOld[col_tag] = nullptr;                             //test
  memcpy(TagNames[col_tag], TagName->c_str(), size + 1);
  rez = col_tag;
  col_tag++;
  TagUseTimeStamps[rez] = UseTimeStamp;

  if(UseTimeStamp){
    String TagNameT = *TagName + ".t";
    size = TagNameT.length();
    TagNames[col_tag] = new char[size + 1];
    TagValsOld[col_tag] = nullptr;                             //test
    memcpy(TagNames[col_tag], TagNameT.c_str(), size + 1);
    col_tag++;    
  }
  settings->data.Tags_count = col_tag;
  return rez;
}
//-----------------(методы класса MyClass_MQTT_Tag)------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT_Tag::SetTagVal(uint16_t TagLink, String *TagVal){
  if (TagVals.count() >= MaxSizeStack) exit; // если 
  char* val = new char[TagVal->length() + 1];   // размечаем память под сроковую переменную
  strcpy(val, TagVal->begin());                 // записываем в эту область памяти значение входное
  TagVals.push(val);                            // записываем указатель на область памяти с значением в стек, потом после чтения надо удалить 
  TagLinks.push(TagLink);                       // записываем значение типа uint16_t в стек
  cur_tagLink++;
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
  if(cur_tagLink > 0 and my_MQTT->is_connect){
    bool GI = timer_general_interogation.is_done();
    settings->data.Tags_queue_count = cur_tagLink;   // число тегов в стеке ---------------------------------------------------------------------------------
    while (!TagLinks.isEmpty()){
      uint16_t TagLink = TagLinks.pop();                  // читаем значение из стека
      char* TagVal = TagVals.pop();                       // читаем указатель из стека, потом по этому указателю надо очистить память
      char* TagValOld = TagValsOld[TagLink];              // change there
      bool logic = true;
      if (TagValOld != nullptr){                          // если предыдущее значение существует   
        logic = (strcmp(TagValOld,TagVal) != 0) or GI;    // сравниваем предыдущее значение с текущим + Общий опрос по времени
      }
      if (logic){
        char* TagName = TagNames[TagLink];
        bool TagUseTimeStamp = TagUseTimeStamps[TagLink];
        my_MQTT->publish(TagName, TagVal);
        if (TagUseTimeStamp){
          TagName = TagNames[TagLink + 1];
          my_MQTT->publish(TagName, settings->data.time_stamp.c_str());
        }
      }
      delete TagValsOld[TagLink];     // удаляем предыдущее значение (область памяти)
      TagValsOld[TagLink] = TagVal;   // переносим указатель на новое значение (область памяти)
    }
    cur_tagLink = 0;
  }
}