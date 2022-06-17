// master тест с определение качества посылки _2 (99.2%, при перезагрузке слейва - температура 0)
// **************************************************************************************************************
#include <Arduino.h>
#include <wire.h>
#include "lib_timer.h"
#include <lib_Mini.h>
// **************************************************************************************************************
uint32_t count_good;
uint32_t count_all;
byte temp_count;
float temp_v[10];
bool temp_q[10];
byte temp_addr[10][8];
bool temp_is_ready;

MyClass_timer t1;
MyClass_timer t2;
// **************************************************************************************************************
void ReadTempFromSlave(byte addr);
void ReadAddrFromSlave(byte addr);
bool ReadDataFromSlave(byte addr, byte *data, byte count_byte);
float get_float_from_2byte(byte a1, byte a2);
// **************************************************************************************************************
void setup() {
  Serial.begin(115200); /* begin serial comm. */
  Wire.begin(); /* join i2c bus as master */
  // Wire.setClock(400000);
  Serial.println();
  Serial.println("I am I2C Master");
  count_good = 0;
  count_all = 0;
  t1.time_delay_const = 1000;
  t1.start(); 
  t2.time_delay_const = 200;
  t2.start();   
}
// **************************************************************************************************************
void loop() {
  if (t2.is_done()){
    count_all++;

    ReadTempFromSlave(8);
  }
  if (t1.is_done()){
    float p = (float)((float)count_good/(float)count_all);
    p = p*100;
    Serial.print(" count_all = ");
    Serial.println(count_all);
    Serial.print(" count_good = ");
    Serial.println(count_good);
    Serial.print(" p = ");
    Serial.println(p);
    for(byte i = 0; i < temp_count; i++){
      Serial.print("temp_v [");
      Serial.println(i);
      Serial.print("] = ");
      Serial.println(temp_v[i]);
      Serial.print("temp_q [");
      Serial.println(i);
      Serial.print("] = ");
      Serial.println(temp_q[i]);
    }
  }
}
// **************************************************************************************************************
void ReadAddrFromSlave(byte addr){
  byte count_byte;
  byte data[100];
  bool ReadGood;
  
  Wire.beginTransmission(8); 
  Wire.write("CMD_GET_ADDR");
  Wire.endTransmission();   
  ReadGood = ReadDataFromSlave(addr, data, count_byte);

  if (ReadGood){
    float a;
    byte start;
    byte count = data[0];
    // byte count = 2;
    for (byte i = 0; i < count; i++){
      start = i*2 + 1;
      for (byte j = 0; j < 8; j++){
        start = i*2 + 0;        
        temp_addr[i][j] = data[start + j];
      }
    }
  }
}
// **************************************************************************************************************
void ReadTempFromSlave(byte addr){
  byte count_byte;
  byte data[100];
  bool ReadGood;
  
  Wire.beginTransmission(8); 
  Wire.write("CMD_GET_TEMP");
  Wire.endTransmission();   
  ReadGood = ReadDataFromSlave(addr, data, count_byte);

  if (ReadGood){
    float a;
    byte start;
    byte count = data[0];
    // byte count = 2;
    for (byte i = 0; i < count; i++){
      start = i*2 + 1;
      // start = i*2 + 0;
      a = get_float_from_2byte(data[start], data[start+1]);
      temp_v[i] = a;
      temp_q[i] = data[start+2];
    }
  }
}
// **************************************************************************************************************
bool ReadDataFromSlave(byte addr, byte *data, byte count_byte){
  byte count_retry = 0;
  byte max_count_retry = 10;
  // byte count_byte;
  // byte data[100];
  byte CRC_read, CRC_calc;
  bool ReadGood = false;

  while(count_retry <= max_count_retry and ReadGood == false){
    Wire.requestFrom(addr, 100);
	  // delay(1);
    if(Wire.available()){
      count_byte = Wire.read();
      byte count = 0;
      // Serial.print(count_byte);
      while(Wire.available() > 1 and count < count_byte){
        byte c = Wire.read();
        data[count] = c;
        count++;
        // Serial.print((char)c);
      }

      CRC_read = Wire.read();
      CRC_calc = Compute_CRC8(data, count);
      // Serial.print((char)CRC_read);
      // Serial.print("      ");
      // проверка данных на целостность
      ReadGood = true;
      if (CRC_read != CRC_calc) ReadGood = false;
      if (count != count_byte) ReadGood = false;
      // Serial.print(ReadGood);
      // Serial.println();
    }
    count_retry++;
  }
  Serial.print(ReadGood);
  Serial.print("      ");
  Serial.print(count_retry);
  Serial.println();

  if (ReadGood){
    count_good++;
  }
  return ReadGood;
}
// **************************************************************************************************************
float get_float_from_2byte(byte a1, byte a2){
  float a, b1, b2;
  b1 = (float)a1;
  b2 = ((float)a2/100);
  a = b1 + b2;
  return a;
}