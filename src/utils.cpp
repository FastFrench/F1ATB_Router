#include "globals.h"


//*************
//* Test Pmax *
//*************

// The single source of truth for clamping a float value.
float PfloatMax(float value) {
  if (value > PmaxReseau) return PmaxReseau;
  if (value < -PmaxReseau) return -PmaxReseau;
  return value;
}

// Surcharge for String type that calls the float version.
float PfloatMax(const String& value) {
  return PfloatMax(value.toFloat());
}

// Clamps an integer by reusing the float clamping logic.
int PintMax(int value) {
    return static_cast<int>(PfloatMax(static_cast<float>(value)));
}


// Utility functions for JSON formatting
String AddInt(String nom, int valeur) {
  return ",\"" + nom + "\":" + String(valeur);
}
String AddByte(String nom, byte valeur) {
  return ",\"" + nom + "\":" + String(valeur);
}
String AddUlong(String nom, unsigned long valeur) {
  return ",\"" + nom + "\":" + String(valeur);
}
String AddUshort(String nom, unsigned short valeur) {
  return ",\"" + nom + "\":" + String(valeur);
}
String AddStr(String nom, String valeur) {
  return ",\"" + nom + "\":\"" + valeur + "\"";
}

void SplitS(String Str, String& Before, String Separ, String& After) {
  int p = Str.indexOf(Separ);
  Before = Str.substring(0, p);
  After = Str.substring(p + 1);
}
// Conversion des adresses IP suivant le coeur

String IP2String(unsigned long IP) {
  byte arr[4];
  arr[0] = IP & 0xFF;            // 0x78
  arr[1] = (IP >> 8) & 0xFF;     // 0x56
  arr[2] = (IP >> 16) & 0xFF;    // 0x34
  arr[3] = (IP >> 24) & 0xFF;    // 0x12
  for (int i = 0; i < 4; i++) {  //Pour eviter corruption données entre coeur 0 et coeur1 (arr en variable local)
    arrIP[i] = arr[i];           //Pour le WIFI au setup
  }
  return String(arr[3]) + "." + String(arr[2]) + "." + String(arr[1]) + "." + String(arr[0]);
}
unsigned long String2IP(String S) {
  unsigned long IP = 0;
  for (int i = 0; i < 3; i++) {
    int p = S.indexOf(".");
    String s = S.substring(0, p);
    S = S.substring(p + 1);
    IP += s.toInt();
    IP = IP * 256;
  }
  IP += S.toInt();
  return IP;
}

//Gestion couleurs
String ULtoHex(unsigned long x) {
  char buffer[15];
  ltoa(x, buffer, 16);
  String S = "000000" + String(buffer);
  int p = S.length();
  S = "#" + S.substring(p - 6);  //Format pour HTML color
  return S;
}

unsigned long ConvCouleur(String V) {  //Notation CSS en UL
  return strtoul(V.c_str(), NULL, 16);
}

// Utility Functions
String IP2String(IPAddress ip) {
    return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

String StringJson(const String& key, const String& json) {
    int start = json.indexOf("\"" + key + "\":");
    if (start == -1) return "";
    
    start = json.indexOf(":", start) + 1;
    while (start < json.length() && json.charAt(start) == ' ') start++;
    
    if (json.charAt(start) == '"') {
        start++;
        int end = json.indexOf("\"", start);
        return json.substring(start, end);
    } else {
        int end = start;
        while (end < json.length() && json.charAt(end) != ',' && json.charAt(end) != '}') end++;
        return json.substring(start, end);
    }
}

String StringJson(String nom, String Json) {
  int p = Json.indexOf(nom + "\":");
  Json = Json.substring(p);
  p = Json.indexOf(":");
  Json = Json.substring(p + 1);
  p = Json.indexOf("\"");
  Json = Json.substring(p + 1);
  p = Json.indexOf("\"");
  Json = Json.substring(0, p);
  return Json;
}


float ValJson(String nom, String Json) {
  int p = Json.indexOf(nom + "\":");
  Json = Json.substring(p);
  p = Json.indexOf(":");
  Json = Json.substring(p + 1);
  int q = Json.indexOf(",");
  p = Json.indexOf("}");
  p = min(p, q);
  float val = 0;
  if (p > 0) {
    Json = Json.substring(0, p);
    val = Json.toFloat();
  }
  return val;
}

float ValJsonSG(String nom, String Json) {
  int p = Json.indexOf(nom);
  Json = Json.substring(p);
  p = Json.indexOf(":");
  Json = Json.substring(p + 1);
  Json.trim();
  p = Json.indexOf(",");
  float val = 0;
  if (p > 0) {
    Json = Json.substring(0, p);
    val = Json.toFloat();
  }
  return val;
}