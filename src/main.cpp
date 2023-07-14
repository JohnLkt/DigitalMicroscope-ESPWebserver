#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

// Digital Microscope WiFi Network Credentials
const char* ssid = "Digimic";
const char* password = "digimicIEEE";

AsyncWebServer server(80);

// Global States
int LEDBrightness = 0;

// absolute values
int XPos = 0;
int YPos = 0;
int ZPos = 0;

// new values
int newXPos = 0;
int newYPos = 0;
int newZPos = 0;

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
  int start = millis();
  Serial.println("Running");
    if (request->url() == "/led") {
      DynamicJsonDocument doc(1024);
      Serial.print("1");
      Serial.println(millis() - start);
      deserializeJson(doc, data);
      Serial.print("2");
      Serial.println(millis() - start);
      LEDBrightness = doc["brightness"];
      Serial.print("3");
      Serial.println(millis() - start);
      analogWrite(BUILTIN_LED, LEDBrightness);
      Serial.print("4");
      Serial.println(millis() - start);
      request->send(200, "text/plain", "end");
      Serial.print("end");
      Serial.println(millis() - start);
    }
  });

  server.onRequestBody([](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
  int start = millis();
  Serial.println("Running");
    if (request->url() == "/pos") {
      DynamicJsonDocument doc(1024);
      Serial.print("1");
      Serial.println(millis() - start);
      deserializeJson(doc, data);
      Serial.print("2");
      Serial.println(millis() - start);
      // get new values
      newXPos = doc["xpos"];
      newYPos = doc["ypos"];
      newZPos = doc["zpos"];

      int moveX;
      int moveY;
      int moveZ;
      
      Serial.print("3");
      Serial.println(millis() - start);
      // calculate servo difference
      if (XPos < newXPos) {
        moveX = newXPos - XPos;
      } else if (XPos > newXPos) {
        moveX = XPos - newXPos;
      } else {
        Serial.println("X axis unchanged / data error");
      }

      if (YPos < newYPos) {
        moveY = newYPos - YPos;
      } else if (YPos > newYPos) {
        moveY = YPos - newYPos;
      } else {
        Serial.println("Y axis unchanged / data error");
      }

      if (ZPos < newZPos) {
        moveZ = newZPos - ZPos;
      } else if (ZPos > newZPos) {
        moveZ = ZPos - newZPos;
      } else {
        Serial.println("Z axis unchanged / data error");
      }

      // send difference to servo
      Serial.println(moveX);
      Serial.println(moveY);
      Serial.println(moveZ);

      Serial.print("4");
      Serial.println(millis() - start);
      request->send(200, "text/plain", "end");
      Serial.print("end");
      Serial.println(millis() - start);
    } 
  });

  // define ports
  pinMode(BUILTIN_LED, OUTPUT);
  
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
