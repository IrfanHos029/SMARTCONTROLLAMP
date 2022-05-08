/*
   DIBUAT TANGGAL 15-04-2022
   JAM 16:47
*/

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#define indikator_1 D7 //kuning
#define indikator_2 D0 //hijau
#define ResetAP D6

char auth[] = "UGMEtVh84Gk4e_x6zMkO2GCqIpFBDIsj";
//char ssid[] = "Irfan.A";
//char pass[] = "irfan0204";

unsigned long Sindi = 0;
int Delay = 500;
int counterWIFI = 0;
bool stateInd = false;
bool connectWIFI;

WiFiManager wifi;

BLYNK_WRITE(V0)
{
  byte pinValue = param.asInt();
  if (pinValue == 1) {
    Blynk.notify("CONTROLLER DIRESET");
    delay(200);
    ESP.restart();
  }
}

void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  pinMode(indikator_1, OUTPUT);
  pinMode(indikator_2, OUTPUT);
  pinMode(D6, INPUT);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);

  // wifi.setConfigPortalBlocking(false);
  //wifi.setConfigPortalTimeout(100);

  connectWIFI = wifi.autoConnect("CONTROLLER", "00000000"); // password protected ap
  if (!connectWIFI) {
    Serial.println("Failed to connect");
    digitalWrite(indikator_1, HIGH);
    digitalWrite(indikator_2, HIGH);
    // ESP.restart();
  }

  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(indikator_1, HIGH);
    Serial.println("WIFI NOT CONNECTED");
  }

  Serial.println("connected...");
  /////////// delay(100);
  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
  digitalWrite(indikator_1, LOW);
  Serial.println("READY");
  Blynk.notify("SMART HOME AKTIF");
}

void loop() {

  cekWIFI();
  resetWifiAP();
  digitalWrite(indikator_2, stateInd);
  Blynk.run();

}

void cekWIFI() {

  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(indikator_1, HIGH);
    warningIndikator();
    Serial.println("WIFI DISCONNECT");
  }

  else
  {
    digitalWrite(indikator_1, LOW);
    stateInd = true;
  }

}

void warningIndikator() {
  unsigned long tmr = millis();
  if (tmr - Sindi > Delay) {
    Sindi = tmr;
    stateInd = !stateInd;
    counterWIFI++;
  }
  //Serial.println(counterWIFI);
  if (counterWIFI >= 300  ) {
    ESP.restart();
  }
}

void resetWifiAP() {
  byte pin = digitalRead(ResetAP);

  if (pin == HIGH) {
    delay(1000);
    Serial.println("CONFIG WIFI DI RESET");
    wifi.resetSettings();
    delay(3000);
    ESP.restart();
  }

}
