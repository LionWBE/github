//version 0.14 date 18/07/2022
#ifndef lib_Ethernet_h
#define lib_Ethernet_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "SPI.h"
#include <FS.h>
#include "Ethernet.h"
#include <EthernetUdp.h>      
#include <ESP8266WiFi.h>
#include "lib_timer.h"
#include "lib_Config.h"
#include "lib_Mini.h"
// #include <lib_FTP.h>

class MyClass_Ethernet{
  public:
    void setup(MyClass_Config*);
    void start();
    bool enable;
  private:
    // MyClass_FTP my_FTP;
    MyClass_Config *settings;
    const uint16_t localPort = 8888; // Local port to listen for UDP packets
    EthernetUDP udp;
    EthernetClient client;
    byte ip[4]; 
    byte mac[6]; 
    char *packetBuffer;
    const byte MaxSizePacketBuffer = 200;
    MyClass_timer timer_link;
    String *MSG_LIST;    
    
    bool if_recieve_file;      // если правда - то выставлен режим получения файла
    bool if_recieve_file_end;  // если правда - то выставлен режим окончания получения файла
    byte recieve_file_crc;     // CRC сумма полученного файла (получена) для сравнения с расчитанной
    String recieve_filename;   // имя получаемого файла

    void EthernetW5500_GetIpByDHCP();
    void EthernetW5500_SetIp();
    byte link_status();
    void receivePacket();
    void Send_UDP_Packet(IPAddress IP_to, String answer);
    void get_status_info(String *mas);
    String Get_Ether_MAC();
    void get_MSG_LIST(String *msg_list);
    void clear_buff(char *buf, byte col);    
    void SendFile(String file, IPAddress IP_to);
    void RecieveFile(String recieve_filename,String msg, byte N);
};
#endif
