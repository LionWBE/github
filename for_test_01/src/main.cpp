#include <Arduino.h>
//************************************************************************************************************************
#include <Wire.h>                                     // подключаем библиотеку для работы с шиной I2C
#include <iarduino_I2C_connect.h>                     // подключаем библиотеку для соединения arduino по шине I2C
// Объявляем переменные и константы:
iarduino_I2C_connect I2C2;                            // объявляем переменную для работы c библиотекой iarduino_I2C_connect
void setup(){
  Serial.begin(115200);
  Serial.println();
  Wire.begin();                                       // инициируем подключение к шине I2C в качестве ведущего (master) устройства
}
void loop(){
//Считываем данные:
  byte cur_reg;
  byte reg[100];
  
  for (int i = 0; i < 30; i++) {
    cur_reg = I2C2.readByte(0x01,i);
    reg[i] = cur_reg;
    Serial.print("REG[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(reg[i]);
  }
  delay(1000);                                     
}