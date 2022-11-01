#include "lib_FTP.h"
//-----------------(методы класса MyClass_FTP)--------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_FTP::setup(){
#ifdef ESP32       //esp32 we send true to format spiffs if cannot mount
  if (SPIFFS.begin(true)) {
#elif defined ESP8266
  if (SPIFFS.begin()) {
#endif
      Serial.println("SPIFFS opened!");
      ftpSrv.begin("esp8266","esp8266");    //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
  }  
  Serial.println("MyClass_FTP setup done");
}
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_FTP::start(){
  ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!! 
} 