#include <SocketIOclient.h>

#define BLYNK_TEMPLATE_ID "TMPL6dyRRN57L"
#define BLYNK_TEMPLATE_NAME "Project 1"
#define BLYNK_AUTH_TOKEN "nY5nfGrZwnWKWaAaXG1jOii_UVgBUWVr"
#define LED D0
#define servoMotor D3
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <Servo.h>
#include "DHT.h"

#define DHTPIN D7     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Nguyen-APIP";
char pass[] = "bruhh523";

int button;
Servo servo;

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(LED, OUTPUT);
  servo.attach(servoMotor);
  dht.begin();
}

void loop()
{
  Blynk.run();
}

BLYNK_WRITE(V1)
{
  servo.write(param.asInt());
}

BLYNK_WRITE(V0)
{
  button = param.asInt();
  if(button == 1){
    digitalWrite(LED, HIGH);
    float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
    float temperature = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    if (humidity > 76) {
      digitalWrite(LED, HIGH);
    } else {
      digitalWrite(LED, LOW);
    }
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");
    delay(2000); // Delay between readings.
    } 
  else{
    digitalWrite(LED, LOW);
  }
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);  
}

