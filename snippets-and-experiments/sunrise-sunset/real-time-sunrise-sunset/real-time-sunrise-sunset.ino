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
    //Puts the time in an easily accessible variable
    DateTime now = rtc.now();
    return now; 
}

void loop() {
    DateTime t = currentTime();

    
     //Three arguement are the chosen locations Longitude, latitude, and that locations difference from UTC
     //the Dusk2Dawn library incorrectly returns British Summer Time as 0 difference from UTC (it should be +1)...
     // ...this means between 27th October and 31st march the third parameter must be at -1, other times it should be 0 
     int UTC_difference = 0;

     //Check that the date is within GMT time, not BST
     if(t.day() <= 31 && t.month() <= 3){
        UTC_difference = -1;
     } 

     if(t.day() <= 27 && t.month() >= 10){
        UTC_difference = -1;
     } 

     //Pass the relevant location and UTC difference into the Dusk2Dawn library
     Dusk2Dawn london(LON_latitude, LON_longitude, UTC_difference);
      // Available methods are sunrise() and sunset(). Arguments are year, month,
      // day, and if Daylight Saving Time is in effect.
      
      int londonSunrise  = london.sunrise(t.year(), t.month(), t.day(), true);
      int londonSunset   = london.sunset(t.year(), t.month(), t.day(), true);

      //The sunrise/sunset times returned as a character array
      char sunset[6];
      char sunrise[6];
      Dusk2Dawn::min2str(sunset, londonSunset);
      Dusk2Dawn::min2str(sunrise, londonSunrise);

      
      /* Because the sunset times are returned as, for example '18:10' it's only possible to perform maths using their ASCII codes
       *  this wouldn't have been reasonable to using these numbers for NeoPixel display, the below converts them to usable numbers
       */

      //Convert the ASCII code of each digit to it's integer
      String sunset_hour_num1 = String((sunset[0]+0) - 48);
      String sunset_hour_num2 = String((sunset[1]+0) - 48);
      //Cocatanate these into a string
      String sunset_hour_str = sunset_hour_num1 + sunset_hour_num2;
      //Convert the string to a number
      int sunset_hour = sunset_hour_str.toInt();

      String sunset_minute_num1 = String((sunset[3]+0) - 48);
      String sunset_minute_num2 = String((sunset[4]+0) - 48);
      String sunset_minute_str = sunset_minute_num1 + sunset_minute_num2;
      int sunset_minute = sunset_minute_str.toInt();

      String sunrise_hour_num1 = String((sunrise[0]+0) - 48);
      String sunrise_hour_num2 = String((sunrise[1]+0) - 48);
      String sunrise_hour_str = sunrise_hour_num1 + sunrise_hour_num2;
      int sunrise_hour = sunrise_hour_str.toInt();

      String sunrise_minute_num1 = String((sunrise[3]+0) - 48);
      String sunrise_minute_num2 = String((sunrise[4]+0) - 48);
      String sunrise_minute_str = sunrise_minute_num1 + sunrise_minute_num2;
      int sunrise_minute = sunrise_minute_str.toInt();
      
      Serial.print((String)"Sunrise: " + sunrise);
      Serial.print((String)" Usable numbers: " + sunrise_hour + "," + sunrise_minute);
      Serial.print((String)" Sunset: " + sunset);
      Serial.print((String)" Usable numbers: " + sunset_hour + "," + sunset_minute);
      Serial.println();
      
      
}
