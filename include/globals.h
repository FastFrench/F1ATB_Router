#ifndef GLOBALS_H
#define GLOBALS_H
#define Version "14.25"
#define LesActionsLength 10
#define LesRouteursMax 8

#include <Arduino.h>
#define LIGHT_ADC 34
#define LGFX_AUTODETECT
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "Actions.h"
#include "teleinfo.h"
#include <ETH.h>

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
extern String nomSondeFixe;
extern String DATE;
extern bool HeureValide;
extern int16_t HeureCouranteDeci; // CORRIGÉ: int -> int16_t
extern unsigned long previousETX;
extern String ES, FS, GS, RS, US;
extern float PmaxReseau;
extern bool LissageLong;
extern WebServer server;
extern PubSubClient client;
extern PubSubClient clientMQTT;
extern WiFiClientSecure clientSecu;
extern const char OtaHtml[];
extern const char *MainHtml;
extern const char *MainJS;
extern const char *ParaHtml;
extern const char *ParaJS;
extern const char *ParaRouteurJS;
extern const char *ActionsHtml;
extern const char *ActionsJS;
extern const char *ConnectAP_Html;
extern const char *ExportHtml;
extern const char *HeureHtml;
extern const char *CouleursHtml;
extern const char *CommunCouleurJS;
extern const char *CommunCSS;
extern const char *ParaCleHtml;
extern const char *PageBrute;
extern const char *PageBruteJS;
extern unsigned long PeriodeProgMillis;
extern byte arrIP[4];
extern byte dhcpOn; // CORRIGÉ: bool -> byte
extern unsigned long Gateway;
extern unsigned long masque;
extern unsigned long dns;
extern byte ModePara;
extern String CleAcces;
extern String CleAccesRef;
extern byte WifiSleep; // CORRIGÉ: bool -> byte
extern byte pSerial;
extern byte pUxI;
extern byte pTemp;
extern byte refTempIP[4];
extern byte canalTempExterne[4];
extern int offsetTemp[4];
extern unsigned int CalibU, CalibI; // CORRIGÉ: int -> unsigned int

// Internal Timers
extern unsigned long previousTimeRMS;
extern float previousTimeRMSMin;
extern float previousTimeRMSMax;
extern float previousTimeRMSMoy;
extern unsigned long previousMQTTMillis;
extern unsigned long PeriodeMQTTMillis;
extern unsigned long previousMQTTenvoiMillis;
extern unsigned long previousTempMillis;
extern int LastHeureRTE; // CORRIGÉ: unsigned long -> int
extern float previousLoopMin, previousLoopMoy, previousLoopMax;
extern unsigned long T_On_seconde;
extern volatile bool IT10ms; // Ajouté
extern volatile bool IT10ms_in; // Ajouté

// -- Écran & LED --
extern LGFX lcd;
extern int16_t EthernetBug; // CORRIGÉ: int -> int16_t
extern String Couleurs;
extern byte LEDgroupe;
extern byte LEDyellow[14];
extern byte LEDgreen[14];
extern byte rotation;
extern uint16_t Calibre[8];
extern String MessageH[10];
extern int idxMessage;
extern int cptLEDyellow;
extern int cptLEDgreen;
extern int8_t NumPage;
extern int WIFIbug; // Ajouté

// -- Puissance & Énergie --
extern String Source;
extern String Source_data;
extern bool Pva_valide;
extern bool EnergieActiveValide;
extern long Energie_M_Soutiree;
extern long Energie_M_Injectee;
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
extern float PuissanceS_M_inst;
extern float PuissanceI_M_inst;
extern float PVAS_M_inst;
extern float PVAI_M_inst;
extern float PVA_T_moy, PVA_M_moy;
extern long PuissanceS_T, PuissanceI_T, PVAS_T, PVAI_T; // CORRIGÉ: int -> long
extern long Energie_T_Soutiree, Energie_T_Injectee; // CORRIGÉ: int -> long
extern float Puissance_M_moy;

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
extern int8_t tab_histo_ouverture[LesActionsLength][600];
extern int8_t tab_histo_2s_ouverture[LesActionsLength][300];

// -- Température --
extern float temperature[4];
extern int TemperatureValide[4];
extern String Source_Temp[4];
extern String nomTemperature[4];
extern int Nbr_DS18B20;
extern String AllTemp;

// -- Actions & Triac --
extern Action LesActions[LesActionsLength];
extern volatile int NbActions;
extern volatile int Retard[LesActionsLength];
extern bool erreurTriac;
extern byte pTriac;
extern int8_t pulseTriac, zeroCross;
extern int8_t tabPulseSinusOn[101];
extern int8_t tabPulseSinusTotal[101];
extern volatile int16_t ITmode; // CORRIGÉ: byte -> volatile int16_t
extern int P_cent_EEPROM;

// -- Linky & Teleinfo --
extern HardwareSerial MySerial;
extern const int SER_BUF_SIZE;
extern int8_t RXD2, TXD2;
extern volatile char DataRawLinky[10000];
extern volatile int IdxDataRawLinky;
extern volatile int IdxBufDecodLinky;
extern TInfo tinfo;
extern bool LFon;
extern String LTARF;
extern int LTARFbin;
extern String STGEt;
extern String NGTF;
extern String JourLinky;
extern int16_t Int_HeureLinky, Int_MinuteLinky, Int_SecondeLinky;
extern int16_t Int_Heure, Int_Minute; // CORRIGÉ: int -> int16_t
extern long EASF01, EASF02, EASF03, EASF04, EASF05, EASF06, EASF07, EASF08, EASF09, EASF10;
extern long LastwhDlvdCum;

// -- MQTT --
extern unsigned long MQTTIP;
extern unsigned int MQTTRepet;
extern byte subMQTT;
extern String MQTTPrefix;
extern String MQTTPrefixEtat;
extern String MQTTUser;
extern String MQTTPwd;
extern String MQTTdeviceName;
extern unsigned int MQTTPort;
extern String TopicT[4];
extern String TopicP;
extern String P_MQTT_Brute;
extern float PwMQTT; // CORRIGÉ: int -> float
extern float PvaMQTT; // CORRIGÉ: int -> float
extern float PfMQTT; // CORRIGÉ: int -> float
extern unsigned long LastPwMQTTMillis;
extern bool Discovered;

// -- Sources Spécifiques (Enphase, Shelly, etc.) --
extern String Session_id;
extern String EnphaseUser, EnphasePwd, EnphaseSerial, JsonToken;
extern String TokenEnphase;
extern String HW_dataBrute;
extern String ShEm_dataBrute;
extern String SG_dataBrute;
extern String MK333_dataBrute;
extern int ShEm_comptage_appels;
extern String Shelly_Name, Shelly_Profile;
extern float Tension_T, Intensite_T, PowerFactor_T, Frequence;
extern float Tension_M, Intensite_M, PowerFactor_M;
extern float voltM[100];
extern float ampM[100];
extern bool RAZ_JSY;

// -- RMS (Routeurs Externes) --
extern unsigned long RMS_IP[LesRouteursMax];
extern String RMS_NomEtat[LesRouteursMax];
extern int8_t RMS_Note[LesRouteursMax];
extern int8_t RMS_NbCx[LesRouteursMax];
extern int8_t RMSextIdx;
extern unsigned long RMSextIP;
extern int RMS_Datas_idx;

// -- Tempo --
extern byte TempoRTEon; // CORRIGÉ: bool -> byte
extern String TempoToday, TempoTomorrow;

// =================================================================
// FONCTIONS GLOBALES
// =================================================================

// -- Fonctions de Main.cpp --
extern void StockMessage(String msg);
extern void EcritureEnROM();
extern String urlEncode(String str);
extern String IP2String(unsigned long ip);
extern unsigned long String2IP(String ip);
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
extern void init_puissance();
extern void InitTemperature();
extern void INIT_EEPROM();
extern unsigned long LectureCle();
extern void LectureEnROM();
extern void LectureConsoMatinJour();
extern void RAZ_Histo_Conso();
extern void InitGPIOs();
extern void Liste_WIFI();
extern void Init_Server();
extern void Liste_des_Noms();
extern void time_sync_notification(struct timeval *tv);
extern void JourHeureChange();
extern void infoSerie();
extern void LireSerial();
extern void Call_RTE_data();
extern void Setup_UxI();
extern void Setup_Enphase();
extern void Setup_UxIx2();
extern void Setup_Linky();
extern void Setup_JSY333();
extern void Requete_JSY333();
extern void IndexSource();
extern void LectureUxIx2();
extern void Lecture_JSY333();
extern void UpdatePmqtt();
extern void InfoActionExterne();
extern void GestionIT_10ms();
extern void EnergieQuotidienne();
extern void H_Ouvre_Equivalent(int dt);
extern void GestionOverproduction();
extern void Task_LectureRMS(void *pvParameters); // Ajouté

// -- Fonctions des autres fichiers --
extern void Actions_Loop();
extern void Setup_Actions();
extern void AccueilForceClick();
extern void OngletsTrace(int page);
extern void PrintCentre(String S, int X, int Y, float Sz);
extern void PrintGauche(String S, int X, int Y, float Sz);
extern void PrintDroite(String S, int X, int Y, float Sz);
extern void PrintCentreO(String text, int x, int y, int size);
extern void PrintGaucheO(String text, int x, int y, int size);
extern void PrintDroiteO(String text, int x, int y, int size);
extern bool testMQTTconnected();
extern void envoiVersMQTT();
extern void sendMQTTDiscoveryMsg_global();
extern void SendDataToHomeAssistant();
extern void DeviceToDiscover(String Name, String Titre, String unit, String deviceClass, String decim);
extern void DeviceToDiscoverWithoutUnit(String Name, String Titre, String decim);
extern void DeviceToDiscoverWithoutClass(String Name, String Titre, String unit, String decim);
extern void DeviceTextToDiscover(String Name, String Titre);
extern void DeviceBin2Discover(String Name, String Titre);
extern void Liste_NomsEtats(int index);
extern void LectureEnphase();
extern void CallESP32_Externe();
extern void LectureHomeW();
extern void LectureLinky();
extern void LectureShellyEm();
extern void LectureShellyProEm();
extern void LectureSmartG();
extern void LectureUxI();
extern void checkTempo();
extern String StringJson(String key, String json);
extern String PrefiltreJson(String key, String separator, String json);
extern void Triac_loop();
extern void MesurePower();
extern void GestionMQTT();
extern void Gestion_LEDs();
extern void LectureTemperature();
extern String Liste_AP;
extern void Ecran_Init(void);
extern void Ecran_Loop(void);
extern void Init_LED_OLED();

// -- Fonctions de utils.cpp --
extern float PfloatMax(float value);
extern float PfloatMax(const String& value);
extern int PintMax(int value);
extern float ValJsonSG(String key, String json);
extern float ValJson(String key, String json);
extern String Ascii(String S);
extern long LongJson(String key, String json);

// -- Fonctions de utils.cpp (JSon) --
extern String AddInt(String nom, int valeur);
extern String AddByte(String nom, byte valeur);
extern String AddUlong(String nom, unsigned long valeur);
extern String AddUshort(String nom, unsigned short valeur);
extern String AddStr(String nom, String valeur);

#endif // GLOBALS_H