/*
 * DIBUAT TANGGAL 15-04-2022
 * JAM 16:47
 */

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define indikator_1 D7 //kuning
#define indikator_2 D0 //hijau

char auth[] = "UGMEtVh84Gk4e_x6zMkO2GCqIpFBDIsj";
char ssid[] = "Irfan.A";
char pass[] = "irfan0204";

unsigned long Sindi = 0;
int Delay = 500;
bool stateInd = false;

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
    Serial.begin(9600);
    pinMode(D7,OUTPUT);
    pinMode(D0,OUTPUT);
     if(WiFi.status() != WL_CONNECTED){
       digitalWrite(D7,HIGH);
       //warningIndikator();
       Serial.println("WIFI NOT CONNECT");
     }
    delay(100);
    Blynk.begin(auth,ssid,pass);
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
