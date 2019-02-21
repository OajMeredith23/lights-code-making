
    int pressed = LOW;
    // defines pins numbers
    const int trigPin = 11;
    const int echoPin = 12;
    // defines variables
    long duration;
    int distance;

    int light = 13;

    // Set mode 

    int counter = 0;
    int currentState = 0;
    int previousState = 0;

    // debounce
    int triggerDistance = 10;

    int transientState = 0;
    unsigned long debounceTime = 0;
    const unsigned long DEBOUNCE_DELAY = 50;
    
    // NeoPixel 

    #include <Adafruit_NeoPixel.h>
    #ifdef __AVR__
    #include <avr/power.h>
    #endif
    
    // Which pin on the Arduino is connected to the NeoPixels?
    // On a Trinket or Gemma we suggest changing this to 1
    #define PIN            7
    
    // How many NeoPixels are attached to the Arduino?
    #define NUMPIXELS      24
    
    // When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
    // Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
    // example for more information on possible values.
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    Serial.begin(9600); // Starts the serial communication

    pinMode(light, OUTPUT);

    for(int i = 2; i < 10; i++){
      pinMode(i, OUTPUT);
    }

    pixels.begin();
}
    
void loop() {

      
      
     digitalWrite(trigPin, LOW); 
     delayMicroseconds(2); 
     digitalWrite(trigPin, HIGH);
     delayMicroseconds(10); 
     digitalWrite(trigPin, LOW);
     
     duration = pulseIn(echoPin, HIGH);
     distance = duration*0.034/2;

    
      
      int x = distance < triggerDistance ? 1 : 0;
  
      // See if the button is still changing state; if so, nudge the record of the last time it changed:
      
      if (x != transientState) {
        debounceTime = millis();
      }
    
      transientState = x;
    
      /* Has the button been stable for a few milliseconds; if so, do the "pressed" vs current comparison
         to see whether it's an actual button change, and output serial data accordingly.
       */
      if (millis() - debounceTime > DEBOUNCE_DELAY) {
        if (x == HIGH && pressed == LOW) {
          pressed = HIGH;
          currentState = 1;
        } else if (x == LOW && pressed == HIGH) {
          pressed = LOW;
          currentState = 0;
        }
      }


    if(currentState != previousState){
        if(currentState == 1){
           counter = (counter + 1) % 2;
       }
       previousState = currentState;
     }
     
    if(counter == 1){
      digitalWrite(light, HIGH);

      for(int i = 2; i < 10; i++){

      for(int i = 0; i < NUMPIXELS; i++){
        pixels.setPixelColor(i, pixels.Color(255, 255, 0));
      }
        digitalWrite(light, HIGH);
        if(i == 6 || i == 8){
          digitalWrite(i, HIGH);
        } else {
          digitalWrite(i, LOW);
        }
        
      }
      
    } else { 
      for(int i = 0; i < NUMPIXELS; i++){
        pixels.setPixelColor(i, pixels.Color(0, 255, 255));
      }
      
      digitalWrite(light, LOW);

      for(int i = 2; i < 10; i++){

        digitalWrite(light, LOW);
        if(i == 9){
          digitalWrite(i, LOW);
        } else {
          digitalWrite(i, HIGH);
        }
        
      }
      
    }


    pixels.show();
     
    
}
