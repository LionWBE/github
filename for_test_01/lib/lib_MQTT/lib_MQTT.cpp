//version 0.17 date 01/11/2022
#include "lib_MQTT.h"
MyClass_Config *volatile global_link_to_settings;
volatile byte *global_link_to_DI;
volatile byte global_col_link_to_DI;
volatile byte global_MQTT_in_links[10];  // номера DI (для последующей обработке вне прерывания)
volatile bool global_MQTT_in_val[10];    // значения DI (для последующей обработке вне прерывания)
volatile byte global_MQTT_in_col;        // количество принятых сообщений (для последующей обработке вне прерывания)
//-----------------(методы класса MyClass_MQTT)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT::start() {
  uint32_t t[2];
  t[0] = micros();  
  if (!client.connected()) {
    reconnect();
    // client.subscribe("$SYS/broker/uptime"); // работает
    subscribe();
  }
  client.loop();

  byte col = global_MQTT_in_col;
  for(byte i = 0; i < col; i++){
    settings->config.DIs.DI[global_MQTT_in_links[i]].Val = global_MQTT_in_val[i];
    global_MQTT_in_col--;
  }
  t[1] = micros();
  settings->data.dt[3] = t[1] - t[0];      
}  
//-----------------(методы класса MyClass_MQTT)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool MQTT_GET_VAL_BOOL(byte* payload, unsigned int length) {
  bool rez;
  String s = "";
  for (int i = 0; i < length; i++) {
    s += String((char)payload[i]);
  }  
  if (s == "ON") rez = true;
  if (s == "OFF") rez = false;
  return rez;
}
//-----------------(методы класса MyClass_MQTT)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MQTT_PRINT_INPUT_MESSANGE(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
//-----------------(методы класса MyClass_MQTT)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void callback2(char* topic, byte* payload, unsigned int length) {
  // печать - работает, убрал чтобы не мусорила
  // MQTT_PRINT_INPUT_MESSANGE(topic, payload, length);
  String s1, s2;
  bool val;
  s2 = String(topic);
  for (byte i = 0; i < global_col_link_to_DI; i++){
    s1 = global_link_to_settings->config.DIs.DI[global_link_to_DI[i]].MQTT_in.topic;
    if (s1 == s2){
      val = MQTT_GET_VAL_BOOL(payload, length);
      if(global_MQTT_in_col < 10){ //проверка на переполнение очереди команд
        global_MQTT_in_val[global_MQTT_in_col] = val;
        global_MQTT_in_links[global_MQTT_in_col] = global_link_to_DI[i];
        global_MQTT_in_col++;
      }
    }
  }  
}
//-----------------(методы класса MyClass_MQTT)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT::reconnect() {
  // Loop until we're reconnected
  if(timer_reconect.is_done() and !client.connected()){
  //while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    Serial.print(" client = ");
    Serial.print(clientId);
    Serial.print(" login = ");
    Serial.print(settings->config.MQTT.Login);
    Serial.print(" passwrd = ");
    Serial.print(settings->config.MQTT.Password);    

    // Attempt to connect
    if (client.connect(clientId.c_str(), settings->config.MQTT.Login.c_str(), settings->config.MQTT.Password.c_str())) {
      Serial.println(" connected");
      is_connect = true;
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      //client.subscribe("inTopic");
      // subscribe();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      is_connect = false;
      delay(5000);
    }
  }
}
//-----------------(методы класса MyClass_MQTT)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
boolean MyClass_MQTT::publish(const char* topic, const char* payload){
  return client.publish(topic, payload);
}
//-----------------(методы класса MyClass_MQTT)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT::setup(MyClass_Config *my_config) {
  settings = my_config;
  settings->data.MQTT = (int*)this;

  // client.setClient(espClient);  // работало с WiFi
  MyClass_Ethernet *clients;
  clients = (MyClass_Ethernet*)settings->data.Ethernet;

  client.setClient(clients->client);
  client.setServer(settings->config.MQTT.Server.c_str(), (uint16_t)settings->config.MQTT.port);
  client.setCallback(callback2);
  timer_reconect.time_delay_const = 500;
  timer_reconect.start();

  setup_DI();
  global_MQTT_in_col = 0;

  settings->status.lib_MQTT.version_lib = "0.17";
  settings->status.lib_MQTT.date_lib    = "01.11.2022";  
  Serial.println("MyClass_MQTT setup done");
}
//-----------------(методы класса MyClass_MQTT)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT::setup_DI(){
  byte mas[256];
  for (byte i = 0; i < settings->config.DIs.col; i++){
    if (settings->config.DIs.DI[i].Type == 3){ // external_MQTT
      mas[col_input_messange] = i;
      col_input_messange++;
    }
  }
  link_to_DI = new byte[col_input_messange];
  for (byte i = 0; i < col_input_messange; i++){
    link_to_DI[i] = mas[i];
  }
  
  global_col_link_to_DI = col_input_messange;
  global_link_to_DI = new byte[global_col_link_to_DI];
  for (byte i = 0; i < global_col_link_to_DI; i++){
    global_link_to_DI[i] = link_to_DI[i];
  }
  global_link_to_settings = settings;
}
//-----------------(методы класса MyClass_MQTT)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT::subscribe(){
  for (byte i = 0; i < col_input_messange; i++){
    client.subscribe(settings->config.DIs.DI[link_to_DI[i]].MQTT_in.topic.c_str());
  }  
}