#include <Arduino.h>
#include <lib_timer.h>
#include <lib_mini.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>                  
#include <iarduino_I2C_connect.h>  
// ************************************************************************************************************
OneWire oneWire(D3);
DallasTemperature sensors(&oneWire);
DeviceAddress *sensorsUnique;
int countSensors;
float temperature[10];

uint32_t count_in_second;
MyClass_timer t_cicle;
MyClass_timer t_print_temp;

iarduino_I2C_connect I2C2;                            // объявляем переменную для работы c библиотекой iarduino_I2C_connect
byte REG_Array[200];                            // объявляем массив, данные которого будут доступны для чтения/записи по шине I2C
// 0    - КОЛИЧЕСТВО ДАТЧИКОВ
// 1    - количество циклов в секунду
// 1-8  - АДРЕС ПЕРВОГО ДАТЧИКА
// 9-12 - ЗНАЧЕНИЕ ПЕРВОГО ДАТЧИКА
// 13   - НАЛИЧИЕ СВЯЗИ С ДАТЧИКОМ
// 13 байт на 1 датчик
// ************************************************************************************************************
void copy_data_to_REG(byte n);
// ************************************************************************************************************
void printAddress(DeviceAddress deviceAddress){
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
// ************************************************************************************************************
void setup(){
  Serial.begin(115200);
  Serial.println();
  while(!Serial);
  
  sensors.begin();
  countSensors = sensors.getDeviceCount();
  Serial.print("Found sensors: ");
  Serial.println(countSensors);
  sensorsUnique = new DeviceAddress[countSensors];
 
  if (sensors.isParasitePowerMode()) {
    Serial.println("Mode power is Parasite");
  } else {
    Serial.println("Mode power is Normal");
  }
 
  for (int i = 0; i < countSensors; i++) {
    sensors.getAddress(sensorsUnique[i], i);
  }
  for (int i = 0; i < countSensors; i++) {
    Serial.print("Device ");
    Serial.print(i);
    Serial.print(" Address: ");
    printAddress(sensorsUnique[i]);
    Serial.println();
  }
  Serial.println();
  for (int i = 0; i < countSensors; i++) {
    sensors.setResolution(sensorsUnique[i], 12);
  }

  t_cicle.time_delay_const = 1000;
  t_cicle.start();
  count_in_second = 0;
  t_print_temp.time_delay_const = 500;
  t_print_temp.start();   

  Wire.begin(0x01);                                   // инициируем подключение к шине I2C в качестве ведомого (slave) устройства, с указанием своего адреса на шине.
  I2C2.begin(REG_Array);                              // инициируем возможность чтения/записи данных по шине I2C, из/в указываемый массив  
  REG_Array[0] = byte(countSensors);
}
// ************************************************************************************************************
void loop(){
  count_in_second++;

  sensors.requestTemperatures();
  for (int i = 0; i < countSensors; i++) {
    temperature[i] = sensors.getTempCByIndex(i);
  }

  if (t_cicle.is_done()){  
    REG_Array[1] = byte(count_in_second);
    Serial.print("count in second = ");
    Serial.println(count_in_second);
    Serial.print("cicle time = ");
    Serial.print(1000000/count_in_second);
    Serial.println("us");
    count_in_second = 0;

    long fh = ESP.getFreeHeap();
    Serial.print("free memory = ");
    Serial.println(fh);    
  }
  if (t_print_temp.is_done()){
    for (int i = 0; i < countSensors; i++) {
      Serial.print("Device ");
      Serial.print(i);
      Serial.print(" Temp C: ");
      Serial.print(temperature[i]);
      Serial.println();
      bool a;
      if (temperature[i] == -127){
        a = false;
      } else{
        a = true;
      }
      Serial.print("is connect = ");
      Serial.println(a);

      copy_data_to_REG(i);
    }

    for (int i = 0; i < 28; i++) {
      Serial.print("REG[]");
      Serial.println(REG_Array[i]);
    }
    Serial.println();
  }
}
// ************************************************************************************************************
// 2-9  - АДРЕС ПЕРВОГО ДАТЧИКА
// 10-13 - ЗНАЧЕНИЕ ПЕРВОГО ДАТЧИКА
// 14   - НАЛИЧИЕ СВЯЗИ С ДАТЧИКОМ
// 15-22  - АДРЕС ПЕРВОГО ДАТЧИКА
// 23-26 - ЗНАЧЕНИЕ ПЕРВОГО ДАТЧИКА
// 27   - НАЛИЧИЕ СВЯЗИ С ДАТЧИКОМ
void copy_data_to_REG(byte n){   // n - number or device
  byte a[4];
  byte start = n * 13;
  float_to_4_byte(temperature[n], a);
  bool c;
  if (temperature[n] == -127){
    c = false;
  } else{
    c = true;
  }
  REG_Array[start + 2] = sensorsUnique[n][0];
  REG_Array[start + 3] = sensorsUnique[n][1];
  REG_Array[start + 4] = sensorsUnique[n][2];
  REG_Array[start + 5] = sensorsUnique[n][3];
  REG_Array[start + 6] = sensorsUnique[n][4];
  REG_Array[start + 7] = sensorsUnique[n][5];
  REG_Array[start + 8] = sensorsUnique[n][6];
  REG_Array[start + 9] = sensorsUnique[n][7];
  REG_Array[start + 10] = a[0];
  REG_Array[start + 11] = a[0];
  REG_Array[start + 12] = a[0];
  REG_Array[start + 13] = a[0];
  REG_Array[start + 14] = byte(c);
}