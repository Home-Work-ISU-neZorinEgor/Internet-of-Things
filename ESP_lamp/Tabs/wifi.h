#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;
WiFiClient wifiClient;

String ip = "IP not set";

String id() {
  int mac_len = WL_MAC_ADDR_LENGTH;
  uint8_t mac[mac_len];
  WiFi.softAPmacAddress(mac);
  String mac_id = String(mac[mac_len - 2], HEX) + String(mac[mac_len - 1], HEX);
  return mac_id;
}

bool start_AP_MODE() {
  String ssid_id = AP_NAME + "_" + id();
  IPAddress ap_IP(192, 168, 4, 1);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ap_IP, ap_IP, IPAddress(255, 255, 255, 0));
  WiFi.softAP((AP_NAME + id()).c_str(), AP_PASSWORD.c_str());
  Serial.print("WiFi started in AP mode " + ssid_id);
  return true;
}

bool start_client_mode(String login, String password) {
  wifiMulti.addAP(login.c_str(), password.c_str());
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(10);
  }
  return true;
}

bool init_WIFI(bool ap_mode, String login, String password) {
  if (ap_mode) {
    start_AP_MODE();
    ip = WiFi.softAPIP().toString();
  } else {
    start_client_mode(login, password);
    ip = WiFi.localIP().toString();
  }
  Serial.println("IP address: " + ip);
  return true; 
}