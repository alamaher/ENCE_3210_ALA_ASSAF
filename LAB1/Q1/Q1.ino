
#include <Arduino.h>
int CalculateTwiceSquare( int num){
  return 2* (num*num);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  int results[512];

  for ( int i =0; i<=511;i++){
    results[i]= CalculateTwiceSquare(i);
    Serial.println(results[i]);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
