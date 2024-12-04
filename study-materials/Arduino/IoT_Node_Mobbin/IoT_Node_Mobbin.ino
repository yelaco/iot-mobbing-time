#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Update these with values suitable for your network.

const char* ssid = "Nguyen-APIP";//John Cena
const char* password = "bruhh523"; //hiengiangmybinro
const char* mqtt_server = "172.20.10.4";

#define LED D2
#define trig D6
#define echo D5

#define MSG_BUFFER_SIZE (100)

//values sensed from Prox
long duration;
int distance;

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<200> doc;

unsigned long lastMsg = 0;

char msg[MSG_BUFFER_SIZE];

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");

  //Control LED
  char payloadString[length + 1]; 
  memcpy(payloadString, payload, length);
  payloadString[length] = '\0';

  Serial.println(payloadString); // Print the full payload

  // Control LED based on "ON" or "OFF"
  if (strcmp(payloadString, "ON") == 0) {
    digitalWrite(LED, HIGH); // Turn LED ON
  } else if (strcmp(payloadString, "OFF") == 0) {
    digitalWrite(LED, LOW); // Turn LED OFF
  } else {
    Serial.println("Invalid command received");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-TESTN";
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("iot_mobbin/senseProximity", "hello IoT Gateway...");
      // ... and resubscribe
      client.subscribe("iot_mobbin/controlLED");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial) continue;
  setup_wifi();
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(LED, OUTPUT);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    distance = calculateDistance();
    char payload[256];
    dtostrf(distance, 5, 4, payload);

    /*
  JsonDocument doc;
  doc["id"] = "HNN32Z";
  doc["fill_level"] = 40;
  doc["location"][0] = 105.7825;
  doc["location"][1] = 21.0284;

  char output[256];
  serializeJson(doc, output);
  */
    doc["id"] = "HNN32Z";
    doc["distance(cm)"] = payload;
    doc["location"][0] = 105.7825;
    doc["location"][1] = 21.0284;

    serializeJson(doc, payload);
    snprintf(msg, MSG_BUFFER_SIZE, "%s", payload);
    Serial.print("Publish message: ");
    Serial.println(msg);
    delay(1000);
    client.publish("iot_mobbin/senseProximity", msg);
  }
}

int calculateDistance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig, HIGH);
  delay(1000);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);   // Reads the echoPin, returns the sound wave travel time in microseconds
  distance = duration * 0.034 / 2;  //
  return distance;
}