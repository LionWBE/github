#include "lib_WiFi.h"
//-----------------(методы класса MyClass_WiFi)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_WiFi::setup(MyClass_Config *my_config){
  settings = my_config;
  first_start = true;
  timer_reconect.time_delay_const = 5*1000;
  timer_reconect.start();
  start();  
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_WiFi::start(){
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
