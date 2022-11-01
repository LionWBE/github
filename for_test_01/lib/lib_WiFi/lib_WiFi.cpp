//version 0.11 date 01/11/2022
#include "lib_WiFi.h"
//-----------------(методы класса MyClass_WiFi)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_WiFi::setup(MyClass_Config *my_config){
  settings = my_config;
  first_start = true;
  timer_reconect.time_delay_const = 5*1000;
  timer_reconect.start();
  if(settings->config.WiFi.Enable)start(); 
  settings->status.WiFi.version_lib = "0.11";
  settings->status.WiFi.date_lib    = "01.11.2022";   
  Serial.println("MyClass_WiFi setup done");
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_WiFi::start(){
  uint32_t t[2];
  t[0] = micros();  

  if(settings->config.WiFi.Enable){
    if((!WiFi.isConnected() and timer_reconect.is_done()) or first_start){
      Serial.println();
      Serial.print("Connecting to ");
      Serial.println(settings->config.WiFi.SSID);
      WiFi.mode(WIFI_AP_STA);

      WiFi.begin(settings->config.WiFi.SSID.c_str(), settings->config.WiFi.Password.c_str());
      WiFi.waitForConnectResult();    
      Serial.println("WiFi failed, retrying.");
      first_start = false;
      reconect = true;
    }
    if(WiFi.isConnected() and reconect){
      randomSeed(micros());
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      reconect = false;
      settings->data.WiFi_IP = WiFi.localIP().toString();
      settings->data.WiFi_MAC = WiFi.macAddress();
    }
  }
  t[1] = micros();
  settings->data.dt[0] = t[1] - t[0];   
}
