#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <Adafruit_NeoPixel.h>
#include "windmills.h"
SoftwareSerial SerialGPS(D1, D2); // TX, RX (on GPS module)
TinyGPSPlus gps;
Adafruit_NeoPixel pixels(12, D4, NEO_GRB + NEO_KHZ800);
unsigned long waitingMillis = 0;
void setup()
{
  Serial.begin(115200);
  pixels.begin();
  pixels.clear();
  pixels.setBrightness(30); // 1 -> 255
  SerialGPS.begin(9600);
  // Spews gargage in serial monitor after upload but.. should calm down and work; certainly after a reset. No idea why (might just be platformio)
  WiFi.mode(WIFI_OFF); // Poss also sleep to save power (in case car runs USB overnight?)
  delay(4000);         // Pause for upload issues
  Serial.println(F("\nLet's go!"));
}
// void loop()
// {
//   // ******** Debugging raw data output *********
//   // $GPRMC,224940.00,A,5318.29323,N,00122.51004,W,2.116,,100721,,,A*6D
//   // $GPVTG,,T,,M,2.116,N,3.919,K,A*25
//   // $GPGGA,224940.00,5318.29323,N,00122.51004,W,1,05,6.16,92.9,M,47.5,M,,*70
//   // $GPGSA,A,3,23,13,18,05,15,,,,,,,,8.32,6.16,5.60*02
//   // $GPGSV,2,1,06,05,59,219,17,13,74,286,26,14,42,126,,15,42,286,33*78
//   // $GPGSV,2,2,06,18,21,304,30,23,07,322,15*70
//   // $GPGLL,5318.28408,N,00122.53474,W,225459.00,A,A*7F
//   // if (SerialGPS.available())
//   // {
//   //   Serial.write(SerialGPS.read());
//   // }
//   // ********************************************
//   while (SerialGPS.available())
//   {
//     gps.encode(SerialGPS.read());
//   }
//   if (!gps.location.isValid() && millis() > waitingMillis + 10000)
//   {
//     waitingMillis = millis();
//     Serial.println("Waiting for lock on");
//   }
//   if (gps.location.isUpdated())
//   {
//     // Serial.print(F("Satellite Count: "));
//     // Serial.println(gps.satellites.value());
//     // Serial.print(F("Speed MPH: "));
//     // Serial.println(gps.speed.mph());
//     // Serial.print(F("Altitude(m): "));
//     // Serial.println(gps.altitude.meters());
//     // Serial.print(F("Year: "));
//     // Serial.println(gps.date.year());
//     float lat = (float)gps.location.lat();
//     float lng = (float)gps.location.lng();
//     Serial.print(F("\nLat: "));
//     Serial.print(lat, 6);
//     Serial.print(F("Lng: "));
//     Serial.println(lng, 6);
//     Serial.print(F("Course deg: "));
//     Serial.println(gps.course.deg());
//     Serial.print(F("Course human: "));
//     Serial.println(gps.cardinal(gps.course.value()));
//     // Windmill windmillNearest = {-40, 175, "C sucks"};
//     double nearestDistance = 9999999;
//     int nearestIndex = 0;
//     for (unsigned int i = 0; i < sizeof(windmills) / sizeof(windmills[0]); i++)
//     {
//       double distance = gps.distanceBetween(lat, lng, windmills[i].lat, windmills[i].lng);
//       // Serial.print(windmills[i].name);
//       // Serial.print(" ");
//       // Serial.println(distance);
//       if (distance < nearestDistance)
//       {
//         // Don't understand C enough to know why this won't compile
//         // windmillNearest = windmills[i];
//         nearestIndex = i;
//         nearestDistance = distance;
//       }
//       // delay(100);
//     }
//     Serial.println(windmills[nearestIndex].name);
//     Serial.print(nearestDistance);
//     Serial.println("m ");
//     Serial.print(gps.courseTo(
//         gps.location.lat(),
//         gps.location.lng(),
//         windmills[nearestIndex].lat,
//         windmills[nearestIndex].lng));
//     Serial.println("deg from course ");
//     delay(1000); // A nice big fat delay
//   }
// }
// LED ring test
void loop()
{
  pixels.clear();
  for (int i = 0; i < 12; i++)
  {
    Serial.println(i);
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    delay(500);
  }
}