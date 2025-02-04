#include <Arduino.h>
#define RED 5
#define GREEN 4
#define BLUE 12
#define BUTTON 2




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
  pinMode(BUTTON,INPUT);
  digitalWrite(RED,LOW);
  digitalWrite(GREEN,LOW);
  digitalWrite(BLUE,LOW);
  while (true){
    if (digitalRead(BUTTON)){
      break;
    }
  }
  while (true){
    digitalWrite(BLUE,LOW);
    digitalWrite(RED,HIGH);
    delay(200);
    if(digitalRead(BUTTON)){
      break;
    }
    digitalWrite(GREEN,HIGH);
    delay(200);
    if(digitalRead(BUTTON)){
      break;
    }
    digitalWrite(BLUE,HIGH);
    delay(200);
    if(digitalRead(BUTTON)){
      break;
    }
    digitalWrite(RED,LOW);
    delay(200);
    if(digitalRead(BUTTON)){
      break;
    }
    digitalWrite(GREEN,LOW);
    delay(200);
    if(digitalRead(BUTTON)){
      break;
    }
    digitalWrite(RED,HIGH);
    delay(200);
    if(digitalRead(BUTTON)){
      break;
    }


  }

  digitalWrite(BLUE,LOW);
  digitalWrite(RED,LOW);
  digitalWrite(GREEN,LOW);
  

}

void loop() {
  

}
