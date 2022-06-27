#include "lib_Config.h"
//-----------------(методы класса MyClass_WiFi)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print(){
  print_WiFi(config.WiFi, 3);
  print_PCF8575(config.PCF8575, 3);
  print_MQTT(config.MQTT, 3);
  print_FTP(config.FTP, 3);
  print_DIs(config.DIs, 3);
  print_DOs(config.DOs, 3);
  print_NTP(config.NTP, 3);
  print_DS18B20(config.DS18B20, 3);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_DIs(MyStruct_DIs DIs, byte k){
  print_param("DIs", k);
  print_param("col", DIs.col, k+3);
  for (byte i = 0; i < DIs.col; i++) {
    print_DI(DIs.DI[i], k+3);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_DI(MyStruct_DI DI, byte k){
  print_param("DI", k);
  print_param("Type", DI.Type, k+3);
  print_param("Description", DI.Description, k+3);
  print_PCF8575_adr(DI.PCF8575_adr, k+3);
  print_MQTT_out(DI.MQTT_out,k+3);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_DOs(MyStruct_DOs DOs, byte k){
  print_param("DOs", k);
  print_param("col", DOs.col, k+3);
  for (byte i = 0; i < DOs.col; i++) {
    print_DO(DOs.DO[i], k+3);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_DO(MyStruct_DO DO, byte k){
  print_param("DO", k);
  print_param("Type", DO.Type, k+3);
  print_param("Description", DO.Description, k+3);
  print_PCF8575_adr(DO.PCF8575_adr, k+3);
  print_MQTT_out(DO.MQTT_out,k+3);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_PCF8575_adr(MyStruct_PCF8575_adr PCF8575_adr, byte k){
  print_param("PCF8575", k);
  print_param("board_num", PCF8575_adr.board_num, k+3);
  print_param("input_num", PCF8575_adr.input_num, k+3); 
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_MQTT_out(MyStruct_MQTT_out MQTT_out, byte k){
  print_param("MQTT_out", k);
  print_param("enable", MQTT_out.enable, k+3);
  print_param("col", MQTT_out.col, k+3);
  for(byte i=0; i<MQTT_out.col; i++){
    print_param("topic["+String(i)+"]", MQTT_out.topic[i], k+3);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_WiFi(MyStruct_WiFi WiFi, byte k){
  print_param("WiFi", k);
  print_param("Enable", WiFi.Enable, k+3);
  print_param("SSID", WiFi.SSID, k+3);
  print_param("Password", WiFi.Password, k+3);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_NTP(MyStruct_NTP NTP, byte k){
  print_param("NTP", k);
  print_param("Enable", NTP.Enable, k+3);
  print_param("Server", NTP.Server, k+3);
  print_param("offset", NTP.offset, k+3);  
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_MQTT(MyStruct_MQTT MQTT, byte k){
  print_param("MQTT", k);
  print_param("Enable", MQTT.Enable, k+3);
  print_param("Server", MQTT.Server, k+3);
  print_param("port", MQTT.port, k+3);
  print_param("Login", MQTT.Login, k+3);
  print_param("Password", MQTT.Password, k+3);
  print_param("Home_topic", MQTT.Home_topic, k+3);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_FTP(MyStruct_FTP FTP, byte k){
  print_param("FTP", k);
  print_param("Enable", FTP.Enable, k+3);
  print_param("Login", FTP.Login, k+3);
  print_param("Password", FTP.Password, k+3);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_PCF8575(MyStruct_PCF8575 PCF8575, byte k){
  print_param("PCF8575", k);
  print_param("col", PCF8575.col, k+3);
  for(byte i=0; i<PCF8575.col; i++){
    print_board_PCF8575(PCF8575.board[i], k+3);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_board_PCF8575(MyStruct_board_PCF8575 board_PCF8575, byte k){
  print_param("board_PCF8575", k);
  print_param("I2C_adr", board_PCF8575.I2C_adr, k+3);
  print_param("Type", board_PCF8575.Type, k+3);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_DS18B20(MyStruct_DS18B20 DS18B20, byte k){
  print_param("DS18B20", k);
  print_param("col", DS18B20.col, k+3);
  for(byte i=0; i<DS18B20.col; i++){
    print_sensor_DS18B20(DS18B20.sensors[i], k+3);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_sensor_DS18B20(MyStruct_sensor_DS18B20 sensor_DS18B20, byte k){
  print_param("sensor_DS18B20", k);
  print_param("address", sensor_DS18B20.address, k+3);
  print_MQTT_out(sensor_DS18B20.MQTT_out,k+3);
}
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_(byte k){
  for (byte i=0; i<k; i++){
    Serial.print(" ");
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_param(String name_param, String val_param, byte k){
  print_(k);
  Serial.print(name_param + " = ");
  Serial.println(val_param);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_param(String name_param, byte val_param, byte k){
  print_(k);
  Serial.print(name_param + " = ");
  Serial.println(val_param);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_param(String name_param, bool val_param, byte k){
  print_(k);
  Serial.print(name_param + " = ");
  Serial.println(val_param);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_param(String name_param, int val_param, byte k){
  print_(k);
  Serial.print(name_param + " = ");
  Serial.println(val_param);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_param(String name_param, byte k){
  print_(k);
  Serial.println(name_param );
}