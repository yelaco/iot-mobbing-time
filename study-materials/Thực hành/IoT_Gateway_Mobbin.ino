#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MSG_BUFFER_SIZE	(100)

const char* ssid = "Nguyen-APIP";//John Cena
const char* password = "bruhh523"; //hiengiangmybinro
const char* mqtt_server = "172.20.10.4";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];

StaticJsonDocument<200> doc;

bool isLEDOn = false;
int currDist = -1;
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

  //convert payload to null-term string
  char payloadString[length + 1];
  memcpy(payloadString, payload, length);
  payloadString[length] = '\0'; // Null-terminate the string
  Serial.println(payloadString);

  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  currDist = doc["distance(cm)"];
  Serial.print("Distance (in cm): ");
  Serial.println(currDist);
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-TESTGW";
    // clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("iot_mobbin/controlLED", "hello IoT Node...");
      // ... and resubscribe
      client.subscribe("iot_mobbin/senseProximity");
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
  setup_wifi();
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
    if (currDist != -1) {
      bool shouldTurnOn = currDist < 7;
      if (shouldTurnOn != isLEDOn) { //decide if LED state change is needed 
        snprintf(msg, MSG_BUFFER_SIZE, shouldTurnOn ? "ON" : "OFF");
        Serial.print("Publish message: ");
        Serial.println(msg);
        client.publish("iot_mobbin/controlLED", msg);
        isLEDOn = shouldTurnOn;
      }
      currDist = -1; // Reset distance
    }
  }
}