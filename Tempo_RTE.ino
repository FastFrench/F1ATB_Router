// *******************************************************
// * Recherche Info Tempo RTE pour les sources non Linky *
// *******************************************************


void Call_RTE_data() {
  char buffer[MAX_SIZE_T];
  const char* adr_RTE_Host = "www.services-rte.com";
  String Host = String(adr_RTE_Host);
  String urlJSON = "/cms/open_data/v1/tempoLight";
  String RTEdata = "";
  String line = "";
  String DateRTE = "";                //an-mois-jour
  String DateRTE2 = "";               //an-mois-jour lendemain
  int Hcour = HeureCouranteDeci / 2;  //Par pas de 72secondes pour faire 2 appels si un bug
  int LastH = LastHeureRTE / 2;

  if ((DATEvalid) && ((LastH != Hcour) && (Hcour == 301 || Hcour == 310 || Hcour == 530 || Hcour == 560 || Hcour == 600 || Hcour == 900 || Hcour == 1150) || LastHeureRTE < 0)) {
    if (TempoRTEon == 1) {
      // Use clientSecu class to create TCP connections
      clientSecuRTE.setInsecure();  //skip verification
      if (!clientSecuRTE.connect(adr_RTE_Host, 443)) {
        StockMessage("Connection failed to RTE server :" + Host);
      } else {
        time_t timestamp = time(NULL) - 21600;  //Decallage début période couleur  RTE de 6h.
        struct tm* pTime = localtime(&timestamp);
        strftime(buffer, MAX_SIZE_T, "%Y-%m-%d", pTime);
        DateRTE = String(buffer);
        time_t timestamp2 = time(NULL) + 64800;  //Decallage début période couleur  RTE de 18h pour le lendemainh.
        struct tm* pTime2 = localtime(&timestamp2);
        strftime(buffer, MAX_SIZE_T, "%Y-%m-%d", pTime2);
        DateRTE2 = String(buffer);
        Serial.print("DateRTE:");
        Serial.println(DateRTE);
        Serial.print("DateRTE lendemain:");
        Serial.println(DateRTE2);
        Serial.println(urlJSON);
        clientSecuRTE.print(String("GET ") + urlJSON + " HTTP/1.1\r\n" + "Host: " + Host + "\r\n" + "Connection: close\r\n\r\n");
        Serial.println("Request vers RTE Envoyé");
        unsigned long timeout = millis();
        while (clientSecuRTE.available() == 0) {
          if (millis() - timeout > 5000) {
            StockMessage(">>> clientSecuRTE RTE Timeout !");
            clientSecuRTE.stop();
            return;
          }
        }
        timeout = millis();
        // Lecture des données brutes distantes
        int fin = 0;
        while (clientSecuRTE.connected() && (millis() - timeout < 5000) && fin < 2) {
          line = clientSecuRTE.readStringUntil('\n');
          RTEdata += line;
          if (line.indexOf("}}") >= 0) fin = 2;
        }
        clientSecuRTE.stop();
        Serial.print("RTEdata:");
        Serial.println(RTEdata);
        // C'est RTE qui donne la couleur
        int p = RTEdata.indexOf("\"" + DateRTE + "\"");
        int q = RTEdata.indexOf("\"" + DateRTE2 + "\"");
        if (p > 0 || q > 0) {
          String LTARFrecu = StringJson(DateRTE, RTEdata);  //Remplace code du Linky
          if (LTARFrecu == "BLUE") LTARF = "BLEU";
          if (LTARFrecu == "WHITE") LTARF = "BLANC";
          if (LTARFrecu == "RED") LTARF = "ROUGE";
          line = "0";
          String lendemain = "NON_DEFINI";
          LTARFrecu = StringJson(DateRTE2, RTEdata);
          if (LTARFrecu.indexOf("BLUE") >= 0) {
            line = "4";
            lendemain = "BLEU";
          }
          if (LTARFrecu.indexOf("WHITE") >= 0) {
            line = "8";
            lendemain = "BLANC";
          }
          if (LTARFrecu.indexOf("RED") >= 0) {
            line = "C";
            lendemain = "ROUGE";
          }
          STGE = line;  //Valeur Hexa code du Linky
          StockMessage(DateRTE + " : " + LTARF + " | " + DateRTE2 + " : " + lendemain);
          RTEdata = "";
          LastHeureRTE = HeureCouranteDeci;  //Heure lecture Tempo RTE
        } else {
          StockMessage(DateRTE + " : Pas de données RTE valides");
        }
      }
    } else {
      if (Source != "Linky" && Source != "Ext") {
        LTARF = "";
        STGE = "0";
      }
    }
  }
}
