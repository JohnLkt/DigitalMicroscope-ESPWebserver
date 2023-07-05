#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

// Digital Microscope WiFi Network Credentials
const char* ssid = "Digital Microscope";
const char* password = "digimicIEEE";

AsyncWebServer server(80);

// Global States
int LEDBrightness = 0;
int Xpos = 0;
int Ypos = 0;
int Zpos = 0;

// code
void setup() {
  Serial.begin(115200);

  Serial.println("Setting up AP");
  WiFi.softAP(ssid, password);
  Serial.println("WiFi setup done");

  if(!MDNS.begin("esp")) {
     Serial.println("Error starting mDNS");
     return;
  }
  
  Serial.println(WiFi.localIP());
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello World");
  });

  server.onRequestBody([](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
  Serial.println("Running");
    if (request->url() == "/led") {
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, data);
      
      int brightness = doc["brightness"];
      analogWrite(BUILTIN_LED, brightness);

      request->send(200, "text/plain", "end");
    }
  });

  // define ports
  pinMode(BUILTIN_LED, OUTPUT);
  
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
