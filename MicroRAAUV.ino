
#include <ESP8266WiFi.h>  //essa biblioteca já vem com a IDE. Portanto, não é preciso baixar nenhuma biblioteca adicional
#include "DHT.h"

//defines
#define SSID_REDE "Garibaldi"            //coloque aqui o nome da rede que se deseja conectar
#define SENHA_REDE "170595je"              //coloque aqui a senha da rede que se deseja conectar
#define INTERVALO_ENVIO_THINGSPEAK  3000  //intervalo entre envios de dados ao ThingSpeak (em ms)
#define DHTPIN D2                           // what digital pin we're connected to
#define DHTTYPE DHT22                       // DHT 22  (AM2302), AM2321
 
//constantes e variáveis globais
const int AnalogIn  = A0;
int valor_sensor = 0;
String UV_index = "0";

const char EnderecoAPIThingSpeak[] = "api.thingspeak.com";
const char* server = "api.thingspeak.com";
String ChaveEscritaThingSpeak = "26TO989HAZBG1DR4";
long lastConnectionTime; 
WiFiClient client;
 
//prototypes
void EnviaInformacoesThingspeak(String StringDados);
void FazConexaoWiFi(void);

//Função: envia informações ao ThingSpeak
//Parâmetros: String com a  informação a ser enviada
//Retorno: nenhum
void EnviaInformacoesThingspeak(String StringDados)
{
    if (client.connect(EnderecoAPIThingSpeak, 80))
    {         
        //faz a requisição HTTP ao ThingSpeak
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+ChaveEscritaThingSpeak+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(StringDados.length());
        client.print("\n\n");
        client.print(StringDados);
   
        lastConnectionTime = millis();
        Serial.println("- Informações enviadas ao ThingSpeak!");
     }   
}
//Função: faz a conexão WiFI
//Parâmetros: nenhum
//Retorno: nenhum
void FazConexaoWiFi(void)
{
    client.stop();
    Serial.println("Conectando-se a rede WiFi...");
    Serial.println();  
    delay(100);
    WiFi.begin(SSID_REDE, SENHA_REDE);
 
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connectado com sucesso!");  
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}

  void Calcula_nivel_UV(){
  //Calcula tensao em milivolts
     
  int tensao = (valor_sensor * (5.0 / 1023.0)) * 1000;
  
  //Compara com valores tabela UV_Index
  if (tensao > 0 && tensao < 50)
  {
    UV_index = "0";
  }
  else if (tensao > 50 && tensao <= 227)
  {
    UV_index = "0";
  }
  else if (tensao > 227 && tensao <= 318)
  {
    UV_index = "1";
  }
  else if (tensao > 318 && tensao <= 408)
  {
    UV_index = "2";
  }
  else if (tensao > 408 && tensao <= 503)
  {
    UV_index = "3";
  }
  else if (tensao > 503 && tensao <= 606)
  {
    UV_index = "4";
  }
  else if (tensao > 606 && tensao <= 696)
  {
    UV_index = "5";
  }
  else if (tensao > 696 && tensao <= 795)
  {
    UV_index = "6";
  }
  else if (tensao > 795 && tensao <= 881)
  {
    UV_index = "7";
  }
  else if (tensao > 881 && tensao <= 976)
  {
    UV_index = "8";
  }
  else if (tensao > 976 && tensao <= 1079)
  {
    UV_index = "9";
  }
  else if (tensao > 1079 && tensao <= 1170)
  {
    UV_index = "10";
  }
  else if (tensao > 1170)
  {
    UV_index = "11";
  }
 }

DHT dht(DHTPIN, DHTTYPE);

void setup()
 {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output 
  digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on (Note that LOW is the voltage level
  delay(1000);
  Serial.begin(9600);
  delay(100);
  dht.begin();
  lastConnectionTime = 0; 
  FazConexaoWiFi();
  }

void loop() {
  delay(3000);
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  valor_sensor = analogRead(AnalogIn);
  Calcula_nivel_UV();
  //Força desconexão ao ThingSpeak (se ainda estiver desconectado)
    if (client.connected())
    {
        client.stop();
        Serial.println("- Desconectado do ThingSpeak");
        Serial.println();
    }
//Inicia um client TCP para o envio dos dados
  if (client.connect(server,80)) {
    String postStr = ChaveEscritaThingSpeak;
           postStr +="&amp;field1=";
           postStr += String(UV_index);
           postStr +="&amp;field2=";
           postStr += String(t);
           postStr +="&amp;field3=";
           postStr += String(h);
           postStr += "\r\n\r\n";
     digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
     client.print("POST /update HTTP/1.1\n");
     digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on (Note that LOW is the voltage level
     client.print("Host: api.thingspeak.com\n");
     digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
     client.print("Connection: close\n");
     digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on (Note that LOW is the voltage level
     client.print("X-THINGSPEAKAPIKEY: "+ChaveEscritaThingSpeak+"\n");
     digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);
     digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on (Note that LOW is the voltage level
  }
  client.stop();}


  


