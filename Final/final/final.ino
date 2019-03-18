
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

    // debounce
    int triggerDistance = 10;

    int transientState = 0;
    unsigned long debounceTime = 0;
    const unsigned long DEBOUNCE_DELAY = 50;
    
    
void setup() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    Serial.begin(9600); // Starts the serial communication


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
           counter +=1 ;
       }
       previousState = currentState;
     }
     
    Serial.println(counter);
}
    
void loop() {

      modeSwitch();
    
}
