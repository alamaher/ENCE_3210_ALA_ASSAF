#define button 2 // digital macro
volatile int x[]= {1,2,3,4,5,6,7,8,9,10}; // input of random numbers
volatile int y[10]; // rsultant array
// isr routine 
volatile int trigger = 0; // this will be the variable i use to trigger the isr

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(button), isr_button, RISING); // the button has to be pressed becayse this line of codee attaches button  to the isr.
  // loop keeps going unless the value changes from 0; 
  while(true){
    if (!trigger == 0){
      break;
    }
  }

  for (int i = 0; i<10; i++){
    Serial.println(y[i]);

  

}

void loop() {
  // put your main code here, to run repeatedly:
  void isr_button(){
  for (int i = 0; i < 10; i++){
    y[i] = 2*x[i]-x[i-1];
  }
  trigger++;
}
