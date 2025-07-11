// *************************************************
// * Client lecture JSY-MK-333       *    Triphasé *
// * Développement initial de Pierre F (Mars 2024) *
// *************************************************


void Setup_JSY333() {
  MySerial.setRxBufferSize(SER_BUF_SIZE);
  MySerial.begin(9600, SERIAL_8N1, RXD2, TXD2);  //PORT DE CONNEXION AVEC LE CAPTEUR JSY-MK-333
}

void Lecture_JSY333() {
  float Tension_M1, Tension_M2, Tension_M3;
  float Intensite_M1, Intensite_M2, Intensite_M3;
  float PVAS_M_inst1, PVAS_M_inst2, PVAS_M_inst3;
  byte Lecture333[200];
  bool injection;
  bool sens1, sens2, sens3;

  int i;
  byte msg_send[] = { 0x01, 0x03, 0x01, 0x00, 0x00, 0x44, 0x44, 0x05 };
  for (i = 0; i < 8; i++) {
    MySerial.write(msg_send[i]);
  }

  int a = 0;
  while (MySerial.available()) {
    Lecture333[a] = MySerial.read();
    a++;
  }

  if (a == 141) {  //message complet reçu
    Tension_M1 = ((float)(Lecture333[3] * 256 + Lecture333[4])) / 100;
    Tension_M2 = ((float)(Lecture333[5] * 256 + Lecture333[6])) / 100;
    Tension_M3 = ((float)(Lecture333[7] * 256 + Lecture333[8])) / 100;
    Intensite_M1 = ((float)(Lecture333[9] * 256 + Lecture333[10])) / 100;
    Intensite_M2 = ((float)(Lecture333[11] * 256 + Lecture333[12])) / 100;
    Intensite_M3 = ((float)(Lecture333[13] * 256 + Lecture333[14])) / 100;

    sens1 = (Lecture333[104]) & 0x01;
    sens2 = (Lecture333[104] >> 1) & 0x01;
    sens3 = (Lecture333[104] >> 2) & 0x01;

    if (sens1) { Intensite_M1 *= -1; }
    if (sens2) { Intensite_M2 *= -1; }
    if (sens3) { Intensite_M3 *= -1; }

    injection = (Lecture333[104] >> 3) & 0x01;  //si sens est true, injection

    if (injection) {
      PuissanceS_M_inst = 0;
      PuissanceI_M_inst = ((float)((float)(Lecture333[21] * 16777216) + (float)(Lecture333[22] * 65536) + (float)(Lecture333[23] * 256) + (float)Lecture333[24]));
      PVAS_M_inst = 0;
      PVAI_M_inst = ((float)((float)(Lecture333[41] * 16777216) + (float)(Lecture333[42] * 65536) + (float)(Lecture333[43] * 256) + (float)Lecture333[44]));
    } else {
      PuissanceI_M_inst = 0;
      PuissanceS_M_inst = ((float)((float)(Lecture333[21] * 16777216) + (float)(Lecture333[22] * 65536) + (float)(Lecture333[23] * 256) + (float)Lecture333[24]));
      PVAI_M_inst = 0;
      PVAS_M_inst = ((float)((float)(Lecture333[41] * 16777216) + (float)(Lecture333[42] * 65536) + (float)(Lecture333[43] * 256) + (float)Lecture333[44]));
    }

    //Pour obtenir la même valeur de puissance apparente que le Linky
    PVAS_M_inst1 = (float)(Lecture333[35] * 256) + (float)Lecture333[36];
    if (sens1) { PVAS_M_inst1 = -PVAS_M_inst1; }
    PVAS_M_inst2 = (float)(Lecture333[37] * 256) + (float)Lecture333[38];
    if (sens2) { PVAS_M_inst2 = -PVAS_M_inst2; }
    PVAS_M_inst3 = (float)(Lecture333[39] * 256) + (float)Lecture333[40];
    if (sens3) { PVAS_M_inst3 = -PVAS_M_inst3; }
    // PVAS_M_inst = PVAS_M_inst1 + PVAS_M_inst2 + PVAS_M_inst3; // A CONFIRMER **********************************************

    PowerFactor_M = ((float)(Lecture333[53] * 256 + Lecture333[54])) / 1000;
    Energie_M_Soutiree = ((float)((float)(Lecture333[119] * 16777216) + (float)(Lecture333[120] * 65536) + (float)(Lecture333[121] * 256) + (float)Lecture333[122])) * 10;
    Energie_M_Injectee = ((float)((float)(Lecture333[135] * 16777216) + (float)(Lecture333[136] * 65536) + (float)(Lecture333[137] * 256) + (float)Lecture333[138])) * 10;

    MK333_dataBrute = "<strong>Triphasé</strong><br>Phase1 : " + String(int(Tension_M1)) + "V " + String(Intensite_M1) + "A</br>";
    MK333_dataBrute += "<br>Phase2 : " + String(int(Tension_M2)) + "V " + String(Intensite_M2) + "A</br>";
    MK333_dataBrute += "<br>Phase3 : " + String(int(Tension_M3)) + "V " + String(Intensite_M3) + "A</br>";
    MK333_dataBrute += "<br>Puissance active soutirée : " + String(PuissanceS_M_inst) + "W</br>";
    MK333_dataBrute += "<br>Puissance active injectée : " + String(PuissanceI_M_inst) + "W</br>";
    MK333_dataBrute += "<br>Puissance apparente soutirée : " + String(PVAS_M_inst) + "VA</br>";
    MK333_dataBrute += "<br>Puissance apparente injectée : " + String(PVAI_M_inst) + "VA</br>";
    MK333_dataBrute += "<br>Facteur de puissance : " + String(PowerFactor_M) + "</br>";
    //MK333_dataBrute += "<br>Octet de sens: " + String(SensJSY,BIN) + "</br>";
    //MK333_dataBrute += "<br>registre 0113h : " + String(Lecture333[41],HEX) + "/" + String(Lecture333[42],HEX) +"</br>";
    //MK333_dataBrute += "<br>registre 0114h : " + String(Lecture333[43],HEX) + "/" + String(Lecture333[44],HEX) +"</br>";
    Pva_valide=true;
    filtre_puissance();
    esp_task_wdt_reset();  //Reset du Watchdog à chaque trame du JSY reçue
    EnergieActiveValide = true;
    if (cptLEDyellow > 30) {
      cptLEDyellow = 4;
    }
  } else {
    StockMessage("Pas tout reçu, pas traité... nombre de données : " + String(a));
  }
}