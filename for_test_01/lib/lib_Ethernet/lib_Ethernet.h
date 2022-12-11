//version 0.22 date 28/10/2022
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
// #include "lib_NTP.h"

class MyClass_Ethernet{
  public:
    bool enable;
    void setup(MyClass_Config*);
    void start();
    void DEBUG(String packet);
    void DEBUG(byte packet);
    void DEBUG(String packet1, byte packet2);
    void DEBUG(String packet1, unsigned long packet2);
    void DEBUG(String packet1, String packet2);
    void DEBUG(String packet1, IPAddress packet2);
    void DEBUG(String packet1, uint16_t packet2);
    EthernetClient client;
  private:
    MyClass_Config *settings;
    const uint16_t localPort = 8888; // Local port to listen for UDP packets
    const uint8_t pinSS = D3;        // работало стабильно, но D3 нужен для прерывания INT
    EthernetUDP udp;
    IPAddress IP_debug;     // ip  адрес для отправки дебагов
    bool debug_mode;        // если правда - то включить отправку сообщений
    byte ip[4]; 
    byte mac[6]; 
    byte crc_calc;
    char *packetBuffer;
    const uint16_t MaxSizePackage = 1000;         // максимальный размер посылки в байтах 
    const uint16_t MaxSizePacketBuffer = 1100;    // максимальный размер пакета, должен быть больше MaxSizePackage
    MyClass_timer timer_link;
    String *MSG_LIST;    
    byte recieve_file_crc;     // CRC сумма полученного файла (получена) для сравнения с расчитанной
    String recieve_filename;   // имя получаемого файла
    String WiFiMAC;             
    File fileWrite;
    int packetSize;            // размер полученного сообщения в байтах
    IPAddress remote_ip;       // IP адрес удаленного компа, который прислал запрос
    MyClass_timer timeout_recieve_file;          // таймер проверки ошибки получения файла 2000 мс

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

    void get_file_start(String file_name);                                            // прием файла - инициализация начала получения файла
    void get_file_end();                                                              // прием файла - инициализация окончания получения файла
    void get_file_part(String val);                                                   // прием файла - получения куска файла
    void get_file_stop();                                                             // прием файла - инициализация окончания получения файла по ошибке таймаута
};
#endif