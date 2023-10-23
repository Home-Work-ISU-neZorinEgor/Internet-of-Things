#include "HardwareSerial.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;
WiFiClient wifiClient;
String ip = "IP not set";

String id(){
  int mac_len = WL_MAC_ADDR_LENGTH;
  uint8_t mac[mac_len];
  WiFi.softAPmacAddress(mac);
  String mac_id = String(mac[mac_len-2], HEX) + 
                  String(mac[mac_len-1], HEX);
  return mac_id;
}

bool start_AP_mode(){
  String ssis_id = AP_NAME + "_" + id();
  IPAddress ap_IP(192, 168, 9, 9);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ap_IP, ap_IP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssis_id.c_str(), AP_PASSWORD.c_str());
  Serial.println("WiFi started in AP mode" + AP_NAME + id());
  return true;
}

bool start_client_mode(){
  wifiMulti.addAP(CLI_SSID, CLI_PASS); // Change addAp to addAP
  while (wifiMulti.run() != WL_CONNECTED) { // Change WiFiMulti.run to WiFiMulti.run()
    delay(10);
  }
  return true; // Add a return statement
}

bool init_WIFI(bool AP_mode){ // Remove the stray backslash
  if (AP_mode){
    start_AP_mode();
    ip = WiFi.softAPIP().toString();
  } else {
    start_client_mode();
    ip = WiFi.localIP().toString();
  }
  Serial.print("IP address: " + ip);
  return true;
}
