//version 0.10 date 11/07/2022
#include "lib_Ethernet.h"
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::setup(MyClass_Config *my_config){
  settings = my_config;
  // mac = ;

  // first_start = true;
  // timer_reconect.time_delay_const = 5*1000;
  // timer_reconect.start();
  // start();  
  timer_link.time_delay_const = 1000;
  timer_link.start();  
  EthernetW5500_Get_IP();
  udp.begin(localPort);
  MSG_LIST = new String[10];
  get_MSG_LIST(MSG_LIST);  
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::start(){
  receivePacket(); 
  if (timer_link.is_done()){
    link_status();
  }  
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::receivePacket() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    IPAddress remote_ip = udp.remoteIP();
    clear_buff(packetBuffer, 50);
    udp.read(packetBuffer, 50);
    String msg = String(packetBuffer);
    Serial.println(msg);

    if(msg == MSG_LIST[0]){
      Serial.println("получена команда выслать общий отчет");
      byte col = 6;
      String *a = new String[10];
      get_status_info(a);
      for (byte i = 0; i < col; i++) {
        Send_UDP_Packet(remote_ip , a[i]);
      }
    }else if(msg == MSG_LIST[1]){
      Serial.println("получена команда включить светодиод");
      digitalWrite(BUILTIN_LED, LOW);
    }else if(msg == MSG_LIST[2]){
      Serial.println("получена команда выключить светодиод");
      digitalWrite(BUILTIN_LED, HIGH);
    }
  }
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::clear_buff(char *buf, byte col){
  for (byte i = 0; i < col; i++) {
    buf[i] = 0;
  }
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::get_MSG_LIST(String *msg_list){
  String str1;
  msg_list[0] = "ESP_CMD_GET_STATUS";
  msg_list[1] = "ESP_" + WiFi.macAddress() + "_CMD_LED_ON";
  msg_list[2] = "ESP_" + WiFi.macAddress() + "_CMD_LED_OFF";
  Serial.println(msg_list[0]);
  Serial.println(msg_list[1]);
  Serial.println(msg_list[2]);
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::get_status_info(String *mas){
  String a;
  a = "NAME = ESP_1";
  mas[0] = a;
  a = "IP = " + String(Ethernet.localIP().toString());
  mas[1] = a;
  a = "ETHER_MAC = " + Get_Ether_MAC();
  mas[2] = a;
  a = "WiFi_MAC = " + WiFi.macAddress();
  mas[3] = a;
  a = "ESP_ChipId = " + String(ESP.getChipId());
  mas[4] = a;
  a = "ESP_FlashChipId = " + String(ESP.getFlashChipId());
  mas[5] = a;
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
String MyClass_Ethernet::Get_Ether_MAC(){
  String rez = "";
  String b;
  byte macBuffer[6];
  Ethernet.MACAddress(macBuffer);
  for (byte octet = 0; octet < 6; octet++) {
    b = String(macBuffer[octet], 16);
    b.toUpperCase();
    rez += b;
    if (octet<5){
      rez += ":";
    }
  }
  return rez;
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::Send_UDP_Packet(IPAddress IP_to, String packet) {
  udp.beginPacket(IP_to, 8889);
  udp.write(packet.c_str());
  udp.endPacket();
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::link_status(){
  //  Запрос статуса соединения
  auto link = Ethernet.linkStatus();
  //  Вывод текста в монитор последовательного порта
  Serial.print("Link status: ");
  //  Проверяем значение переменной link
  switch (link) {
    //  Если "Статус: неизвестно", то
    case Unknown:
      //  выводим текст "Unknown" в монитор последовательного порта
      Serial.println("Unknown");
      break;
    //  Если "Статус: подключение есть", то
    case LinkON:
      //  выводим текст "ON" в монитор последовательного порта
      Serial.println("ON");
      break;
    //  Если "Статус: подключения нет", то
    case LinkOFF:
      //  выводим текст "OFF" в монитор последовательного порта
      Serial.println("OFF");
      break;
  }  
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::EthernetW5500_Get_IP(){
  Serial.println("Initialize Ethernet with DHCP:");
  //  Если соединение с динамической адресацией не было установлено, то
  Ethernet.init(D3);  // на стандартном пине D8 не работает нормально

  if (Ethernet.begin(mac) == 0) {
    //  Выводим сообщение об этом в монитор последовательного порта и
    Serial.println("Failed to configure Ethernet using DHCP");
    //  проверяем наличие самого Shield'а
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      //  Если Shield не найден, то выводим соответствующее сообщение в монитор порта
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :( ");
      //  Ничего больше не выполняется
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    //  Проверяем наличие соединения
    if (Ethernet.linkStatus() == LinkOFF) {
      //  Если соеднинение не установлено, то выводим соответствующее сообщение в монитор порта
      Serial.println("Ethernet cable is not connected.");
    }
    // Пробуем установить соединение, используя статический IP-адрес
    // Ethernet.begin(mac, ip, myDns);
  }
  //  Если соединение с динамической адресацией было установлено, то
  else {
    //  Выводим в монитор порта соответствующее сообщение об этом и выводим назначенный устройству IP-адрес
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }  
}