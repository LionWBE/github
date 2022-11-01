//version 0.10 date 01/11/2022
#include "lib_SysInfo.h"
//-----------------(методы класса MyClass_SysInfo)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_SysInfo::setup(MyClass_Config *my_config) {
  settings = my_config;
  Tags = (MyClass_MQTT_Tag*)settings->data.Tags;
  
  timer_publish_status.time_delay_const = 1000;
  timer_publish_status.start();
  counter = 0;
  time_since_start.start();

  // setut own tags
  String diag, adr, val;
  diag = settings->config.MQTT.Home_topic + "diagnostic/";
  adr = diag + "counter"; Tags->CreateNewTag(&adr);            // 0
  adr = diag + "free_memory"; Tags->CreateNewTag(&adr);        // 1
  adr = diag + "time_since_start"; Tags->CreateNewTag(&adr);   // 2
  adr = diag + "time_cicle_us"; Tags->CreateNewTag(&adr);      // 3
  adr = diag + "WiFi_IP"; Tags->CreateNewTag(&adr);            // 4
  adr = diag + "WiFi_MAC"; Tags->CreateNewTag(&adr);           // 5
  adr = diag + "FIRMWARE_ver"; Tags->CreateNewTag(&adr);       // 6
  adr = diag + "local_time"; Tags->CreateNewTag(&adr);         // 7
  adr = diag + "local_date"; Tags->CreateNewTag(&adr);         // 8
  adr = diag + "time_cicle_ms"; Tags->CreateNewTag(&adr);      // 9
  adr = diag + "cicle_per_sec"; Tags->CreateNewTag(&adr);      // 10
  adr = diag + "Tags_queue_count"; Tags->CreateNewTag(&adr);   // 11
  adr = diag + "Tags_count"; Tags->CreateNewTag(&adr);         // 12
  adr = diag + "time_cicle_us/my_WiFi"; Tags->CreateNewTag(&adr);        // 13 - 0 +
  adr = diag + "time_cicle_us/empty"; Tags->CreateNewTag(&adr);          // 14 - 1 del
  adr = diag + "time_cicle_us/empty"; Tags->CreateNewTag(&adr);          // 15 - 2 del
  adr = diag + "time_cicle_us/my_MQTT"; Tags->CreateNewTag(&adr);        // 16 - 3 +
  adr = diag + "time_cicle_us/my_NTP"; Tags->CreateNewTag(&adr);         // 17 - 4 +
  adr = diag + "time_cicle_us/my_PCF8575"; Tags->CreateNewTag(&adr);     // 18 - 5 +
  adr = diag + "time_cicle_us/my_temper"; Tags->CreateNewTag(&adr);      // 19 - 6 -
  adr = diag + "time_cicle_us/my_SysInfo"; Tags->CreateNewTag(&adr);     // 20 - 7 +
  adr = diag + "time_cicle_us/my_Tags"; Tags->CreateNewTag(&adr);        // 21 - 8 +
  adr = diag + "time_cicle_us/my_DIs"; Tags->CreateNewTag(&adr);         // 22 - 9 +
  adr = diag + "time_cicle_us/my_Ethernet"; Tags->CreateNewTag(&adr);    // 23 - 10 +

  val = "0.05 (01/11/2022)";
  cicle_in_sec = 0;
  Tags->SetTagVal(6, &val);

  settings->status.SysInfo.version_lib = "0.10";
  settings->status.SysInfo.date_lib    = "01.11.2022";   
  Serial.println("MyClass_SysInfo setup done");
}
//-----------------(методы класса MyClass_SysInfo)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_SysInfo::start() {
  uint32_t t[2];
  t[0] = micros();

  time_cur = micros();
  time_cicle = time_cur - time_prev;
  time_prev = time_cur;
  cicle_in_sec++;

  if(timer_publish_status.is_done()){
    String val;
    long fh = ESP.getFreeHeap();
    val = String(counter);
    Tags->SetTagVal(0, &val);
    val = String(fh) + " byte";
    Tags->SetTagVal(1, &val);
    val = String(time_since_start.get_time());
    Tags->SetTagVal(2, &val);
    val = String(time_cicle) + " us";
    Tags->SetTagVal(3, &val);
    val = String(time_cicle/1000) + " ms";
    Tags->SetTagVal(9, &val);
    val = String(cicle_in_sec);
    Tags->SetTagVal(10, &val);
    Tags->SetTagVal(4, &settings->data.WiFi_IP);
    Tags->SetTagVal(5, &settings->data.WiFi_MAC);
    Tags->SetTagVal(7, &settings->data.time);
    Tags->SetTagVal(8, &settings->data.date);
    Tags->SetTagVal(11, settings->data.Tags_queue_count);
    Tags->SetTagVal(12, settings->data.Tags_count);
    for (int i = 13; i < 24; i++){Tags->SetTagVal(i, settings->data.dt[i-13]);}
    counter++;
    cicle_in_sec = 0;
  }
  t[1] = micros();
  settings->data.dt[7] = t[1] - t[0];  
}  