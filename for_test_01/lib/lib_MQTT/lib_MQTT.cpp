#include "lib_MQTT.h"
//-----------------(методы класса MyClass_MQTT)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT::start() {
  if (!client.connected()) {
    reconnect();
    //client.subscribe("SmartHOME/arduino_1/counter");
    //client.subscribe("SmartHOME/arduino_1/free_memory");
  }
  client.loop();
}  
//-----------------(методы класса MyClass_MQTT)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MyClass_MQTT::callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    // digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    // digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}
//-----------------(методы класса MyClass_MQTT)----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
void callback2(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    // digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    // digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
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

  client.setClient(espClient);
  client.setServer(settings->config.MQTT.Server.c_str(), (uint16_t)settings->config.MQTT.port);
  client.setCallback(callback2);
  timer_reconect.time_delay_const = 500;
  timer_reconect.start();
  Serial.println("MyClass_MQTT setup done");
}