#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6q7GzN-Mk"
#define BLYNK_TEMPLATE_NAME "VnAMonitoring"
#define BLYNK_AUTH_TOKEN "9X5WG5qdg5N_Cy1mu5nP3-QzldzSvV65"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "ACS712.h"

char ssid[] = "airframe";
char pass[] = "airframe";

// voltage sensor initialization
#define sensorPinVoltage 36

int sensorRawVoltage;
double sensorValueVoltage;
double voltage;
double R1 = 30200;
double R2 = 7500;

// current sensor initialization
ACS712  ACS(26, 3.3, 4095, 185);

void setup() {
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  ACS.autoMidPoint();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);                     
  digitalWrite(LED_BUILTIN, LOW);                      


  Blynk.run();

  // show voltage data
  sensorRawVoltage = analogRead(sensorPinVoltage);
  sensorValueVoltage = sensorRawVoltage * 3.3/4096;
  sensorValueVoltage = sensorValueVoltage / (R2/(R1+R2));
  voltage = sensorValueVoltage - 1;
  Serial.println("Voltage : ");
  Serial.print(voltage);
  Serial.println(" V");

  // show current data
  int current = ACS.mA_DC();

  if (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == '+') ACS.incMidPoint();
    if (c == '-') ACS.decMidPoint();
    if (c == '0') ACS.setMidPoint(512);
    Serial.println("ACS.getMidPoint()");
    Serial.println(ACS.getMidPoint());

    if (c == '*') ACS.setmVperAmp(ACS.getmVperAmp() * 1.05);
    if (c == '/') ACS.setmVperAmp(ACS.getmVperAmp() / 1.05);
    Serial.println("ACS.getmVperAmp()");
    Serial.println(ACS.getmVperAmp());
  }
  Serial.println("Current: ");
  Serial.print(current);
  Serial.println(" mA");

  Blynk.virtualWrite(V0, voltage);
  Blynk.virtualWrite(V1, current);
  Blynk.virtualWrite(V2, "Success in VCM");
  
  delay(1000);
}
