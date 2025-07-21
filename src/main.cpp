/*
   F1ATB Solar Router – PlatformIO monolith
   Auto-merged from Kimi-K2 branch, ESP32 only
*/
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncMqttClient.h>
#include <ModbusMaster.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

/* ---------- Global variables ---------- */
const char *WIFI_SSID = "YOUR_SSID";
const char *WIFI_PASS = "YOUR_PASS";
AsyncWebServer server(80);
AsyncMqttClient mqtt;
ModbusMaster node;
float surplusPuissance = 0.0;
uint8_t relayStates[8] = {0};
unsigned long lastModbus = 0;
unsigned long lastMqttRetry = 0;
unsigned long lastTelemetry = 0;

/* ---------- DS18B20 ---------- */
OneWire oneWire(4);
DallasTemperature sensors(&oneWire);
float tempC = 0.0;

/* ---------- NTP ---------- */
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000);

/* ---------- EEPROM ---------- */
#include <EEPROM.h>
#define EEPROM_SIZE 512
struct Config {
  uint16_t mqttPort = 1883;
  char     mqttHost[64] = "192.168.1.100";
  char     mqttUser[32] = "";
  char     mqttPass[32] = "";
} config;

/* ---------- Prototypes ---------- */
void setup_wifi();
void mqtt_setup();
void mqtt_loop();
void setup_web();
void read_modbus();
void read_temp();
void update_relays();
void publish_telemetry();
void load_config();
void save_config();
String get_status_json();
void handle_root(AsyncWebServerRequest *req);
void handle_api(AsyncWebServerRequest *req);
void handle_save_config(AsyncWebServerRequest *req);
void handle_reboot(AsyncWebServerRequest *req);
void handle_not_found(AsyncWebServerRequest *req);
void mqtt_publish(const char *topic, const char *payload, bool retained = false);
void mqtt_subscribe();
void on_mqtt_message(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);

/* ---------- Setup ---------- */
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nF1ATB Solar Router – PlatformIO");

  sensors.begin();
  timeClient.begin();
  EEPROM.begin(EEPROM_SIZE);
  load_config();

  setup_wifi();
  mqtt_setup();
  setup_web();

  node.begin(1, Serial2);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  for (uint8_t i = 0; i < 8; i++) {
    pinMode(25 + i, OUTPUT);
    digitalWrite(25 + i, LOW);
  }
}

/* ---------- Loop ---------- */
void loop() {
  timeClient.update();
  mqtt_loop();
  read_temp();
  read_modbus();
  update_relays();
  publish_telemetry();
  delay(1000);
}

/* ---------- Wi-Fi ---------- */
void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi ready – IP " + WiFi.localIP().toString());
}

/* ---------- MQTT ---------- */
void mqtt_setup() {
  mqtt.setServer(config.mqttHost, config.mqttPort);
  mqtt.setCredentials(config.mqttUser, config.mqttPass);
  mqtt.onConnect([](bool sessionPresent) {
    Serial.println("MQTT connected");
    mqtt_subscribe();
  });
  mqtt.onDisconnect([](AsyncMqttClientDisconnectReason reason) {
    Serial.println("MQTT disconnected");
  });
  mqtt.onMessage(on_mqtt_message);
}

void mqtt_loop() {
  if (!mqtt.connected() && millis() - lastMqttRetry > 5000) {
    lastMqttRetry = millis();
    mqtt.connect();
  }
}

void mqtt_subscribe() {
  mqtt.subscribe("router/cmd", 0);
}

void on_mqtt_message(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  payload[len] = 0;
  Serial.printf("MQTT %s : %s\n", topic, payload);
  // simple command parser example
  if (strcmp(topic, "router/cmd") == 0) {
    if (strcmp(payload, "reboot") == 0) ESP.restart();
  }
}

void mqtt_publish(const char *topic, const char *payload, bool retained) {
  if (mqtt.connected()) mqtt.publish(topic, 0, retained, payload);
}

/* ---------- Web ---------- */
void setup_web() {
  server.on("/", HTTP_GET, handle_root);
  server.on("/api", HTTP_GET, handle_api);
  server.on("/save", HTTP_POST, handle_save_config);
  server.on("/reboot", HTTP_POST, handle_reboot);
  server.onNotFound(handle_not_found);
  server.begin();
}

void handle_root(AsyncWebServerRequest *req) {
  req->send(200, "text/html",
    "<h1>F1ATB Router</h1><br>"
    "<a href=\"/api\">JSON API</a><br>"
    "<form action=\"/reboot\" method=\"post\"><input type=\"submit\" value=\"Reboot\"></form>");
}

void handle_api(AsyncWebServerRequest *req) {
  req->send(200, "application/json", get_status_json());
}

void handle_save_config(AsyncWebServerRequest *req) {
  if (req->hasArg("host")) strncpy(config.mqttHost, req->arg("host").c_str(), sizeof(config.mqttHost));
  if (req->hasArg("port")) config.mqttPort = req->arg("port").toInt();
  if (req->hasArg("user")) strncpy(config.mqttUser, req->arg("user").c_str(), sizeof(config.mqttUser));
  if (req->hasArg("pass")) strncpy(config.mqttPass, req->arg("pass").c_str(), sizeof(config.mqttPass));
  save_config();
  req->send(200, "text/plain", "Saved");
}

void handle_reboot(AsyncWebServerRequest *req) {
  req->send(200, "text/plain", "Rebooting...");
  delay(1000);
  ESP.restart();
}

void handle_not_found(AsyncWebServerRequest *req) {
  req->send(404, "text/plain", "Not found");
}

/* ---------- Modbus ---------- */
void read_modbus() {
  uint16_t data[2];
  if (node.readInputRegisters(0x0000, 2) == node.ku8MBSuccess) {
    surplusPuissance = (float)((int16_t)node.getResponseBuffer(0)) / 10.0f;
  }
}

/* ---------- DS18B20 ---------- */
void read_temp() {
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0);
}

/* ---------- Relays ---------- */
void update_relays() {
  for (uint8_t i = 0; i < 8; i++) {
    bool on = surplusPuissance > (i + 1) * 500.0f;
    if (on != relayStates[i]) {
      digitalWrite(25 + i, on ? HIGH : LOW);
      relayStates[i] = on;
    }
  }
}

/* ---------- Telemetry ---------- */
void publish_telemetry() {
  if (millis() - lastTelemetry < 10000 || !mqtt.connected()) return;
  lastTelemetry = millis();

  JsonDocument doc;
  doc["surplus"] = surplusPuissance;
  doc["temp"]    = tempC;
  JsonArray arr = doc["relays"].to<JsonArray>();
  for (uint8_t i = 0; i < 8; i++) arr.add(relayStates[i]);
  String out; serializeJson(doc, out);
  mqtt_publish("router/telemetry", out.c_str());
}

/* ---------- EEPROM helpers ---------- */
void load_config() {
  EEPROM.get(0, config);
  if (config.mqttPort == 0xFFFF) {           // fresh EEPROM
    strcpy(config.mqttHost, "192.168.1.100");
    config.mqttPort = 1883;
  }
}

void save_config() {
  EEPROM.put(0, config);
  EEPROM.commit();
}

/* ---------- JSON helper ---------- */
String get_status_json() {
  JsonDocument doc;
  doc["surplus"] = surplusPuissance;
  doc["temp"]    = tempC;
  JsonArray arr = doc["relays"].to<JsonArray>();
  for (uint8_t i = 0; i < 8; i++) arr.add(relayStates[i]);
  String out; serializeJson(doc, out);
  return out;
}
