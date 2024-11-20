#include <ESP8266WiFi.h>
#include <espMqttClient.h>
#include <ArduinoJson.h>

#define WIFI_SSID "Quang_UET"
#define WIFI_PASSWORD "UET19052003"

#define MQTT_HOST IPAddress(192, 168, 0, 102)
#define MQTT_PORT 1883

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
espMqttClient mqttClient;
bool reconnectMqtt = false;
uint32_t lastReconnect = 0;

void connectToWiFi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  if (!mqttClient.connect()) {
    reconnectMqtt = true;
    lastReconnect = millis();
    Serial.println("Connecting failed.");
  } else {
    reconnectMqtt = false;
  }
}

void onWiFiConnect(const WiFiEventStationModeGotIP& event) {
  (void) event;
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

void onWiFiDisconnect(const WiFiEventStationModeDisconnected& event) {
  (void) event;
  Serial.println("Disconnected from Wi-Fi.");
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(espMqttClientTypes::DisconnectReason reason) {
  Serial.printf("Disconnected from MQTT: %u.\n", static_cast<uint8_t>(reason));

  if (WiFi.isConnected()) {
    reconnectMqtt = true;
    lastReconnect = millis();
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(true);
  wifiConnectHandler = WiFi.onStationModeGotIP(onWiFiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWiFiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWiFi();
}

void loop() {
  JsonDocument doc;
  doc["id"] = "HNN32Z";
  doc["fill_level"] = 40;
  doc["location"][0] = 105.7825;
  doc["location"][1] = 21.0284;

  char output[256];
  serializeJson(doc, output);

  static uint32_t currentMillis = millis();

  mqttClient.loop();
  if (reconnectMqtt && currentMillis - lastReconnect > 5000) {
    connectToMqtt();
  }

  static unsigned long lastSendTime = 0;
  if (millis() - lastSendTime > 5000) {
    if (mqttClient.connected()) {
      mqttClient.publish("iot-messages", 0, true, output);
      Serial.println("Message sent");
    } else {
      Serial.println("MQTT not connected, attempting to reconnect...");
      connectToMqtt();
    }
    lastSendTime = millis();
  }
}
