#include <TinyGsmClient.h>
#include <ArduinoJson.h>

String setupAPI(TinyGsm modemGSM, HttpClient http, String data) {
    if (modemGSM.isGprsConnected()) {
        String endpoint = "/setData";
        String requestBody = data;

        http.beginRequest();
        http.post(endpoint);
        http.sendHeader("Content-Type", "application/json");
        http.sendHeader("Content-Length", String(requestBody.length()));
        http.beginBody();
        http.print(requestBody);
        http.endRequest();

        int statusCode = http.responseStatusCode();
        String response = http.responseBody();

        return response;
    } else {
        Serial.println("Falha na conexão GPRS");
        return "";
    }
}

extern String setData(TinyGsm modemGSM, HttpClient http, String data) {
    String response;
    response = setupAPI(modemGSM, http, data);
    return response;
}