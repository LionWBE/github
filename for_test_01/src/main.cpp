#include <Arduino.h>
//***************************************************************************************************
#include <lib_JSON.h>
#include <lib_Config.h>
#include <lib_pcf8575.h>

#include <lib_WiFi.h>
#include <lib_FTP.h>
// **************************************************************************************************************
MyClass_JSON my_MyClass_JSON;
MyClass_Config my_config;
// MyClass_MQTT_Tag my_Tags;
MyClass_all_PCF8575 my_PCF8575;

MyClass_WiFi my_WiFi;
MyClass_FTP my_FTP;

MyClass_timer t1;
MyClass_timer t2;
bool cmd_1;
// **************************************************************************************************************
bool RSTrig(byte col, bool *param);
bool trig(byte num);
// **************************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");

  my_MyClass_JSON.loadConfiguration("/settings.json", &my_config); //загрузка конфигурации

  // delay(2000); 
  my_WiFi.setup(&my_config);
  // delay(2000); 
  my_FTP.setup();
  // delay(2000); 

  my_config.print(); 
  // delay(1000);
  // my_Tags.setup(&my_config);
  my_PCF8575.setup(&my_config);
  Serial.println("All setup done !!!");

  t1.time_delay_const = 1000;
  t1.start(); 
  t2.time_delay_const = 200;
  t2.start();  
  // delay(5000); 
  cmd_1 = false;
}
// **************************************************************************************************************
void loop() {
  uint32_t t[10];

  my_WiFi.start();
  my_FTP.start();

  t[0] = micros();
  my_PCF8575.start();
  t[1] = micros();
  
  // my_config.config.DOs.DO[2].Cmd = trig(0); 

  if(t1.is_done()){
    for (int i = 0; i < 1; i++){Serial.println(t[i+1] - t[i]);}
    for (int i = 0; i < my_PCF8575.col_board; i++){
      Serial.print("board [");
      Serial.print(i);
      Serial.print("] online = ");
      Serial.println(my_PCF8575.is_online[i]);
    }
    // if (cmd_1){
    //   cmd_1 = false;
    // }else{
    //   cmd_1 = true;
    // }
    // my_config.config.DOs.DO[0].Cmd = cmd_1;
    // my_config.config.DOs.DO[1].Cmd = !cmd_1;

    // Serial.print("DI [0] = ");
    // Serial.println(my_config.config.DIs.DI[0].Val);
    // Serial.print("DI [1] = ");
    // Serial.println(my_config.config.DIs.DI[1].Val);
    // Serial.print("RS_Trig = ");
    // Serial.println(my_config.config.DOs.DO[2].Cmd);
  }

}
