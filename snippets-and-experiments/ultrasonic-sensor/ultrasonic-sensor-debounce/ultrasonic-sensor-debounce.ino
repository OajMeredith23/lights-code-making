    int a = 2;  //For displaying segment "a"
    int b = 3;  //For displaying segment "b"
    int c = 4;  //For displaying segment "c"
    int d = 5;  //For displaying segment "d"
    int e = 6;  //For displaying segment "e"
    int f = 8;  //For displaying segment "f"
    int g = 9;  //For displaying segment "g"

    int BUTTON = 10;
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
    int prevDistance = 0;

    int hover = 0;
    int transientState = 0;
    unsigned long debounceTime = 0;
    const unsigned long DEBOUNCE_DELAY = 100;
    
    
void setup() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    Serial.begin(9600); // Starts the serial communication

    pinMode(light, OUTPUT);
    
    pinMode(a, OUTPUT);  //A
    pinMode(b, OUTPUT);  //B
    pinMode(c, OUTPUT);  //C
    pinMode(d, OUTPUT);  //D
    pinMode(e, OUTPUT);  //E
    pinMode(f, OUTPUT);  //F
    pinMode(g, OUTPUT);  //G
}
    
void loop() {

      
      
     digitalWrite(trigPin, LOW); 
     delayMicroseconds(2); 
     digitalWrite(trigPin, HIGH);
     delayMicroseconds(10); 
     digitalWrite(trigPin, LOW);
     
     duration = pulseIn(echoPin, HIGH);
     distance = (duration/2) / 29.1;

    
      
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
          Serial.write('X');
        } else if (x == LOW && pressed == HIGH) {
          pressed = LOW;
          Serial.write('Y');
        }
      }


     
//      if(currentState == 1){
//         counter = (counter + 1) % 2;
//     }
//     previousState = currentState;
//   
//    if(counter == 1){
//      for(int i = 2; i < 10; i++){
//        
//        digitalWrite(light, HIGH);
//        
//        if(i == 6 || i == 8){
//          digitalWrite(i, HIGH);
//        } else {
//          digitalWrite(i, LOW);
//        }
//        
//      }
//    } else {
//
//      for(int i = 2; i < 10; i++){
//
//        digitalWrite(light, LOW);
//        if(i == 9){
//          digitalWrite(i, LOW);
//        } else {
//          digitalWrite(i, HIGH);
//        }
//        
//      }
//    }


//     Serial.println(counter);

      


      prevDistance = distance;
}
