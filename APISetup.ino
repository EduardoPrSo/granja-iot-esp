#include <TinyGsmClient.h>
#include <ArduinoJson.h>

const char* serverAddress = "149.56.132.14";
const int serverPort = 3000;

extern String setupAPI(TinyGsmClient gsmClient, String endpoint, String requestBody) {
    if (gsmClient.connect(serverAddress, serverPort)) {
        if (gsmClient.connect(serverAddress, serverPort)) {
            gsmClient.print(String("POST /") + endpoint + " HTTP/1.1\r\n");
            gsmClient.print(String("Host: ") + serverAddress + "\r\n");
            gsmClient.println("Connection: close");
            gsmClient.println("Content-Type: application/json");
            gsmClient.print("Content-Length: ");
            gsmClient.println(requestBody.length());
            gsmClient.println();
            gsmClient.println(requestBody);

            while (gsmClient.connected()) {
                String line = gsmClient.readStringUntil('\n');
                if (line == "\r") {
                    break;
                }
            }

            while (gsmClient.available()) {
                String response = gsmClient.readStringUntil('\n');
                return response;
            }

        } else {
            Serial.println("Falha na conexão com a API!");
            return "Erro";
        }

        gsmClient.stop();
    }
}

extern String startSetup(TinyGsmClient gsmClient, String data) {
    String response;
    String body = "{'id': '" + data + "'}";
    response = setupAPI(gsmClient, "initialSetup", body);
    return response;
}

extern String getData(TinyGsmClient gsmClient, String data) {
    String response;
    String body = "{'id': '" + data + "'}";
    response = setupAPI(gsmClient, "getData", body);
    return response;
}

extern String sendData(TinyGsmClient gsmClient, DynamicJsonDocument data) {
    String response;
    response = setupAPI(gsmClient, "sendData", jsonStrinify(data));
    return response;
}

String jsonStrinify(DynamicJsonDocument data) {
    String output;
    serializeJson(data, output);
    return output;
}