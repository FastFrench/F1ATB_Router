#ifndef GLOBALS_H
#define GLOBALS_H
#define Version "14.25"

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HardwareSerial.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "Actions.h"
#include "teleinfo.h"

// =================================================================
// VARIABLES GLOBALES
// =================================================================

// -- Système & Réseau --
extern byte ModeReseau;
extern byte Horloge;
extern int ESP32_Type; // Ajouté
extern String ssid;
extern String password;
extern String hostname;
extern String nomRouteur;
extern String nomSondeMobile;
extern String DATE;
extern bool HeureValide;
extern float HeureCouranteDeci;
extern unsigned long previousETX;
extern String ES, FS, GS, RS, US;
extern float PmaxReseau;
extern bool LissageLong;
extern WebServer server;
extern PubSubClient client;
extern PubSubClient clientMQTT; 
extern WiFiClientSecure clientSecu;
extern const char OtaHtml[]; // Ajouté

// -- Écran & LED --
extern byte LEDgroupe;
extern byte LEDyellow[14];
extern byte LEDgreen[14];
extern byte rotation;
extern uint16_t Calibre[8];
extern String MessageH[10];
extern int idxMessage;
extern int cptLEDyellow;
extern int cptLEDgreen;

// -- Puissance & Énergie --
extern String Source;
extern String Source_data;
extern bool Pva_valide;
extern bool EnergieActiveValide;
extern int Energie_M_Soutiree;
extern int Energie_M_Injectee;
extern int PuissanceS_M;
extern int PuissanceI_M;
extern int PVAS_M;
extern int PVAI_M;
extern float EASfloat;
extern float EAIfloat;
extern bool PuissanceRecue;
extern int PactConso_M, PactProd;

// -- Historique --
extern int16_t tabPw_Maison_5mn[600];
extern int16_t tabPw_Triac_5mn[600];
extern int16_t tabTemperature_5mn[4][600];
extern int16_t tabPw_Maison_2s[300];
extern int16_t tabPw_Triac_2s[300];
extern int16_t tabPva_Maison_2s[300];
extern int16_t tabPva_Triac_2s[300];
extern int16_t IdxStock2s;
extern int16_t IdxStockPW;

// -- Température --
extern float temperature[4];
extern int TemperatureValide[4];
extern String Source_Temp[4];
extern String nomTemperature[4];

// -- Actions & Triac --
extern Action LesActions[100];
extern volatile int NbActions;
extern int8_t Retard[8]; // Ajouté
extern bool erreurTriac;
extern byte pTriac;
extern int8_t pulseTriac, zeroCross;
extern int8_t tabPulseSinusOn[101];
extern int8_t tabPulseSinusTotal[101];

// -- Linky & Teleinfo --
extern HardwareSerial MySerial;
extern const int SER_BUF_SIZE;
extern int8_t RXD2, TXD2;
extern volatile char DataRawLinky[10000]; // Ajouté
extern volatile int IdxDataRawLinky;      // Ajouté
extern TInfo tinfo;
extern bool LFon;
extern String LTARF;
extern String STGEt;
extern String JourLinky;
extern int16_t Int_HeureLinky, Int_MinuteLinky, Int_SecondeLinky;

// -- MQTT --
extern unsigned int MQTTRepet;
extern int subMQTT;
extern String P_MQTT_Brute;
extern String PwMQTT;
extern String PvaMQTT;
extern String PfMQTT;
extern unsigned long LastPwMQTTMillis;

// -- Sources Spécifiques (Enphase, Shelly, etc.) --
extern String EnphaseUser, EnphasePwd, EnphaseSerial, JsonToken;
extern String HW_dataBrute;
extern String ShEm_dataBrute;
extern int ShEm_comptage_appels;
extern String Shelly_Name, Shelly_Profile;
extern float Tension_T, Intensite_T, PowerFactor_T, Frequence;
extern float Tension_M, Intensite_M, PowerFactor_M;

// -- RMS (Routeurs Externes) --
extern const int LesRouteursMax;
extern unsigned long RMS_IP[8];
extern String RMS_NomEtat[8];
extern int8_t RMS_Note[8];
extern int RMSextIdx;
extern unsigned long RMSextIP;
extern int RMS_NbCx[8];

// =================================================================
// FONCTIONS GLOBALES
// =================================================================

// -- Fonctions de Main.cpp --
extern void StockMessage(String msg);
extern void EcritureEnROM();
extern String urlEncode(String str);
extern String IP2String(unsigned long ip);
extern void SplitS(String& source, String& part1, String separator, String& remaining);
extern unsigned long ConvCouleur(String hex);
extern void SetCouleurs();
extern void GoPage(int page);
extern void AccueilLoop();
extern void AccueilClick();
extern void GrapheTrace(int param);
extern void TraceReseau();
extern void TraceTarif();
extern void TraceCalibr();
extern void ClickPreCalibr();
extern float PfloatMax(float value);
extern void filtre_puissance();
extern void handleRoot();

// -- Fonctions des autres fichiers --
extern void Actions_Loop(); // Actions.cpp
extern void Setup_Actions(); // Actions.cpp
extern void AccueilForceClick(); // Ajouté (défini dans EcranLCD.cpp)
extern void PrintCentreO(String text, int x, int y, int size); // EcranLED.cpp
extern bool testMQTTconnected(); // MQTT.cpp
extern void Liste_NomsEtats(int index); // RMS_Externes.cpp
extern void Init_Server(); // Server.cpp
extern void LectureEnphase(); // Source_EnphaseEnvoy.cpp
extern void CallESP32_Externe(); // Source_Externe.cpp
extern void LectureHomeW(); // Source_HomeWizard.cpp
extern String ValJsonSG(String key, String json); // Source_JSY.cpp
extern void LectureLinky(); // Source_Linky.cpp
extern void UpdatePmqtt(); // Source_MQTT.cpp
extern void LectureShellyEm(); // Source_ShellyEm.cpp
extern void LectureShellyProEm(); // Source_ShellyProEm.cpp
extern void LectureSmartG(); // Source_SmartG.cpp
extern void LectureUxI(); // Source_UxI.cpp
extern void checkTempo(); // Tempo.cpp
extern String StringJson(String key, String json); // Ajouté (défini dans UxI.cpp)
extern void Triac_loop(); // Triac.cpp
extern void MesurePower(); // UxI.cpp

#endif // GLOBALS_H