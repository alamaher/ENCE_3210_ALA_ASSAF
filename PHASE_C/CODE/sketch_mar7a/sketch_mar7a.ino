/// Including necessary libraries
#include "I2Cdev.h" // Allows for communication of the I2C
#include "MPU6050.h"

/// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
/// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"  // Helps in communication with the MPU6050 and OLED screen
#endif

#include "convert.h"

#include <SPI.h>
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <OneWire.h> // Communication with temperature sensor
#include <DallasTemperature.h>
#include <PinChangeInterrupt.h>

// Macro definitions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
#define LED 9
#define LED_BUTTON 6
#define ONOFF_BUTTON 2
#define LOCK_BUTTON 3
#define UNITS_BUTTON 4
#define TIMER1_VALUE 62500
#define UNIT_DELAY 1500
#define ONE_WIRE_BUS 8
#define OUTPUT_READABLE_ACCELGYRO

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

volatile unsigned long savedTime = 0;
const unsigned long givenDelay = 250;
volatile unsigned long savedTime1 = 0;
volatile unsigned long savedTime2 = 0;
volatile unsigned long savedTime3 = 0;
volatile unsigned long unitTime = 0;

volatile float currentTemp = 0;

bool lightToggle = 0;
bool onOffState = 0;
bool lockState = 0;
bool unitsState = 0;
bool unitButtonPressed = 0;

typedef enum {
  LED_ON,
  LED_OFF
} LightStatus;

LightStatus currentStatus = LED_OFF;
LightStatus *current = &currentStatus;

void toggleLightMode(LightStatus *choice) {
    switch (*choice) {
      case LED_ON:
        digitalWrite(LED, HIGH);
        *choice = LED_OFF;
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

void unitsStillPressed() {
    unitTime = millis();
    if (!digitalRead(UNITS_BUTTON)) { 
        savedTime3 = unitTime;
    } else if ((digitalRead(UNITS_BUTTON)) && (unitTime - savedTime3 > UNIT_DELAY)) {
        unitsState = !unitsState;
        savedTime3 = unitTime;
        currentTemp = getTemperature();
    }
}

float getTemperature() {
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);
    float tempF = sensors.getTempFByIndex(0);

    if (tempC == -127.0) {
        Serial.println("Error: Could not read temperature data");
        return -127.0;
    }
    return unitsState ? tempF : tempC;
}

void drawLock() {
    if (ax < 0) {
        display.setRotation(2);
    } else {
        display.setRotation(0);
    }

    if (!lockState) {
        display.drawBitmap(0, 0, convert[1], SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    } else {
        display.drawBitmap(0, 0, convert[0], SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    }

    if (!unitsState) {
        display.drawBitmap(0, 0, convert[2], SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    } else {
        display.drawBitmap(0, 0, convert[3], SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    }
    
    display.print(currentTemp);
}

void setup() {
    Wire.begin();
    accelgyro.initialize();

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    display.display();
    delay(2000);
    display.clearDisplay();
    
    Serial.begin(9600);
    sensors.begin();
    
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
    pinMode(LED_BUTTON, INPUT);

    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    OCR1A = TIMER1_VALUE;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12);
    TIMSK1 |= (1 << OCIE1A);
    interrupts();

    attachPCINT(digitalPinToPinChangeInterrupt(LED_BUTTON), ISR_LIGHT, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ONOFF_BUTTON), ISR_ONOFF, RISING);
    attachInterrupt(digitalPinToInterrupt(LOCK_BUTTON), ISR_LOCK, RISING);
    attachPCINT(digitalPinToPinChangeInterrupt(UNITS_BUTTON), ISR_UNITS, CHANGE);
}

void loop() {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(8, 10);

    if (!onOffState) {
        lightToggle = 0;
        lockState = 0;
        currentStatus = LED_OFF;
        toggleLightMode(current);
    } else {
        toggleLightMode(current);
        unitsStillPressed();
        drawLock();
    }

    display.display();
}

ISR(TIMER1_COMPA_vect) {
    if (onOffState && !lockState) {
        currentTemp = getTemperature();
    }
}

void ISR_LIGHT() {
    unsigned long time = millis();
    if (time - savedTime > givenDelay) {
        lightToggle = 1;
    }
    savedTime = time;
}

void ISR_ONOFF() {
    unsigned long time1 = millis();
    if (time1 - savedTime1 > givenDelay) {
        onOffState = !onOffState;
    }
    savedTime1 = time1;
}

void ISR_LOCK() {
    unsigned long time2 = millis();
    if (time2 - savedTime2 > givenDelay) {
        lockState = !lockState;
    }
    savedTime2 = time2;
}

void ISR_UNITS() {
    savedTime3 = millis();
}
