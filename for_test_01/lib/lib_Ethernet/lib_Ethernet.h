//version 0.10 date 11/07/2022
#ifndef lib_Ethernet_h
#define lib_Ethernet_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "SPI.h"
#include "Ethernet.h"
#include <EthernetUdp.h>      
#include <ESP8266WiFi.h>
#include "lib_timer.h"
#include "lib_Config.h"

class MyClass_Ethernet{
  public:
    void setup(MyClass_Config*);
    void start();
  private:
    MyClass_Config *settings;
    const uint16_t localPort = 8888; // Local port to listen for UDP packets
    EthernetUDP udp;
    EthernetClient client;
    // IPAddress ip(192, 168, 0, 177);
    // IPAddress myDns(192, 168, 0, 1);
    //  Создаём объект client класса EthernetClient
    // Задаём MAC-адрес устройству. Главное, чтобы в сети не было уже зарегистрированно устройство с таким адресом
    byte mac[6]; // = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    char packetBuffer[50];
    MyClass_timer timer_link;
    String *MSG_LIST;    

    void EthernetW5500_Get_IP();
    void link_status();
    void receivePacket();
    void Send_UDP_Packet(IPAddress IP_to, String answer);
    void get_status_info(String *mas);
    String Get_Ether_MAC();
    void get_MSG_LIST(String *msg_list);
    void clear_buff(char *buf, byte col);    
};
#endif
