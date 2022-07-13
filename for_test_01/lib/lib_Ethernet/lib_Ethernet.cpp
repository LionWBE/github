//version 0.12 date 13/07/2022
#include "lib_Ethernet.h"
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::setup(MyClass_Config *my_config){
  settings = my_config;
  enable = settings->config.Ethernet.enable;
  if(enable){
    for(byte i = 0; i < 6; i++){
      mac[i] = settings->config.Ethernet.mac[i];
    }
    for(byte i = 0; i < 4; i++){
      ip[i] = settings->config.Ethernet.ip[i];
    }
    // first_start = true;
    // timer_reconect.time_delay_const = 5*1000;
    // timer_reconect.start();
    // start();  
    timer_link.time_delay_const = 1000;
    timer_link.start();  
    if(settings->config.Ethernet.dhcp){
      EthernetW5500_GetIpByDHCP();
    }else{
      EthernetW5500_SetIp();
    }
    udp.begin(localPort);
    MSG_LIST = new String[10];
    get_MSG_LIST(MSG_LIST);  
  }
  Serial.println("MyClass_Ethernet setup done");
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::start(){
  if(enable){
    receivePacket(); 
    if (timer_link.is_done()){
      link_status();
    }  
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
    }else if(msg == MSG_LIST[3]){
      Serial.println("получена команда передать свою конфигурацию");
      SendFile("/settings.json", remote_ip);
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
  // формирование готовых команд, в дальнейшем их будуем сравнивать с полученными
  msg_list[0] = "ESP_CMD_GET_STATUS";
  msg_list[1] = "ESP_" + WiFi.macAddress() + "_CMD_LED_ON";
  msg_list[2] = "ESP_" + WiFi.macAddress() + "_CMD_LED_OFF";
  msg_list[3] = "ESP_" + WiFi.macAddress() + "_CMD_GET_CONFIG";
  // Serial.println(msg_list[0]);
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::get_status_info(String *mas){
  String a;
  byte n = StrWithSeparator_GetNumSeparator(settings->config.MQTT.Home_topic, '/');
  a = "NAME = " + StrWithSeparator_GetNStr(settings->config.MQTT.Home_topic, '/', n - 1);
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
byte MyClass_Ethernet::link_status(){
  //  Запрос статуса соединения
  auto link = Ethernet.linkStatus();
  //  Вывод текста в монитор последовательного порта
  // Serial.print("Link status: ");
  //  Проверяем значение переменной link
  byte status;
  switch (link) {
    //  Если "Статус: неизвестно", то
    case Unknown:
      //  выводим текст "Unknown" в монитор последовательного порта
      // Serial.println("Unknown");
      status = 0;
      break;
    //  Если "Статус: подключение есть", то
    case LinkON:
      //  выводим текст "ON" в монитор последовательного порта
      // Serial.println("ON");
      status = 1;
      break;
    //  Если "Статус: подключения нет", то
    case LinkOFF:
      //  выводим текст "OFF" в монитор последовательного порта
      // Serial.println("OFF");
      status = 2;
      break;
  }  
  return status;
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::EthernetW5500_GetIpByDHCP(){
  Serial.println("Initialize Ethernet with DHCP:");
  //  Если соединение с динамической адресацией не было установлено, то
  Ethernet.init(D3);  // на стандартном пине D8 не работает нормально, но работает на D3 - проверено

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
  } else {//  Если соединение с динамической адресацией было установлено, то
    //  Выводим в монитор порта соответствующее сообщение об этом и выводим назначенный устройству IP-адрес
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }  
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::EthernetW5500_SetIp(){
  Serial.println("Initialize Ethernet without DHCP:");
  //  Если соединение с динамической адресацией не было установлено, то
  Ethernet.init(D3);  // на стандартном пине D8 не работает нормально, но работает на D3 - проверено
  
  Ethernet.begin(mac, ip);
  delay(100);
  if ( Ethernet.linkStatus() != LinkON) {
    //  Выводим сообщение об этом в монитор последовательного порта и
    Serial.println("Failed to configure Ethernet using IP");
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
  }else {  //  Если соединение со статической адресацией было установлено, то
    //  Выводим в монитор порта соответствующее сообщение об этом и выводим назначенный устройству IP-адрес
    Serial.print("  assigned IP ");
    Serial.println(Ethernet.localIP());
  }  
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::SendFile(String file, IPAddress IP_to){
  SPIFFS.begin();
  File myFile;
  String str;
  uint16_t num_byte = 0;  // общее число байт в файле
  byte num_packet = 0;    // число пакетов по 100 байт
  byte num_send_packet = -1; // число отправленных пакетов по 100 байт
  byte packet[100];       // будем упаковывать посылку в пакет по 100 байт и посылать
  byte num_byte_in_curr_packet = 0;  // число байт в текущем пакете - для отслеживания переполнения
  byte read_byte;         // считанный байт с файла
  byte crc = 0;           // CRC8 для файла
  uint16_t file_size;     // размер файла в байтах

  myFile = SPIFFS.open(file, "r");
  file_size = myFile.size();
  str = "FILE_NAME = " + file + " FILE_SIZE = " + String(file_size);  // подготовка строки с заголовком
  Send_UDP_Packet(IP_to, str);    // отправка строки с заголовком
  while(myFile.available()){ 
    read_byte = myFile.read();
    num_byte++;
    packet[num_byte_in_curr_packet] = read_byte;    
    num_byte_in_curr_packet++;
    if (num_byte_in_curr_packet == 100){
      num_byte_in_curr_packet = 0;
      num_packet++;
      str = " DATA "+ String(byte(num_send_packet + 1)) + " = ";
      Send_UDP_Packet(IP_to, str + byte_mas_to_str(packet, 100));
      num_send_packet++;
      crc = Compute_CRC8_for_file(packet, 100, crc);
    }
  }
  if(num_send_packet!=num_packet){
    str = " DATA "+ String(num_packet) + " = ";
    Send_UDP_Packet(IP_to, str + byte_mas_to_str(packet, num_byte_in_curr_packet));
    crc = Compute_CRC8_for_file(packet, num_byte_in_curr_packet, crc);
  }
  str = "CRC8 = " + String(crc);  // подготовка строки с CRC8
  Send_UDP_Packet(IP_to, str);    // отправка строки с CRC8

  myFile.close();
  SPIFFS.end();
}