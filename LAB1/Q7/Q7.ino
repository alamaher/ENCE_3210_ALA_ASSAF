#include <Arduino.h>
#define RED 5
#define GREEN 4
#define BLUE 12
#define BUTTON_1 2
#define BUTTON_2 3

// Initialize arrays
int array1[]={1,2,3,4,5,6,7,8,9,10};
int array2[]={1,2,3,4,5,6,7,8,9,10};
int counter = 0;
int* resultARR = (int*)malloc(10*sizeof(int));




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
  pinMode(BUTTON_1,INPUT);
  pinMode(BUTTON_2,INPUT);

  for ( int i= 0; i<10; i++){
    while(!digitalRead(BUTTON_2)| counter== 0){
      if (digitalRead(BUTTON_1)){
        delay(200);
        counter++;
      }
      if ( counter == 1){
        digitalWrite(RED,HIGH);
      }
      if (counter ==2){
        digitalWrite(RED,LOW);
        digitalWrite(GREEN,HIGH);
    }
      if (counter ==3){
        digitalWrite(GREEN,LOW);
        digitalWrite(BLUE,HIGH);
    }
      if (counter >3 | counter == 0){
        digitalWrite(GREEN,LOW);
        digitalWrite(BLUE,LOW);
        digitalWrite(RED,LOW);
        counter = 0;  
    }
  }
  delay(250);
  if (counter ==1 ){
    resultARR[i]= array1[i]+array2[i];
  }
  if (counter ==2){
    resultARR[i]=array1[i]-array2[i];
  }
  if (counter == 3){
    resultARR[i]= array1[i]*array2[i];
  }
  counter = 0;
  }
for ( int j = 0; j<10; j++){
  Serial.println(resultARR[j]);
}
delay(250);
digitalWrite(GREEN,HIGH);
digitalWrite(BLUE,HIGH);
digitalWrite(RED,HIGH);
free(resultARR);

}

void loop() {
  // put your main code here, to run repeatedly:

}
