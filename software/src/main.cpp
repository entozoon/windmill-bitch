//
// Variable     Under the hood
// LED_BUILTIN  2  LOW IS ON(!)
// D0           16
// D1           5
// D2           4
// D3           0
// D4           2  Also LED_BUILTIN
//
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
SoftwareSerial gpsSerial(D0, D5); // TX, RX (on GPS module)
TinyGPSPlus gps;
void setup()
{
  Serial.begin(76800);
  gpsSerial.begin(9600);
  // Spews gargage in serial monitor after upload but.. should calm down and work; certainly after a reset. No idea why (might just be platformio)
  WiFi.mode(WIFI_OFF); // Poss also sleep to save power (in case car runs USB overnight?)
  delay(4000);         // Pause for upload issues
  Serial.println(F("\nLet's go!"));
}
void loop()
{
  while (gpsSerial.available())
  {
    gps.encode(gpsSerial.read());
  }
  if (gps.location.isUpdated())
  {
    // if (gps.location.isValid()) //
    Serial.println(gps.location.lat(), 6);
    Serial.println(gps.location.lng(), 6);
    delay(500);
  }
}