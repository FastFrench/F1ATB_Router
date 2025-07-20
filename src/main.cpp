#include "glue.h"

const char *WIFI_SSID = "YOUR_SSID";
const char *WIFI_PASS  = "YOUR_PASS";
AsyncWebServer server(80);
AsyncMqttClient mqtt;
ModbusMaster node;
float surplusPuissance = 0.0;
uint8_t relayStates[8] = {0};
unsigned long lastModbus = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nF1ATB Router – PlatformIO");

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

void loop() {
  if (millis() - lastModbus >= 1000) {
    read_modbus();
    lastModbus = millis();
  }
  update_relays();
}

/* Wi-Fi */
void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi ready – IP " + WiFi.localIP().toString());
}

/* MQTT */
void mqtt_setup() {
  mqtt.setServer("192.168.1.100", 1883);
  mqtt.onConnect([](bool sessionPresent) {
    Serial.println("MQTT connected");
  });
  mqtt.onDisconnect([](AsyncMqttClientDisconnectReason reason) {
    Serial.println("MQTT disconnected");
  });
}

/* No .loop() in AsyncMqttClient – nothing to call */
void mqtt_loop() {
  static uint32_t t0 = 0;
  if (millis() - t0 > 5000) {
    t0 = millis();
    if (!mqtt.connected()) mqtt.connect();
  }
}

/* Web */
void setup_web() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *r) {
    r->send(200, "text/html", "<h1>F1ATB Router</h1><a href=\"/api\">API</a>");
  });
  server.on("/api", HTTP_GET, [](AsyncWebServerRequest *r) {
    JsonDocument doc;
    doc["surplus"] = surplusPuissance;
    JsonArray arr = doc["relays"].to<JsonArray>();
    for (uint8_t i = 0; i < 8; i++) arr.add(relayStates[i]);
    String out; serializeJson(doc, out);
    r->send(200, "application/json", out);
  });
  server.begin();
}

/* Modbus */
void read_modbus() {
  uint16_t data[2];
  if (node.readInputRegisters(0x0000, 2) == node.ku8MBSuccess) {
    surplusPuissance = (float)((int16_t)node.getResponseBuffer(0)) / 10.0f;
  }
}

/* Relays */
void update_relays() {
  for (uint8_t i = 0; i < 8; i++) {
    bool on = surplusPuissance > (i + 1) * 500.0f;
    if (on != relayStates[i]) {
      digitalWrite(25 + i, on ? HIGH : LOW);
      relayStates[i] = on;
    }
  }
}
