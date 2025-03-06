//question 1
#include <math.h>
// macros
#define BITSIZE 14// 2^14 = 16384. this is related to v(ref).  if v(in) is 3, then equated to 16384.

float Vref = 3; // reference voltage. can go from 0-3 volts. 
float Vin = 0.42; // input voltage that will be coverted
float final = 0; // this will be store the final value in binary
int bitval; 
int comparison[BITSIZE] = {0};
int bits[BITSIZE];
float v_step; // voltage step per bit

uint16_t new_ref(int arr[],int size){
  uint16_t result = 0; 
  for (int i = 0; i < size; i++){
    int power = size-1-i;
    double rawfactor = pow(2,power) + 0.1;
    uint16_t factor = (uint16_t)rawfactor;
    result += ((uint16_t)arr[i])*factor;
    // Serial.println(result);
  }
  return result;
}


void setup() {
  Serial.begin(9600);
  v_step = Vref/(pow(2,BITSIZE));

  for (int count = 0; count < BITSIZE; count++) {
    comparison[count] = 1;
    Vref = v_step*new_ref(comparison,BITSIZE);
    Serial.println(Vref);
    if (Vin >= Vref) {
      bitval = 1;
    } else {
      bitval = 0;
      comparison[count] = 0;
    }

    bits[count] = bitval;
    final += 2 * Vref * bitval; //
  }

}

void loop() {
    // Infinite loop
    while (1);
}
