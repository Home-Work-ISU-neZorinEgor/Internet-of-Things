#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

bool signed_up = false;

void handle_root() {
  String page_code = "<form action=\"/LED\" method=\"POST\">";
  page_code += "<input type=\"text\" name=\"login\" placeholder=\"Enter login\">";
  page_code += "<input type=\"password\" name=\"password\" placeholder=\"Enter password\">";
  page_code += "<input type=\"submit\"></form>";
  server.send(200, "text/html", page_code);
}

void handle_auth() {
  String message = "Number of arguments received: ";
  message += server.args();
  Serial.println(message);
  String login;
  String password;
  bool isValidLogin = false;
  bool isValidPass = false;
  bool mode_AP = true;                         

  for (int i = 0; i < server.args(); i++) {
    message += server.argName(i) + ": ";     
    message += server.arg(i) + "\n";             
    if (i == 0) {
      login = server.arg(i);
    } else if (i == 1) {
      password = server.arg(i);
    }
  }
  server.send(200, "text/plain", "");      
  bool res = init_WIFI(false, login, password);
  if (res) {
    Serial.println("MQTT started successfully");
    init_MQTT();
    String topic = "house/bulb1";
    mqtt_client.subscribe(topic.c_str());
  }
  Serial.println(message);
}

void handle_not_found() {
  server.send(404, "text/html", "404: Check the URL");
}

void server_init() {
  server.on("/", HTTP_GET, handle_root);
  server.on("/LED", HTTP_POST, handle_auth);
  server.onNotFound(handle_not_found);

  server.begin();
  Serial.print("Web server started on port ");
  Serial.println(WEB_SERVER_PORT);
}
