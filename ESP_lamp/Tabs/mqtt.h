#include <PubSubClient.h>

// Обновленные сообщения
const char* connected_message = "MQTT client connected successfully.";
const char* failed_message = "Failed to connect to MQTT broker.";

PubSubClient mqtt_client(wifiClient);

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Received a message on topic: ");
  Serial.println(topic);
  Serial.println("Message content: ");
  char cmd = (char) payload[0];
  Serial.println(cmd);
  if (cmd == '0') {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED is turned ON.");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED is turned OFF.");
  }
}

bool init_MQTT() {
  mqtt_client.setServer(mqtt_broker, mqtt_port); 
  mqtt_client.setCallback(callback);
  while (!mqtt_client.connected()) {
    String client_id = "esp8266_138" + id();
    if (mqtt_client.connect(client_id.c_str())) {
      Serial.println(connected_message + " ID: " + client_id);
    } else {
      Serial.println(failed_message + " ID: " + client_id);
      delay(500);
    }
  }
  return true;
}
