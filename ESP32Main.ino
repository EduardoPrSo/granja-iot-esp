#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <ArduinoJson.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"

const char* ssid = "SistIoTGranja";
const char* password = "123456789";

HardwareSerial SerialGSM(1);
TinyGsm modemGSM(SerialGSM);
TinyGsmClient gsmClient(modemGSM);

DynamicJsonDocument dataJson(1024);
#define CENTRAL_ID = "CENTRAL1"

//Valores Padrão
dataJson["temperature_1"] = 0;
dataJson["humidity_1"] = 0;
dataJson["luminosity_1"] = 0;
dataJson["temperature_2"] = 0;
dataJson["humidity_2"] = 0;
dataJson["luminosity_2"] = 0;

DynamicJsonDocument parseJson(String payload);
void setupSIM800L(HardwareSerial &SerialGSM, TinyGsm &modemGSM);
String startSetup(TinyGsmClient gsmClient, String data);
String getData(TinyGsmClient gsmClient, String data);
String sendData(TinyGsmClient gsmClient, DynamicJsonDocument data);

void updateVariables(String data) {
    dataJson = parseJson(data);
}

void setup() {
    Serial.begin(115200);

    dataJson["id"] = CENTRAL_ID;
    
    setupSIM800L(SerialGSM, modemGSM);
    startSetup(gsmClient, dataJson["id"]);

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
        Serial.println(dataJson["temperature_1"]);
        }
        request->send_P(200, "text/plain", "");
    });
    server.on("/read_1_humidity", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
        AsyncWebParameter* p = request->getParam(i);
        dataJson["humidity_1"] = p->value();
        Serial.print("Humidity node 1: ");
        Serial.println(dataJson["humidity_1"]);
        }
        request->send_P(200, "text/plain", "");
    });
    server.on("/read_1_luminosity", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
        AsyncWebParameter* p = request->getParam(i);
        dataJson["luminosity_1"] = p->value();
        Serial.print("Luminosity node 1: ");
        Serial.println(dataJson["luminosity_1"]);
        }
        request->send_P(200, "text/plain", "");
    });
        server.on("/read_2_temperature", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
        AsyncWebParameter* p = request->getParam(i);
        dataJson["temperature_2"] = p->value();
        Serial.print("Temperature node 2: ");
        Serial.println(dataJson["temperature_2"]);
        }
        request->send_P(200, "text/plain", "");
    });
    server.on("/read_2_humidity", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
        AsyncWebParameter* p = request->getParam(i);
        dataJson["humidity_2"] = p->value();
        Serial.print("Humidity node 2: ");
        Serial.println(dataJson["humidity_2"]);
        }
        request->send_P(200, "text/plain", "");
    });
    server.on("/read_2_luminosity", HTTP_GET, [](AsyncWebServerRequest *request){
        int paramsNr = request->params();
        for (int i=0; i < paramsNr; i++) {
        AsyncWebParameter* p = request->getParam(i);
        dataJson["luminosity_2"] = p->value();
        Serial.print("Luminosity node 2: ");
        Serial.println(dataJson["luminosity_2"]);
        }
        request->send_P(200, "text/plain", "");
    });
    server.begin();
}

void loop() {
    delay(15000);
    sendData(gsmClient, dataJson);
    updateVariables(getData(gsmClient, dataJson["id"]));
}