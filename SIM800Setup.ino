#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>

#define MODEM_RST 5
#define MODEM_PWRKEY 4
#define MODEM_POWER_ON 23
#define MODEM_TX 27
#define MODEM_RX 26

extern void escrita_oled_inicio();
extern void escrita_oled_inicio_falha();
extern void escrita_oled_rede();
extern void escrita_oled_rede_falha();
extern void escrita_oled_conexao();
extern void escrita_oled_conexao_falha();
extern void escrita_oled_sucesso();

extern void setupSIM800L(HardwareSerial &SerialGSM, TinyGsm &modemGSM) {
    Serial.println("Setup SIM800L...");

    pinMode(MODEM_RST, OUTPUT);
    pinMode(MODEM_PWRKEY, OUTPUT);
    pinMode(MODEM_POWER_ON, OUTPUT);
    digitalWrite(MODEM_RST, HIGH);
    digitalWrite(MODEM_POWER_ON, HIGH);
    digitalWrite(MODEM_PWRKEY, HIGH);
    delay(100);
    digitalWrite(MODEM_PWRKEY, LOW);
    delay(1000);
    digitalWrite(MODEM_PWRKEY, HIGH);
    SerialGSM.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
    escrita_oled_inicio();
    delay(3000);

    Serial.println(modemGSM.getModemInfo());

    if (!modemGSM.restart()) {
        Serial.print("Restarting GSM Modem failed");
        escrita_oled_inicio_falha();
        delay(10000);
        ESP.restart();
        return;
    }
    escrita_oled_rede();
    Serial.print("Aguardando Rede...");
    if (!modemGSM.waitForNetwork()) {
        Serial.println("Fail");
        escrita_oled_rede_falha();
        delay(10000);
        return;
    }

    Serial.println("\nSucesso");
    escrita_oled_conexao();
    Serial.print("Conectando GPRS...");
    if (!modemGSM.gprsConnect("claro.com.br", "claro", "claro")) {
        Serial.println("Fail");
        escrita_oled_conexao_falha();
        delay(10000);
        return;
    }

    Serial.println("\nSucesso");
    Serial.println("Setup SIM800L");
    escrita_oled_sucesso();
    delay(2000);
}