#include <Arduino.h>
#define ARR_SIZE 50
int dataArray[ARR_SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50};


void findSecondMax(){
  int max = -32768 ;// smallest integer value for Arduino
  int secondMax =  -32768;
  for ( int i =0; i<ARR_SIZE; i++){
    if( dataArray[i]>max){
      secondMax = max;
      max= dataArray[i];
    } else if ( dataArray[i]>secondMax && dataArray[i] < max){
      secondMax = dataArray[i];
    }

  }
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  findSecondMax();

  Serial.println("Array: ");
  for (int i = 0; i<ARR_SIZE; i++){
    Serial.print(dataArray[i]);
    Serial.print(i<ARR_SIZE-1?"," : "\n");
  }
  
}
Serial.print("Second Max Value: ");
Serial.println(secondMax);


void loop() {
  // put your main code here, to run repeatedly:

}
