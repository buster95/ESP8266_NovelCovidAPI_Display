#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
// This library to json management
#include <Arduino_JSON.h>

const char* ssid = "WYLCD";
const char* pwd = "corrales@2020.*.";
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;
const char* serverPath = "https://disease.sh/v2/all";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, pwd);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  // put your main code here, to run repeatedly:
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      JSONVar myObject = JSON.parse(httpGETRequest(serverPath));
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      Serial.println(myObject["cases"]);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient http;

  // Your IP address with path or Domain name with URL path 
  http.begin(*client, serverName);

  // Send HTTP POST request
  int httpCode = http.GET();

  String payload = "{}"; 

  if (httpCode == HTTP_CODE_OK) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpCode);
  }
  // Free resources
  http.end();

  return payload;
}
