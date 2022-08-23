//version 0.19 date 22/07/2022
// MISO - D6
// MOSI - D7
// SCS (SS) - D3, на стандартном пине D8 не работает нормально, но работает на D3 - проверено
// SCK - D5
#include "lib_Ethernet.h"
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::setup(MyClass_Config *my_config){
  settings = my_config;
  
  settings->status.lib_Ethernet.version_lib = "0.19";
  settings->status.lib_Ethernet.date_lib    = "22.07.2022";

  enable = settings->config.Ethernet.enable;
  debug_mode = false;
  if(enable){
    for(byte i = 0; i < 6; i++){
      mac[i] = settings->config.Ethernet.mac[i];
    }
    for(byte i = 0; i < 4; i++){
      ip[i] = settings->config.Ethernet.ip[i];
    }
    timer_link.time_delay_const = 1000;
    timer_link.start();  
    if(settings->config.Ethernet.dhcp){
      EthernetW5500_GetIpByDHCP();
    }else{
      EthernetW5500_SetIp();
    }
    udp.begin(localPort);
    MSG_LIST = new String[20];
    WiFiMAC = WiFi.macAddress();
    get_MSG_LIST(MSG_LIST);  
    
    packetBuffer = new char[MaxSizePacketBuffer];
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
    String msg;
    char char_ESP[4];
    char char_MAC[17];
    IPAddress remote_ip = udp.remoteIP();

    clear_buff(char_ESP, 4);
    udp.read(char_ESP, 4);
    msg = String(char_ESP);

    if(msg == "ESP_"){                                    // сообщение содержит ESP_
      clear_buff(char_MAC, 17);
      udp.read(char_MAC, 17);
      msg = String(char_MAC);   

      if(msg == "CMD_GET_STATUS"){                        // команда выслать общий отчет
        Serial.println("получена команда выслать общий отчет");
        byte col = 6;
        String *a = new String[10];
        get_status_info(a);
        for (byte i = 0; i < col; i++) {
          Send_UDP_Packet(remote_ip , a[i]);
        }
      }
      if(msg.startsWith(WiFiMAC)){                        // сообщение содержит свой МАС
        clear_buff(packetBuffer, MaxSizePacketBuffer);
        udp.read(packetBuffer, MaxSizePacketBuffer);
        msg = "ESP_" + WiFiMAC + String(packetBuffer);

        if(msg == MSG_LIST[1]){                           // команда включить светодиод
          Serial.println("получена команда включить светодиод");
          digitalWrite(BUILTIN_LED, LOW);
        }else if(msg == MSG_LIST[2]){                     // команда выключить светодиод
          Serial.println("получена команда выключить светодиод");
          digitalWrite(BUILTIN_LED, HIGH);
        }else if(msg == MSG_LIST[3]){                     // команда передать свою конфигурацию
          Serial.println("получена команда передать свою конфигурацию");
          SendFile("/settings.json", remote_ip);
        }else if(msg.startsWith(MSG_LIST[4])){            // команда передать файл
          msg.replace(MSG_LIST[4], "");
          String file_name = "/" + msg;
          Serial.print("получена команда передать файл ");
          Serial.println(file_name);
          SendFile(file_name, remote_ip);
        }else if(msg.startsWith(MSG_LIST[5])){            // команда СТАРТ получения файла
          msg.replace(MSG_LIST[5], "");
          String file_name = "/" + msg;
          Serial.print("получена команда получить файл ");
          Serial.println(file_name);
          file_name.replace(".json", "_temp.json");
          file_name.replace(".bin", "_temp.bin");
          Serial.println(file_name);
          recieve_filename = file_name;
          is_file_exist_delete(recieve_filename);             // удаляем старый временный файл
          
          SPIFFS.begin();
          fileWrite = SPIFFS.open(recieve_filename, "a");  // создаем или открываем для дозаписи в конец файла
          crc_calc = 0;
        }else if(msg == MSG_LIST[6]){                     // команда REBOOT
          Serial.println("получена команда REBOOT");
          ESP.restart();
        }else if(msg == MSG_LIST[7]){                     // команда завершения приема файла
          SPIFFS.end();
          fileWrite.close();

          Serial.println("получена команда завершения приема файла ");
          Serial.println(String(recieve_file_crc));
          Serial.println(String(crc_calc));
          if (crc_calc == recieve_file_crc){
            String file_name_old, file_name_new;
            file_name_old = recieve_filename;
            recieve_filename.replace("_temp", "");
            file_name_new = recieve_filename;
            DEBAG(file_name_old);
            DEBAG(file_name_new);
            file_rename(file_name_old, file_name_new);
            DEBAG("Файл получен удачно");
            
            if(file_name_new == "/firmware.bin")UpdateFirmware();  // если была передана прошивка
          }else{
            DEBAG("Ошибка получения файла");
            is_file_exist_delete(recieve_filename);
          }
          recieve_filename = "";

        }else if(msg.startsWith(MSG_LIST[8])){            // команда для сравнения CRC переданного файла
          msg.replace(MSG_LIST[8], "");
          recieve_file_crc = byte(msg.toInt());
          Serial.print("получена команда для сравнения CRC переданного файла = ");
          Serial.println(String(recieve_file_crc));
        }else if(msg.startsWith(MSG_LIST[9])){            // получен фрагмент файла
          Serial.println("получен фрагмент файла");
          msg.replace(MSG_LIST[9], "");
          String val = msg;
          byte pos = val.indexOf(" = ");
          val = val.substring(0, pos);
          uint16_t num = val.toInt();
          msg.replace(val + " = ", "");
          
          // "ESP_" + WiFiMAC + "_CMD_FILE_PART_" + val + " = " - высчитать длинну в байтах
          uint16_t data_start = 15 + 3 + val.length();                        //длинна ненужной части сообщения (оно же начало нужной)
          uint16_t data_stop  = packetSize - 4 - WiFiMAC.length();            //конец потока
          uint16_t data_len  = data_stop - data_start;                        //длинн данных
          char *char_file_part = &packetBuffer[data_start];                     // указатель на начало куска файла
          fileWrite.write(char_file_part, data_len);                            // запись в конец файла полученного куска
          crc_calc = Compute_CRC8_for_file((byte*)char_file_part, data_len, crc_calc); // считаем CRC для полученного куска с учетом предыдущего значения CRC
          String answer = "file part #" + String(num) + " is received";
          Send_UDP_Packet(remote_ip , answer);
        }else if(msg == MSG_LIST[11]){                    // команда передать список файлов
          Serial.println("команда передать список файлов");
          String answer = GetFileList();
          Serial.println(answer);
          Send_UDP_Packet(remote_ip , answer);
        }else if(msg == MSG_LIST[12]){                    // команда включить  DEBUG
          Serial.println("команда включить DEBUG");
          IP_debug = remote_ip;
          debug_mode = true;
        }else if(msg == MSG_LIST[13]){                    // команда отключить DEBUG
          Serial.println("команда отключить DEBUG");
          debug_mode = false;
        }else if(msg == MSG_LIST[14]){                    // команда передать версии модулей
          Serial.println("команда передать версии модулей");
          String answer = settings->GetALLStatus();
          Serial.println(answer);
          Send_UDP_Packet(remote_ip , answer);
        }
      }
    }
  }
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::clear_buff(char *buf, uint16_t col){
  for (uint16_t i = 0; i < col; i++) {
    buf[i] = 0;
  }
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::get_MSG_LIST(String *MSG_LIST){
  // формирование готовых команд, в дальнейшем их будуем сравнивать с полученными
  MSG_LIST[0] = "ESP_CMD_GET_STATUS";
  MSG_LIST[1] = "ESP_" + WiFiMAC + "_CMD_LED_ON";
  MSG_LIST[2] = "ESP_" + WiFiMAC + "_CMD_LED_OFF";
  MSG_LIST[3] = "ESP_" + WiFiMAC + "_CMD_GET_CONFIG";
  MSG_LIST[4] = "ESP_" + WiFiMAC + "_CMD_GET_FILE_";
  MSG_LIST[5] = "ESP_" + WiFiMAC + "_CMD_SET_FILE_";
  MSG_LIST[6] = "ESP_" + WiFiMAC + "_CMD_REBOOT";
  MSG_LIST[7] = "ESP_" + WiFiMAC + "_CMD_END_FILE_SET";
  MSG_LIST[8] = "ESP_" + WiFiMAC + "_CMD_CRC_FILE_SET_";
  MSG_LIST[9] = "ESP_" + WiFiMAC + "_CMD_FILE_PART_";
  MSG_LIST[10] = "ESP_" + WiFiMAC;
  MSG_LIST[11] = "ESP_" + WiFiMAC + "_CMD_GET_LIST_FILE";
  MSG_LIST[12] = "ESP_" + WiFiMAC + "_CMD_DEBUG_ON";
  MSG_LIST[13] = "ESP_" + WiFiMAC + "_CMD_DEBUG_OFF";
  MSG_LIST[14] = "ESP_" + WiFiMAC + "_CMD_GET_INFO";
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
void MyClass_Ethernet::Send_UDP_Packet(IPAddress IP_to, byte *packet, uint16_t packet_size) {
  udp.beginPacket(IP_to, 8889);
  udp.write(packet, packet_size);
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
  uint16_t num_packet = 0;      // число пакетов по MaxSizePackage байт
  byte *packet;                 // будем упаковывать посылку в пакет по MaxSizePackage байт и посылать
  packet = new byte [MaxSizePackage];
  uint32_t file_size;           // размер файла в байтах
  uint16_t num_byte = 0;        // числи отправляемых байт
  uint16_t num_send_packet = 0; // номер отправляемого пакета

  myFile = SPIFFS.open(file, "r");
  file_size = myFile.size();
  str = "FILE_NAME = " + file + " FILE_SIZE = " + String(file_size);  // подготовка строки с заголовком
  Send_UDP_Packet(IP_to, str);    // отправка строки с заголовком
  
  if(file_size % MaxSizePackage > 0){
    num_packet = file_size / MaxSizePackage + 1;
  }else{
    num_packet = file_size / MaxSizePackage;
  }
  
  while(file_size > 0){
    if (file_size >= MaxSizePackage){
      num_byte = MaxSizePackage;
    }else{
      num_byte = file_size;
    }  
    myFile.readBytes((char*)packet, num_byte);
    str = " DATA "+ String(byte(num_send_packet + 1)) + " = ";

    Send_UDP_Packet(IP_to, str + byte_mas_to_str(packet, num_byte));

    num_send_packet++;
    file_size = file_size - num_byte;
  }

  byte CRC_ = Compute_CRC8_for_full_file(file);
  str = "CRC8 = " + String(CRC_);  // подготовка строки с CRC8
  Send_UDP_Packet(IP_to, str);    // отправка строки с CRC8

  myFile.close();
  SPIFFS.end();
  delete packet;
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::UpdateFirmware(){
  String file_name = "/firmware.bin";
  size_t FileSize = GetFileSize(file_name);
  size_t WriteSize = 0;

  uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
  if (!Update.begin(maxSketchSpace, U_FLASH)){
  // if (!Update.begin(FileSize, U_FS)){
    Serial.println("Недостаточно места для записи прошивки");
  }else{
    SPIFFS.begin();
    File file = SPIFFS.open(file_name, "r");    
    WriteSize = Update.write(file);
    file.close();
    SPIFFS.end();
    if (WriteSize !=FileSize){
      Serial.println("Ошибка записи прошивки");
    }else{
      Serial.println("Запись прошивки прошла успешно");
    }
    Update.end(true);
    ESP.restart();
  }
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::DEBAG(String packet){
  if (debug_mode){   // если стоит режим debug_mode, то сообщение шлем в сеть
    udp.beginPacket(IP_debug, 8890);
    udp.write(packet.c_str());
    udp.endPacket();
  }
  Serial.println(packet);  // печатаем в порт Serial
}