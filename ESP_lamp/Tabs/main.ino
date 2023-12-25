#include "Config.h"
#include "Wifi.h"
#include "Server.h"
#include "Mqtt.h"


void setup() {
  Serial.begin(9600);
  Serial.println();
  pinMode(LED_BUILTIN, OUTPUT);
  start_AP_mode();
  bool wifi_is_on = init_WIFI(false);
  if (wifi_is_on) {
    Serial.println(" working");
    init_MQTT();
    server_init();
  } else {
    Serial.println("not working");
  }                         
  String topic = "esp8266Egor/command";
  String state_topic = "esp8266Egor/state";
  mqtt_client.subscribe(topic.c_str());
  mqtt_client.publish(state_topic.c_str(), "hello");
  Serial.println("See me at " + state_topic);
}

void loop() {
  server.handleClient();
  mqtt_client.loop();
}