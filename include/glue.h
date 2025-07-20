#pragma once
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

extern const char *WIFI_SSID;
extern const char *WIFI_PASS;
extern AsyncWebServer server;
extern AsyncMqttClient mqtt;
extern ModbusMaster node;
extern float surplusPuissance;
extern uint8_t relayStates[8];

void setup_wifi();
void mqtt_setup();
void mqtt_loop();
void setup_web();
void read_modbus();
void update_relays();
String get_status_json();
