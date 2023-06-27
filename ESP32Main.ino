#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <Wire.h>

const char* serverAddress = "149.56.132.14";
const int serverPort = 3000;

const char* ssid = "SistIoTGranja";
const char* password = "123456789";

String central_id = "1";

String temperature_1 = "10";
String humidity_1 = "10";
String luminosity_1 = "10";
String temperature_2 = "10";
String humidity_2 = "10";
String luminosity_2 = "10";

String setup_temperature_1 = "0";
String setup_luminosity_1 = "0";
String setup_temperature_2 = "0";
String setup_luminosity_2 = "0";

HardwareSerial SerialGSM(1);
TinyGsm modemGSM(SerialGSM);
TinyGsmClient gsmClient(modemGSM);
AsyncWebServer server(80);
HttpClient http(gsmClient, serverAddress, serverPort);

void setupSIM800L(HardwareSerial &SerialGSM, TinyGsm &modemGSM);
String setData(TinyGsm modemGSM, HttpClient http, String data);

void updateVariables(String data) {
    DynamicJsonDocument newData(1024);
    deserializeJson(newData, data);

    setup_temperature_1 = newData["temperature_1"].as<String>();
    setup_luminosity_1 = newData["luminosity_1"].as<String>();
    setup_temperature_2 = newData["temperature_2"].as<String>();
    setup_luminosity_2 = newData["luminosity_2"].as<String>();
}

extern void setup_display();
extern void escrita_oled_wifi();

void setup() {
    Serial.begin(115200);
    delay(1000);
    Wire.begin();

    delay(1000);
    setup_display();
    delay(1000);

    setupSIM800L(SerialGSM, modemGSM);

    Serial.print("Setting AP (Access Point)…");

    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    escrita_oled_wifi();

    server.on("/read_1_temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
            AsyncWebParameter* p = request->getParam(i);
            temperature_1 = p->value();
            Serial.print("Temperature node 1: ");
            Serial.println(temperature_1);
        }
        request->send_P(200, "text/plain", "");
    });
    server.on("/read_1_humidity", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
            AsyncWebParameter* p = request->getParam(i);
            humidity_1 = p->value();
            Serial.print("Humidity node 1: ");
            Serial.println(humidity_1);
        }
        request->send_P(200, "text/plain", "");
    });
    server.on("/read_1_luminosity", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
            AsyncWebParameter* p = request->getParam(i);
            luminosity_1 = p->value();
            Serial.print("Luminosity node 1: ");
            Serial.println(luminosity_1);
        }
        request->send_P(200, "text/plain", "");
    });
        server.on("/read_2_temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
            AsyncWebParameter* p = request->getParam(i);
            temperature_2 = p->value();
            Serial.print("Temperature node 2: ");
            Serial.println(temperature_2);
        }
        request->send_P(200, "text/plain", "");
    });
    server.on("/read_2_humidity", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
            AsyncWebParameter* p = request->getParam(i);
            humidity_2 = p->value();
            Serial.print("Humidity node 2: ");
            Serial.println(humidity_2);
        }
        request->send_P(200, "text/plain", "");
    });
    server.on("/read_2_luminosity", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
            AsyncWebParameter* p = request->getParam(i);
            luminosity_2 = p->value();
            Serial.print("Luminosity node 2: ");
            Serial.println(luminosity_2);
        }
        request->send_P(200, "text/plain", "");
    });

    server.on("/send_1_temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", temperature_1.c_str());
    });
    server.on("/send_1_humidity", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", humidity_1.c_str());
    });
    server.on("/send_1_luminosity", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", luminosity_1.c_str());
    });
    server.on("/send_2_temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", temperature_2.c_str());
    });
    server.on("/send_2_humidity", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", humidity_2.c_str());
    });
    server.on("/send_2_luminosity", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", luminosity_2.c_str());
    });

    server.on("/send_1_temperature_setup", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", setup_temperature_1.c_str());
    });
    server.on("/send_2_temperature_setup", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", setup_temperature_2.c_str());
    });
    server.on("/send_1_luminosity_setup", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", setup_luminosity_1.c_str());
    });
    server.on("/send_2_luminosity_setup", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", setup_luminosity_2.c_str());
    });
    server.begin();
}

void loop() {
    String data = "{'id':"+central_id+",'temperature_1':"+temperature_1+",'luminosity_1':"+luminosity_1+",'humidity_1':"+humidity_1+",'temperature_2':"+temperature_2+",'luminosity_2':"+luminosity_2+",'humidity_2':"+humidity_2+"}";
    updateVariables(setData(modemGSM, http, data));
    escrita_oled_wifi();
    delay(30000);
}