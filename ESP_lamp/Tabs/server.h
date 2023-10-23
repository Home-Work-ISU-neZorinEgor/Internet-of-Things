#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void handle_root(){
String page_code = "<form action=\"/LED\" method=\"POST\">";
page_code += "<input type=\"submit\" value=\"Switch LED\" style=\"background-color: #4CAF50; color: white; padding: 10px 20px; border: none; cursor: pointer; width: 100%; height: 100vh; font-size: 220px;\">";
page_code += "</form>";

  server.send(200, "text/html", page_code);
}

void handle_wifi_setup() {
  String wifi_setup_page = "<form action=\"/set_wifi\" method=\"POST\">";
  wifi_setup_page += "SSID: <input type=\"text\" name=\"ssid\"><br>";
  wifi_setup_page += "Password: <input type=\"password\" name=\"password\"><br>";
  wifi_setup_page += "<input type=\"submit\" value=\"Set Wi-Fi\" style=\"background-color: #4CAF50; color: white; padding: 10px 20px; border: none; cursor: pointer;\">";
  wifi_setup_page += "</form>";

  server.send(200, "text/html", wifi_setup_page);
}

void handle_set_wifi() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    String new_ssid = server.arg("ssid");
    String new_password = server.arg("password");

    // Подключитесь к новой Wi-Fi сети
    WiFi.begin(new_ssid.c_str(), new_password.c_str());

    server.send(200, "text/html", "Wi-Fi setup complete. Rebooting...");
    delay(1000);
    ESP.restart();
  } else {
    server.send(200, "text/html", "Please provide both SSID and password.");
  }
}


void handle_led(){
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  server.sendHeader("Location", "/");
  server.send(303);
}

void handle_not_found(){
  server.send(404, "text/html", "404 check URL");
}

void server_init(){
  server.on("/", HTTP_GET, handle_root);
  server.on("/LED", HTTP_POST, handle_led);
  server.on("/wifi_setup", HTTP_GET, handle_wifi_setup); // Добавлен новый обработчик
  server.on("/set_wifi", HTTP_POST, handle_set_wifi); // Добавлен новый обработчик
  server.onNotFound(handle_not_found);

  server.begin();
  Serial.println("Server started on port 80");
}
