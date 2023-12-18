#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void handle_root(){
String page_code = "<form action=\"/LED\" method=\"POST\">";
page_code += "<input type=\"submit\" value=\"Switch LED\" style=\"background-color: #4CAF50; color: white; padding: 10px 20px; border: none; cursor: pointer; width: 100%; height: 100vh; font-size: 220px;\">";
page_code += "</form>";

  server.send(200, "text/html", page_code);
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
  server.on("/LED", HTTP_POST, handle_led); // Исправлено на HTTP_POST
  server.onNotFound(handle_not_found);

  server.begin();
  Serial.println("server start on port 80"); // Исправлено на Serial.println
}