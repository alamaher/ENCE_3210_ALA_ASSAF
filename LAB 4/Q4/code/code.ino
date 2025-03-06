//q4

/// Define hardware connections and parameters
#define TIMER_THRESHOLD 255
#define TEMP_SENSOR_PIN A0
#define POWER_BUTTON 2
#define MODE_BUTTON 3
#define FAN_OUTPUT 9

volatile float totalTemperature = 0;
volatile int sampleCount = 0;
volatile float currentTemperature = 0;
volatile int timerOverflowCount = 0;
const unsigned long debounceDelay = 50;
volatile unsigned long lastPressTime1 = 0;
volatile unsigned long lastPressTime2 = 0;
volatile bool fanState = 0;
volatile int sensitivityLevel = 0;

float readTemperature() {
  
  volatile float rawValue = analogRead(TEMP_SENSOR_PIN);
  return rawValue / 10;
}

float calculateAverage(float tempSum) {
  return tempSum / 100;
}

void adjustFanSpeed(float temperature, int multiplier) {
  
  int fanIntensity = (int)((temperature / 102.3) * multiplier);
  analogWrite(FAN_OUTPUT, fanIntensity);
}

int selectSensitivity() {
  
  if (sensitivityLevel == 0) {
    return 50;
  } else if (sensitivityLevel == 1) {
    return 150;
  } else {
    return 255;
  }
}

void setup() {
  /// Initialize serial communication
  Serial.begin(9600);
  
  /// Attach interrupts to control buttons
  attachInterrupt(digitalPinToInterrupt(POWER_BUTTON), toggleFan, RISING);
  attachInterrupt(digitalPinToInterrupt(MODE_BUTTON), changeSensitivity, RISING);

  /// Configure timers and PWM for fan control
  noInterrupts();
  TCCR1A = (1 << COM1A1) | (1 << WGM10);
  TCCR1B = (1 << WGM12) | (1 << CS12);
  OCR1A = TIMER_THRESHOLD;

  /// Timer setup for periodic temperature sampling
  TCCR0A = 0;
  TCCR0B = 0;
  OCR0A = TIMER_THRESHOLD;
  TCCR0A = (1 << WGM01);
  TCCR0B = (1 << CS02) | (1 << CS00);
  TIMSK0 |= (1 << OCIE0A);
  interrupts();
}

void loop() {
  /// Control fan operation based on its state
  if (fanState) {
    adjustFanSpeed(currentTemperature, selectSensitivity());
  } else {
    analogWrite(FAN_OUTPUT, 0);
  }
}

ISR(TIMER0_COMPA_vect) {
  /// Timer interrupt for sampling temperature at regular intervals
  if (timerOverflowCount == 3) {
    if (sampleCount == 100) {
      currentTemperature = calculateAverage(totalTemperature);
      totalTemperature = 0;
      sampleCount = 0;
    }
    float temp = readTemperature();
    totalTemperature += temp;
    sampleCount++;
    timerOverflowCount = 0;
  }
  timerOverflowCount++;
}

void toggleFan() {
  /// Debounce the power button and toggle fan state
  unsigned long currentTime = millis();
  if (currentTime - lastPressTime1 > debounceDelay) {
    fanState = !fanState;
  }
  lastPressTime1 = currentTime;
}

void changeSensitivity() {
  /// Debounce the mode button and cycle through sensitivity levels
  unsigned long currentTime = millis();
  if (currentTime - lastPressTime2 > debounceDelay) {
    sensitivityLevel = (sensitivityLevel + 1) % 3;
  }
  lastPressTime2 = currentTime;
}

