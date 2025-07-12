#ifndef GLOBALS_H
#define GLOBALS_H
#define Version "14.25"

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HardwareSerial.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Variables réseau
extern byte ModeReseau;        // ligne 228 main.cpp
extern byte Horloge;           // ligne 229 main.cpp
extern byte LEDgroupe;         // ligne 230 main.cpp
extern const int LesRouteursMax; // ligne 203 main.cpp
extern WebServer server;       // ligne 705 main.cpp

// Variables écran
extern byte rotation;          // ligne 249 main.cpp
extern uint16_t Calibre[8];    // ligne 250 main.cpp

// Variables réseau/WiFi
extern String ssid;            // ligne 238 main.cpp
extern String password;        // ligne 239 main.cpp
extern String hostname;        // ligne 242 main.cpp
extern String Source;          // ligne 241 main.cpp
extern String Source_data;     // ligne 240 main.cpp

// Variables Enphase
extern String EnphaseUser;     // ligne 326 main.cpp
extern String EnphasePwd;      // ligne 327 main.cpp
extern String EnphaseSerial;   // ligne 328 main.cpp
extern String JsonToken;       // ligne 329 main.cpp
extern WiFiClientSecure clientSecu; // ligne 599 main.cpp

// Variables Linky
extern HardwareSerial MySerial; // ligne 706 main.cpp
extern const int SER_BUF_SIZE;  // ligne 198 main.cpp
extern int8_t RXD2, TXD2;       // ligne 707 main.cpp
extern volatile char DataRawLinky[10000]; // ligne 373 main.cpp - taille spécifiée
extern volatile int IdxDataRawLinky; // ligne 370 main.cpp
extern volatile int IdxBufDecodLinky; // ligne 371 main.cpp
extern bool LFon;               // ligne 364 main.cpp
extern String LTARF;            // ligne 378 main.cpp
extern String STGEt;            // ligne 379 main.cpp
extern String JourLinky;        // ligne 381 main.cpp
extern int16_t Int_HeureLinky;  // ligne 382 main.cpp
extern int16_t Int_MinuteLinky; // ligne 383 main.cpp
extern int16_t Int_SecondeLinky; // ligne 384 main.cpp
extern long EASF01, EASF02, EASF03, EASF04, EASF05; // lignes 384-388 main.cpp
extern long EASF06, EASF07, EASF08, EASF09, EASF10; // lignes 389-393 main.cpp

// Variables puissance
extern bool Pva_valide;        // ligne 296 main.cpp
extern bool EnergieActiveValide; // ligne 261 main.cpp
extern int Energie_M_Soutiree; // ligne 266 main.cpp
extern int Energie_M_Injectee; // ligne 267 main.cpp
extern int PuissanceS_M;       // ligne 280 main.cpp
extern int PuissanceI_M;       // ligne 281 main.cpp
extern int PVAS_M;             // ligne 282 main.cpp
extern int PVAI_M;             // ligne 283 main.cpp
extern float EASfloat;         // ligne 284 main.cpp
extern float EAIfloat;         // ligne 285 main.cpp
extern bool PuissanceRecue;    // ligne 606 main.cpp

// Variables historique
extern int16_t tabPw_Maison_5mn[600]; // ligne 286 main.cpp
extern int16_t tabPw_Maison_2s[300];  // ligne 287 main.cpp
extern int16_t IdxStock2s;     // ligne 295 main.cpp
extern int16_t IdxStockPW;     // ligne 296 main.cpp

// Variables température
extern String Source_Temp[4];  // ligne 524 main.cpp
extern float temperature[4];   // ligne 519 main.cpp
extern int TemperatureValide[4]; // ligne 521 main.cpp
extern String nomTemperature[4]; // ligne 525 main.cpp

// Variables LED/OLED
extern byte LEDyellow[14];     // ligne 231 main.cpp
extern byte LEDgreen[14];      // ligne 232 main.cpp
extern String MessageH[10];    // ligne 232 main.cpp
extern int idxMessage;         // ligne 233 main.cpp
extern int cptLEDyellow;       // ligne 234 main.cpp
extern int cptLEDgreen;        // ligne 235 main.cpp

// Variables RMS/Réseau
extern unsigned long RMS_IP[8]; // ligne 603 main.cpp
extern int8_t RMS_Note[8];     // ligne 605 main.cpp
extern String RMS_NomEtat[8];  // ligne 604 main.cpp
extern int RMSextIdx;          // ligne 610 main.cpp
extern unsigned long RMSextIP; // ligne 611 main.cpp
extern int RMS_NbCx[8];        // ligne 612 main.cpp

// Variables MQTT
extern String PwMQTT;          // ligne 431 main.cpp
extern String PvaMQTT;         // ligne 432 main.cpp
extern String PfMQTT;          // ligne 433 main.cpp
extern String P_MQTT_Brute;    // ligne 430 main.cpp
extern unsigned int MQTTRepet; // ligne 332 main.cpp - TYPE CORRIGÉ (unsigned int)
extern int subMQTT;            // ligne 333 main.cpp
extern PubSubClient client;    // ligne 598 main.cpp
extern unsigned long LastPwMQTTMillis; // ligne 582 main.cpp

// Variables Shelly
extern String HW_dataBrute;    // ligne 426 main.cpp
extern String ShEm_dataBrute;  // ligne 427 main.cpp
extern int ShEm_comptage_appels; // ligne 428 main.cpp
extern String Shelly_Name;     // ligne 434 main.cpp
extern String Shelly_Profile;  // ligne 435 main.cpp

// Variables routeur
extern String nomRouteur;      // ligne 256 main.cpp
extern String nomSondeMobile;  // ligne 258 main.cpp

// Variables système
extern String DATE;            // ligne 512 main.cpp
extern bool HeureValide;       // ligne 514 main.cpp
extern float HeureCouranteDeci; // ligne 515 main.cpp
extern unsigned long previousETX; // ligne 580 main.cpp
extern String ES, FS, GS, RS, US; // lignes 261-265 main.cpp
extern Action LesActions[100]; // ligne 444 main.cpp
extern volatile int NbActions; // ligne 445 main.cpp

// Fonctions utilisées dans plusieurs fichiers
extern void SetCouleurs();
extern void GoPage(int page);
extern void AccueilLoop();
extern void AccueilClick();
extern void GrapheTrace(int param);
extern void TraceReseau();
extern void TraceCalibr();
extern void ClickPreCalibr();
extern void handleRoot();
extern void handleMainJS();
extern void handlePara();
extern void Liste_NomsEtats(int index);
extern void StockMessage(String msg);
extern String urlEncode(String str);
extern String IP2String(unsigned long ip);
extern float PfloatMax(float value);
// extern String PfloatMax(String value); // Impossible en C++ - surcharge à supprimer
extern float PfloatMaxString(String value); // Remplacer par cette fonction
extern bool testMQTTconnected();
extern void PrintCentreO(String text, int x, int y, int size);
extern String ValJsonSG(String key, String json);
extern void SplitS(String& source, String& part1, String separator, String& remaining);
extern unsigned long ConvCouleur(String hex);
extern void filtre_puissance();
extern void UpdatePmqtt();
extern void EcritureEnROM();
extern void TraceTarif();

#endif // GLOBALS_H