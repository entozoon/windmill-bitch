#include <Arduino.h>
// #include <ESP8266WiFi.h>
#include "TinyGPS++.h"
#include "SoftwareSerial.h"
// SoftwareSerial gpsSerial(D0, D5); // TX, RX (on GPS module) maybe only certain pins?
// SoftwareSerial gpsSerial(4, 5);           // TX, RX (on GPS module)
SoftwareSerial serial_connection(3, 4);
TinyGPSPlus gps;
void setup()
{
  Serial.begin(115200);
  serial_connection.begin(9600);
  // Spews gargage in serial monitor after upload but.. should calm down and work; certainly after a reset. No idea why (might just be platformio)
  // WiFi.mode(WIFI_OFF); // Poss also sleep to save power (in case car runs USB overnight?)
  // delay(4000); // Pause for upload issues
  Serial.println(F("\nLet's go!"));
}
void loop()
{
  // if (serial_connection.available())
  // {
  //   Serial.write(serial_connection.read());
  // }
  while (serial_connection.available()) //While there are characters to come from the GPS
  {
    gps.encode(serial_connection.read()); //This feeds the serial NMEA data into the library one char at a time
  }
  if (gps.location.isUpdated()) //This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    Serial.println("Satellite Count:");
    Serial.println(gps.satellites.value());
    Serial.println("Latitude:");
    Serial.println(gps.location.lat(), 6);
    Serial.println("Longitude:");
    Serial.println(gps.location.lng(), 6);
    Serial.println("Speed MPH:");
    Serial.println(gps.speed.mph());
    Serial.println("Altitude Feet:");
    Serial.println(gps.altitude.feet());
    Serial.println("");
  }
}

// $GPRMC,224940.00,A,5318.29323,N,00122.51004,W,2.116,,100721,,,A*6D
// $GPVTG,,T,,M,2.116,N,3.919,K,A*25
// $GPGGA,224940.00,5318.29323,N,00122.51004,W,1,05,6.16,92.9,M,47.5,M,,*70
// $GPGSA,A,3,23,13,18,05,15,,,,,,,,8.32,6.16,5.60*02
// $GPGSV,2,1,06,05,59,219,17,13,74,286,26,14,42,126,,15,42,286,33*78
// $GPGSV,2,2,06,18,21,304,30,23,07,322,15*70
// $GPGLL,5318.28408,N,00122.53474,W,225459.00,A,A*7F
