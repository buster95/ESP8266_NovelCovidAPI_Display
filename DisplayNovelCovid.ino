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

// int lcdColumns = 16;
// int lcdRows = 2;
// LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

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
  Serial.println("This will publish each 5min");

  /*
   * Follow Instructions in Readme for connections
   */
  // set LCD address, number of columns and rows
  // if you don't know your display address, run an I2C scanner sketch
  // lcd.init();
  // lcd.backlight();
  // lcd.setCursor(0, 0); // Set cursor to row column 0, row 0
  // lcd.print("COVID19 CASES");
  // lcd.setCursor(0, 1); // Set cursor to column 0, row 1
  // lcd.print("Reading...");
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
      // lcd.print(myObject["cases"]); // print message
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
