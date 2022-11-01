//version 0.12
#ifndef lib_FTP_h
#define lib_FTP_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ESP8266FtpServer.h>
#define FTP_DEBUG

class MyClass_FTP {
  public:
    FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial
    void setup();
    void start();
  private:
};
#endif
