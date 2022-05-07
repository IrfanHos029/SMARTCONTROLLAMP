/*
 * DIBUAT TANGGAL 15-04-2022
 * JAM 16:47
 */

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#define indikator_1 D7 //kuning
#define indikator_2 D0 //hijau

char auth[] = "UGMEtVh84Gk4e_x6zMkO2GCqIpFBDIsj";
char ssid[] = "Irfan.A";
char pass[] = "irfan0204";

unsigned long Sindi = 0;
int Delay = 500;
bool stateInd = false;
bool connectWIFI;

BLYNK_WRITE(V0)
{
  byte pinValue = param.asInt();
  if(pinValue == 1){
    Blynk.notify("CONTROLLER DIRESET");
    delay(200);
    ESP.restart();
  }
}

void setup() {
   WiFi.mode(WIFI_STA);
    Serial.begin(9600);
    pinMode(D7,OUTPUT);
    pinMode(D0,OUTPUT);

   WiFiManager wifi;
   connectWIFI = wifi.autoConnect("CONTROLLER","00000000"); // password protected ap
    if(!connectWIFI) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
    delay(100);
    Blynk.begin(auth,WiFi.SSID().c_str(), WiFi.psk().c_str());
    digitalWrite(D7,LOW);
    Serial.println("READY");
    Blynk.notify("SMART HOME AKTIF");
}

void loop() {
    cekWIFI();
    digitalWrite(D0,stateInd);
    Blynk.run();
    
}

void cekWIFI(){
  
   if(WiFi.status() != WL_CONNECTED){
       digitalWrite(D7,HIGH);
       warningIndikator();
       Serial.println("WIFI DISCONNECT");
     }

    else
    {
      digitalWrite(D7,LOW);
      stateInd = true;
    }
    
}

void warningIndikator(){
  unsigned long tmr = millis();
  if(tmr - Sindi > Delay){
    Sindi = tmr;
    stateInd = !stateInd;
  }
}
