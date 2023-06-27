#include <Adafruit_SSD1306.h>
#include <EEPROM.h> 
#include <Wire.h>
//#include <SSD1306Wire.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET    15 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup_display();
void escrita_oled_inicio();
void escrita_oled_inicio_falha();
void escrita_oled_rede();
void escrita_oled_rede_falha();
void escrita_oled_conexao();
void escrita_oled_conexao_falha();
void escrita_oled_sucesso();
void escrita_oled_wifi();

void setup_display(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 Falha na inicializacao"));
    Serial.println("Erro oled");
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  Serial.println("Setup Display Oled");
}
void escrita_oled_inicio(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Iniciando ");
  display.setTextSize(2);
  display.setCursor(0,25);
  display.print("GPRS. ");
  display.display();
  Serial.println("Entrou na oled inicio");
}
void escrita_oled_inicio_falha(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("FALHA ");
  display.setTextSize(2);
  display.setCursor(0,25);
  display.print("GPRS. ");
  display.display();
}
void escrita_oled_rede(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Aguardando ");
  display.setTextSize(2);
  display.setCursor(0,25);
  display.print("Rede GPRS. ");
  display.display();
}
void escrita_oled_rede_falha(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("FALHA ");
  display.setTextSize(2);
  display.setCursor(0,25);
  display.print("Rede GPRS. ");
  display.display();
}
void escrita_oled_conexao(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Aguardando ");
  display.setTextSize(2);
  display.setCursor(0,25);
  display.print("Conexao GPRS.");
  display.display();
}
void escrita_oled_conexao_falha(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("FALHA ");
  display.setTextSize(2);
  display.setCursor(0,25);
  display.print("Conexao GPRS");
  display.display();
}
void escrita_oled_sucesso(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Conexao");
  display.setTextSize(2);
  display.setCursor(0,25);
  display.print("bem");
  display.setTextSize(2);
  display.setCursor(0,50);
  display.print("Sucedida.");
  display.display();
}
void escrita_oled_wifi(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Conectado");
  display.setTextSize(2);
  display.setCursor(0,25);
  display.print("WIFI");
  display.setTextSize(1);
  display.setCursor(0,50);
  display.print("IP:192.168.4.1");
  display.display();
}