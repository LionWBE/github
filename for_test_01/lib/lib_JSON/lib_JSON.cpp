//version 0.16 date 09/11/2022
#include "lib_JSON.h"
// есть проблема с большими файлами (более 2048 байт) - не хватает буфера для чтения, при увеличении буфера появляется ошибка загрузки
// поэтому было решено разделить файлы JSON на мелкие до 2048 байт
// 
//-----------------(методы класса MyClass_JSON)----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_JSON::loadConfiguration(const char *filename, MyClass_Config *my_config) {
  my_config->status.lib_JSON.version_lib = "0.16";
  my_config->status.lib_JSON.date_lib    = "09.11.2022";
  
  SPIFFS.begin();
  Serial.println(F("Loading configuration..."));
  File file = SPIFFS.open(filename,"r");
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, file);
  Serial.print("Read file ");
  Serial.print(filename);
  if (error){
    Serial.println(" - ERROR");
    error_load_config = true;
  }else{
    Serial.println(" - OK");
    error_load_config = false;
  }
  file.close();
  
  if (!error_load_config){
    byte n1, n2, n3;
    String s;
    s = "/" + doc["WiFi"].as<String>();
    Config_WiFi(s.c_str(), my_config);
    s = "/" + doc["NTP"].as<String>();
    Config_NTP(s.c_str(), my_config);
    s = "/" + doc["FTP"].as<String>();
    Config_FTP(s.c_str(), my_config);
    s = "/" + doc["MQTT"].as<String>();
    Config_MQTT(s.c_str(), my_config);
    s = "/" + doc["Ethernet"].as<String>();
    Config_Ethernet(s.c_str(), my_config);
    s = "/" + doc["PCF8575"].as<String>();
    Config_PCF8575(s.c_str(), my_config);
    s = "/" + doc["DS18B20"].as<String>();
    Config_DS18B20(s.c_str(), my_config);
    s = "/" + doc["RS_Trigers"].as<String>();
    Config_Trigers(s.c_str(), my_config);

    n1 = doc["DOs"]["Col"].as<byte>();
    n2 = doc["DOs"]["ColJSON"].as<byte>();
    n3 = 0; // позиция старта
    my_config->config.DOs.col = n1;
    my_config->config.DOs.DO = new MyStruct_DO[n1];    
    for (byte a=0; a<n2; a++){
      s = "/" + doc["DOs"]["file"][a].as<String>();
      n3 = Config_DOs(s.c_str(), my_config, n3);
    }   

    n1 = doc["DIs"]["Col"].as<byte>();
    n2 = doc["DIs"]["ColJSON"].as<byte>();
    n3 = 0; // позиция старта
    my_config->config.DIs.col = n1;
    my_config->config.DIs.DI = new MyStruct_DI[n1];    
    for (byte a=0; a<n2; a++){
      s = "/" + doc["DIs"]["file"][a].as<String>();
      n3 = Config_DIs(s.c_str(), my_config, n3);
    }

  }else{
    SetDefaultConfig(my_config);
  }
  SPIFFS.end();
}
//-----------------(методы класса MyClass_JSON)----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_JSON::SetDefaultConfig(MyClass_Config *my_config){
  my_config->config.Ethernet.enable = true;
  my_config->config.Ethernet.dhcp = true;
  my_config->config.Ethernet.mac = new byte[6];
  my_config->config.Ethernet.mac[0] = 0;
  my_config->config.Ethernet.mac[1] = 1;
  my_config->config.Ethernet.mac[2] = 2;
  my_config->config.Ethernet.mac[3] = 3;
  my_config->config.Ethernet.mac[4] = 4;
  my_config->config.Ethernet.mac[5] = 5;
  my_config->config.Ethernet.ip = new byte[4];
  my_config->config.Ethernet.ip[0] = 0;
  my_config->config.Ethernet.ip[1] = 0;
  my_config->config.Ethernet.ip[2] = 0;
  my_config->config.Ethernet.ip[3] = 0;
}
//-----------------(методы класса MyClass_JSON)----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_JSON::Config_WiFi(const char *filename, MyClass_Config *my_config){
  File file = SPIFFS.open(filename,"r");
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, file);
  Serial.print("Read file ");
  Serial.print(filename);
  if (error){
    Serial.println(" - ERROR");
    error_load_config = true;
  }else{
    Serial.println(" - OK");
    error_load_config = false;
  }
  file.close();

  if (!error_load_config){
    byte i1,n1,i2,n2,s1;
    String s;
    my_config->config.WiFi.Enable = doc["WiFi"]["Enable"].as<bool>();
    my_config->config.WiFi.Password = doc["WiFi"]["Password"].as<String>();
    my_config->config.WiFi.SSID = doc["WiFi"]["SSID"].as<String>(); 
  }
  doc.clear();
}
//-----------------(методы класса MyClass_JSON)----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_JSON::Config_NTP(const char *filename, MyClass_Config *my_config){
  File file = SPIFFS.open(filename,"r");
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, file);
  Serial.print("Read file ");
  Serial.print(filename);
  if (error){
    Serial.println(" - ERROR");
    error_load_config = true;
  }else{
    Serial.println(" - OK");
    error_load_config = false;
  }
  file.close();

  if (!error_load_config){
    byte i1,n1,i2,n2,s1;
    String s;    
    my_config->config.NTP.Enable = doc["NTP"]["Enable"].as<bool>();
    my_config->config.NTP.Server = doc["NTP"]["Server"].as<String>();
    my_config->config.NTP.offset = doc["NTP"]["Offset"].as<unsigned int>();
  }
  doc.clear();
}
//-----------------(методы класса MyClass_JSON)----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_JSON::Config_FTP(const char *filename, MyClass_Config *my_config){
  File file = SPIFFS.open(filename,"r");
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, file);
  Serial.print("Read file ");
  Serial.print(filename);
  if (error){
    Serial.println(" - ERROR");
    error_load_config = true;
  }else{
    Serial.println(" - OK");
    error_load_config = false;
  }
  file.close();

  if (!error_load_config){
    byte i1,n1,i2,n2,s1;
    String s;    
    my_config->config.FTP.Enable = doc["FTP"]["Enable"].as<bool>();
    my_config->config.FTP.Password = doc["FTP"]["Password"].as<String>();
    my_config->config.FTP.Login = doc["FTP"]["Login"].as<String>();
  }
  doc.clear();
}
//-----------------(методы класса MyClass_JSON)----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_JSON::Config_MQTT(const char *filename, MyClass_Config *my_config){
  File file = SPIFFS.open(filename,"r");
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, file);
  Serial.print("Read file ");
  Serial.print(filename);
  if (error){
    Serial.println(" - ERROR");
    error_load_config = true;
  }else{
    Serial.println(" - OK");
    error_load_config = false;
  }
  file.close();

  if (!error_load_config){
    byte i1,n1,i2,n2,s1;
    String s;    
    my_config->config.MQTT.Enable = doc["MQTT"]["Enable"].as<bool>();
    my_config->config.MQTT.Password = doc["MQTT"]["Password"].as<String>();
    my_config->config.MQTT.Login = doc["MQTT"]["Login"].as<String>();
    my_config->config.MQTT.Home_topic = doc["MQTT"]["Home_topic"].as<String>();
    my_config->config.MQTT.port = doc["MQTT"]["Port"].as<unsigned int>();
    my_config->config.MQTT.Server = doc["MQTT"]["Server"].as<String>();
  }
  doc.clear();
}
//-----------------(методы класса MyClass_JSON)----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_JSON::Config_Ethernet(const char *filename, MyClass_Config *my_config){
  File file = SPIFFS.open(filename,"r");
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, file);
  Serial.print("Read file ");
  Serial.print(filename);
  if (error){
    Serial.println(" - ERROR");
    error_load_config = true;
  }else{
    Serial.println(" - OK");
    error_load_config = false;
  }
  file.close();

  if (!error_load_config){
    byte i1,n1,i2,n2,s1;
    String s;    
    my_config->config.Ethernet.enable = doc["Ethernet"]["Enable"].as<bool>();
    my_config->config.Ethernet.dhcp = doc["Ethernet"]["DHCP"].as<bool>();
    s = doc["Ethernet"]["IP"].as<String>();
    my_config->config.Ethernet.ip = new byte[4];
    get_byte_ip_from_string(my_config->config.Ethernet.ip, s);
    s = doc["Ethernet"]["MAC"].as<String>();
    my_config->config.Ethernet.mac = new byte[6];
    get_byte_mac_from_string(my_config->config.Ethernet.mac, s);
  }else{
    SetDefaultConfig(my_config);
  }
  doc.clear();
}
//-----------------(методы класса MyClass_JSON)----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_JSON::Config_PCF8575(const char *filename, MyClass_Config *my_config){
  File file = SPIFFS.open(filename,"r");
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, file);
  Serial.print("Read file ");
  Serial.print(filename);
  if (error){
    Serial.println(" - ERROR");
    error_load_config = true;
  }else{
    Serial.println(" - OK");
    error_load_config = false;
  }
  file.close();

  if (!error_load_config){
    byte i1,n1,i2,n2,s1;
    String s;    
    my_config->config.PCF8575.col = doc["PCF8575"]["Col"].as<unsigned char>();
    n1 = my_config->config.PCF8575.col;
    my_config->config.PCF8575.board = new MyStruct_board_PCF8575[n1];
    for (i1 = 0; i1 < n1; i1++){
      my_config->config.PCF8575.board[i1].I2C_adr = doc["PCF8575"]["board"][i1]["I2C_adr"].as<unsigned char>();
  
      s = doc["PCF8575"]["board"][i1]["Type"].as<String>();
      if (s == "DI"){s1 = 1;}
      if (s == "DO"){s1 = 2;}
      if (s == "DIO"){s1 = 3;}
      my_config->config.PCF8575.board[i1].Type = s1;
    } 
  }
  doc.clear();
}
//-----------------(методы класса MyClass_JSON)----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_JSON::Config_DS18B20(const char *filename, MyClass_Config *my_config){
  File file = SPIFFS.open(filename,"r");
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, file);
  Serial.print("Read file ");
  Serial.print(filename);
  if (error){
    Serial.println(" - ERROR");
    error_load_config = true;
  }else{
    Serial.println(" - OK");
    error_load_config = false;
  }
  file.close();

  if (!error_load_config){
    byte i1,n1,i2,n2,s1;
    String s;    
    my_config->config.DS18B20.col = doc["DS18B20"]["Col"].as<unsigned char>();
    n1 = my_config->config.DS18B20.col;
    my_config->config.DS18B20.sensors = new MyStruct_sensor_DS18B20[n1];
    for (i1 = 0; i1 < n1; i1++){
      my_config->config.DS18B20.sensors[i1].address = doc["DS18B20"]["sensors"][i1]["address"].as<String>();
  
      my_config->config.DS18B20.sensors[i1].MQTT_out.enable = doc["DS18B20"]["sensors"][i1]["MQTT_out"]["Enable"].as<bool>();
      my_config->config.DS18B20.sensors[i1].MQTT_out.col = doc["DS18B20"]["sensors"][i1]["MQTT_out"]["Col"].as<unsigned char>();
      n2 = my_config->config.DS18B20.sensors[i1].MQTT_out.col;
      my_config->config.DS18B20.sensors[i1].MQTT_out.topic = new String[n2];
      for (i2 =0; i2<n2; i2++){ 
        my_config->config.DS18B20.sensors[i1].MQTT_out.topic[i2] = doc["DS18B20"]["sensors"][i1]["MQTT_out"]["topic"][i2].as<String>();
      }
    } 
  }
  doc.clear();
}
//-----------------(методы класса MyClass_JSON)----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_JSON::Config_Trigers(const char *filename, MyClass_Config *my_config){
  File file = SPIFFS.open(filename,"r");
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, file);
  Serial.print("Read file ");
  Serial.print(filename);
  if (error){
    Serial.println(" - ERROR");
    error_load_config = true;
  }else{
    Serial.println(" - OK");
    error_load_config = false;
  }
  file.close();

  if (!error_load_config){
    byte i1,n1,i2,n2,s1;
    String s;    
    my_config->config.Trigers.col = doc["RS_Trigers"]["Col"].as<unsigned char>();
    n1 = my_config->config.Trigers.col;
    my_config->config.Trigers.Trigers = new MyStruct_RS_Triger[n1];
    for (i1 = 0; i1 < n1; i1++){
      my_config->config.Trigers.Trigers[i1].col = doc["RS_Trigers"]["Triger"][i1]["Col"].as<byte>();
      my_config->config.Trigers.Trigers[i1].Description = doc["RS_Trigers"]["Triger"][i1]["Description"].as<String>();
      n2 = my_config->config.Trigers.Trigers[i1].col;
      my_config->config.Trigers.Trigers[i1].DI = new byte[n2];
      for (i2 =0; i2<n2; i2++){ 
        my_config->config.Trigers.Trigers[i1].DI[i2] = doc["RS_Trigers"]["Triger"][i1]["DI"][i2].as<byte>();
      }
    } 
  }
  doc.clear();
}
//-----------------(методы класса MyClass_JSON)----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
byte MyClass_JSON::Config_DIs(const char *filename, MyClass_Config *my_config, byte start_num){
  File file = SPIFFS.open(filename,"r");
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, file);
  Serial.print("Read file ");
  Serial.print(filename);
  if (error){
    Serial.println(" - ERROR");
    error_load_config = true;
  }else{
    Serial.println(" - OK");
    error_load_config = false;
  }
  file.close();

  byte i1,n1,i2,n2,s1,i3;
  String s;    
  if (!error_load_config){
    //**********************************(DI)**************************************************
    n1 = doc["DIs"]["Col"].as<unsigned char>();
    for (i1 = 0; i1 < n1; i1++){
      i3 = start_num + i1;
      s = doc["DIs"]["DI"][i1]["Type"].as<String>();
      if (s == "inner_PCF8575"){my_config->config.DIs.DI[i3].Type = 2;}
      if (s == "external_MQTT"){my_config->config.DIs.DI[i3].Type = 3;}
      my_config->config.DIs.DI[i3].Description = doc["DIs"]["DI"][i1]["Description"].as<String>();

      my_config->config.DIs.DI[i3].PCF8575_adr.board_num = doc["DIs"]["DI"][i1]["PCF8575"]["board_num"].as<unsigned char>();
      my_config->config.DIs.DI[i3].PCF8575_adr.input_num = doc["DIs"]["DI"][i1]["PCF8575"]["input_num"].as<unsigned char>();

      my_config->config.DIs.DI[i3].MQTT_out.enable = doc["DIs"]["DI"][i1]["MQTT_out"]["Enable"].as<bool>();
      my_config->config.DIs.DI[i3].MQTT_out.col = doc["DIs"]["DI"][i1]["MQTT_out"]["Col"].as<byte>();
      n2 = my_config->config.DIs.DI[i3].MQTT_out.col;
      my_config->config.DIs.DI[i3].MQTT_out.topic = new String[n2];
      for (i2 = 0; i2 < n2; i2++){ 
        my_config->config.DIs.DI[i3].MQTT_out.topic[i2] = doc["DIs"]["DI"][i1]["MQTT_out"]["topic"][i2].as<String>();
      }
      my_config->config.DIs.DI[i3].MQTT_in.enable = doc["DIs"]["DI"][i1]["MQTT_in"]["Enable"].as<bool>();
      my_config->config.DIs.DI[i3].MQTT_in.topic  = doc["DIs"]["DI"][i1]["MQTT_in"]["topic"].as<String>();
    }
  }
  doc.clear();
  return (start_num + n1);
}
//-----------------(методы класса MyClass_JSON)----------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
byte MyClass_JSON::Config_DOs(const char *filename, MyClass_Config *my_config, byte start_num){
  File file = SPIFFS.open(filename,"r");
  // StaticJsonDocument<2048> doc;
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, file);
  Serial.print("Read file ");
  Serial.print(filename);
  if (error){
    Serial.println(" - ERROR");
    error_load_config = true;
  }else{
    Serial.println(" - OK");
    error_load_config = false;
  }
  file.close();

  byte i1,n1,i2,n2,s1,i3;
  String s;    
  if (!error_load_config){
    //**********************************(DO)**************************************************
    n1 = doc["DOs"]["Col"].as<unsigned char>();
    for (i1 = 0; i1 < n1; i1++){
      i3 = start_num + i1;
      s = doc["DOs"]["DO"][i1]["Type"].as<String>();
      if (s == "inner_PCF8575"){my_config->config.DOs.DO[i3].Type = 2;}
      my_config->config.DOs.DO[i3].Description = doc["DOs"]["DO"][i1]["Description"].as<String>();

      my_config->config.DOs.DO[i3].PCF8575_adr.board_num = doc["DOs"]["DO"][i1]["PCF8575"]["board_num"].as<unsigned char>();
      my_config->config.DOs.DO[i3].PCF8575_adr.input_num = doc["DOs"]["DO"][i1]["PCF8575"]["input_num"].as<unsigned char>();

      my_config->config.DOs.DO[i3].MQTT_out.enable = doc["DOs"]["DO"][i1]["MQTT_out"]["Enable"].as<bool>();
      my_config->config.DOs.DO[i3].MQTT_out.col = doc["DOs"]["DO"][i1]["MQTT_out"]["Col"].as<byte>();
      n2 = my_config->config.DOs.DO[i3].MQTT_out.col;
      my_config->config.DOs.DO[i3].MQTT_out.topic = new String[n2];
      for (i2 = 0; i2 < n2; i2++){ 
        my_config->config.DOs.DO[i3].MQTT_out.topic[i2] = doc["DOs"]["DO"][i1]["MQTT_out"]["topic"][i2].as<String>();
      }
      my_config->config.DOs.DO[i3].LinkTo.Enable = doc["DOs"]["DO"][i1]["LinkTo"]["Enable"].as<bool>();
      my_config->config.DOs.DO[i3].LinkTo.link = doc["DOs"]["DO"][i1]["LinkTo"]["link"].as<byte>();
      s = doc["DOs"]["DO"][i1]["LinkTo"]["type"].as<String>();
      if (s == "RS_Triger") my_config->config.DOs.DO[i3].LinkTo.type = 1;
      if (s == "DI") my_config->config.DOs.DO[i3].LinkTo.type = 2;
      my_config->config.DOs.DO[i3].Cmd = 0;      // зануляем выхода
    }
  }
  doc.clear();
  return (start_num + n1);
}