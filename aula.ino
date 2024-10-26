#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTTYPE DHT11 //dht11 ou dht22
#define dht_dpin 0 //D3

DHT dht (dht_dpin, DHTTYPE);

int LEDred = 5; //D1
int LEDgreen = 4; //D2

const char* ssid= "FIBRA-A066";
const char* password= "0Z48058288";

ESP8266WebServer server(80);

float humidity, temp; //valores que o sensor DHT11 vai informar
String value = ""; //quando o sensor fizer a leitura, ele vai colocar o valor requisitado do DHT11 dentro dessa string vazia

unsigned long previousMillis = 0; //vai gravar a ultima leitura do sensor
const long interval = 2000; //intervalo 

void setup() {

  Serial.begin(115200);

  dht.begin();

  pinMode(LEDred, OUTPUT);
  digitalWrite(LEDred, LOW);

  pinMode(LEDgreen, OUTPUT);
  digitalWrite(LEDgreen, LOW);
  
  WiFi.begin(ssid, password);

  while(WiFi.status()!= WL_CONNECTED){
    delay(500);
    Serial.print(".");
    }
    Serial.println("");
    Serial.print("NodeMCU conectado no IP: ");
    Serial.println (WiFi.localIP());
    Serial.print("Conexão efetuada com sucesso!!!");

    server.begin();
    Serial.println("Webserver inicializado");
    delay(500);
    Serial.println("Acesse o endereço pelo: ");
    Serial.println (WiFi.localIP());

    server.on("/", [](){
      server.send(200, "cabeçalho da requisição", "BEM VINDO AO SERVIDOR DO NODEmcu" );
    });

    server.on("/onred", [] (){
      server.send(200, "cabeçalho", "Led Vermelha acesa");
      digitalWrite(LEDred, HIGH);
      delay(1000);      
      });

    server.on("/offred", [] (){
      server.send(200, "cabeçalho", "Led Vermelha apagada");
      digitalWrite(LEDred, LOW);
      delay(1000);      
      });

    server.on("/ongreen", [] (){
      server.send(200, "cabeçalho", "Led verde acesa");
      digitalWrite(LEDgreen, HIGH);
      delay(1000);      
      });

    server.on("/offgreen", [] (){
      server.send(200, "cabeçalho", "Led verde apagada");
      digitalWrite(LEDgreen, LOW);
      delay(1000);      
      });

    server.on("/dht11/temp", [](){
      getdatas();
      value = String((int) temp);
      server.send(200, "cabeçalho", value);
      });

     server.on("/dht11/humi", [](){
      getdatas();
      value = String((int) humidity);
      server.send(200, "cabeçalho", value);
      });


  
    }

    void loop() {
      server.handleClient(); 
    }

    void getdatas(){

      unsigned long currentMillis = millis();

      if(currentMillis - previousMillis >= interval){
        previousMillis = currentMillis;
      
        temp = dht.readTemperature();
        humidity = dht.readHumidity();

         if(isnan (temp) || (humidity)){

        Serial.println("Falha na leitura do sensor!!!");

        return;                
        }
       }
      }
