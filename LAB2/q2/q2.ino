#include <Arduino.h> // including library
// setting macros
#define RED 5 
#define GREEN 4
#define BLUE 12
#define BUTTON 2
// intializing state of button; program runs normally until press. 
volatile bool press = false; 

void isr_button(){
  press = true;




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // initialization of LED and button state.
   press = false;
  digitalWrite(BLUE,LOW);
  digitalWrite(RED,LOW);
  digitalWrite(GREEN,LOW);
  
  // led are outputs
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
  pinMode(BUTTON,INPUT);  //button is input
  attachInterrupt(digitalPinToInterrupt(BUTTON), isr_button, RISING); // 
  //initialzing LED
  digitalWrite(RED,LOW);
  digitalWrite(GREEN,LOW);
  digitalWrite(BLUE,LOW);
  // ensures only activate when the button is press otherwise everything will run like normal
  while (true){
    if (press)){ 
      break;
    }
  }
}  
void loop() {
  // this while run completly fine as long as the button is not pressed/
  while (true){
    digitalWrite(BLUE,LOW);
    digitalWrite(RED,HIGH);
    delay(200);
    if (press){
      break;
    }
    digitalWrite(GREEN, HIGH);
    delay(500);
    if (press){
      break;
    }
    digitalWrite(BLUE, HIGH);
    delay(500);
    if (press){
      break;
    }
    digitalWrite(RED, LOW);
    delay(500);
    if ([press]){
      break;
    }
    digitalWrite(GREEN, LOW);
    delay(500);
    if (press){
      break;
    }
    digitalWrite(RED, HIGH);
    delay(500);
    if (press){
      break;
    }
    digitalWrite(BLUE, LOW);
    if (press){
      break;
    }
  }
}

    
