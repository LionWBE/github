// Подключаем библиотеки:
#include <Arduino.h>
#include <Wire.h>                                     // подключаем библиотеку для работы с шиной I2C
#include <iarduino_I2C_connect.h>                     // подключаем библиотеку для соединения arduino по шине I2C

// Объявляем переменные и константы:
iarduino_I2C_connect I2C2;                            // объявляем объект I2C2 для работы c библиотекой iarduino_I2C_connect
byte           REG_Array[2];                    // объявляем массив из двух элементов, данные которого будут доступны мастеру (для чтения/записи) по шине I2C

void setup(){
  Wire.begin(0x01);                                   // инициируем подключение к шине I2C в качестве ведомого (slave) устройства, с указанием своего адреса на шине.
  I2C2.begin(REG_Array);                              // инициируем возможность чтения/записи данных по шине I2C, из/в указываемый массив
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop(){
  REG_Array[0] = 1;             // Сохраняем состояние кнопки в 0 ячейку массива REG_Array
  digitalWrite(BUILTIN_LED, REG_Array[1]);                // Управляем светодиодом в соответствии со значением 1 ячейки массива REG_Array
}

