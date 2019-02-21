#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUMPIXELS 24

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

RTC_DS1307 rtc;


void setup () {

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  pixels.begin();
}

DateTime currentTime() {
    DateTime now = rtc.now();
    return now;
    
}

void neoPixelTime(int secs, int mins, int hrs){
  int s = map(secs, 0, 60, 0, NUMPIXELS);
  int m = map(mins, 0, 60, 0, NUMPIXELS);
  int h = map(hrs % 12, 0, 23, 0, NUMPIXELS);

  for(int i = 0; i < NUMPIXELS; i++){
    if(i == s){
      pixels.setPixelColor(i, pixels.Color(150,0,0));
    } else if (i == m){
      pixels.setPixelColor(i, pixels.Color(0,150,0));
    } else if(i == h){
        pixels.setPixelColor(i, pixels.Color(0,0,150));
    } else {
      pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
  }
//  Serial.println(s);
  pixels.show();
}

void loop () {

   DateTime t = currentTime();
   neoPixelTime(t.second(), t.minute(), t.hour());
   
}
