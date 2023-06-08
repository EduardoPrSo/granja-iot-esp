#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"

const char* serverAddress = "149.56.132.14";
const int serverPort = 3000;

const char* ssid = "SistIoTGranja";
const char* password = "123456789";

String temperature_1;
String humidity_1;
String luminosity_1;
String temperature_2;
String humidity_2;
String luminosity_2;

String setup_temperature_1;
String setup_luminosity_1;
String setup_temperature_2;
String setup_luminosity_2;

HardwareSerial SerialGSM(1);
TinyGsm modemGSM(SerialGSM);
TinyGsmClient gsmClient(modemGSM);
AsyncWebServer server(80);
HttpClient http(gsmClient, serverAddress, serverPort);

DynamicJsonDocument dataJson(1024);
#define CENTRAL_ID  1

void setupSIM800L(HardwareSerial &SerialGSM, TinyGsm &modemGSM);
String setData(TinyGsm modemGSM, HttpClient http, DynamicJsonDocument data);

void updateVariables(String data) {
    DynamicJsonDocument newData(1024);
    deserializeJson(newData, data);

    setup_temperature_1 = newData["temperature_1"].as<String>();
    setup_luminosity_1 = newData["luminosity_1"].as<String>();
    setup_temperature_2 = newData["temperature_2"].as<String>();
    setup_luminosity_2 = newData["luminosity_2"].as<String>();
}

void setup() {
    Serial.begin(115200);

    dataJson["temperature_1"] = 10;
    dataJson["humidity_1"] = 10;
    dataJson["luminosity_1"] = 10;
    dataJson["temperature_2"] = 10;
    dataJson["humidity_2"] = 10;
    dataJson["luminosity_2"] = 10; 

    dataJson["id"] = CENTRAL_ID;
    
    setupSIM800L(SerialGSM, modemGSM);

    Serial.print("Setting AP (Access Point)…");

    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.on("/read_1_temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
            AsyncWebParameter* p = request->getParam(i);
            dataJson["temperature_1"] = p->value();
            Serial.print("Temperature node 1: ");
            temperature_1 = dataJson["temperature_1"].as<String>();
            Serial.println(temperature_1);
        }
        request->send_P(200, "text/plain", "");
    });
    server.on("/read_1_humidity", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
            AsyncWebParameter* p = request->getParam(i);
            dataJson["humidity_1"] = p->value();
            Serial.print("Humidity node 1: ");
            humidity_1 = dataJson["humidity_1"].as<String>();
            Serial.println(humidity_1);
        }
        request->send_P(200, "text/plain", "");
    });
    server.on("/read_1_luminosity", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
            AsyncWebParameter* p = request->getParam(i);
            dataJson["luminosity_1"] = p->value();
            Serial.print("Luminosity node 1: ");
            luminosity_1 = dataJson["luminosity_1"].as<String>();
            Serial.println(luminosity_1);
        }
        request->send_P(200, "text/plain", "");
    });
        server.on("/read_2_temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
            AsyncWebParameter* p = request->getParam(i);
            dataJson["temperature_2"] = p->value();
            Serial.print("Temperature node 2: ");
            temperature_2 = dataJson["temperature_2"].as<String>();
            Serial.println(temperature_2);
        }
        request->send_P(200, "text/plain", "");
    });
    server.on("/read_2_humidity", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
            AsyncWebParameter* p = request->getParam(i);
            dataJson["humidity_2"] = p->value();
            Serial.print("Humidity node 2: ");
            humidity_2 = dataJson["humidity_2"].as<String>();
            Serial.println(humidity_2);
        }
        request->send_P(200, "text/plain", "");
    });
    server.on("/read_2_luminosity", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
            AsyncWebParameter* p = request->getParam(i);
            dataJson["luminosity_2"] = p->value();
            Serial.print("Luminosity node 2: ");
            luminosity_2 = dataJson["luminosity_2"].as<String>();
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
    updateVariables(setData(modemGSM, http, dataJson));
    delay(30000);
}