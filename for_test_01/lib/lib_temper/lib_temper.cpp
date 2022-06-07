#include "lib_temper.h"
//-----------------(методы класса MyClass_temper)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_temper::start(){
  // sensors.requestTemperatures();
  for (int i = 0; i < deviceCount; i++){
    // SensorTemp[i] = sensors.getTempC(temperatureSensors[i]); // считываем температуру с датчика
    // my_Tags->SetTagVal(MQTT_link_temp_val[i], &SensorTemp[i]);
  }
}
//-----------------(методы класса MyClass_temper)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_temper::setup(MyClass_Config *my_config){
  settings = my_config;
  // my_Tags = (MyClass_MQTT_Tag*)settings->data.Tags;
  deviceCount = settings->config.DS18B20.col;
  sensors = new MicroDS18B20[deviceCount];
  for (byte i = 0; i < deviceCount; i++){
    String addr = settings->config.DS18B20.sensors[i].address;
    uint8_t s_addr[8];
    for (byte m = 0; m < 8; m++){
      String addr_p = addr.substring(m*2,m*2+2);
      s_addr[m] = hex_to_dec(addr_p);
    }
    sensors[i].setting(D3, s_addr);
  }
  SensorTemp = new float[deviceCount];

  String topic1, topic2;
  MQTT_link_temp_val = new uint16_t[deviceCount];
  MQTT_link_sensor_addres = new uint16_t[deviceCount];
  for (int i = 0; i < deviceCount; i++){
    topic1 = settings->config.MQTT.Home_topic + "analog/tempers/temper" + String(i);
    topic2 = topic1 + "/val";
    // MQTT_link_temp_val[i] = my_Tags->CreateNewTag(&topic2, true);
    topic2 = topic1 + "/addres";
    // MQTT_link_sensor_addres[i] = my_Tags->CreateNewTag(&topic2);
    // my_Tags->SetTagVal(MQTT_link_sensor_addres[i], &settings->config.DS18B20.sensors[i].address);
  }
}
//-----------------(методы класса MyClass_temper)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_temper::printTemperatures(){
  for (int i = 0; i < deviceCount; i++){
    Serial.print(settings->config.DS18B20.sensors[i].address); // Выводим название датчика
    Serial.print(" : ");
    Serial.println(SensorTemp[i]); // Выводим температуру с датчика
  }
}