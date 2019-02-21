// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      24

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int col1 = 50;
int col2 = 50;
int col3 = 50;

const int BUTTON_A = 3;
const int BUTTON_B = 4;

int pressed_A = LOW;
int transientState_A = LOW;

int pressed_B = LOW;
int transientState_B = LOW;

unsigned long debounceTime_A = 0;
unsigned long debounceTime_B = 0;

const unsigned long DEBOUNCE_DELAY = 50;

int mode = 0;
int c;
int colors[] = {};
void setup() {
  pixels.begin();
  
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  Serial.begin(9600);

  
  
}

// Debouncing: reference: https://www.arduino.cc/en/Tutorial/Debounce

void loop() {

  digitalWrite(d,HIGH);
  
  if(mode == -1){
    mode = NUMPIXELS - 1;
  }
//  
  int x = digitalRead(BUTTON_A);

  // See if the button is still changing state; if so, nudge the record of the last time it changed:
  
  if (x != transientState_A) {
    debounceTime_A = millis();
  }

  transientState_A = x;

  /* Has the button been stable for a few milliseconds; if so, do the "pressed" vs current comparison
     to see whether it's an actual button change, and output serial data accordingly.
   */

   //BUTTON ONE
  if (millis() - debounceTime_A > DEBOUNCE_DELAY) {
    if (x == HIGH && pressed_A == LOW) {

      
      
      mode = (mode + 1) % 24;
      pressed_A = HIGH; 
    } else if (x == LOW && pressed_A == HIGH) {
      pressed_A = LOW;
      
    }
  }

  x = digitalRead(BUTTON_B);

  if (x != transientState_B) {
    debounceTime_B = millis();
  }

  transientState_B = x;

  //BUTTON TWO
  if (millis() - debounceTime_B > DEBOUNCE_DELAY) {
    if (x == HIGH && pressed_B == LOW) {
      mode -= 1;
      pressed_B = HIGH;   
  
      Serial.write('A');
    } else if (x == LOW && pressed_B == HIGH) {
      pressed_B = LOW;
    }
  }

 

  int col = map(mode, 0, NUMPIXELS, 0, 255);
  for(int i = 0; i < NUMPIXELS; i++){
    if(i == mode){
      pixels.setPixelColor(i, pixels.Color(col, col, col));
    } else {
      pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
  }

      
  
  pixels.show();
  Serial.println(mode);
}
