// ********************
// Gestion des Actions
// ********************
class Action {
private:
  int Idx;  //Index
  void CallExterne(String host, String url, int port);
  int T_LastAction = 0;
  int tempoTimer = 0;



public:
  Action();  //Constructeur par defaut
  Action(int aIdx);

  void Definir(String ligne);
  String Lire();
  void Activer(float Pw, int Heure, float Temperature, int Ltarfbin);
  void Arreter();
  void RelaisOn();
  void Prioritaire();


  byte TypeEnCours(int Heure, float Temperature, int Ltarfbin,int Retard);
  int Valmin(int Heure);
  int Valmax(int Heure);
  void InitGpio();
  byte Actif; //0=Inactif,1=Decoupe ou On/Off, 2=Multi, 3= Train
  int Port;
  int Repet;
  int Tempo;
  String Titre;
  String Host;
  String OrdreOn;
  String OrdreOff;
  int Gpio;
  int OutOn;
  int OutOff;
  int tOnOff;
  byte Reactivite;
  byte NbPeriode;
  byte SelectAct; //Ref ESP/Action. 255=pas exploité
  int ExtValide; //Condition Action externe
  int ExtHequiv; //Duree heure *100 action externe
  int ExtOuvert; //Pourcent ouverture
  int CanalT;
  bool On;
  float H_Ouvre;
  byte Type[8]; //0=NO(pas utilisé),1=OFF,2=ON,3=PW,4=Triac
  int Hdeb[8];
  int Hfin[8];
  int Vmin[8]; //Seuil Pw On ou decoupe
  int Vmax[8]; //Seuil Pw Off ou ouverture max triac
  int Tinf[8]; //Temperarure
  int Tsup[8];
  int Hmin[8]; //Heure deci *100 Min pour actif. 0=non utilisé
  int Hmax[8]; //Heure deci *100 Max pour actif
  byte Ooff[8]; //Ouvre Min Action pour Actif. 0 non utilisé
  byte O_on[8]; 
  byte Tarif[8];
};


extern void StockMessage(String m);
