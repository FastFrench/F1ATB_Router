//********************************************************************
// Connexion au Shelly, avec TimeOut et second essai si échec. 
//********************************************************************
bool Connect(WiFiClient& clientESP, String host, int port, int timeOutMs)
{
    // Protocole monophasé ou triphasé FIN
    if (!clientESP.connect(host.c_str(), port, timeOutMs)) {
        clientESP.stop();
        delay(200);
        if (!clientESP.connect(host.c_str(), port, timeOutMs)) {
            clientESP.stop();
            Debug(String("connection to Shelly Pro Em failed twice : ") + host);
            delay(200);
            return false;
        }
        else
            Debug(String("connection to Shelly Pro Em failed to connect in ") + String(timeOutMs) + "ms, but succeeded on 2nd try : " + host);
    }
    return true;
}

// Global variables used for profiling the Shelly Pro EM reading process
unsigned long connectionDelay = 0;
unsigned long getDelay = 0;
unsigned long readDataDelay = 0;
int nbPackets = 0;

void ReportShellyProfiling() {
    LastStatusRMS = String("delay: C=") + connectionDelay + " G=" + getDelay + " R*" + nbPackets + "=" + readDataDelay + " Lissage=" + LissageLong;
}

//********************************************************************
// Lecture de la réponse du Shelly à une url (appel REST API Shelly)
// Gère la connection, déconnection, timeOut et remontée d'erreurs.  
// Si stopAtString est fourni, on s'arrête dès qu'on trouve cette chaîne,
// on tronque la réponse et on remplace la chaîne par '}' pour fermer le JSON
//********************************************************************
String ReadShellyData(WiFiClient& clientESP, String url, String host, int port, int timeOutMsConnect, int timeOutMsReading, String stopAtString)
{
    connectionDelay = 0;
    getDelay = 0;
    readDataDelay = 0;
    nbPackets = 0;
    String Shelly_Data = "";

    // 1 - Connect
    unsigned long t0 = millis();
    if (!Connect(clientESP, host, port, timeOutMsConnect)) return Shelly_Data;
    connectionDelay = millis() - t0;

    // 2 - REST API Request
    t0 = millis();
    clientESP.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    while (clientESP.available() == 0) {
        if (millis() - t0 > timeOutMsReading) {
            Debug("client Shelly Em Reading Timeout ! : " + host);
            clientESP.stop();
            return Shelly_Data;
        }
        delay(10);
    }
    getDelay = millis() - t0;

    // 3 - Read Data avec optimisation optionnelle
    t0 = millis();
    const int BUFFER_SIZE = 2048;
    char buffer[BUFFER_SIZE];
    bool headersComplete = false;
    int contentLength = -1;
    int bodyReceived = 0;
    int headerEndPos = -1;
    bool useOptimization = (stopAtString.length() > 0);

    while (clientESP.connected() && (millis() - t0 < timeOutMsReading)) {
        int bytesAvailable = clientESP.available();
        if (bytesAvailable > 0) {
            int bytesToRead = min(bytesAvailable, BUFFER_SIZE - 1);
            int bytesRead = clientESP.readBytes(buffer, bytesToRead);
            buffer[bytesRead] = '\0';
            Debug(String(nbPackets) + " - Read " + String(bytesRead) + " bytes at " + String(millis() - t0) + "ms");
            Shelly_Data += String(buffer);
            nbPackets++;

            // **OPTIMISATION : Vérifier si on a trouvé la chaîne d'arrêt**
            if (useOptimization) {
                int stopPos = Shelly_Data.indexOf(stopAtString);
                if (stopPos >= 0) {
                    // Tronquer les données à la position de la chaîne d'arrêt
                    Shelly_Data = Shelly_Data.substring(0, stopPos);
                    // Ajouter '}' pour fermer le JSON
                    Shelly_Data += "}";
                    Debug("Found stop string '" + stopAtString + "' at position " + String(stopPos) + ", truncating and stopping read");
                    break;
                }
            }

            // Analyser les en-têtes HTTP pour Content-Length (mode sécurisé)
            if (!headersComplete) {
                headerEndPos = Shelly_Data.indexOf("\r\n\r\n");
                if (headerEndPos >= 0) {
                    headersComplete = true;
                    int clPos = Shelly_Data.indexOf("Content-Length: ");
                    if (clPos >= 0 && clPos < headerEndPos) {
                        int clStart = clPos + 16;
                        int clEnd = Shelly_Data.indexOf('\r', clStart);
                        if (clEnd > clStart) {
                            String clValue = Shelly_Data.substring(clStart, clEnd);
                            contentLength = clValue.toInt();
                            Debug("Content-Length found: " + String(contentLength));
                        }
                    }
                    bodyReceived = Shelly_Data.length() - (headerEndPos + 4);
                    Debug("Headers complete, body received so far: " + String(bodyReceived) + "/" + String(contentLength));
                }
            }
            else {
                bodyReceived += bytesRead;
            }

            // Si on a reçu tout le contenu selon Content-Length (mode sécurisé)
            if (!useOptimization && headersComplete && contentLength > 0 && bodyReceived >= contentLength) {
                Debug("All content received (" + String(bodyReceived) + "/" + String(contentLength) + "), stopping read");
                break;
            }
        }
        else {
            // Logique de timeout (mode sécurisé uniquement)
            if (!useOptimization) {
                if (headersComplete && contentLength <= 0) {
                    unsigned long waitStart = millis();
                    while ((millis() - waitStart < 50) && clientESP.connected()) {
                        if (clientESP.available() > 0) break;
                        delay(1);
                    }
                    if (clientESP.available() == 0) {
                        Debug("No Content-Length found, timeout reached, stopping read");
                        break;
                    }
                }
                else {
                    delay(1);
                }
            }
            else {
                delay(1);
            }
        }
    }
    readDataDelay = millis() - t0;

    // 4 - Close connection
    clientESP.stop();
    return Shelly_Data;
}

//*********************************************************************
// Variante Shelly Pro Em proposé par Raphael591 (Juillet 2024)       *
//  + Correction Octobre 2024  et Janvier 2025                        *
// ********************************************************************
// dash (février 2025) - Adaptation à l'utilisation du Pro 3EM en     *
// monophasé tout en ayant conservé le profile du matériel en triphasé*
// ********************************************************************
// * Paramétrage de la voie pour le shelly pro 3 em :                 *
// *   - 3 sur le triphasé.                                           *
// *   - 0 ou 1 ou 2 sur le monophasé fonction de la pince à utiliser.*
// ********************************************************************

void LectureShellyProEm() {
  String S = "";
  String Shelly_Data = "";
  bool Shelly_Triphase_As_Monophase = false;
  String Shelly_Phase = "";
  float Pw = 0;
  float voltage = 0;
  float pf = 0;
  
  // ADD PERSO : AJOUT VARIABLE JSON pour facilité la lecture des infos EM PRO
  String tmp;  // ADD PERSO

  // Use WiFiClient class to create TCP connections
  WiFiClient clientESP_RMS;
  String host = IP2String(RMSextIP);

  int voie = EnphaseSerial.toInt();
  int p;
  // **** Identification du modèle du shelly (une seule fois) *****
  if (Shelly_Name == "") {
    Shelly_Data = ReadShellyData(clientESP_RMS, "/rpc/Shelly.GetDeviceInfo", host);
    Shelly_Name = StringJson("id", Shelly_Data);
    p = Shelly_Name.indexOf("-");
    Shelly_Name = Shelly_Name.substring(0, p);
    Shelly_Profile = StringJson("profile", Shelly_Data);
  }
  // **** Modèle shelly FIN ******

  // Protocole monophasé ou triphasé DÉBUT
  if (Shelly_Profile == "triphase" && voie != 3) {
    Shelly_Triphase_As_Monophase = true;
  }
  // Protocole monophasé ou triphasé FIN
  Shelly_Data = ReadShellyData(clientESP_RMS, "/rpc/Shelly.GetStatus", host, 80, 3000, 5000, ",\"wifi\":");
  ShEm_comptage_appels = (ShEm_comptage_appels + 1) % 5;  // 1 appel sur 6 vers la deuxième voie qui ne sert pas au routeur
  unsigned long t0 = millis();  
  p = Shelly_Data.indexOf("{");
  Shelly_Data = Shelly_Data.substring(p);
  if (Shelly_Name == "shellypro3em" && voie == 3) {
    // 3 em Triphasé
    ShEm_dataBrute = "<strong>" + Shelly_Name + "</strong><br>" + Shelly_Data;

    float pf1, pf2, pf3;
    float volt1, volt2, volt3;

    tmp = PrefiltreJson("em:0", ":", Shelly_Data);

    Pw = ValJson("total_act_power", tmp);

    volt1 = ValJson("a_voltage", tmp);
    pf1 = ValJson("a_pf", tmp);

    volt2 = ValJson("b_voltage", tmp);
    pf2 = ValJson("b_pf", tmp);

    volt3 = ValJson("c_voltage", tmp);
    pf3 = ValJson("c_pf", tmp);

    voltage = (volt1 + volt2 + volt3) / 3;
    pf = abs((pf1 + pf2 + pf3) / 3);
    if (pf > 1) pf = 1;
    if (Pw >= 0) {
      PuissanceS_M_inst = Pw;
      PuissanceI_M_inst = 0;
      if (pf > 0.01) 
        PVAS_M_inst = PfloatMax(Pw / pf);
      else
        PVAS_M_inst = 0;      
      PVAI_M_inst = 0;
    } else {
      PuissanceS_M_inst = 0;
      PuissanceI_M_inst = -Pw;
      if (pf > 0.01)
        PVAI_M_inst = PfloatMax(-Pw / pf);
      else
        PVAI_M_inst = 0;      
      PVAS_M_inst = 0;
    }
    tmp = PrefiltreJson("emdata:0", ":", Shelly_Data);      // ADD PERSO
    Energie_M_Soutiree = myLongJson("total_act", tmp);      // ADD PERSO
    Energie_M_Injectee = myLongJson("total_act_ret", tmp);  // ADD PERSO
    PowerFactor_M = pf;
    Tension_M = voltage;
    Pva_valide = true;
  } else if (Shelly_Name == "shellypro3em" && Shelly_Triphase_As_Monophase) {
    // on utilise le code ASCII de a (97) pour obtenir le prefix de la phase. Voie 0=a 1=b 2=c
    Shelly_Phase = char(voie + 97);

    // 3 em Monophasé : Voie != 3 et profile Triphase actif
    ShEm_dataBrute = "<strong>" + Shelly_Name + " " + "Phase " + Shelly_Phase + "</strong><br>" + Shelly_Data;

    tmp = PrefiltreJson("em:0", ":", Shelly_Data);

    Pw = ValJson(Shelly_Phase + "_act_power", tmp);
    voltage = ValJson(Shelly_Phase + "_voltage", tmp);
    pf = abs(ValJson(Shelly_Phase + "_pf", tmp));

    if (pf > 1) pf = 1;
    if (Pw >= 0) {
      PuissanceS_M_inst = Pw;
      PuissanceI_M_inst = 0;
      if (pf > 0.01)
        PVAS_M_inst = PfloatMax(Pw / pf);
      else 
        PVAS_M_inst = 0;
      PVAI_M_inst = 0;
    } else {
      PuissanceS_M_inst = 0;
      PuissanceI_M_inst = -Pw;
      if (pf > 0.01) 
        PVAI_M_inst = PfloatMax(-Pw / pf);
      else
        PVAI_M_inst = 0;
      PVAS_M_inst = 0;
    }
    tmp = PrefiltreJson("emdata:0", ":", Shelly_Data);                             // ADD PERSO
    Energie_M_Soutiree = myLongJson(Shelly_Phase + "_total_act_energy", tmp);      // ADD PERSO
    Energie_M_Injectee = myLongJson(Shelly_Phase + "_total_act_ret_energy", tmp);  // ADD PERSO
    PowerFactor_M = pf;
    Tension_M = voltage;
    Pva_valide = true;
  } else if (Shelly_Name == "shellypro3em") {
    // 3 em Monophasé : Voie != 3
    ShEm_dataBrute = "<strong>" + Shelly_Name + "</strong><br>" + Shelly_Data;
    String voiePrincipale = String(voie);
    String voieSecondaire = String((voie+1)%3); // Très arbitraire, à changer à terme 

    tmp = PrefiltreJson("em1:" + voiePrincipale, ":", Shelly_Data);  // Voie mono
    Pw = ValJson("act_power", tmp);
    voltage = ValJson("voltage", tmp);
    pf = ValJson("pf", tmp);
    pf = abs(pf);
    if (pf > 1) pf = 1;
    // voie du routeur
    if (Pw >= 0) {
    PuissanceS_M_inst = Pw;
    PuissanceI_M_inst = 0;
    if (pf > 0.01)
        PVAS_M_inst = PfloatMax(Pw / pf);
    else
        PVAS_M_inst = 0;
    PVAI_M_inst = 0;
    } else {
    PuissanceS_M_inst = 0;
    PuissanceI_M_inst = -Pw;
    if (pf > 0.01)
        PVAI_M_inst = PfloatMax(-Pw / pf);
    else
        PVAI_M_inst = 0;        
    PVAS_M_inst = 0;
    }
    tmp = PrefiltreJson("em1data:" + voiePrincipale, ":", Shelly_Data);  // ADD PERSO
    Energie_M_Soutiree = myLongJson("total_act_energy", tmp);          // ADD PERSO
    Energie_M_Injectee = myLongJson("total_act_ret_energy", tmp);      // ADD PERSO
    PowerFactor_M = pf;
    Tension_M = voltage;
    Pva_valide = true;

    // voie secondaire
    tmp = PrefiltreJson("em1:" + voieSecondaire, ":", Shelly_Data);  // Voie mono
    Pw = ValJson("act_power", tmp);
    voltage = ValJson("voltage", tmp);
    pf = ValJson("pf", tmp);
    pf = abs(pf);
    if (pf > 1) pf = 1;
    
    if (Pw >= 0) {
    PuissanceS_T_inst = Pw;
    PuissanceI_T_inst = 0;
    if (pf > 0.01)
        PVAS_T_inst = PfloatMax(Pw / pf);
    else
        PVAS_T_inst = 0;        
    PVAI_T_inst = 0;
    } else {
    PuissanceS_T_inst = 0;
    PuissanceI_T_inst = -Pw;
    if (pf > 0.01) 
        PVAI_T_inst = PfloatMax(-Pw / pf);
    else 
        PVAI_T_inst = 0;        
    PVAS_T_inst = 0;
    }
    tmp = PrefiltreJson("em1data:" + voieSecondaire, ":", Shelly_Data);  // ADD PERSO
    Energie_T_Soutiree = myLongJson("total_act_energy", tmp);          // ADD PERSO
    Energie_T_Injectee = myLongJson("total_act_ret_energy", tmp);      // ADD PERSO
    PowerFactor_T = pf;
    Tension_T = voltage;    
  } else if (Shelly_Name == "shellyproem50") {  // Monophasé pro EM
    String voiePrincipale = String(voie);
    String voieSecondaire = String((voie + 1) % 2); // Très arbitraire, à changer à terme 

    //proEm50 2 voie (0 ou 1)

    ShEm_dataBrute = "<strong>" + Shelly_Name + "</strong><br>" + Shelly_Data;
    Shelly_Data = Shelly_Data + ",";
    if (Shelly_Data.indexOf("act_power") > 0) {                      // Donnée valide
      // voie du routeur
        tmp = PrefiltreJson("em1:" + voiePrincipale, ":", Shelly_Data);  // ADD PERSO
        Pw = ValJson("act_power", tmp);                                // ADD PERSO
        voltage = ValJson("voltage", tmp);                             // ADD PERSO
        pf = ValJson("pf", tmp);                                       // ADD PERSO
        pf = abs(pf);
        if (pf > 1) pf = 1;
        if (Pw >= 0) {
            PuissanceS_M_inst = Pw;
            PuissanceI_M_inst = 0;
            if (pf > 0.01) 
            PVAS_M_inst = PfloatMax(Pw / pf);
            else
            PVAS_M_inst = 0;          
            PVAI_M_inst = 0;
        } else {
            PuissanceS_M_inst = 0;
            PuissanceI_M_inst = -Pw;
            if (pf > 0.01) 
            PVAI_M_inst = PfloatMax(-Pw / pf);
            else 
            PVAI_M_inst = 0;          
            PVAS_M_inst = 0;
        }
        tmp = PrefiltreJson("em1data:" + voiePrincipale, ":", Shelly_Data);  // ADD PERSO
        Energie_M_Soutiree = myLongJson("total_act_energy", tmp);          // ADD PERSO
        Energie_M_Injectee = myLongJson("total_act_ret_energy", tmp);      // ADD PERSO
        PowerFactor_M = pf;
        Tension_M = voltage;
        Pva_valide = true;
      // voie secondaire
        tmp = PrefiltreJson("em1:" + voieSecondaire, ":", Shelly_Data);  // ADD PERSO
        Pw = ValJson("act_power", tmp);                                // ADD PERSO
        voltage = ValJson("voltage", tmp);                             // ADD PERSO
        pf = ValJson("pf", tmp);                                       // ADD PERSO
        pf = abs(pf);
        if (pf > 1) pf = 1;
        if (Pw >= 0) {
          PuissanceS_T_inst = Pw;
          PuissanceI_T_inst = 0;
          if (pf > 0.01)
            PVAS_T_inst = PfloatMax(Pw / pf);
          else
            PVAS_T_inst = 0;          
          PVAI_T_inst = 0;
        } else {
          PuissanceS_T_inst = 0;
          PuissanceI_T_inst = -Pw;
          if (pf > 0.01) 
            PVAI_T_inst = PfloatMax(-Pw / pf);
           else 
            PVAI_T_inst = 0;          
          PVAS_T_inst = 0;
        }
        tmp = PrefiltreJson("em1data:" + voieSecondaire, ":", Shelly_Data);  // ADD PERSO
        Energie_T_Soutiree = myLongJson("total_act_energy", tmp);          // ADD PERSO
        Energie_T_Injectee = myLongJson("total_act_ret_energy", tmp);      // ADD PERSO
        PowerFactor_T = pf;
        Tension_T = voltage;
      }    
  }  
  filtre_puissance();
  PuissanceRecue = true;  // Reset du Watchdog à chaque trame du Shelly reçue
  EnergieActiveValide = true;
  if (cptLEDyellow > 30) 
    cptLEDyellow = 4;  
  ReportShellyProfiling();
}
