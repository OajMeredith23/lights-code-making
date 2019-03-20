
    // MODE SWITCHING DONE USING AN ULTRASONIC SENSOR
    int inRange = LOW;
    // defines pins numbers
    const int trigPin = 12;
    const int echoPin = 11;
    // defines variables
    long duration;
    int distance;

    // Set mode 

    int counter = 0;
    int mode = 0;
    int currentState = 0;
    int previousState = 0;
    int previousCounter = 0;
    int changing = false;
    // debounce
    int triggerDistance = 10;

    int transientState = 0;
    unsigned long debounceTime = 0;
    const unsigned long DEBOUNCE_DELAY = 50;

    bool fadeIn = true;

    //Change mode animation 

    bool reverse = false;

    //NEOPIXEL

    #include <Adafruit_NeoPixel.h>
    #ifdef __AVR__
      #include <avr/power.h>
    #endif

    // Which pin on the Arduino is connected to the NeoPixels?
    #define PIN            6
    
    // How many NeoPixels are attached to the Arduino?
    #define NUMPIXELS      24
    
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


    //RTC

    // Date and time functions using a DS1307 RTC connected via I2C and Wire lib
    #include <Wire.h>
    
    #include "RTClib.h"
    
    #if defined(ARDUINO_ARCH_SAMD)
    // for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
       #define Serial SerialUSB
    #endif
    
    RTC_DS1307 rtc;


    //Daylight mode 

    int light = 0;
    float light_pos = 0.0;   // The position of imaginary floating light source
    float light_pos2 = 0.0;
    int spin, minute_color, red_output, blue_output;

    //Sunrise-sunset 
    //the Dusk2Dawn library (https://github.com/dmkishi/Dusk2Dawn) is used to get the sunrise/sunset times from the hard coded location
    #include <Dusk2Dawn.h>
    int LON_latitude = 51.474454; //LONDON Coordinates
    int LON_longitude = 00.00000;
    int sunrise, sunset;

    // On the hour animation
void setup() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
 

    pixels.begin();

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

// Mode switch function
void modeSwitch(){

     
     
     digitalWrite(trigPin, LOW); 
     delayMicroseconds(2); 
     digitalWrite(trigPin, HIGH);
     delayMicroseconds(10); 
     digitalWrite(trigPin, LOW);

    
     duration = pulseIn(echoPin, HIGH);

     //Converts the return result from the ultrasonic sensor into a usable distance
     distance = duration*0.034/2;

     // return 1 if the ultrasonic sensor returns a distance less than the set trigger distance (see var at top of file for this distance)
     int x = distance < triggerDistance ? 1 : 0;
  
      // See if the sensor is still changing state; if so, nudge the record of the last time it changed:
      
      if (x != transientState) {
        debounceTime = millis();
      }
    
      transientState = x;
    
      /* Has the sensor been stable for a few milliseconds; if so, do the "inRange" vs current comparison
         to see whether it's an actual button change, and output serial data accordingly.
       */
      if (millis() - debounceTime > DEBOUNCE_DELAY) {
        if (x == HIGH && inRange == LOW) {
          inRange = HIGH;
          currentState = 1;
        } else if (x == LOW && inRange == HIGH) {
          inRange = LOW;
          currentState = 0;
        }
      }

    if(currentState != previousState){
      
        
        
        if(currentState == 1){
            for(int i = 0; i < NUMPIXELS; i++){
              pixels.setPixelColor(i, pixels.Color(150,0,0));
              delay(0);
            }
            counter = (counter + 1) % 2;
        }

       previousState = currentState;
     }
      
}


DateTime currentTime() {
    DateTime now = rtc.now();
    return now; 
}

void neoPixelTime(int secs, int mins, int hrs){

    pixels.setBrightness(140);
    int s = map(secs, 0, 60, 0, NUMPIXELS);
    int m = map(mins, 0, 60, 0, NUMPIXELS);
    int h = map(hrs % 12, 0, 12, 0, NUMPIXELS);
  
    for(int i = 0; i < NUMPIXELS; i++){
        if(i == 0){
          
          pixels.setPixelColor(i, pixels.Color(50,50,50));
          if(i == h){
            pixels.setPixelColor(i, pixels.Color(0,0,150));
          }
        } else if (i == s){
          pixels.setPixelColor(i, pixels.Color(150,0,0));
        } else if(i == m){
          pixels.setPixelColor(i, pixels.Color(0,150,0));
        } else if(i == h) {
          pixels.setPixelColor(i, pixels.Color(0,0,150));    
        } else {
          pixels.setPixelColor(i, pixels.Color(0,0,0));
        }
    }
    
//    Serial.println(mins);
}

void changeModeAnimation(){
  if(counter != previousCounter){
            
            for(int i = 0; i > NUMPIXELS + 1; i++){
              pixels.setPixelColor(i, pixels.Color(0, 0, 0));
              pixels.show();
            }

            for(int i = 0; i < 28; i++){


              int lightUp = map(i, 0, NUMPIXELS, 0, 70);
              
              if(i < NUMPIXELS / 2){
                pixels.setBrightness(lightUp);
              } else {
                pixels.setBrightness(70 - lightUp);
              }
              
              
              pixels.setPixelColor(i, pixels.Color(map(i, NUMPIXELS, 0, 0, 155), 0, map(i, 0, NUMPIXELS, 0, 155)));
              pixels.show();
              delay(max(10, i * 3));
              reverse = true;
            }
    
              Serial.println("changing");

            
         }
}

void sunriseSunset(int yr, int mnth, int dy, int secs, int mins, int hrs){

      //Three arguement are the chosen locations Longitude, latitude, and that locations difference from UTC
     //the Dusk2Dawn library incorrectly returns British Summer Time as 0 difference from UTC (it should be +1)...
     // ...this means between 27th October and 31st march the third parameter must be at -1, other times it should be 0 
     int UTC_difference = 0;

     //Check that the date is within GMT time, not BST
     if(dy <= 31 && mnth <= 3){
        UTC_difference = -1;
     } 

     if(dy <= 27 && mnth >= 10){
        UTC_difference = -1;
     } 

     //Pass the relevant location and UTC difference into the Dusk2Dawn library
     Dusk2Dawn london(LON_latitude, LON_longitude, UTC_difference);
      // Available methods are sunrise() and sunset(). Arguments are year, month,
      // day, and if Daylight Saving Time is in effect.
      // Time is returned in minutes elapsed since midnight. If no sunrises or
      // sunsets are expected, a "-1" is returned.
      int londonSunrise  = london.sunrise(yr, mnth, dy, true);
      int londonSunset   = london.sunset(yr, mnth, dy, true);

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
      
//      Serial.print((String)"Sunrise: " + sunrise);
//      Serial.print((String)" Usable numbers: " + sunrise_hour + "," + sunrise_minute);
//      Serial.print((String)" Sunset: " + sunset);
//      Serial.print((String)" Usable numbers: " + sunset_hour + "," + sunset_minute);
//      Serial.println();

      //A countdown to sunrise and sunset, if sunrise/sunset has passed it returns a negative number equal to the minutes since
      int sunriseCountdown = londonSunrise - ((hrs * 60) + mins);
      int sunsetCountdown = londonSunset - ((hrs * 60) + mins);


      light_pos = light_pos + 0.6;


      if(light_pos > NUMPIXELS) {
        light_pos = 0;
      }
     

      pixels.setBrightness(50);
      for(int i = 0; i < NUMPIXELS; i++){
        
        
        float distFrom_light_pos1 = abs(light_pos - i);
        float distFrom_light_pos2 = abs(light_pos - i + NUMPIXELS);
        float distFrom_light_pos3 = abs(light_pos - i - NUMPIXELS);
    
        float distFrom_light_pos = min(distFrom_light_pos1, min(distFrom_light_pos2, distFrom_light_pos3));
    
        int brightness = max(150 - distFrom_light_pos * 40, 0);

        minute_color = map(secs, 0, 60, 0, 255);

        
        int sunriseParam = sunriseCountdown;  // Change these variables to test the change of colour at different time of day
        int sunsetParam = sunsetCountdown;   // between 15 and -15 for each is at sunrise and sunset respectively
        
        int sunrising = sunriseParam < 15 && sunriseParam > -15 ? 1 : 0; 
        int daytime = sunriseParam < -15 && sunsetParam > 15 ? 1 : 0;
        int sunsetting = sunsetParam < 15 && sunsetParam > -15 ? 1 : 0;
//        int nighttime = sunsetParam < -15 && sunriseParam > 15 ? 1 : 0;

        
//        Serial.println((String)sunriseParam + " " + sunsetParam + " " + sunsetting);
        
        if(sunrising){
            red_output = map(sunriseParam, -15, 15, 100, 255);
            blue_output = map(sunriseParam, 15, -15, 100, 255);
            Serial.println("Sun is rising");
            
        } else if(daytime){
             if(secs < 30){
                red_output = map(secs, 60, 0, 0, 25);
                
                blue_output =  map(secs, 0, 60, 150, 255);
              } else {
                red_output = map(secs, 0, 60, 0, 25);
                
                blue_output =  map(secs, 60, 0, 150, 255);
              }
              
              Serial.println("Sunny Day");
          
        } else if (sunsetting){
            red_output = map(sunsetParam, -15, -15, 100, 255);
            blue_output = map(sunsetParam, -15, 30, 100, 255);
//            Serial.println("The suns getting low");
          
        } else if (!daytime){
           if(secs < 30){
                red_output = map(secs, 60, 0, 100, 255);
                
                blue_output =  map(secs, 0, 60, 100, 255);
              } else {
                red_output = map(secs, 0, 60, 100, 255);
                
                blue_output =  map(secs, 60, 0, 100, 255);
              }
              
          Serial.println("Pub?");
        }
        Serial.println((String)blue_output + " " + red_output + "  Mins until sunset: " + sunriseCountdown);
//Serial.println(daytime);
        pixels.setPixelColor(i, pixels.Color(red_output, brightness, blue_output));
      }
      
      
     
}
void loop() {

//      Serial.println(counter);
      DateTime t = currentTime();

      
        if(counter == 0){

          changeModeAnimation();
          sunriseSunset(t.year(), t.month(), t.day(),t.second(), t.minute(), t.hour());    
        
      } else {

       
          changeModeAnimation();
          neoPixelTime(t.second(), t.minute(), t.hour());
          
      }


      previousCounter = counter;

      Serial.println((String)"Time: " + t.hour() + ":" + t.minute() + ":" + t.second());
      modeSwitch();
      pixels.show();
}
