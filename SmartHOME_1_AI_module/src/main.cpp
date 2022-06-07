#include <Arduino.h>
#include <lib_timer.h>
#include <OneWire.h>
//***************************************************************************************************
OneWire ds(D3);  // on pin 10 (a 4.7K resistor is necessary)
String str[100];
byte device_count;
MyClass_timer t_cicle;
MyClass_timer t_print_temp;
uint32_t count_in_second;
//***************************************************************************************************
void setup(void) {
  Serial.begin(115200);
  device_count = 0;

  t_cicle.time_delay_const = 1000;
  t_cicle.start();
  count_in_second = 0;
  t_print_temp.time_delay_const = 500;
  t_print_temp.start();    
}
//***************************************************************************************************
void print_addr(byte *addr){
  Serial.print(" addr = ");
  for(byte i = 0; i < 8; i++){
    if (addr[i] < 16)	Serial.print('0');
    Serial.print(addr[i], HEX);
  }
  Serial.println("");
}
//***************************************************************************************************
void temper_work(){
  // byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius;
  
  if (!ds.search(addr)) {
    ds.reset_search();
    device_count = 0;
    return;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for (byte i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  }
  celsius = (float)raw / 16.0;
  str[device_count] = "  temp = " + String(celsius);
  str[device_count] += " addr = " + String(addr[0]) +String(addr[1]) +String(addr[2]) +String(addr[3]) +String(addr[4]) +String(addr[5]) +String(addr[6]) +String(addr[7]) ;
  device_count++;
}
//***************************************************************************************************
void loop(void) {
  temper_work();
  count_in_second++;

  if (t_cicle.is_done()){
    Serial.println(count_in_second);
    Serial.print(1000000/count_in_second);
    Serial.println("us");
    count_in_second = 0;

    long fh = ESP.getFreeHeap();
    Serial.print("free memory = ");
    Serial.println(fh); 
  }
  if (t_print_temp.is_done()){
    Serial.println(str[0]);
    Serial.println(str[1]);
  }
}
