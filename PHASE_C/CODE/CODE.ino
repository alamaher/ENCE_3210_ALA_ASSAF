
/// Including necessary libraries

#include "I2Cdev.h" :// this will aloow for the communication of the I2C
#include "MPU6050.h"

/// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
/// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"  /*this will help in the communcation with the mpu 6050 and also the oled screen 
that will be used for display*/ 
#endif

#include "convert.h"

#include <SPI.h>
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <OneWire.h> // this will help in communication with temperature senseor
#include <DallasTemperature.h>
#include <PinChangeInterrupt.h>

// these are the different asssigment of macros
#define SCREEN_WIDTH 128 // OLED screen width in pixels
#define SCREEN_HEIGHT 32 // OLED screen height in pixels 
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C /// 0x3C for 128x32
#define LED 9
#define LED_BUTTON 6
#define ONOFF_BUTTON 2
#define LOCK_BUTTON 3
#define UNITS_BUTTON 4
#define TIMER1_VALUE 62500 // this timer will executed to record temperature every 100m.
#define UNIT_DELAY 1500
#define ONE_WIRE_BUS 8
#define OUTPUT_READABLE_ACCELGYRO

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // object with parameters//
OneWire oneWire(ONE_WIRE_BUS); // temperature sensor 
DallasTemperature sensors(&oneWire); 
MPU6050 accelgyro; //gyrospcope

// these are the acceleration valueas 
int16_t ax, ay, az; 
int16_t gx, gy, gz;
volatile unsigned long savedTime = 0; // variable to store value of last time LED button was pressed
const unsigned long givenDelay = 250; //delay for long press
///Initializing timing parameters by setting them to 0 initially
volatile unsigned long savedTime1 = 0; // laset time on/off was pressed
volatile unsigned long savedTime2 = 0;//  last time lock was pressed
volatile unsigned long savedTime3 =0; // last time Units button was pressed
volatile unsigned long unitTime = 0; // // Stores current time for Units button

volatile float currentTemp = 0; // current temperature reading


/// Boolean values storing state of system; values that are either true or false = 0 or 1.
bool lightToggle = 0; // LED state
bool onOffState = 0;// on or off
bool lockState = 0;// locked/unlocked 
bool unitsState = 0;// celius or F
bool unitButtonPressed = 0; // if unit button is pressed

/// enumerated array to help with setting the state of the LED
typedef enum {
  LED_ON,
  LED_OFF
} LightStatus;

LightStatus currentStatus = LED_OFF; // initial state led is off
LightStatus *current = &currentStatus; // instanitanous or current state of the LED 

/// Function which changes LED mode either on or off.
void toggleLightMode(LightStatus *choice) {
  switch (*choice) {
    case LED_ON:
      digitalWrite(LED, HIGH);
      if (lightToggle){
        *choice = LED_OFF;  // Ensure state change
        lightToggle = 0;
      }
      break;

    case LED_OFF:
      digitalWrite(LED, LOW);
      if (lightToggle) {
        *choice = LED_ON;
        lightToggle = 0;
      }
      break;
  }
}
// to determine if a long press is initialized; 1.5 s. if it is, coversion b/w C and F
void unitsStillPressed(){
/// we take the current time
  unitTime = millis();
  if (!digitalRead(UNITS_BUTTON)){ 
    savedTime3 = unitTime; // saves time if the button is not pressed.
  } else if ((digitalRead(UNITS_BUTTON)) && (unitTime - savedTime3 > UNIT_DELAY)){ //Unit delay is equated to 1.5 seconds.
  // update temperature
    unitsState = !unitsState; // unitsState is the state of the bool for C or F; this getes toggled.
    savedTime3 = unitTime; 
    currentTemp = getTemperature(); 
  }
}
/// gets temperature values.
float getTemperature(){
  sensors.requestTemperatures(); // requests temperature from Dallas temperature sensor
  float tempC = sensors.getTempCByIndex(0); // celsius
  float tempF = sensors.getTempFByIndex(0); // fahrenheit
/// depending on what the bool of unitsState returns; unit is selected
  if (!unitsState){
    if (tempC != DEVICE_DISCONNECTED_C) 
    {
      Serial.print("Temperature is: ");
      Serial.println(tempC);
      return tempC;

    }
    // if the sensor is disconnected or there is no connection.
    else
    {
      Serial.println("Error: Could not read temperature data");
    }
  } else{
    if (tempF != DEVICE_DISCONNECTED_F)
    {
      Serial.print("Temperature is: ");
      Serial.println(tempF);
      return tempF;
    }
    else
    {
      Serial.println("Error: Could not read temperature data");
    }
  }
}
/// Function which draws the current state of the system on OLED screen
void drawLock(){ 
/// Will rotate the display if it is flipped
  if (ax < 0){
    display.setRotation(2);
  } else{
    display.setRotation(0);
  }
///  draw  unlocked or locked on screen depending on lock state
// check lock state
  if (!lockState){
    display.drawBitmap(0,0,convert[1], SCREEN_WIDTH, SCREEN_HEIGHT, WHITE); //check the orientation of screen to display it properly
  } else{
    display.drawBitmap(0,0,convert[0], SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  }
/// check units state;
  if (!unitsState){ // checks which unit is chossen
    display.drawBitmap(0,0,convert[2],SCREEN_WIDTH,SCREEN_HEIGHT, WHITE);
  } else{
    display.drawBitmap(0,0,convert[3],SCREEN_WIDTH,SCREEN_HEIGHT, WHITE);
  }
  display.print(currentTemp);

}

void setup() {
///Initialization
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

  accelgyro.initialize();
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  Serial.begin(9600);
  sensors.begin();
  digitalWrite(LED,LOW);
  pinMode(LED, OUTPUT);
  pinMode(LED_BUTTON, INPUT);
/// Sets up 100 ms timer
  noInterrupts();
  // setting the resgisters of the i2c to 0
  TCCR1A = 0; // wire 1
  TCCR1B = 0;// wire 2
  OCR1A = TIMER1_VALUE;
  TCCR1B |= (1<<WGM12);
  TCCR1B |= (1<<CS12);
  TIMSK1 |= (1<<OCIE1A);
  interrupts();
///Attaches Isrs to all buttons 
  attachPCINT(digitalPinToPCINT(LED_BUTTON), ISR_LIGHT, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ONOFF_BUTTON), ISR_ONOFF, RISING);
  attachInterrupt(digitalPinToInterrupt(LOCK_BUTTON), ISR_LOCK, RISING);
  attachPCINT(digitalPinToPCINT(UNITS_BUTTON), ISR_UNITS, CHANGE);

}

void loop() {
///Every iteration of this loop, gyro information is collected to determine whether there is a flip in screen orientation
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
/// Initialization of screen every itereration of the loop
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(8,10);
/// Only do anything if onoffstate is true  meaning shut down when closed.
  if (!onOffState){
    lightToggle = 0;
    lockState = 0;
    currentStatus = LED_OFF;
    toggleLightMode(current);
  } else{
      ///update light state if button is pressed, check to see if units is pressed and draw to display if thermo pro is on.
    toggleLightMode(current);
    unitsStillPressed();
    drawLock();
  }
  display.display();

}

ISR(TIMER1_COMPA_vect){
    /// Every 100 ms, temperature is recorded iff the state is on and it is not locked.
  if (onOffState && !lockState){
    currentTemp = getTemperature();
  }
}

void ISR_LIGHT(){
    /// Isr for switching status of led. There is a cooldown period for debouncing.
  unsigned long time = millis();
  if (time - savedTime > givenDelay){
    lightToggle = 1;
  }
  savedTime = time;
}

void ISR_ONOFF(){
    /// This flips the onoff state of the system if on/off button is pressed. It uses time debounce
  unsigned long time1 = millis();
  if (time1 - savedTime1 > givenDelay){
    onOffState = !onOffState;
  }
  savedTime1 = time1;
}

void ISR_LOCK(){
/// This flips the lock state of the system if lock button is pressed. It also uses time debounce.
  unsigned long time2 = millis();
  if (time2 - savedTime2 > givenDelay){
    lockState = !lockState;
  }
  savedTime2 = time2;

}
///This resets savedTime3, which is not updated by unitsSTillPressed() until 1.5 seconds has elapsed, unless button is no longer held down.
void ISR_UNITS(){

  savedTime3 = millis();

}

