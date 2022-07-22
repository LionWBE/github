//version 0.18 date 22/07/2022
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
    void DEBAG(String packet);
    bool enable;
  private:
    MyClass_Config *settings;
    const uint16_t localPort = 8888; // Local port to listen for UDP packets
    EthernetUDP udp;
    EthernetClient client;
    IPAddress IP_debug;     // ip  адрес для отправки дебагов
    bool debug_mode;        // если правда - то включить отправку сообщений
    byte ip[4]; 
    byte mac[6]; 
    byte crc_calc;
    char *packetBuffer;
    const uint16_t MaxSizePackage = 1000;         // максимальный размер посылки в байтах 
    const uint16_t MaxSizePacketBuffer = 1100;        // максимальный размер пакета, должен быть больше MaxSizePackage
    MyClass_timer timer_link;
    String *MSG_LIST;    
    byte recieve_file_crc;     // CRC сумма полученного файла (получена) для сравнения с расчитанной
    String recieve_filename;   // имя получаемого файла
    String WiFiMAC;             
    File fileWrite;

    void EthernetW5500_GetIpByDHCP();
    void EthernetW5500_SetIp();
    byte link_status();
    void receivePacket();
    void Send_UDP_Packet(IPAddress IP_to, String answer);
    void Send_UDP_Packet(IPAddress IP_to, byte *packet, uint16_t packet_size);
    void get_status_info(String *mas);
    String Get_Ether_MAC();
    void get_MSG_LIST(String *msg_list);
    void clear_buff(char *buf, uint16_t col);    
    void SendFile(String file, IPAddress IP_to);
    void UpdateFirmware();
};
#endif
