//problem 1
// Digitial Pin Connections
#define BUTTON1 2
#define BUTTON2 3
#define RED 5
#define BLUE 6
#define GREEN 4
// last time button was pressed
volatile unsigned long time1 = 0; //initialized as 0
const unsigned long long_press = 250; // this will be the delay time
int counter1 = 0; // counter for how many times button_1 is pressed
int counter2 = 0; // same but for button2





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //LED ARE OUTPUTS
  pinMode(RED, OUTPUT);
  pinMoDE(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  //buttons are inputs
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  // This is attaching the ISR to the buttons
  attachInterrupt(digitalPinToInterrupt(BUTTON1), ISR_Button1, RISING); // for button1
  attachInterrupt(digitalPinToInterrupt(BUTTON2), ISR_Button2, RISING); // for button2




}

void loop() {
  // put your main code here, to run repeatedly:
  // if button1 is pressed more than button 2 ; greenn led tur on
  if (counter1 > counter2){
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, LOW);
    digitalWrite(GREEN, HIGH);
    // if counter 2 is greater than counter 1 then red is on
  } 
  else if (counter2 > counter1){
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, HIGH); // Red LED ON
  } // if both are equal, then blue is on
  else {
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, HIGH); // tuns blue on

}
// ISR for button1 
void isr_button1(){
  unsigned long time = millis(); // this function will return current time
  //// if difference between current time and last time button was pressed is more than 250 ms, detect a press.
  if (time - time1 > long_press){ 
    counter1++; // add 1 copunt to counter 1
    Serial.print("Number of times pressing first button: ");
    Serial.println(counter1);
    Serial.print("Number of times pressing second button: ");
    Serial.println(counter2);
  }
  time1 = time; // ensures the time is updated to the time for last press.
}
// same for botton 2
void isr_buttontwo(){
  unsigned long time = millis();
  if (time - time1 > long_press){
    buttonTwoCounter++;
    Serial.print("Number of times pressing first button: ");
    Serial.println(counter1);
    Serial.print("Number of times pressing second button: ");
    Serial.println(counter2);
  }
  time1 = time;
  
}
