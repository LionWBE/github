//version 0.23 date 01/11/2022
// MISO - D6
// MOSI - D7
// SCS (SS) - D3, на стандартном пине D8 не работает нормально, но работает на D3 - проверено
// SCK - D5
#include "lib_Ethernet.h"
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::setup(MyClass_Config *my_config){
  settings = my_config;
  settings->data.Ethernet = (int*)this;

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
        
    packetBuffer = new char[MaxSizePacketBuffer+1];
    timeout_recieve_file.time_delay_const = 2000;
  }

  settings->status.lib_Ethernet.version_lib = "0.23";
  settings->status.lib_Ethernet.date_lib    = "01.11.2022";
  DEBUG("MyClass_Ethernet setup done");
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::start(){
  uint32_t t[2];
  t[0] = micros();    
  if(enable){
    receivePacket(); 
    if (timer_link.is_done())link_status();
    if(timeout_recieve_file.is_start){
      if(timeout_recieve_file.is_done())get_file_stop();
    }
  }
  t[1] = micros();
  settings->data.dt[10] = t[1] - t[0];      
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::receivePacket() {
  packetSize = udp.parsePacket();
  if (packetSize) {
    // DEBUG("получено байт ", packetSize);
    String msg;
    char char_ESP[5];
    char char_MAC[18];
    remote_ip = udp.remoteIP();

    clear_buff(char_ESP, 5);
    udp.read(char_ESP, 4);
    msg = String(char_ESP);
    // DEBUG("получено сообщение ", msg);

    if(msg == "ESP_"){                                    // сообщение содержит ESP_
      clear_buff(char_MAC, 18);
      udp.read(char_MAC, 17);
      msg = String(char_MAC);   
      // DEBUG("получено сообщение ", msg);

      if(msg == "CMD_GET_STATUS"){                        // команда выслать общий отчет
        DEBUG("получена команда выслать общий отчет");
        byte col = 6;
        String *a = new String[10];
        get_status_info(a);
        for (byte i = 0; i < col; i++) {
          Send_UDP_Packet(remote_ip , a[i]);
        }
      }
      if(msg.startsWith(WiFiMAC)){                        // сообщение содержит свой МАС
        clear_buff(packetBuffer, MaxSizePacketBuffer+1);
        udp.read(packetBuffer, MaxSizePacketBuffer);
        msg = "ESP_" + WiFiMAC + String(packetBuffer);

        if(msg == MSG_LIST[1]){                           // команда включить светодиод
          DEBUG("получена команда включить светодиод");
          digitalWrite(BUILTIN_LED, LOW);
        }else if(msg == MSG_LIST[2]){                     // команда выключить светодиод
          DEBUG("получена команда выключить светодиод");
          digitalWrite(BUILTIN_LED, HIGH);
        }else if(msg == MSG_LIST[3]){                     // команда передать свою конфигурацию
          DEBUG("получена команда передать свою конфигурацию");
          SendFile("/settings.json", remote_ip);
        }else if(msg.startsWith(MSG_LIST[4])){            // команда передать файл
          msg.replace(MSG_LIST[4], "");
          String file_name = "/" + msg;
          DEBUG("получена команда передать файл ", file_name);
          SendFile(file_name, remote_ip);
        }else if(msg.startsWith(MSG_LIST[5])){            // команда СТАРТ получения файла
          msg.replace(MSG_LIST[5], "");
          String file_name = "/" + msg;
          get_file_start(file_name);                      
        }else if(msg == MSG_LIST[6]){                     // команда REBOOT
          DEBUG("получена команда REBOOT");
          ESP.restart();
        }else if(msg == MSG_LIST[7]){                     // команда завершения приема файла
          get_file_end();
        }else if(msg.startsWith(MSG_LIST[8])){            // команда для сравнения CRC переданного файла
          msg.replace(MSG_LIST[8], "");
          recieve_file_crc = byte(msg.toInt());
          DEBUG("получена команда для сравнения CRC переданного файла = ", recieve_file_crc);
        }else if(msg.startsWith(MSG_LIST[9])){            // получен фрагмент файла
          DEBUG("получен фрагмент файла");
          msg.replace(MSG_LIST[9], "");
          String val = msg;
          get_file_part(val);
        }else if(msg == MSG_LIST[11]){                    // команда передать список файлов
          DEBUG("команда передать список файлов");
          String answer = GetFileList();
          DEBUG(answer);
          Send_UDP_Packet(remote_ip , answer);
        }else if(msg == MSG_LIST[12]){                    // команда включить  DEBUG
          DEBUG("команда включить DEBUG");
          IP_debug = remote_ip;
          debug_mode = true;
        }else if(msg == MSG_LIST[13]){                    // команда отключить DEBUG
          DEBUG("команда отключить DEBUG");
          debug_mode = false;
        }else if(msg == MSG_LIST[14]){                    // команда передать версии модулей
          DEBUG("команда передать версии модулей");
          String answer = settings->GetALLStatus();
          DEBUG(answer);
          Send_UDP_Packet(remote_ip , answer);
        }else if(msg.startsWith(MSG_LIST[15])){ 
          DEBUG("команда синхронизировать часы с компьютером");
          msg.replace(MSG_LIST[15], "");
          // settings.
          // MyClass_NTP *my_NTP = (MyClass_NTP*)settings->data.NTP;
          // my_NTP->SetLocalTime(msg);
        }
      }
    }
  }
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::get_file_start(String file_name){
  DEBUG("получена команда получить файл ", file_name);
  file_name.replace(".json", "_temp.json");
  file_name.replace(".bin", "_temp.bin");
  DEBUG(file_name);
  recieve_filename = file_name;
  is_file_exist_delete(recieve_filename);          // удаляем старый временный файл
  
  SPIFFS.begin();
  fileWrite = SPIFFS.open(recieve_filename, "a");  // создаем или открываем для дозаписи в конец файла
  crc_calc = 0;                                    // сбрасываем CRC сумму
  timeout_recieve_file.start();                    // запускаем таймер проверки ошибки приема файла
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::get_file_end(){
  SPIFFS.end();
  fileWrite.close();

  DEBUG("получена команда завершения приема файла ");
  DEBUG(String(recieve_file_crc));
  DEBUG(String(crc_calc));
  if (crc_calc == recieve_file_crc){
    String file_name_old, file_name_new;
    file_name_old = recieve_filename;
    recieve_filename.replace("_temp", "");
    file_name_new = recieve_filename;
    DEBUG(file_name_old);
    DEBUG(file_name_new);
    file_rename(file_name_old, file_name_new);
    DEBUG("Файл получен удачно");
    
    if(file_name_new == "/firmware.bin") UpdateFirmware();  // если была передана прошивка
  }else{
    DEBUG("Ошибка получения файла");
    is_file_exist_delete(recieve_filename);
  }
  recieve_filename = "";
  timeout_recieve_file.stop_();                 // останавливаем таймер проверки ошибки приема файла
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::get_file_stop(){
  DEBUG("инициализация окончания получения файла по ошибке таймаута");
  SPIFFS.end();
  fileWrite.close();
  is_file_exist_delete(recieve_filename);
  recieve_filename = "";
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::get_file_part(String val){
  String msg;
  byte pos = val.indexOf(" = ");
  msg = val;
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
  timeout_recieve_file.start();                    // перезапускаем таймер проверки ошибки приема файла
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
  MSG_LIST[15] = "ESP_" + WiFiMAC + "_CMD_SET_CLOCK";
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
      // DEBUG("Unknown");
      status = 0;
      break;
    //  Если "Статус: подключение есть", то
    case LinkON:
      //  выводим текст "ON" в монитор последовательного порта
      // DEBUG("ON");
      status = 1;
      break;
    //  Если "Статус: подключения нет", то
    case LinkOFF:
      //  выводим текст "OFF" в монитор последовательного порта
      // DEBUG("OFF");
      status = 2;
      break;
  }  
  return status;
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::EthernetW5500_GetIpByDHCP(){
  DEBUG("Initialize Ethernet with DHCP:");
  //  Если соединение с динамической адресацией не было установлено, то
  Ethernet.init(pinSS);  // на стандартном пине D8 не работает нормально, но работает на D3 - проверено

  if (Ethernet.begin(mac) == 0) {
    //  Выводим сообщение об этом в монитор последовательного порта и
    DEBUG("Failed to configure Ethernet using DHCP");
    //  проверяем наличие самого Shield'а
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      //  Если Shield не найден, то выводим соответствующее сообщение в монитор порта
      DEBUG("Ethernet shield was not found.  Sorry, can't run without hardware. :( ");
      //  Ничего больше не выполняется
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    //  Проверяем наличие соединения
    if (Ethernet.linkStatus() == LinkOFF) {
      //  Если соеднинение не установлено, то выводим соответствующее сообщение в монитор порта
      DEBUG("Ethernet cable is not connected.");
    }
  } else {//  Если соединение с динамической адресацией было установлено, то
    //  Выводим в монитор порта соответствующее сообщение об этом и выводим назначенный устройству IP-адрес
    DEBUG("  DHCP assigned IP ", Ethernet.localIP()) ;
  }  
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::EthernetW5500_SetIp(){
  DEBUG("Initialize Ethernet without DHCP:");
  //  Если соединение с динамической адресацией не было установлено, то
  Ethernet.init(D3);  // на стандартном пине D8 не работает нормально, но работает на D3 - проверено
  
  Ethernet.begin(mac, ip);
  delay(100);
  if ( Ethernet.linkStatus() != LinkON) {
    //  Выводим сообщение об этом в монитор последовательного порта и
    DEBUG("Failed to configure Ethernet using IP");
    //  проверяем наличие самого Shield'а
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      //  Если Shield не найден, то выводим соответствующее сообщение в монитор порта
      DEBUG("Ethernet shield was not found.  Sorry, can't run without hardware. :( ");
      //  Ничего больше не выполняется
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    //  Проверяем наличие соединения
    if (Ethernet.linkStatus() == LinkOFF) {
      //  Если соеднинение не установлено, то выводим соответствующее сообщение в монитор порта
      DEBUG("Ethernet cable is not connected.");
    }
  }else {  //  Если соединение со статической адресацией было установлено, то
    //  Выводим в монитор порта соответствующее сообщение об этом и выводим назначенный устройству IP-адрес
    DEBUG("  assigned IP ", Ethernet.localIP());
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
    DEBUG("Недостаточно места для записи прошивки");
  }else{
    SPIFFS.begin();
    File file = SPIFFS.open(file_name, "r");    
    WriteSize = Update.write(file);
    file.close();
    SPIFFS.end();
    if (WriteSize !=FileSize){
      DEBUG("Ошибка записи прошивки");
    }else{
      DEBUG("Запись прошивки прошла успешно");
    }
    Update.end(true);
    ESP.restart();
  }
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::DEBUG(String packet){
  if (debug_mode){   // если стоит режим debug_mode, то сообщение шлем в сеть
    udp.beginPacket(IP_debug, 8890);
    udp.write(packet.c_str());
    udp.endPacket();
  }
  Serial.println(packet);  // печатаем в порт Serial
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::DEBUG(byte packet){
  DEBUG(String(packet));
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::DEBUG(String packet1, byte packet2){
  String str = packet1 + String(packet2);
  DEBUG(str);
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::DEBUG(String packet1, unsigned long packet2){
  String str = packet1 + String(packet2);
  DEBUG(str);
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::DEBUG(String packet1, String packet2){
  DEBUG(packet1 + packet2);
}
//-----------------(методы класса MyClass_Ethernet)------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
void MyClass_Ethernet::DEBUG(String packet1, IPAddress packet2){
  String str = packet1 + packet2.toString();
  DEBUG(str);
}