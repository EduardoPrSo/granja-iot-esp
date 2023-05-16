#include <ArduinoJson.h>

extern DynamicJsonDocument parseJson(String payload) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    return doc;
}