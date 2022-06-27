#include "lib_JSON.h"
//-----------------(методы класса MyClass_WiFi)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_JSON::loadConfiguration(const char *filename, MyClass_Config *my_config) {
  SPIFFS.begin();

  Serial.println(F("Loading configuration..."));
  File file = SPIFFS.open(filename,"r");
  
  StaticJsonDocument<2048> doc;
  DeserializationError error = deserializeJson(doc, file);
  if (error){
    Serial.println(F("Failed to read file"));
    error_load_config = false;
  }else{
    error_load_config = true;
  }
  file.close();

  if (error_load_config){
    byte i1,n1,i2,n2,s1;
    String s;
    //**********************************(WiFi)**************************************************
    my_config->config.WiFi.Enable = doc["WiFi"]["Enable"].as<bool>();
    my_config->config.WiFi.Password = doc["WiFi"]["Password"].as<String>();
    my_config->config.WiFi.SSID = doc["WiFi"]["SSID"].as<String>();
    //**********************************(NTP)**************************************************
    my_config->config.NTP.Enable = doc["NTP"]["Enable"].as<bool>();
    my_config->config.NTP.Server = doc["NTP"]["Server"].as<String>();
    my_config->config.NTP.offset = doc["NTP"]["Offset"].as<unsigned int>();
    //**********************************(FTP)**************************************************
    my_config->config.FTP.Enable = doc["FTP"]["Enable"].as<bool>();
    my_config->config.FTP.Password = doc["FTP"]["Password"].as<String>();
    my_config->config.FTP.Login = doc["FTP"]["Login"].as<String>();
    //**********************************(MQTT)**************************************************
    my_config->config.MQTT.Enable = doc["MQTT"]["Enable"].as<bool>();
    my_config->config.MQTT.Password = doc["MQTT"]["Password"].as<String>();
    my_config->config.MQTT.Login = doc["MQTT"]["Login"].as<String>();
    my_config->config.MQTT.Home_topic = doc["MQTT"]["Home_topic"].as<String>();
    my_config->config.MQTT.port = doc["MQTT"]["Port"].as<unsigned int>();
    my_config->config.MQTT.Server = doc["MQTT"]["Server"].as<String>();
    //**********************************(PCF8575)**************************************************
    my_config->config.PCF8575.col = doc["PCF8575"]["Col"].as<unsigned char>();
    n1 = my_config->config.PCF8575.col;
    my_config->config.PCF8575.board = new MyStruct_board_PCF8575[n1];
    for (i1 = 0; i1 < n1; i1++){
      my_config->config.PCF8575.board[i1].I2C_adr = doc["PCF8575"]["board"][i1]["I2C_adr"].as<unsigned char>();
  
      s = doc["PCF8575"]["board"][i1]["Type"].as<String>();
      if (s == "DI"){s1 = 1;}
      if (s == "DO"){s1 = 2;}
      my_config->config.PCF8575.board[i1].Type = s1;
    } 
    //**********************************(DI)**************************************************
    my_config->config.DIs.col = doc["DIs"]["Col"].as<unsigned char>();
    n1 = my_config->config.DIs.col;
    my_config->config.DIs.DI = new MyStruct_DI[n1];
    for (i1 = 0; i1<n1; i1++){
      s = doc["DIs"]["DI"][i1]["Type"].as<String>();
      if (s == "inner_PCF8575"){my_config->config.DIs.DI[i1].Type = 1;}
      my_config->config.DIs.DI[i1].Description = doc["DIs"]["DI"][i1]["Description"].as<String>();

      my_config->config.DIs.DI[i1].PCF8575_adr.board_num = doc["DIs"]["DI"][i1]["PCF8575"]["board_num"].as<unsigned char>();
      my_config->config.DIs.DI[i1].PCF8575_adr.input_num = doc["DIs"]["DI"][i1]["PCF8575"]["input_num"].as<unsigned char>();

      my_config->config.DIs.DI[i1].MQTT_out.enable = doc["DIs"]["DI"][i1]["MQTT_out"]["Enable"].as<bool>();
      my_config->config.DIs.DI[i1].MQTT_out.col = doc["DIs"]["DI"][i1]["MQTT_out"]["Col"].as<unsigned char>();
      n2 = my_config->config.DIs.DI[i1].MQTT_out.col;
      my_config->config.DIs.DI[i1].MQTT_out.topic = new String[n2];
      for (i2 =0; i2<n2; i2++){ 
        my_config->config.DIs.DI[i1].MQTT_out.topic[i2] = doc["DIs"]["DI"][i1]["MQTT_out"]["topic"][i2].as<String>();
      }
    }
    //**********************************(DO)**************************************************
    my_config->config.DOs.col = doc["DOs"]["Col"].as<unsigned char>();
    n1 = my_config->config.DOs.col;
    my_config->config.DOs.DO = new MyStruct_DO[n1];
    for (i1 = 0; i1<n1; i1++){
      s = doc["DOs"]["DO"][i1]["Type"].as<String>();
      if (s == "inner_PCF8575"){my_config->config.DOs.DO[i1].Type = 1;}
      my_config->config.DOs.DO[i1].Description = doc["DOs"]["DO"][i1]["Description"].as<String>();

      my_config->config.DOs.DO[i1].PCF8575_adr.board_num = doc["DOs"]["DO"][i1]["PCF8575"]["board_num"].as<unsigned char>();
      my_config->config.DOs.DO[i1].PCF8575_adr.input_num = doc["DOs"]["DO"][i1]["PCF8575"]["input_num"].as<unsigned char>();

      my_config->config.DOs.DO[i1].MQTT_out.enable = doc["DOs"]["DO"][i1]["MQTT_out"]["Enable"].as<bool>();
      my_config->config.DOs.DO[i1].MQTT_out.col = doc["DOs"]["DO"][i1]["MQTT_out"]["Col"].as<unsigned char>();
      n2 = my_config->config.DOs.DO[i1].MQTT_out.col;
      my_config->config.DOs.DO[i1].MQTT_out.topic = new String[n2];
      for (i2 =0; i2<n2; i2++){ 
        my_config->config.DOs.DO[i1].MQTT_out.topic[i2] = doc["DOs"]["DO"][i1]["MQTT_out"]["topic"][i2].as<String>();
      }
    }  
    //**********************************(DS18B20)**************************************************
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
}