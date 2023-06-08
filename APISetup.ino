#include <TinyGsmClient.h>
#include <ArduinoJson.h>

String setupAPI(TinyGsm modemGSM, HttpClient http, DynamicJsonDocument data) {
    if (modemGSM.isGprsConnected()) {
        String endpoint = "/setData";
        String requestBody = jsonStringify(data);

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

String jsonStringify(DynamicJsonDocument data) {
    String output;
    serializeJson(data, output);
    return output;
}

extern String setData(TinyGsm modemGSM, HttpClient http, DynamicJsonDocument data) {
    String response;
    response = setupAPI(modemGSM, http, data);
    return response;
}