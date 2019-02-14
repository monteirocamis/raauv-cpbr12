// Definindo as bibliotecas necessarias
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Configurações de acesso a rede e ao Blynk
char chaveEscrita[] = "blynkKey";
char ssid [] = "SSID da sua rede";
char pass [] = "Senha da sua rede";

// Configurações dod sensor DHT
#define DHTPIN D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

void enviaDHT () {
// Lê os dados enviados do sensor DHT
  float t = dht.readTemperature(); 
  float u =  dht.readHumidity();

// Verifica se existe dados passando, caso não encontre nada ele retorna um erro.

  if (isnan(t) || isnan(u)) {
    Serial.println("Erro ao ler dados do sensor!");
  } else {

// Imprime os resultados na porta serial

    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.print("\t");
    Serial.print("URA: ");
    Serial.print(u);
    Serial.print("\n");

// Envia esses dados para as portas virtuais da plataforma

  Blynk.virtualWrite(V1, t); 
  Blynk.virtualWrite(V2, u);
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(chaveEscrita, ssid, pass);
  dht begin();
  timer.setInterval(5000L, enviaDHT);
}

void loop() {
  blynk.run();
  timer.run();
}
