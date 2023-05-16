#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <ArduinoJson.h>

HardwareSerial SerialGSM(1);
TinyGsm modemGSM(SerialGSM);
TinyGsmClient gsmClient(modemGSM);

DynamicJsonDocument dataJson(1024);
#define CENTRAL_ID = "CENTRAL1"

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
}

void loop() {
    delay(15000);
    sendData(gsmClient, dataJson);
    updateVariables(getData(gsmClient, dataJson["id"]));
}


