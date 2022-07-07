#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GPS.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>

#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

uint32_t timer = millis();

String longitude = "";
String latitude = "";

const char* ssid = "testRes";
const char* password = "test1234";

AsyncWebServer server(80); // option de création de l'AsyncServer écoute port 80


String test();
String Sommeil();

//fonction qui send l'erreur 404 lorsque l'url est introuvable 
void onRequest(AsyncWebServerRequest *request){
    //Handle Unknown Request
    request->send(404);
}


void setup() {

    Serial.begin(115200);
    delay(1000);
    Serial.println("\n");

    Serial.println("création du point d'accès");
    WiFi.softAP(ssid,password); //crée le point d'accès 

    Serial.println("adresse IP");
    Serial.println(WiFi.softAPIP());

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

    // différents points d'entrés du serveur (API)
    server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request){
        request -> send_P(200, "text/plain", test().c_str());
    });
    server.on("/sommeil", HTTP_GET, [](AsyncWebServerRequest *request){
         request ->send_P(200, "text/plain", Sommeil().c_str());
    });
    server.on("/gps", HTTP_GET, [](AsyncWebServerRequest *request){
        if(latitude == "" or longitude == ""){
            request ->send(200, "text/plain", "Aucun Gps détecté");
        }else{
            request ->send_P(200, "text/plain", (latitude.c_str(),longitude.c_str()));
        }

    });

    // permet de renvoyer une erreur 404 lorsque le serveur reçois une url inconnue, appelle la fonction onRequest
    server.onNotFound(onRequest); 

    ArduinoOTA.begin(); //OTA Start
    server.begin(); // AsyncServer start


    Serial.println("Serveur actif");

}

//fonction de test du point d'entrée /test voir ligne 86
String test() {
    return "je suis un WebServer YOUHOUUUUU";
}

//fonction de sommeil de l'esp -> DeepSleep
String Sommeil(){
    esp_deep_sleep_start();
    return "sleep";
}


void loop() {
   ArduinoOTA.handle();
    //server.handleClient();
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
        if (c) Serial.print(c);
    // if a sentence is received, we can check the checksum, parse it...
    if (GPS.newNMEAreceived()) {
        // a tricky thing here is if we print the NMEA sentence, or data
        // we end up not listening and catching other sentences!
        // so be very wary if using OUTPUT_ALLDATA and trying to print out data
        Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
        if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
            return; // we can fail to parse a sentence in which case we should just wait for another
    }

    // approximately every 2 seconds or so, print out the current stats
    if (millis() - timer > 2000) {
        timer = millis(); // reset the timer
        Serial.print("\nTime: ");
        if (GPS.hour < 10) { Serial.print('0'); }
        Serial.print(GPS.hour, DEC); Serial.print(':');
        if (GPS.minute < 10) { Serial.print('0'); }
        Serial.print(GPS.minute, DEC); Serial.print(':');
        if (GPS.seconds < 10) { Serial.print('0'); }
        Serial.print(GPS.seconds, DEC); Serial.print('.');
        if (GPS.milliseconds < 10) {
            Serial.print("00");
        } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
            Serial.print("0");
        }
        Serial.println(GPS.milliseconds);
        Serial.print("Date: ");
        Serial.print(GPS.day, DEC); Serial.print('/');
        Serial.print(GPS.month, DEC); Serial.print("/20");
        Serial.println(GPS.year, DEC);
        Serial.print("Fix: "); Serial.print((int)GPS.fix);
        Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
        Serial.print("test OTA1");
        if (GPS.fix) {
            Serial.print("Location: ");
            Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
            latitude = String(GPS.lat);
            Serial.print(", ");
            Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
            longitude = String(GPS.lon);
            Serial.print("Speed (knots): "); Serial.println(GPS.speed);
            Serial.print("Angle: "); Serial.println(GPS.angle);
            Serial.print("Altitude: "); Serial.println(GPS.altitude);
            Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
        }
    }
}