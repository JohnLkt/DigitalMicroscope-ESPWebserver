#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AccelStepper.h>

// Digital Microscope WiFi Network Credentials
const char* ssid = "Digimic";
const char* password = "digimicIEEE";

// PINS

const int XDIR_PIN = 12;
const int XSTEP_PIN = 13;

const int YDIR_PIN = 27;
const int YSTEP_PIN = 14;

const int ZDIR_PIN = 25;
const int ZSTEP_PIN = 26;

AsyncWebServer server(80);

#define motorInterfaceType 1
AccelStepper XAxis(1, XSTEP_PIN, XDIR_PIN);
AccelStepper YAxis(1, YSTEP_PIN, YDIR_PIN);
AccelStepper ZAxis(1, ZSTEP_PIN, ZDIR_PIN);

// Global States
int LEDBrightness = 0;

// new values
int newXPos = 0;
int newYPos = 0;
int newZPos = 0;

// code
void setup() {
  Serial.begin(115200);

  // Motor Setup  
  XAxis.setMaxSpeed(3000);
  XAxis.setAcceleration(100);
  XAxis.setSpeed(200);

  YAxis.setMaxSpeed(3000);
  YAxis.setAcceleration(100);
  YAxis.setSpeed(200);

  ZAxis.setMaxSpeed(3000);
  ZAxis.setAcceleration(100);
  ZAxis.setSpeed(200);

  // define hardware pins
  pinMode(BUILTIN_LED, OUTPUT);

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
    if (request->url() == "/pos") {
      DynamicJsonDocument doc(1024);

      // Serial.println(millis() - start);
      deserializeJson(doc, data);
      // Serial.println(millis() - start);
      // get new values
      newXPos = doc["xpos"];
      newYPos = doc["ypos"];
      newZPos = doc["zpos"];

      LEDBrightness = doc["brightness"];
      

      Serial.printf("Moving X Axis to step: %d", newXPos);
      XAxis.moveTo(newXPos);

      Serial.printf("Moving Y Axis to step: %d", newYPos);
      YAxis.moveTo(newYPos);

      Serial.printf("Moving Z Axis to step: %d", newZPos);
      ZAxis.moveTo(newZPos);

      // Serial.println(millis() - start);
      request->send(200, "text/plain", "End Pos");
      Serial.println("End Pos");
      Serial.println(millis() - start);
    } 
  });

  server.begin();
}

void loop() {

  analogWrite(BUILTIN_LED, LEDBrightness);

  if (XAxis.distanceToGo() != 0) {
    XAxis.run();
  }
  if (YAxis.distanceToGo() != 0) {
    YAxis.run();
  }
  if (ZAxis.distanceToGo() != 0) {
    ZAxis.run();
  }
  
}
