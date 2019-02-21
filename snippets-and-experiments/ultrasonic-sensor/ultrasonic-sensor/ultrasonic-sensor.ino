    int a = 2;  //For displaying segment "a"
    int b = 3;  //For displaying segment "b"
    int c = 4;  //For displaying segment "c"
    int d = 5;  //For displaying segment "d"
    int e = 6;  //For displaying segment "e"
    int f = 8;  //For displaying segment "f"
    int g = 9;  //For displaying segment "g"

    
    // defines pins numbers
    const int trigPin = 11;
    const int echoPin = 12;
    // defines variables
    long duration;
    int distance;
    

    // Set mode 

    int counter = 0;
   int currentState = 0;
   int previousState = 0;
    
void setup() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    Serial.begin(9600); // Starts the serial communication
    
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
     
     if (distance <= 10){
        currentState = 1;
     }
     else {
         currentState = 0;
     }
     
     if(currentState != previousState){
        if(currentState == 1){
           counter = (counter + 1) % 2;
       }
       previousState = currentState;
     }

    if(counter == 1){
      for(int i = 2; i < 10; i++){
        if(i == 6 || i == 8){
          digitalWrite(i, HIGH);
        } else {
          digitalWrite(i, LOW);
        }
        
      }
    } else {

      for(int i = 2; i < 10; i++){
        if(i == 9){
          digitalWrite(i, LOW);
        } else {
          digitalWrite(i, HIGH);
        }
        
      }
    }
     

     Serial.println(counter);

}
