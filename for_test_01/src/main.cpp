// slave - прием данных по I2C
// **************************************************************************************************************
#include <Arduino.h>
#include <Wire.h>                                     
#include "lib_timer.h"
// **************************************************************************************************************
// **************************************************************************************************************
void GetData(int i);
MyClass_timer t1;
// **************************************************************************************************************
void setup(){
  Serial.begin(115200);
  Serial.println();  
  Wire.onReceive(GetData);
  Wire.begin(1);      

  pinMode(BUILTIN_LED, OUTPUT);
  t1.time_delay_const = 1000;
  t1.start();    
}
// **************************************************************************************************************
void loop(){       
  if (t1.is_done()){
    Serial.println("прошла 1 секунда");
  }     
}
// **************************************************************************************************************
void GetData(int i){
  uint8_t data;
  for(int j=1; j<i; j++){	
    data = Wire.read();
    Serial.print("byte ");
    Serial.print(j);
    Serial.print(" = ");
    Serial.println(data);    
  }
}