#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <Adafruit_NeoPixel.h>
#include "windmills.h"
SoftwareSerial SerialGPS(D1, D2); // TX, RX (on GPS module)
TinyGPSPlus gps;
Adafruit_NeoPixel pixels(12, D6, NEO_GRB + NEO_KHZ800);
unsigned long waitingMillis = 0;
#define sgn(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))
void ringCompass(double deg, double distance)
{
  double maxDistance = 3000;
  // https://www.desmos.com/calculator/osfcrnbs08
  double brightnessFactor = constrain(1 - distance / maxDistance, 0.004, 1); // 0.004->1 (3000m->0m)
  int x = round(deg / 360 * 12);
  x = constrain(x, 0, 11); // JIC exactly 360 (although should wrap to 0)
  // https://www.desmos.com/calculator/rlnypbqo7v
  int y = x - sgn(x - 5.5) * 6;
  y = constrain(y, 0, 11);
  // Serial.println(F("ringCompass (") + String(deg) + F(") ") + String(x) + F(" ") + String(y));
  pixels.clear();
  // int brightness = round(brightnessFactor * 200 + 1);
  pixels.setPixelColor(x, pixels.Color(round(brightnessFactor * 255), 0, round(brightnessFactor * 255)));
  pixels.setPixelColor(y, pixels.Color(0, 0, round(brightnessFactor * 100)));
  // Probably should flash when real close, but yeah..
  pixels.show();
}
// ******** Debugging raw data output *********
// $GPRMC, 224940.00, A, 5318.29323, N, 00122.51004, W, 2.116, , 100721, , , A * 6D $GPVTG, , T, , M, 2.116, N, 3.919, K, A * 25 $GPGGA, 224940.00, 5318.29323, N, 00122.51004, W, 1, 05, 6.16, 92.9, M, 47.5, M, , *70 $GPGSA, A, 3, 23, 13, 18, 05, 15, , , , , , , , 8.32, 6.16, 5.60 * 02 $GPGSV, 2, 1, 06, 05, 59, 219, 17, 13, 74, 286, 26, 14, 42, 126, , 15, 42, 286, 33 * 78 $GPGSV, 2, 2, 06, 18, 21, 304, 30, 23, 07, 322, 15 * 70 $GPGLL, 5318.28408, N, 00122.53474, W, 225459.00, A, A * 7F if (SerialGPS.available())
// void setup()
// {
//   Serial.begin(76800);
//   while (!Serial)
//   {
//     ;
//   }
//   SerialGPS.begin(9600);
// }
// void loop()
// {
//   if (SerialGPS.available())
//     Serial.write(SerialGPS.read());
// }
void setup()
{
  Serial.begin(76800);
  pixels.begin();
  pixels.clear();
  pixels.setBrightness(255); // 1 -> 255 (always set 255)
  pixels.fill(pixels.Color(150, 0, 150));
  pixels.setPixelColor(1, pixels.Color(0, 50, 0)); // North star..
  pixels.show();
  SerialGPS.begin(9600);
  // Spews gargage in serial monitor after upload but.. should calm down and work; certainly after a reset. No idea why (might just be platformio)
  // Poss also sleep to save power (in case car runs USB overnight?)
  WiFi.mode(WIFI_OFF);
  // delay(4000);         // Pause for upload issues
  for (int i = 0; i < 10; i++)
  {
    for (int d = 0; d <= 360; d += 10)
    {
      ringCompass(d, 0);
      delay(10);
    }
  }
  Serial.println(F("\nLet's go!"));
}
void loop()
{
  while (SerialGPS.available())
  {
    gps.encode(SerialGPS.read());
  }
  if (!gps.location.isValid())
  {
    float millisFactor = (float(millis() % 2500) / 2500);
    //  I reckon varying colour is slightly smoother than brightness
    double brightness = (round(constrain(((abs(millisFactor - .5) * 2 - .5) * 2), 0, 1) * 9 + 1)); // 1->1->10<-1<-1
    // pixels.setBrightness(round((constrain(abs(tan(float(millis()) / 1000)) / 50, 0, 1)) * 10)); // 0->10<-0 (abs tan style, quite annoying actually)
    pixels.fill(pixels.Color(brightness, 0, brightness));
    pixels.show();
    delay(50); // brightness is based on millis and rounded to int, so delay is just to relax CPU
    if (millis() > waitingMillis + 10000)
    {
      waitingMillis = millis();
      Serial.println("Waiting for lock on");
    }
  }
  if (gps.location.isUpdated())
  {
    // Serial.print(F("Satellite Count: "));
    // Serial.println(gps.satellites.value());
    // Serial.print(F("Speed MPH: "));
    // Serial.println(gps.speed.mph());
    // Serial.print(F("Altitude(m): "));
    // Serial.println(gps.altitude.meters());
    // Serial.print(F("Year: "));
    // Serial.println(gps.date.year());
    float lat = (float)gps.location.lat();
    float lng = (float)gps.location.lng();
    Serial.print(F("\n\nLat: "));
    Serial.print(lat, 6);
    Serial.print(F("\nLng: "));
    Serial.print(lng, 6);
    Serial.print(F("\nCourse(deg): "));
    Serial.print(gps.course.deg());
    // Serial.print(F("\nCourse human: "));
    // Serial.print(gps.cardinal(gps.course.value()));
    // Windmill windmillNearest = {-40, 175, "C sucks"};
    double nearestDistance = 9999999;
    int nearestIndex = 0;
    for (unsigned int i = 0; i < sizeof(windmills) / sizeof(windmills[0]); i++)
    {
      double distance = gps.distanceBetween(lat, lng, windmills[i].lat, windmills[i].lng);
      // Serial.print(windmills[i].name);
      // Serial.print(" ");
      // Serial.println(distance);
      if (distance < nearestDistance)
      {
        // Don't understand C enough to know why this won't compile
        // windmillNearest = windmills[i];
        nearestIndex = i;
        nearestDistance = distance;
      }
      // delay(100);
    }
    double nearestCourse = gps.courseTo(
        gps.location.lat(),
        gps.location.lng(),
        windmills[nearestIndex].lat,
        windmills[nearestIndex].lng);
    double bearing = nearestCourse - gps.course.deg();
    // // Keep it in the positive realm
    bearing = bearing < 0 ? 360 + bearing : bearing;
    Serial.print(F("\nnearestWindmill: "));
    Serial.print(windmills[nearestIndex].name);
    Serial.print(F("\nnearestDistance(m): "));
    Serial.print(nearestDistance);
    Serial.print(F("\nnearestCourse(deg): "));
    Serial.print(nearestCourse);
    Serial.print(F("\nbearing(deg): "));
    Serial.print(bearing);
    Serial.print(F("\n"));
    ringCompass(bearing, nearestDistance);
    delay(1000); // A nice big fat delay
  }
}