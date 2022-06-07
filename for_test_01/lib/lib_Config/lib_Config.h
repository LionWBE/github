//version 0.12
#ifndef lib_Config_h
#define lib_Config_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//********************************
struct MyStruct_PCF8575_adr {
  byte board_num;
  byte input_num;
};
struct MyStruct_MQTT_out {
  bool enable;
  byte col;
  String *topic;
};
struct MyStruct_MQTT_in {
  bool enable;
  String topic;
};
struct MyStruct_Time_setting {
  uint16 t_in;
  uint16 t_out;
};
struct MyStruct_WiFi {
  bool Enable;
  String SSID;
  String Password;
};
struct MyStruct_FTP {
  bool Enable;
  String Login;
  String Password;
};
struct MyStruct_NTP {
  bool Enable;
  String Server;
  int offset;
};
struct MyStruct_MQTT {
  bool Enable;
  String Server;
  String Login;
  String Password;
  String Home_topic;
  int port;
};
struct MyStruct_board_PCF8575 {
  byte I2C_adr;
  byte Type; // 1 - DI, 2 - DO
};
//********************************
struct MyStruct_DI {
  byte Type; // 1 - inner_SPI, 2 - 
  String Description;
  MyStruct_PCF8575_adr PCF8575_adr;
  MyStruct_MQTT_out MQTT_out;
  MyStruct_MQTT_in MQTT_in;
  MyStruct_Time_setting Time_setting;
};
//********************************
struct MyStruct_DIs {
  byte col; 
  MyStruct_DI *DI;
};
struct MyStruct_PCF8575 {
  byte col;
  MyStruct_board_PCF8575 *board;
};
//********************************
//********************************
struct MyStruct_sensor_DS18B20 {
  String address;
  String Description;
  MyStruct_MQTT_out MQTT_out;
};
struct MyStruct_DS18B20 {
  byte col;
  MyStruct_sensor_DS18B20 *sensors;
};
//********************************
struct MyStruct_Config {
  MyStruct_DIs DIs;
  MyStruct_WiFi WiFi;
  MyStruct_MQTT MQTT;
  MyStruct_FTP FTP;
  MyStruct_NTP NTP;
  MyStruct_PCF8575 PCF8575;
  MyStruct_DS18B20 DS18B20;
};
//********************************
struct MyStruct_data{
  String WiFi_IP;
  String WiFi_MAC;
  String time;
  String date;
  String time_stamp;
  int *MQTT;
  int *Tags;
  uint16_t Tags_queue_count; // количество значений в стеке для публикации
  uint16_t Tags_count;       // количество тегов в базе
  uint32_t dt[10];
};
//********************************
class MyClass_Config {
  public:
    MyStruct_Config config;
    MyStruct_data data;
    void print();
  private:
    void print_DIs(MyStruct_DIs, byte);
    void print_DI(MyStruct_DI, byte);
    void print_PCF8575_adr(MyStruct_PCF8575_adr, byte);
    void print_MQTT_out(MyStruct_MQTT_out , byte);
    void print_MQTT(MyStruct_MQTT , byte);
    void print_WiFi(MyStruct_WiFi, byte);
    void print_NTP(MyStruct_NTP, byte);
    void print_FTP(MyStruct_FTP, byte);
    void print_PCF8575(MyStruct_PCF8575, byte);
    void print_board_PCF8575(MyStruct_board_PCF8575, byte);
    void print_sensor_DS18B20(MyStruct_sensor_DS18B20, byte);
    void print_DS18B20(MyStruct_DS18B20, byte);
    void print_(byte);
    void print_param(String, String, byte);
    void print_param(String, byte, byte);
    void print_param(String, bool, byte);
    void print_param(String, int, byte);
    void print_param(String, byte);
};
#endif
