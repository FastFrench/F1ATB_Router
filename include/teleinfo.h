#ifndef TELEINFO_H
#define TELEINFO_H

#include <Arduino.h> // Pour le type String

// Définition unique de la structure TInfo
struct TInfo {
  String ADCO;
  String OPTARIF;
  String ISOUSC;
  long BASE;
  long HCHC;
  long HCHP;
  long EJP_HN;
  long EJP_HPM;
  long BBR_HC_JB;
  long BBR_HP_JB;
  long BBR_HC_JW;
  long BBR_HP_JW;
  long BBR_HC_JR;
  long BBR_HP_JR;
  long PEJP;
  int PTEC;
  long IINST;
  long ADPS;
  long IMAX;
  long PAPP;
  String HHPHC;
  String MOTDETAT;
};

#endif // TELEINFO_H