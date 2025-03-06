#include <Arduino.h>
#define RED 5
#define GREEN 4
#define BLUE 12
#define BUTTON_1 2
#define BUTTON_2 3


// Initialize arrays
int array1[]={1,2,3,4,5,6,7,8,9,10};
int array2[]={1,2,3,4,5,6,7,8,9,10};
volatile int mode = 0; // tracks the count
volatile int confirm = 0; // this will confirm the operatiob
int* resultARR = (int*)malloc(10*sizeof(int));
volatile unsigned long time1 = 0 ; ;//last time button pressed
volatile unsigned long givenDelay = 250; //delay


//q7


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
  pinMode(BUTTON_1,INPUT);
  pinMode(BUTTON_2,INPUT);

  // implenting the interuppts:
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), isr2, RISING);  // isr for button 1  
  attachInterrupt(digitalPinToInterrupt(BUTTON_2), isr1, RISING);  // is nfor button 2


// this loop will do the calculation and operations
  for ( int i= 0; i<10; i++){
    //ressetting these values before starting anything
    mode =0;
    confirm = 0;
    digitalWrite(BLUE,LOW);
    digitalWrite(GREEN,LOW)

  while(true)| mode== 0){ 
    if (mode == 0)){
      // to add
      digitalWrite(BLUE, LOW);
        digitalWrite(GREEN, LOW);
        digitalWrite(RED, HIGH);
// to subract
      } else if (mode == 1) { 
        digitalWrite(GREEN, HIGH);
        digitalWrite(RED, LOW);
        digitalWrite(BLUE, LOW);
        // multiply operation
      } else if (mode == 2) {  
        digitalWrite(GREEN, LOW);
        digitalWrite(BLUE, HIGH);
        digitalWrite(RED, LOW);
      } 

      /// The while loop exits only when BUTTON_2 is pressed (ISR updates confirm variable)
      if (confirm != 0) {
        break;
      }
    }

    /// Perform arithmetic operation based on selected mode
    if (mode == 0) {  // Addition
      resultArray[i] = firstArray[i] + secondArray[i];
    } else if (mode == 1) {  // Subtraction
      resultArray[i] = firstArray[i] - secondArray[i];
    } else {  // Multiplication
      resultArray[i] = firstArray[i] * secondArray[i];
    }

    delay(500);  // Delay before moving to the next element
  }

  /// Indicate completion by turning on all LEDs
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, HIGH);
  digitalWrite(BLUE, HIGH);
// prevent memory leaks
  free(resultArray);
}

void loop() {
}

void isr1() {
  unsigned long time = millis();  //current time in milliseconds
  if (time - time1 > givenDelay) {  
    if (mode == 2) {
      mode = 0;  // Reset mode to 0 after reaching 2
    } else {
      mode++;  // Increment mode (0 -> 1 -> 2)
    }
  }
  time1 = time;  // Update last button press time
}


void isr2() {
  confirm++;  // Set confirmation flag to exit the operation selection loop
}
        