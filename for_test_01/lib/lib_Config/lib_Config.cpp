//version 0.17 01/11/2022
#include "lib_Config.h"
//-----------------(методы класса MyClass_WiFi)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::setup(){
  status.lib_Config.version_lib = "0.17";
  status.lib_Config.date_lib    = "01.11.2022";
}
//-----------------(методы класса MyClass_WiFi)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
String MyClass_Config::GetStatus(MyStruct_Status status, String name){
  String rez;
  rez = name + ", ver = " + status.version_lib + ", date = " + status.date_lib;
  return rez;
}
//-----------------(методы класса MyClass_WiFi)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
String MyClass_Config::GetALLStatus(){
  String rez = "";
  rez += GetStatus(status.lib_Ethernet, "lib_Ethernet") + "\n";
  rez += GetStatus(status.lib_JSON, "lib_JSON") + "\n";
  rez += GetStatus(status.lib_Config, "lib_Config") + "\n";
  rez += GetStatus(status.lib_Mini, "lib_Mini") + "\n";
  rez += GetStatus(status.lib_PCF8575, "lib_PCF8575") + "\n";
  rez += GetStatus(status.lib_MQTT, "lib_MQTT") + "\n";
  rez += GetStatus(status.SysInfo, "lib_SysInfo") + "\n";
  rez += GetStatus(status.DIs, "lib_DIs") + "\n";
  rez += GetStatus(status.NTP, "lib_NTP") + "\n";
  rez += GetStatus(status.MQTT_Tag, "lib_Tags") + "\n";
  rez += GetStatus(status.WiFi, "lib_WiFi") + "\n";
  return rez;
}
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
  print_RS_Trigers(config.Trigers, 3);
  print_Ethernet(config.Ethernet, 3);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_Ethernet(MyStruct_Ethernet Eternet, byte k){
  print_param("Eternet", k);
  print_param("enable", Eternet.enable, k+3);
  print_param("dhcp", Eternet.dhcp, k+3);
  print_param("mac", Eternet.mac, 6, k+3, ':');
  print_param("ip", Eternet.ip, 4, k+3, '.');
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
  print_MQTT_in(DI.MQTT_in,k+3);
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
  print_LinkTo(DO.LinkTo,k+3);
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
void MyClass_Config::print_MQTT_in(MyStruct_MQTT_in MQTT_in, byte k){
  print_param("MQTT_in", k);
  print_param("enable", MQTT_in.enable, k+3);
  print_param("topic", MQTT_in.topic, k+3);
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
void MyClass_Config::print_RS_Trigers(MyStruct_RS_Trigers Trigers, byte k){
  print_param("RS_Trigers", k);
  print_param("col", Trigers.col, k+3);
  for(byte i=0; i<Trigers.col; i++){
    print_RS_Triger(Trigers.Trigers[i], k+3);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_RS_Triger(MyStruct_RS_Triger Triger, byte k){
  print_param("RS_Triger", k);
  print_param("col", Triger.col, k+3);
  print_param("Description", Triger.Description, k+3);
  for(byte i=0; i<Triger.col; i++){
    print_param("DI", Triger.DI[i], k+3);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_LinkTo(MyStruct_LinkTo LinkTo, byte k){
  print_param("LinkTo", k);
  print_param("Enable", LinkTo.Enable, k+3);
  print_param("type", LinkTo.type, k+3);
  print_param("link", LinkTo.link, k+3);
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
void MyClass_Config::print_param(String name_param, byte *val_param, byte len, byte k, char separator){ 
  print_(k);
  Serial.print(name_param + " = ");
  for (byte i = 0; i < len; i++){
    Serial.print(val_param[i]);
    if(i < len - 1) Serial.print(separator);
  }
  Serial.println();
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Config::print_param(String name_param, byte k){
  print_(k);
  Serial.println(name_param );
}