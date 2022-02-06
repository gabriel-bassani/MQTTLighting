#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#define portaLDR 34

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 
char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;
const char *SSID = "brisa-1471954";
const char *PWD = "l5kvx6ru";
void connectToWiFi() {
  Serial.print("Connecting to ");
WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected.");
  
}
void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
}
void setup() {
  Serial.begin(115200);
  delay(1000);
  /*
  while(true){
    delay(500);
    int val = analogRead(portaLDR);
    Serial.println(val);  
  }
  */
  connectToWiFi();
  setupMQTT();
}
void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("lighting");
      }
      
  }
}
void loop() {
  int val = analogRead(portaLDR);
  char str[80];
  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
  Serial.println(val);
  delay(1000);
  sprintf(str, "%d", val);
  Serial.println(str);
  mqttClient.publish("lighting", str);
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}
