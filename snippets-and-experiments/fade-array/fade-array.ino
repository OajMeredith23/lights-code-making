// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

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

int delayval = 500; // delay for half a second

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}


float light_pos = 0.0;   // The position of imaginary floating light source
 float light_pos2 = 0.0;  
void loop() {

  light_pos = light_pos + 0.06;


  if(light_pos > NUMPIXELS) {
    light_pos = 0;
  }
 
  
  for(int i=0;i<NUMPIXELS;i++){

    float distFrom_light_pos1 = abs(light_pos - i);
    float distFrom_light_pos2 = abs(light_pos - i + NUMPIXELS);
    float distFrom_light_pos3 = abs(light_pos - i - NUMPIXELS);

    float distFrom_light_pos = min(distFrom_light_pos1, min(distFrom_light_pos2, distFrom_light_pos3));
    
    int brightness = max(150 - distFrom_light_pos * 40, 0);

    
    pixels.setPixelColor(i, pixels.Color(0,brightness,0));

    

  }

  pixels.show();
}
