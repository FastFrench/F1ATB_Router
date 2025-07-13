#ifndef GLOBALS_H
#define GLOBALS_H
#define Version "14.25"
//Nombre Actions Max
#define LesActionsLength 10  //Ne pas toucher -Javascript connais pas
//Nombre Routeurs réseau Max
#define LesRouteursMax 8  //Ne pas toucher -Javascript connais pas

#include <Arduino.h>
#define LIGHT_ADC 34
#define LGFX_AUTODETECT  // Autodetect board
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h> //Librairie pour la gestion Mqtt
#include "Actions.h"
#include "teleinfo.h"


// =================================================================
// VARIABLES GLOBALES
// =================================================================

// -- Système & Réseau --
extern byte ModeReseau;
extern byte Horloge;
extern unsigned long LastRMS_Millis; 
extern byte ESP32_Type; 
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
extern const char *MainHtml;
extern unsigned long PeriodeProgMillis;
//Adressage IP coeur0 et coeur1
extern byte arrIP[4];

// Internal Timers
extern unsigned long previousTimeRMS;
extern float previousTimeRMSMin;
extern float previousTimeRMSMax;
extern float previousTimeRMSMoy;

// -- Écran & LED --
extern LGFX lcd;
extern bool EthernetBug; // Ajouté
extern String Couleurs; // Ajouté
extern byte LEDgroupe;
extern byte LEDyellow[14];
extern byte LEDgreen[14];
extern byte rotation;
extern uint16_t Calibre[8];
extern String MessageH[10];
extern int idxMessage;
extern int cptLEDyellow;
extern int cptLEDgreen;
extern int8_t NumPage; // Ajouté
extern void Ecran_Loop();
extern void Init_LED_OLED(); 

// -- Puissance & Énergie --
extern String Source;
extern String Source_data;
extern bool Pva_valide;
extern bool EnergieActiveValide;
extern int Energie_M_Soutiree;
extern int Energie_M_Injectee;
extern long EnergieJour_T_Injectee;
extern long EnergieJour_M_Injectee;
extern long EnergieJour_T_Soutiree;
extern long EnergieJour_M_Soutiree;
extern int PuissanceS_M;
extern int PuissanceI_M;
extern int PVAS_M;
extern int PVAI_M;
extern float EASfloat;
extern float EAIfloat;
extern bool PuissanceRecue;
extern int PactConso_M, PactProd;
extern float PuissanceS_M_inst; // Ajouté
extern float PuissanceI_M_inst; // Ajouté
extern float PVAS_M_inst; // Ajouté
extern float PVAI_M_inst; // Ajouté
extern float PVA_T_moy, PVA_M_moy;

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
extern volatile int IdxBufDecodLinky; // Ajouté
extern TInfo tinfo;
extern bool LFon;
extern String LTARF;
extern String STGEt;
extern String JourLinky;
extern int16_t Int_HeureLinky, Int_MinuteLinky, Int_SecondeLinky;
extern void Call_RTE_data();

// -- MQTT --
extern unsigned long MQTTIP; // Ajouté
extern unsigned int MQTTRepet;
extern byte subMQTT;
extern String MQTTPrefix; // Ajouté
extern String MQTTPrefixEtat; // Ajouté
extern String MQTTUser; // Ajouté
extern String MQTTPwd; // Ajouté
extern String MQTTdeviceName; // Ajouté
extern unsigned int MQTTPort; // Ajouté
extern String TopicT[4]; // Ajouté
extern String TopicP; // Ajouté
extern String P_MQTT_Brute;
extern String PwMQTT;
extern String PvaMQTT;
extern String PfMQTT;
extern unsigned long LastPwMQTTMillis;

// -- Sources Spécifiques (Enphase, Shelly, etc.) --
extern String Session_id; // Ajouté
extern String EnphaseUser, EnphasePwd, EnphaseSerial, JsonToken;
extern String TokenEnphase; // Ajouté
extern String HW_dataBrute;
extern String ShEm_dataBrute;
extern int ShEm_comptage_appels;
extern String Shelly_Name, Shelly_Profile;
extern float Tension_T, Intensite_T, PowerFactor_T, Frequence;
extern float Tension_M, Intensite_M, PowerFactor_M;
extern void LectureUxIx2(); 
extern void Lecture_JSY333();

// -- RMS (Routeurs Externes) --
//extern unsigned long RMS_IP[8];
//extern String RMS_NomEtat[8];
//extern int8_t RMS_Note[8];
extern int RMSextIdx;
extern unsigned long RMSextIP;
//extern int RMS_NbCx[8];

// Routeurs du réseau
extern unsigned long RMS_IP[LesRouteursMax];  //RMS_IP[0] = adresse IP de cet ESP32
extern String RMS_NomEtat[LesRouteursMax];
extern int8_t RMS_Note[LesRouteursMax];
extern int8_t RMS_NbCx[LesRouteursMax];
extern int RMS_Datas_idx ;

// -- Tempo --
extern int TempoRTEon; // Ajouté
extern String TempoToday, TempoTomorrow;

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
extern void filtre_puissance();
extern void handleRoot();
extern void PrintScroll(String m);

// -- Fonctions des autres fichiers --
extern void Actions_Loop(); // Actions.cpp
extern void Setup_Actions(); // Actions.cpp
extern void AccueilForceClick(); // Ajouté (défini dans EcranLCD.cpp)
extern void OngletsTrace(int page); // Ajouté (défini dans EcranLCD.cpp)
extern void PrintCentre(String S, int X, int Y, float Sz);
extern void PrintGauche(String S, int X, int Y, float Sz);
extern void PrintDroite(String S, int X, int Y, float Sz);
extern void PrintCentreO(String text, int x, int y, int size);
extern void PrintGaucheO(String text, int x, int y, int size);
extern void PrintDroiteO(String text, int x, int y, int size); // Ajouté (défini dans EcranLED.cpp)
extern bool testMQTTconnected(); // MQTT.cpp
extern void Liste_NomsEtats(int index); // RMS_Externes.cpp
extern void Init_Server(); // Server.cpp
extern void LectureEnphase(); // Source_EnphaseEnvoy.cpp
extern void CallESP32_Externe(); // Source_Externe.cpp
extern void LectureHomeW(); // Source_HomeWizard.cpp
extern void LectureLinky(); // Source_Linky.cpp
extern void UpdatePmqtt(); // Source_MQTT.cpp
extern void LectureShellyEm(); // Source_ShellyEm.cpp
extern void LectureShellyProEm(); // Source_ShellyProEm.cpp
extern void LectureSmartG(); // Source_SmartG.cpp
extern void LectureUxI(); // Source_UxI.cpp
extern void checkTempo(); // Tempo.cpp
extern String StringJson(String key, String json); // Ajouté (défini dans UxI.cpp)
extern String PrefiltreJson(String key, String separator, String json); // Ajouté (défini dans UxI.cpp)
extern void Triac_loop(); // Triac.cpp
extern void MesurePower(); // UxI.cpp
extern void JourHeureChange();
extern void GestionMQTT();
extern void Gestion_LEDs(); 
extern void LectureTemperature();
extern void InfoActionExterne();
extern void LireSerial();

// -- Fonctions de utils.cpp --
extern float PfloatMax(float value);
extern float PfloatMax(const String& value);
extern int PintMax(int value);
extern float ValJsonSG(String key, String json); // Source_JSY.cpp
extern float ValJson(String key, String json); // Source_JSY.cpp
extern String Ascii(String S);

// -- Fonctions de utils.cpp (JSon) --
extern String AddInt(String nom, int valeur);
extern String AddByte(String nom, byte valeur);
extern String AddUlong(String nom, unsigned long valeur);
extern String AddUshort(String nom, unsigned short valeur);
extern String AddStr(String nom, String valeur);

#endif // GLOBALS_H