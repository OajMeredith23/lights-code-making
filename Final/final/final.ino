
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
    int spin, minute_color, green_output, blue_output;

    //Sunrise-sunset 
    #include <Dusk2Dawn.h>
    int LON_latitude = 51.474454; //LONDON Coordinates
    int LON_longitude = 00.00000;

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
        if(i == s){
          pixels.setPixelColor(i, pixels.Color(150,0,0));
        } else if (i == m){
          pixels.setPixelColor(i, pixels.Color(0,150,0));
        } else if(i == h){
            pixels.setPixelColor(i, pixels.Color(0,0,150));
          
        } else if(i == 0) {
            pixels.setPixelColor(i, pixels.Color(50,50,50));
            
          } else {
          pixels.setPixelColor(i, pixels.Color(0,0,0));
        }
    }
    
    
}

void dayNight(int secs, int mins, int hrs){

  if(hrs <= 12){
    
    light = map(hrs, 0, 12, 0, 100);
    
  } else {
    
    light = map(hrs % 12, 0, 12, 100, 0);
    
  }
  
//  Serial.println(light);
  light_pos = light_pos + 0.06;


  if(light_pos > NUMPIXELS) {
    light_pos = 0;
  }
 
  
  for(int i = 0; i < NUMPIXELS; i++){
    
    pixels.setBrightness(light);
    
    float distFrom_light_pos1 = abs(light_pos - i);
    float distFrom_light_pos2 = abs(light_pos - i + NUMPIXELS);
    float distFrom_light_pos3 = abs(light_pos - i - NUMPIXELS);

    float distFrom_light_pos = min(distFrom_light_pos1, min(distFrom_light_pos2, distFrom_light_pos3));

    int brightness = max(150 - distFrom_light_pos * 40, 0);

    int spin = map(brightness, 0, 150, 0, 200);
    

     minute_color = map(secs, 0, 60, 0, 255);

     
    if(secs < 30){
      green_output = map(minute_color, 0, 255 / 2, 0 , 255);
      
      blue_output = 255 - map(minute_color, 0, 255 / 2, 0 , 255);
    } else {
      green_output = (255*2) - map(minute_color, 0, 255 / 2, 0 , 255);
      
      blue_output = (-255) + map(minute_color, 0, 255 / 2, 0 , 255);
    }
    
    pixels.setPixelColor(i, pixels.Color(green_output, brightness, blue_output));
    
  }

  

  
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

void sunriseSunset(int yr, int mnth, int dy){
      Dusk2Dawn london(LON_latitude, LON_longitude, 0);
      // Available methods are sunrise() and sunset(). Arguments are year, month,
      // day, and if Daylight Saving Time is in effect.
      int londonSunrise  = london.sunrise(yr, mnth, dy, true);
      int londonSunset   = london.sunset(yr, mnth, dy, true);

        char time[6];
      Dusk2Dawn::min2str(time, londonSunrise);
      Serial.println(time);
}
void loop() {

//      Serial.println(counter);
      DateTime t = currentTime();

      
        if(counter == 0){

          changeModeAnimation();
//          dayNight(t.second(), t.minute(), t.hour());
          sunriseSunset(t.second(), t.minute(), t.hour());    
        
      } else {

       
          changeModeAnimation();
          neoPixelTime(t.second(), t.minute(), t.hour());
          
      }


      previousCounter = counter;


      Serial.println(t.minute());
      modeSwitch();
      pixels.show();
}
