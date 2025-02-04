#include <Arduino.h>
#include <math.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  float results[361];

  for ( int i = 0; i<=360; i ++){
    results[i]= cos(radians(i));
    Serial.println(results[i]);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
