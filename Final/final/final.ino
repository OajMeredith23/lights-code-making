
    // MODE SWITCHING DONE USING AN ULTRASONIC SENSOR
    // defines pin numbers
    const int trigPin = 12;
    const int echoPin = 11;
    // defines variables to calculate distance in the main function
    long duration;
    int distance;

    
    int inRange = LOW;
    // defines the mode
    int counter = 0;
    
    int currentState = 0;
    int previousState = 0;
    int previousCounter = 0;
//    int changing = false;

    // debounce function
    // triggerDistance defines how far from the sensor the user needs to place an object to trigger a mode change
    int triggerDistance = 10;
    
    int transientState = 0;
    unsigned long debounceTime = 0;
    const unsigned long DEBOUNCE_DELAY = 50;

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

//#define DEBUG 1
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
//            for(int i = 0; i < NUMPIXELS; i++){
//              pixels.setPixelColor(i, pixels.Color(150,0,0));
//              delay(0);
//            }
            counter = (counter + 1) % 2;
        }

       previousState = currentState;
     }
      
}


// Put the real time in its own function, it then returns it's data which can be passed into other functions
DateTime currentTime() {
    DateTime now = rtc.now();
    return now; 
}

// Display the current time in a readable format on the NeoPixel
void neoPixelTime(int secs, int mins, int hrs) /* Pass the seconds, minutes and hours in as parameters, arguments provided when the function is called in Void Loop()*/{

    
    pixels.setBrightness(140);

    // Map the times to the NeoPixel
    int s = map(secs, 0, 60, 0, NUMPIXELS);
    int m = map(mins, 0, 60, 0, NUMPIXELS);
    //"hrs % 12" converts the 24 hour time to 12 hour
    int h = map(hrs % 12, 0, 12, 0, NUMPIXELS);


    //Loop over all the LEDs, if their the same as any of the s, m, or h variables then light the appropriate LED with the appropriate colour
    for(int i = 0; i < NUMPIXELS; i++){
        if(i == 0 ){ // Give the the LED, at the equivalent point as the 12 hour mark a white LED, to indicate the top
          pixels.setPixelColor(i, pixels.Color(20,20,20));
//          If the hour is at one of the white lights, show the hour light over the white light
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
    
}


// Simple animation that provides visual feedback when the user changes modes
void changeModeAnimation(){

  // If the user has just changed mode
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
              // Increases the delay on each LED, gives the animation an ease-out effect
              delay(max(10, i * 3));
            }
              Serial.println("changing");
         }
}

void sunriseSunset(int yr, int mnth, int dy, int secs, int mins, int hrs){
      yr = yr;
      mnth = mnth;
      dy = dy;
      hrs = hrs;
      mins = mins;

      //Three arguement are the chosen locations Longitude, latitude, and that locations difference from UTC
     //the Dusk2Dawn library incorrectly returns British Summer Time as 0 difference from UTC (it should be +1)...
     // ...this means between 27th October and 31st March (GMT) the third parameter must be at -1, other times it should be 0 
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

     
      //A countdown to sunrise and sunset, if sunrise/sunset has passed it returns a negative number equal to the minutes since
      int sunriseCountdown = londonSunrise - ((hrs * 60) + mins);
      int sunsetCountdown = londonSunset - ((hrs * 60) + mins);
    
      pixels.setBrightness(50);

      
//    See what time of day it is, related to the sunrise and sunset time and return true for the appropriate time of day, false for the others
      int sunrising = sunriseCountdown < 15 && sunriseCountdown > -15 ? 1 : 0; 
      int daytime = sunriseCountdown < -15 && sunsetCountdown > 15 ? 1 : 0;
      int sunsetting = sunsetCountdown < 15 && sunsetCountdown > -15 ? 1 : 0;

      // Create a rotating fade animation, the number increases round to the length of NUMPIXELS, then resets, resulting in a continious rotation
      light_pos = light_pos + 0.6;
      
      if(light_pos > NUMPIXELS) {
        light_pos = 0;
      }
      
      for(int i = 0; i < NUMPIXELS; i++){

        // Using the light_pos variable, find the leds before and after them and return a decreased number based on it's distance from the main light_pos. This is used as the green value later on.
        float distFrom_light_pos1 = abs(light_pos - i);
        float distFrom_light_pos2 = abs(light_pos - i + NUMPIXELS);
        float distFrom_light_pos3 = abs(light_pos - i - NUMPIXELS);
        float distFrom_light_pos = min(distFrom_light_pos1, min(distFrom_light_pos2, distFrom_light_pos3));
        int brightness = max(150 - distFrom_light_pos * 40, 0);


        // Check to see which of the statements related to the time of day is true, and light the NeoPixel ring accordingly
        if(sunrising){

//          inside each if statement, the blue and red values are mapped at different rates, or to different values. This returns 
//          blue of daytime, purple for night, and fades between the two at sunrise and sunset
            red_output = map(sunriseCountdown, -15, 15, 100, 255);
            blue_output = map(sunriseCountdown, 15, -15, 100, 255);
            Serial.println("Sun is rising");
            
        } else if(daytime){
              // Creates a simple, subtle fade, here from blue, to blue-purple, a similar thing is done during day time. Because it looks pretty
             if(secs < 30){
                red_output = map(secs, 60, 0, 0, 25);
                
                blue_output =  map(secs, 0, 60, 150, 255);
              } else {
                red_output = map(secs, 0, 60, 0, 25);
                
                blue_output =  map(secs, 60, 0, 150, 255);
              }

              Serial.println("Sunny Day");
          
        } else if (sunsetting){
            red_output = map(sunsetCountdown, -15, -15, 100, 255);
            blue_output = map(sunsetCountdown, -15, 30, 100, 255);
          
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
        
#ifdef DEBUG
Serial.println((String)"Blue val:" + blue_output + " Red val: " + red_output + "  Mins until sunset: " + sunsetCountdown + "  Mins until sunrise: " + sunriseCountdown);
#endif
     
        pixels.setPixelColor(i, pixels.Color(red_output, brightness, blue_output));
      }
      
      
     
}
void loop() {

      DateTime t = currentTime();

      // Using the mode switch function (See the bottom of the document), check what the counter says, and run the appropriate block. 
      if(counter == 0){
          changeModeAnimation();
          // the times from the RTC our passed in as arguments to each function
          sunriseSunset(t.year(), t.month(), t.day(),t.second(), t.minute(), t.hour());    
      } else {
          changeModeAnimation();
          neoPixelTime(t.second(), t.minute(), t.hour());          
      }

      // We need to know the previous counter value for the change mode animation.
      previousCounter = counter;
      
//#ifdef DEBUG
Serial.println((String)"Time: " + t.hour() + ":" + t.minute() + ":" + t.second());
//#endif

      modeSwitch();
      pixels.show();
}
