
    int pressed = LOW;
    // defines pins numbers
    const int trigPin = 12;
    const int echoPin = 11;
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
    #define PIN            6
    
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
}
    
void loop() {

      
      
     digitalWrite(trigPin, LOW); 
     delayMicroseconds(2); 
     digitalWrite(trigPin, HIGH);
     delayMicroseconds(10); 
     digitalWrite(trigPin, LOW);
     
     duration = pulseIn(echoPin, HIGH);
     distance = duration*0.034/2;
     
     
     int dist = map(distance, 40, 0, 0, NUMPIXELS);

     Serial.println(dist);
     for(int i = 0; i < NUMPIXELS; i++){
        if(i <= dist) {
          pixels.setPixelColor(i, pixels.Color(0, 100, 100));
        } else if (i > dist) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        }
     }
    
      
      

    pixels.show();
     
    
}
