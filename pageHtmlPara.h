//***************************************************
// Page HTML et Javascript de gestion des Paramètres
//***************************************************
const char *ParaHtml = R"====(
  <!doctype html>
  <html><head><meta charset="UTF-8">
  <link rel="stylesheet" href="commun.css">
  <style>    
    body{color:white;}
    .form {margin:auto;padding:10px;display: table;text-align:left;width:100%;}
    .form2 {margin:2px;padding:2px;display: block;width:100%;}
    .ligne {display: table-row;padding:10px;}
    .cadre {border-top:1px solid azure;}
    .canalT {display: table;}
    .canalTr {display: table-row;width:100%;}
    .canalTc {display: table-cell;padding:1px;text-align:center;width:100%;font-size:12px;}
    label,.nomR{display: table-cell;margin: 5px;text-align:left;font-size:20px;height:25px;width:70%;}
    input{display: table-cell;margin: 5px;text-align:left;font-size:20px;height:25px;}
    select{display: table-cell;margin: 5px;text-align:left;}
    .source label{display: table-cell;margin: 5px;text-align:left;font-size:16px;height:25px;width:initial;}
    .boldT{text-align:left;font-weight:bold;padding:10px;}   
    .Bparametres{border:inset 10px azure;}
    .Bgeneraux{border:inset 4px azure;}
    #BoutonsBas{text-align:center;}    
    #ligneFixe,.ligneTemperature,#ligneExt,#ligneEnphaseUser,#ligneEnphasePwd,#ligneEnphaseSerial,#infoIP,#ligneTopicP,#ligneTopicT{display:none;}
    .Zone{width:100%;border 1px solid grey;border-radius:10px;margin-top:10px;background-color:rgba(30,30,30,0.3);} 
    #onglets2{display:block;} 
    .don{height:50px;display:block;text-align:center;} 
    .donEnv{display:inline-block;text-align:center;border:azure 3px outset; border-radius:18px;background-color:white;} 
    .donNone{display:none;} 
  </style>
  <script src="/ParaJS"></script>
  <script src="/ParaRouteurJS"></script>
  </head>
  <body onLoad="Init();">
    <div id="lesOnglets"></div>
    <h4>Param&egrave;tres généraux</h4>

    <div class="Zone">
        <div class="boldT">Routeur</div>
        <div class="form"  >
          <div class='ligne'>
            <label for='nomRouteur' >Nom du routeur : </label>
            <input type='text' name='nomRouteur' id='nomRouteur' >
          </div>
          <div class='ligne' id='ligneMobile'>
            <label for='nomSondeMobile' >Nom Données courant Maison : </label>
            <input type='text' name='nomSondeMobile' id='nomSondeMobile' >
          </div>
          <div class='ligne' id='ligneFixe'>
            <label for='nomSondeFixe' >Nom Données courant seconde sonde : </label>
            <input type='text' name='nomSondeFixe' id='nomSondeFixe' >
          </div>         
          <div class='ligne'>
            <label for='TempoRTEon'>Affichage couleur Tempo RTE : </label>
            <input type='checkbox' name='TempoRTEon' id='TempoRTEon' style='width:25px;' >
          </div>
          <div class='ligne'>
            <label for='MQTTRepete'>Puissances et température envoyées au serveur MQTT<br>Période (s) répétition  (0= pas d'envoi) : </label>
            <input type='number' name='MQTTRepete' id='MQTTRepete'  onclick="checkDisabled();" >
          </div>
          <div class='ligne'>
             <label for='WifiSleep'>Wifi Sleep/veille <small>(réduit la consommation mais ralenti la communication)</small>: </label>
             <input type='checkbox' name='WifiSleep' id='WifiSleep' style='width:25px;' ><span class='fsize10'>Nécessite un Reset de l'ESP32</span>
          </div>
          <div class='ligne'>
             <label for='RAZdata'>Remise à zéro historique des mesures</label>
             <input  class='bouton' type='button' onclick="SendRAZ();" value='RAZ' >
          </div>
        </div>
        <div class="form"  >
          <div class="ligne source">
            <label for='Serial0' style='text-align:right;'>Pas de port série</label>
            <input type='radio' name='pSerie' id='Serial0' value="0"  >
            <label for='Serial1' style='text-align:right;'>Port série RX=gpio 16, TX=gpio 17</label>
            <input type='radio' name='pSerie' id='Serial1' value="1"  >
            <label for='Serial2' style='text-align:right;'>Port série RX=gpio 26, TX=gpio 27</label>
            <input type='radio' name='pSerie' id='Serial2' value="2"  >
          </div>
        </div>
        <div class="form"  >
          <div class="ligne source">
            <label for='Triac0' style='text-align:right;'>Pas de Triac</label>
            <input type='radio' name='pTriac' id='Triac0' value="0"  >
            <label for='Triac1' style='text-align:right;'>Triac pulse=gpio 4, Zc=gpio 5</label>
            <input type='radio' name='pTriac' id='Triac1' value="1"  >
            <label for='Triac2' style='text-align:right;'>Triac pulse=gpio 22, Zc=gpio 23</label>
            <input type='radio' name='pTriac' id='Triac2' value="2"  >
          </div>
        </div>
    </div>
    <div class="Zone">
        <div class="boldT">Adresse IP de l'ESP32 du Routeur</div>
        <div class="form"  >
          <div class='ligne'>
            <label for='dhcp'>Adresse IP auto (DHCP) <span id='localIP'><span> : </label>
            <input type='checkbox' name='dhcp' id='dhcp' style='width:25px;' onclick="checkDisabled();"><span class='fsize10'>Nécessite un Reset de l'ESP32</span>
          </div>
        </div>
        <div class="form"  id="infoIP">
          <div class='ligne'>
            <label for='adrIP'>Adresse IP si fixe (ex : 192.168.1.245) : <br><span class='fsize10'>N&eacute;cessite un Reset de l'ESP32</span></label>
            <input type='text' name='adrIP' id='adrIP' >
          </div>
          <div class='ligne'>
            <label for='gateway'>Passerelle / Gateway (ex : 192.168.1.254) :  <br><span class='fsize10'>En g&eacute;n&eacute;ral l'adresse de votre box internet</span></label>
            <input type='text' name='gateway' id='gateway' >
          </div>
          <div class='ligne'>
            <label for='masque'>Masque / Subnet (ex : 255.255.255.0) :  </label>
            <input type='text' name='masque' id='masque' >
          </div>
          <div class='ligne'>
            <label for='dns'>DNS (ex : 192.168.1.254) :  <br><span class='fsize10'>En g&eacute;n&eacute;ral l'adresse de votre box internet</span></label>
            <input type='text' name='dns' id='dns' >
          </div>
        </div>
    </div>
    <div class="Zone">
        <div class="boldT">Liste des Routeurs en réseau <span class="fsize10">Même version de routeur recommandée</span></div>
        <div class="form" id="Routeurs"></div>
    </div>
    <div class="Zone">
        <div class="boldT">Source des mesures de puissance</div>
        <div class="form"  > 
          <div class="ligne source">
            <label for='UxI' style='text-align:right;'>UxI</label>
            <input type='radio' name='sources' id='UxI' value="UxI"  onclick="checkDisabled();">
            <label for='UxIx2' style='text-align:right;'>UxIx2</label>
            <input type='radio' name='sources' id='UxIx2' value="UxIx2"  onclick="checkDisabled();">   
            <label for='UxIx3' style='text-align:right;'>UxIx3</label>
            <input type='radio' name='sources' id='UxIx3' value="UxIx3"  onclick="checkDisabled();">       
            <label for='Linky' style='text-align:right;'>Linky</label>
            <input type='radio' name='sources' id='Linky' value="Linky"  onclick="checkDisabled();">
            <label for='Enphase' style='text-align:right;'>Enphase-Envoy</label>
            <input type='radio' name='sources' id='Enphase' value="Enphase"  onclick="checkDisabled();">
            <label for='SmartG' style='text-align:right;'>SmartGateways</label>
            <input type='radio' name='sources' id='SmartG' value="SmartG"  onclick="checkDisabled();">
            <label for='ShellyEm' style='text-align:right;'>Shelly Em</label>
            <input type='radio' name='sources' id='ShellyEm' value="ShellyEm"  onclick="checkDisabled();">
            <label for='ShellyPro' style='text-align:right;'>Shelly Pro Em</label>
            <input type='radio' name='sources' id='ShellyPro' value="ShellyPro"  onclick="checkDisabled();">
            <label for='Ext' style='text-align:right;'>ESP Externe</label>
            <input type='radio' name='sources' id='Ext' value="Ext"  onclick="checkDisabled();">
            <label for='Pmqtt' style='text-align:right;'>MQTT</label>
            <input type='radio' name='sources' id='Pmqtt' value="Pmqtt"  onclick="checkDisabled();">
          </div>
        </div>
        <div class="form"  >
          <div class='ligne' id="ligneExt">
            <label for='RMSextIP'>Adresse IP <span id='labExtIp'></span> externe (ex : 192.168.1.248) : </label>
            <input type='text' name='RMSextIP' id='RMSextIP' >
          </div>
          <div class='ligne' id="ligneEnphaseUser">
            <label for='EnphaseUser'>Enphase Envoy-S metered User : <span class='fsize10'><br>Pour firmvare Envoy-S V7 seulement</span></label>
            <input type='text' name='EnphaseUser' id='EnphaseUser' >
          </div>
          <div class='ligne' id="ligneEnphasePwd">
            <label for='EnphasePwd'>Enphase Envoy-S metered Password : <span class='fsize10'><br>Pour firmvare Envoy-S V7 seulement</span></label>
            <input type='password' name='EnphasePwd' id='EnphasePwd' >
          </div>
          <div class='ligne' id="ligneEnphaseSerial">
            <label for='EnphaseSerial' id="label_enphase_shelly"></label>
            <input type='text' name='EnphaseSerial' id='EnphaseSerial' onchange='checkDisabled();'>
          </div>
          <div class='ligne' id="ligneTopicP">
            <label for='TopicP' >MQTT Topic Puissance :</label>
            <input type='text' name='TopicP' id='TopicP' >
          </div>
          <div><span class='fsize10'>Nécessite un Reset de l'ESP32</span></div>
        </div> 
    </div>
    <div class="Zone" >
        <div class="boldT">Source des mesures de température</div>
        <div class="form"  >
          <div id="Sources_Temp"></div>
        </div>
        
    </div>
    
    <div class="Zone" id="Zmqtt">
        <div class="boldT">Paramètres serveur MQTT <small>(Home Assistant , Domoticz ...)</small></div>
        <div class="form"  >  
          <div class='ligne'>
            <label for='MQTTIP'>Adresse IP host MQTT (ex : 192.168.1.18) : </label>
            <input type='text' name='MQTTIP' id='MQTTIP' >
          </div>
          <div class='ligne'>
            <label for='MQTTPort'> port (ex : 1883) : </label>
            <input type='number' name='MQTTPort' id='MQTTPort' >
          </div>
          <div class='ligne'>
            <label for='MQTTUser'>MQTT User nom : </label>
            <input type='text' name='MQTTUser' id='MQTTUser' >
          </div>
          <div class='ligne'>
            <label for='MQTTpwd'>MQTT mot de passe : </label>
            <input type='password' name='MQTTpwd' id='MQTTpwd' >
          </div>
          <div class='ligne'>
            <label for='MQTTPrefix'>MQTT Prefix (1 seul mot ex : homeassistant ) : </label>
            <input type='text' name='MQTTPrefix' id='MQTTPrefix' >
          </div>
          <div class='ligne'>
            <label for='MQTTdeviceName'>MQTT Device Name (1 seul mot ex : routeur_rms ) : </label>
            <input type='text' name='MQTTdeviceName' id='MQTTdeviceName' >
          </div>
          <div class='ligne'>
            <label for='subMQTT'>Souscrire au forçage des Actions via le serveur MQTT: </label>
            <input type='checkbox' name='subMQTT' id='subMQTT' style='width:25px;' onclick='checkDisabled();' >
          </div>
        </div>
    </div>
    <div class="Zone" id="Zcalib">
        <div class="boldT" id='Tui'>Calibration Mesures Ueff et Ieff <small>(UxI)</small></div>
        <div class="form"  >    
          <div class='ligne' id='CUi'>
            <label for='CalibU'>Coefficient multiplicateur Ueff (typique : 1000) : </label>
            <input type='number' name='CalibU' id='CalibU'   >
          </div>
          <div class='ligne' id='CuI'>
            <label for='CalibI'>Coefficient multiplicateur Ieff (typique : 1000) : </label>
            <input type='number' name='CalibI' id='CalibI'   >
          </div>
        </div>
    </div>
    <div  id='BoutonsBas'>        
        <br><input  class='bouton' type='button' onclick="SendValues();" value='Sauvegarder' >
        <div class="lds-dual-ring" id="attente"></div>
        <input  class='bouton' type='button' onclick='Reset();' value='ESP32 Reset' >
    </div>
    <br>
    <div id='pied'></div>
    <br>
    <div class="donEnv" >
      <form action="https://www.paypal.com/donate" method="post" target="_top">
        <input type="hidden" name="hosted_button_id" value="Z35E9D5D9N9DN" />
        <input class="don" type="image" src="https://pics.paypal.com/00/s/MGY1NzdhY2YtYTRkNi00YzIwLWI2YzQtNWI3YjM3ZmFiNWUx/file.PNG" border="0" name="submit" title="PayPal - The safer, easier way to pay online!" alt="Bouton Faites un don avec PayPal" />
        <img alt="" class="donNone" border="0" src="https://www.paypal.com/fr_FR/i/scr/pixel.gif" width="1" height="1" />
      </form>

    </div>
  </body></html>
)====";
const char *ParaJS = R"====(
  var refTempIP=[];
  function Init(){
    var S="";
    for (var i=0;i<4;i++){

        S +="<div class='form cadre'>";
          S +="<div class='canalT'><div class='canalTr'><div class='canalTc'>Canal : "+i+"</div></div></div>";
          S +="<div class='ligne source'>";
            S +="<label for='tempNo" + i + "' style='text-align:right;'>Pas de mesure</label>";
            S +="<input type='radio' name='srcTemp" + i + "' id='tempNo" + i + "' value='tempNo'  onclick='checkDisabled();'>";
            S +="<label for='tempInt" + i + "' style='text-align:right;'>Capteur DS18B20 Interne</label>";
            S +="<input type='radio' name='srcTemp" + i + "' id='tempInt" + i + "' value='tempInt'  onclick='checkDisabled();'>  "; 
            S +="<label for='tempExt" + i + "' style='text-align:right;'>Capteur DS18B20 ESP Externe</label>";
            S +="<input type='radio' name='srcTemp" + i + "' id='tempExt" + i + "' value='tempExt'  onclick='checkDisabled();'>";       
            S +="<label for='tempMqtt" + i + "' style='text-align:right;'>MQTT</label>";
            S +="<input type='radio' name='srcTemp" + i + "' id='tempMqtt" + i + "' value='tempMqtt'  onclick='checkDisabled();'>";
          S +="</div>";
        S +="</div>";
        S +="<div class='form ligneTemperature' id='ligneTemperature" + i + "' >";
          S +="<div class='ligne' >";
            S +="<label for='nomTemperature" + i + "' >Nom Température : </label>";
            S +="<input type='text' name='nomTemperature" + i + "' id='nomTemperature" + i + "' >";
          S +="</div>";
          S +="<div class='ligne' id='ligneoffsetTemp" + i + "'>";
            S +="<label for='offsetTemp" + i + "' >Offset mesure température (°C) :</label>";
            S +="<input type='number' name='offsetTemp" + i + "' id='offsetTemp" + i + "' >";
          S +="</div>";
          S +="<div class='ligne' id='ligneTopicT" + i + "'>";
            S +="<label for='TopicT" + i + "' >MQTT Topic Température :</label>";
            S +="<input type='text' name='TopicT" + i + "' id='TopicT" + i + "' >";
          S +="</div>";
          S +="<div class='ligne' id='ligneIPtemp" + i + "'>";
            S +="<label for='refTempIP" + i + "' >Adresse IP ESP-RMS externe :</label>";
            S +="<select  id='refTempIP" + i + "' ></select>";
          S +="</div>";
          S +="<div class='ligne' id='canalEXTtemp" + i + "'>";
            S +="<label for='canalEXT" + i + "' >Canal Température ESP-RMS externe :</label>";
            S +="<select id='canalEXT" + i +"'><option value=0>Canal 0</option><option value=1>Canal 1</option><option value=2>Canal 2</option><option value=3>Canal 3</option></select>";
          S +="</div>";
          
        S +="</div>";

    }
    GH("Sources_Temp",S);
    S="<div class='ligne'><div class='nomR'><strong>Nom</strong></div><div class='nomR'><strong>Adresse IP</strong></div></div>";
    for (var i=0;i<8;i++){
        S +="<div class='ligne' id='Routeur_" + i + "' style='display:none;' >";
          S +="<div id='RMS_Nom" + i +"'  class='nomR'></div><input type='text' id='RMS_IP" + i +"'>";
        S +="</div>";
    }
    GH("Routeurs",S);

    SetHautBas();
    LoadParametres();
    LoadParaRouteur();
  }
  function LoadParametres() {
    var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() { 
          if (this.readyState == 4 && this.status == 200) {
             var BlocsParas=this.responseText;
             var LesParas=BlocsParas.split(GS);
             var Para=LesParas[0].split(RS);  
             GID("dhcp").checked = Para[0]==1  ? true:false;
             GID("adrIP").value=int2ip(Para[1]);
             GID("gateway").value=int2ip(Para[2]);
             GID("masque").value=int2ip(Para[3]);
             GID("dns").value=int2ip(Para[4]);
             GID(Para[5]).checked = true;
             GID("RMSextIP").value=int2ip(Para[6]);
             GID("EnphaseUser").value=Para[7];
             GID("EnphasePwd").value=Para[8];
             GID("EnphaseSerial").value=Para[9];
             GID("TopicP").value=Para[10]; 
             GID("MQTTRepete").value = Para[11];
             GID("MQTTIP").value=int2ip(Para[12]);
             GID("MQTTPort").value=Para[13];
             GID("MQTTUser").value=Para[14];
             GID("MQTTpwd").value=Para[15];
             GID("MQTTPrefix").value=Para[16];
             GID("MQTTdeviceName").value=Para[17];
             GID("subMQTT").checked = Para[18]==1  ? true:false; 
             GID("nomRouteur").value=Para[19];
             GID("nomSondeFixe").value=Para[20];
             GID("nomSondeMobile").value=Para[21];
             GID("CalibU").value=Para[22];
             GID("CalibI").value=Para[23];
             GID("TempoRTEon").checked = Para[24]==1  ? true:false;
             GID("WifiSleep").checked = Para[25]==1  ? true:false;
             GID("Serial" + Para[26]).checked = true;
             GID("Triac" + Para[27]).checked = true;
             for (var c=0;c<4;c++){
                var ParaT=LesParas[c+1].split(RS);
                GID("nomTemperature"+c).value=ParaT[0];
                GID(ParaT[1]+c).checked = true; 
                GID("TopicT"+c).value=ParaT[2]; 
                refTempIP[c]=ParaT[3];
                GID("canalEXT"+c).value=ParaT[4]; 
                GID("offsetTemp"+c).value=ParaT[5]/100; 
             }
             
             checkDisabled();
          }         
        };
        xhttp.open('GET', 'ParaAjax', true);
        xhttp.send();
      }
  function SendValues(){
    GID("attente").style="visibility: visible;";
    var dhcp = GID("dhcp").checked ? 1:0;
    var TempoRTEon = GID("TempoRTEon").checked ? 1:0;
    var Source_new = document.querySelector('input[name="sources"]:checked').value;
    var subMQTT = GID("subMQTT").checked ? 1:0;
    var WifiSleep = GID("WifiSleep").checked ? 1:0;
    var Serial = document.querySelector('input[name="pSerie"]:checked').value;
    var pTriac = document.querySelector('input[name="pTriac"]:checked').value;
    var S=dhcp+RS+ ip2int(GID("adrIP").value)+RS+ ip2int(GID("gateway").value);
    S +=RS+ip2int(GID("masque").value)+RS+ ip2int(GID("dns").value)
    S +=RS+Source_new+RS+ ip2int(GID("RMSextIP").value)+ RS+GID("EnphaseUser").value.trim()+RS+GID("EnphasePwd").value.trim()+RS+GID("EnphaseSerial").value.trim() +RS+GID("TopicP").value.trim();
    S +=RS+GID("MQTTRepete").value +RS+ip2int(GID("MQTTIP").value) +RS+GID("MQTTPort").value +RS+GID("MQTTUser").value.trim()+RS+GID("MQTTpwd").value.trim();
    S +=RS+GID("MQTTPrefix").value.trim()+RS+GID("MQTTdeviceName").value.trim() + RS + subMQTT;
    S +=RS+GID("nomRouteur").value.trim()+RS+GID("nomSondeFixe").value.trim()+RS+GID("nomSondeMobile").value.trim();
    S +=RS+GID("CalibU").value+RS+GID("CalibI").value + RS + TempoRTEon + RS + WifiSleep + RS + Serial + RS + pTriac;
    for (var c=0;c<4;c++){
      var QS='input[name="srcTemp' + c + '"]:checked';
      var Source_Temp = document.querySelector(QS).value;
      S +=RS+GID("nomTemperature"+c).value.trim() + RS + Source_Temp  +RS + GID("TopicT"+c).value.trim() + RS + GID("refTempIP"+c).value + RS + GID("canalEXT"+c).value + RS + GID("offsetTemp"+c).value*100;
    }
    for (var i=0;i<8;i++){
        S +=RS+ip2int(GID("RMS_IP"+i).value.trim());
    }
    S="?lesparas="+clean(S);
    console.log(S);
    if ((GID("dhcp").checked ||  checkIP("adrIP")&&checkIP("gateway"))   && (!GID("MQTTRepete").checked ||  checkIP("MQTTIP"))){
      var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() { 
          if (this.readyState == 4 && this.status == 200) {
            var retour=this.responseText;
            location.reload();
          }         
        };
        xhttp.open('GET', 'ParaUpdate'+S, true);
        xhttp.send();
    }
  }
  function SendRAZ(){
    if (confirm("Confirmez la remise à zéro des historiques") ) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() { 
          if (this.readyState == 4 && this.status == 200) {
            var retour=this.responseText;
          }         
        };
        xhttp.open('GET', 'ajaxRAZhisto', true);
        xhttp.send();
    } 
  }
  function checkDisabled(){ 
    GID("infoIP").style.display = GID("dhcp").checked ? "none" : "table";
    GID("Zmqtt").style.display = (GID("MQTTRepete").value != 0  || GID("Pmqtt").checked ||  GID("subMQTT").checked) ? "table" : "none";
    for (var c=0;c<4;c++){
      GID('ligneTemperature'+c).style.display = (GID("tempNo"+c).checked) ? "none" : "table";
      GID('ligneTopicT'+c).style.display = (GID("tempMqtt"+c).checked) ? "table-row" : "none";
      GID('ligneIPtemp'+c).style.display = (GID("tempExt"+c).checked) ? "table-row" : "none";  
      GID('canalEXTtemp'+c).style.display = (GID("tempExt"+c).checked) ? "table-row" : "none"; 
      GID('ligneoffsetTemp'+c).style.display = (GID("tempInt"+c).checked) ? "table-row" : "none"; 
      if (GID("tempMqtt"+c).checked )  GID("Zmqtt").style.display ="table"; 
    }
    GID('ligneTopicP').style.display = (GID("Pmqtt").checked) ? "table-row" : "none";
    Source = document.querySelector('input[name="sources"]:checked').value;
    if (Source !='Ext') Source_data=Source;
    AdaptationSource();
  }
  function checkIP(id){
    var S=GID(id).value;
    var Table=S.split(".");
    var valide=true;
    if (Table.length!=4) {
      valide=false;
    }else{
      for (var i=0;i<Table.length;i++){
        if (Table[i]>255 || Table[i]<0) valide=false;
      }
    }
    if (valide){
      GID(id).style.color="black";
    } else {
      GID(id).style.color="red";
    }
    return valide;
  }
 
  
  function Reset(){
      var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() { 
          if (this.readyState == 4 && this.status == 200) {
            GID('BoutonsBas').innerHTML=this.responseText;
            setTimeout(location.reload(),3000);
          }         
        };
        xhttp.open('GET', 'restart', true);
        xhttp.send();
  }
  function AdaptationSource(){
      GID('ligneFixe').style.display = (Source_data=='UxIx2' || ((Source_data=='ShellyEm' || Source_data=='ShellyPro') && GID("EnphaseSerial").value <3))? "table-row" : "none";
      GID('Zcalib').style.display=(Source_data=='UxI' && Source=='UxI' ) ? "table" : "none";
      var txtExt = "ESP-RMS";
      if (Source=='Enphase') txtExt = "Enphase-Envoy";
      if (Source=='SmartG') txtExt = "SmartGateways";
      var lab_enphaseShelly= "Numéro série passerelle IQ Enphase : <span class='fsize10'><br>Pour firmvare Envoy-S V7 seulement</span>";
      if (Source=='ShellyEm' || Source=='ShellyPro') {
        txtExt = "Shelly (Pro) Em ";
        lab_enphaseShelly="Monophasé : Numéro de voie (0, 1 ou 2) mesurant l'entrée du courant maison<br>Triphasé : mettre 3";
      }
      GID('labExtIp').innerHTML = txtExt;
      GID('label_enphase_shelly').innerHTML = lab_enphaseShelly;
      GID('ligneExt').style.display = (Source=='Ext' || Source=='Enphase' || Source=='SmartG' || Source=='ShellyEm' || Source=='ShellyPro') ? "table-row" : "none";
      GID('ligneEnphaseUser').style.display = (Source=='Enphase') ? "table-row" : "none";
      GID('ligneEnphasePwd').style.display = (Source=='Enphase') ? "table-row" : "none";
      GID('ligneEnphaseSerial').style.display = (Source=='Enphase' || Source=='ShellyEm' || Source=='ShellyPro') ? "table-row" : "none"; //Numéro de serie ou voie
  }
  function FinParaRouteur(){
    var Soptions="";
    for (var c=0;c<nb_ESP;c++){
      GID("RMS_IP"+c).value =IP_RMS[c];
      GH("RMS_Nom"+c,nomRMS[c]);
      GID("Routeur_"+c).style="display: table-row";
      if (c>0){
        Soptions +="<option value=" +c +" >"+ IP_RMS[c] + " "+nomRMS[c] +"</option>";
      }
      var d=c+1;
      if (c<7) {
        GH("RMS_Nom"+d,"<small>Ajoutez une adresse IP de routeur ----></small>");
        GID("Routeur_"+d).style="display: table-row";
      }
    }
    GID("RMS_IP0").readOnly =true;
    for (var i=0;i<4;i++){      
      GH("refTempIP"+i,Soptions);
      GID("refTempIP"+i).value = refTempIP[i];
    }
                
  }
)====";

//Paramètres du routeur et fonctions générales pour toutes les pages.
const char *ParaRouteurJS = R"====(
  var Source="";
  var Source_data="";
  var RMSextIP="";
  var ES=String.fromCharCode(27); //Escape Separator
  var FS=String.fromCharCode(28); //File Separator
  var GS=String.fromCharCode(29); //Group Separator
  var RS=String.fromCharCode(30); //Record Separator
  var US=String.fromCharCode(31); //Unit Separator
  var nomSondeFixe="Sonde Fixe";
  var nomSondeMobile="Sonde Mobile";
  var nb_ESP = 0;
  var nomRMS=[];
  var IP_RMS=[];
  var nomTemperature=[];
  var nomActions=[];
 
  function LoadParaRouteur() {
    var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() { 
          if (this.readyState == 4 && this.status == 200) {
             var LesParas=this.responseText;
             var Para=LesParas.split(GS);
             Source=Para[0];
             Source_data=Para[1];
             RMSextIP= Para[7]; 
             AdaptationSource();  
             GH("localIP",Para[2]);
             GH("nom_R",Para[3]);
             GH("version",Para[4]);
             GH("nomSondeFixe",Para[5]);
             GH("nomSondeMobile",Para[6]); 
             nomSondeFixe=Para[5];
             nomSondeMobile=Para[6];
             var IpNoms=Para[8].split(RS);
             nb_ESP=IpNoms.length - 1;
             for (var c=0;c<nb_ESP;c++){
              var ip_nom=IpNoms[c].split(US);
              IP_RMS[c]=int2ip(ip_nom[0]); 
              nomRMS[c]=ip_nom[1];
              if (c==0) {
                var LesTemp=ip_nom[2].split(FS); //Noms temperature ESP local
                for (var i=0;i<LesTemp.length -1;i++){
                  var LaTemp=LesTemp[i].split(ES);
                  nomTemperature[LaTemp[0]] = LaTemp[1];
                }
              }
              var lesNomsActions=ip_nom[3].split(FS);
              nomActions[c]=[]; 
              for (var i=0;i<lesNomsActions.length -1;i++){
                  var Nact=lesNomsActions[i].split(ES);
                  nomActions[c][i]=Nact;
              }
             }
             IP_RMS[0] = Para[2]; 
             nomRMS[0] = nomRMS[0] +" (local)";
             FinParaRouteur();
          }         
        };
        xhttp.open('GET', 'ParaRouteurAjax', true);
        xhttp.send();
  }
  function GID(id) { return document.getElementById(id); };
  function GH(id, T) {
    if ( GID(id)){
     GID(id).innerHTML = T; }
    }
  function GV(id, T) { GID(id).value = T; }
  function clean(S){ //Remplace & et ? pour les envois au serveur
    let res=S.replace(/\%/g,"%25");
    res = res.replace(/\&/g, "%26");
    res = res.replace(/\#/g, "%23");
    res = res.replace(/\+/g, "%2B");
    res=res.replace(/amp;/g,"");
    return res.replace(/\?/g,"%3F");
  }
  function int2ip (V) {
    var ipInt=parseInt(V);
    return ( (ipInt>>>24) +'.' + (ipInt>>16 & 255) +'.' + (ipInt>>8 & 255) +'.' + (ipInt & 255) );
  }
  function ip2int(ip) {
    ip=ip.trim();
    return ip.split('.').reduce(function(ipInt, octet) { return (ipInt<<8) + parseInt(octet, 10)}, 0) >>> 0;
  }
  function SetHautBas(){
      var S="<div class='onglets'><div class='Baccueil'><a href='/'>Accueil</a></div><div class='Bbrut'><a href='/Brute'>Donn&eacute;es brutes</a></div><div class='Bparametres'><a href='/Para'>Param&egrave;tres</a></div><div class='Bactions'><a href='/Actions'>Actions</a></div></div>";
      S +="<div id='onglets2'><div class='Bgeneraux'><a href='/Para'>Généraux</a></div><div class='Bexport'><a href='/Export'>Import / Export</a></div><div class='Bota'><a href='/OTA'>Mise à jour par OTA</a></div><div class='Bwifi'><a href='/Change_Wifi'>Modif. Wifi</a></div></div>";
      S +="<h2 id='nom_R'>Routeur Solaire - RMS</h2>";
      GH("lesOnglets",S);
      GH("pied","<div>Routeur Version : <span id='version'></span></div><div><a href='https:F1ATB.fr/fr' >F1ATB.fr</a></div>");
  }
  function Hdeci2Hmn(H){
    var HI=parseInt(H);
    return Math.floor(HI / 100) + ":" + ("0" + Math.floor(0.6 * (HI +0.4 - 100 * Math.floor(HI / 100)))).substr(-2, 2);
  }
  function Hmn2Hdeci(H){
    var separ=":";
    if (H.indexOf(".")>0) separ=".";
    if (H.indexOf("h")>0) separ="h";
    var val=H.split(separ);
    var h = Math.floor(100*parseInt(val[0]) + 0.4 + 100*parseInt(val[1])/60);
    h=Math.max(0,h);h=Math.min(2400,h);
    return h;
   
  }
  
)====";
const char *CommunCSS = R"====(
  * {box-sizing: border-box;}
    body {font-size:150%;text-align:center;width:100%;max-width:1000px;margin:auto;background: linear-gradient(#003,#77b5fe,#003);background-attachment:fixed;padding:10px;}
    h2{text-align:center;color:white;}
    h3,h4,h5{color:white;}
    a:link {color:#aaf;text-decoration: none;}
    a:visited {color:#ccf;text-decoration: none;}
    .onglets{margin-top:4px;left:0px;font-size:130%;}
    #onglets2{margin-top:10px;left:0px;font-size:80%;display:none;}
    .Baccueil,.Bbrut,.Bparametres,.Bactions{margin-left:20px;border:outset 4px grey;background-color:#333;border-radius:6px;padding-left:20px;padding-right:20px;display:inline-block;}
    .Bgeneraux,.Bexport,.Bota,.Bwifi{margin-left:20px;border:outset 2px grey;background-color:#333;border-radius:4px;padding-left:20px;padding-right:20px;display:inline-block;}
    #pied{display:flex;justify-content:space-between;font-size:14px;color:white;}
    .fsize10{font-size:10px;height:14px;}
    .fsize8{font-size:8px;}
    .lds-dual-ring {color: #cccc5b;visibility: hidden;}
    .lds-dual-ring,.lds-dual-ring:after {box-sizing: border-box;}
    .lds-dual-ring {display: inline-block;width: 80px;height: 80px;}
    .lds-dual-ring:after {content: " ";display: block;width: 64px;height: 64px;margin: 8px;border-radius: 50%;border: 6.4px solid currentColor;border-color: currentColor transparent currentColor transparent;animation: lds-dual-ring 1.2s linear infinite;}
    @keyframes lds-dual-ring {0% {transform: rotate(0deg);} 100% {transform: rotate(360deg);}}
    .bouton,input[type=file]::file-selector-button{margin: 5px;text-align:left;font-size:20px;height:28px;border:3px grey outset;border-radius:7px;}
)====";