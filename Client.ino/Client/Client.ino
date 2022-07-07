#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>


const char* ssid = "testRes";
const char* password = "test1234";

const char* serverNameTest = "http://192.168.4.1/gps"; //point d'entré pour le gps -> http://192.168.4.1/sommeil ou http://192.168.4.1/test voir code api pour point d'entré

unsigned long previousMillis = 0;
const long interval = 5000;

String httpGETRequest(const char *test);

void setup() {

    Serial.begin(115200);

    Serial.println("Booting");
    WiFiClass::mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while(WiFi.status()  != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    // Port defaults to 3232
    ArduinoOTA.setPort(3232); // set le port 3232 même s"il reste sur le 8266 ?
    // Hostname defaults to esp3232-[MAC]
    ArduinoOTA.setHostname("esp3232");
    // No authentication by default
    // ArduinoOTA.setPassword("admin");
    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
    ArduinoOTA
            .onStart([]() {
                String type;
                if (ArduinoOTA.getCommand() == U_FLASH)
                    type = "sketch";
                else // U_SPIFFS
                    type = "filesystem";
                // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                Serial.println("Start updating " + type);
            })
            .onEnd([]() {
                Serial.println("\nEnd");
            })
            .onProgress([](unsigned int progress, unsigned int total) {
                Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
            })
            .onError([](ota_error_t error) {
                Serial.printf("Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
                else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
                else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
                else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
                else if (error == OTA_END_ERROR) Serial.println("End Failed");
            });
    
    ArduinoOTA.begin(); // start ota

    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

}


void loop() {

    ArduinoOTA.handle();

    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) { // condition d'interval pour éviter les requêtes chaque ms 
        if (WiFi.status()  == WL_CONNECTED) { // verif de connexion wifi 
            String valueOfServer = httpGETRequest(serverNameTest); // appel fonction httpGETRequest avec l'url contenu dans servernameTest
            Serial.println(valueOfServer);
        }else{
            Serial.println("WiFi disconected");

        }
        previousMillis = currentMillis;
    }

}


String httpGETRequest(const char* serverName) {
    WiFiClient client;
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    // Send HTTP POST request
    int httpResponseCode = http.GET();

    String payload = "--";

    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();

    return payload;
}