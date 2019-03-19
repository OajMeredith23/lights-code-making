// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
    #include <Wire.h>
    
    #include "RTClib.h"
    
    #if defined(ARDUINO_ARCH_SAMD)
    // for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
       #define Serial SerialUSB
    #endif
    
    RTC_DS1307 rtc;


//Sunrise-sunset 
    #include <Dusk2Dawn.h>
    int LON_latitude = 51.474454; //LONDON Coordinates
    int LON_longitude = 00.00000;


void setup() {
  // put your setup code here, to run once:
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

}

DateTime currentTime() {
    DateTime now = rtc.now();
    return now; 
}

void loop() {
    DateTime t = currentTime();

    
     //Three arguement are the chosen locations Longitude, latitude, and that locations difference from UTC
     //the Dusk2Dawn library incorrectly returns British Summer Time as 0 difference from UTC (it should be +1)...
     // ...this means between 27th October and 31st march the third parameter must be at -1, other times it should be 0 
     int UTC_difference = 0;
     if(t.day() <= 31 && t.month() <= 3){
        UTC_difference = -1;
     } 

     if(t.day() <= 27 && t.month() >= 10){
        UTC_difference = -1;
     } 
     
     Dusk2Dawn london(LON_latitude, LON_longitude, UTC_difference);
      // Available methods are sunrise() and sunset(). Arguments are year, month,
      // day, and if Daylight Saving Time is in effect.
//      int londonSunrise  = london.sunrise(t.year(), t.month(), t.day(), true);
      int londonSunrise  = london.sunrise(t.year(), t.month(), t.day(), true);
      int londonSunset   = london.sunset(t.year(), t.month(), t.day(), true);

        char time[6];
      Dusk2Dawn::min2str(time, londonSunset);
      Serial.println(time);
//      Serial.println((String) t.year() +  t.month() + t.day());
}
