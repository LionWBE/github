//version 0.12 date 13/07/2022
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

class MyClass_Ethernet{
  public:
    void setup(MyClass_Config*);
    void start();
    bool enable;
  private:
    MyClass_Config *settings;
    const uint16_t localPort = 8888; // Local port to listen for UDP packets
    EthernetUDP udp;
    EthernetClient client;
    byte ip[4]; 
    byte mac[6]; 
    char packetBuffer[50];
    MyClass_timer timer_link;
    String *MSG_LIST;    

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
};
#endif
