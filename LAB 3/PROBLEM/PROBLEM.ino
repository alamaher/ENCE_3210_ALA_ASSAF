/// digital macros
#define SWITCH 2   // Button input pin
#define GREEN 4     // Green LED pin
#define RED 5     // Red LED pin

/// Define timer compare values
#define TIMER_A_VALUE 125   // Timer1 ; responsible for task 1: blinking green led 
#define TIMER_B_VALUE 6250  // Timer2 for task2: reading the input by the switch. if this equals 1, turn on/off based on intial state

/// Global volatile variables modified by interrupts
volatile int tickCounter = 0; // Counts timer ticks
volatile int timeElapsed = 0; // Tracks elapsed time in seconds

void setup() {
  /// Initialize serial communication at 9600 baud rate
  Serial.begin(9600);
  
  /// Configure pins as input/output
  pinMode(SWITCH, INPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);

  /// Disable interrupts while configuring timers
  noInterrupts();

  /// Configure Timer1 for red LED control
  TCCR1A = 0;     // Clear Timer1 control register A
  TCCR1B = 0;     // Clear Timer1 control register B
  OCR1A = TIMER_B_VALUE;  // Set Timer1 compare match threshold
  TCCR1B |= (1 << WGM12); // Set CTC (Clear Timer on Compare Match) mode
  TCCR1B |= (1 << CS12);  // Set prescaler to 256
  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare match interrupt

  /// Configure Timer0 for green LED blinking
  TCCR0A = 0;     // Clear Timer0 control register A
  TCCR0B = 0;     // Clear Timer0 control register B
  OCR0A = TIMER_A_VALUE; // Set Timer0 compare match threshold
  TCCR0A |= (1 << WGM01); // Set CTC mode
  TCCR0B |= (1 << CS02);  // Set prescaler to 1024
  TCCR0B |= (1 << CS00);
  TIMSK0 |= (1 << OCIE0A); // Enable Timer0 compare match interrupt

  /// Re-enable interrupts
  interrupts();
}

void loop() {
  /// Main task: Print "running" message every 3 seconds
  if (timeElapsed % 3 == 0) {
    Serial.println("running");

    /// Wait until `timeElapsed` is no longer a multiple of 3
    while (true) {
      if (timeElapsed % 3 != 0) {
        break;
      }
    }
  }
}

/// ISR function to control red LED based on button state
ISR(TIMER1_COMPA_vect) {
  if (digitalRead(SWITCH)) {
    digitalWrite(GREEM, LOW); // Ensure green LED is turned off
    digitalWrite(RED, HIGH); // Turn on red LED if button is pressed
  } else {
    digitalWrite(RED, LOW);  // Turn off red LED otherwise
  }
}
/// ISR function to control green LED blinking and time tracking
ISR(TIMER0_COMPA_vect) {
  tickCounter++; 

  /// 125 * 125 = 16M / 1024, meaning 1 second has passed when tickCounter reaches 125
  if (tickCounter == 125) {
    digitalWrite(GREEN, HIGH); // Blink green LED
    timeElapsed++; // Increment time tracker

    /// Reset time tracker after 100 seconds
    if (timeElapsed == 100) {
      timeElapsed = 0;
    }
    
    tickCounter = 0; // Reset tick counter
  }
}