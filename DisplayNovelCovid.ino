#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Arduino_JSON.h>

const char* ssid = "WYLCD";
const char* pwd = "corrales@2020.*.";
unsigned long lastTime = millis()+3E5;
unsigned long timerDelay = 3E5; // Every 5min
const char* serverPath = "https://disease.sh/v2/all";

void setup() {
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
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){ //Check WiFi connection status
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

  http.begin(*client, serverName);
  int httpCode = http.GET(); //Sending Request
  String payload = "{}"; 

  if (httpCode == HTTP_CODE_OK) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpCode);
  }
  
  http.end(); // Free resources
  return payload;
}
