//q2

// panel always powers as long as its above 2.5; panel mode
// battery isused as back up: only used when battery power is more than panel and panel voltage is less than 2.5. 2.5 is minimum voltage for panel to work.
// charging: batttery<panel voltage.
#include "screen.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display configuration
#define DISPLAY_WIDTH 128 // OLED width in pixels
#define DISPLAY_HEIGHT 32 // OLED height in pixels
#define OLED_RESET_PIN -1 // Reset pin # (or -1 if using Arduino reset pin)
#define OLED_I2C_ADDRESS 0x3C ///< Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 oledDisplay(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, OLED_RESET_PIN);

// Define timer preload and sensor pins
#define TIMER_PRELOAD 62500
#define BATTERY_SENSOR A0
#define PANEL_SENSOR A1
#define MOSFET_BATTERY 12
#define MOSFET_PANEL 11
#define CHARGE_CONTROL 10

// Enum to represent different power states
typedef enum {
  POWER_BATTERY,
  POWER_PANEL,
  POWER_CHARGING
} PowerState;

PowerState currentPowerState = POWER_BATTERY;
PowerState *powerMode = &currentPowerState;

// Function to read battery voltage
double getBatteryVoltage() {
  return 5.0 * analogRead(BATTERY_SENSOR) / 1023;
}

// Function to read solar panel voltage
double getPanelVoltage() {
  return 5.0 * analogRead(PANEL_SENSOR) / 1023;
}

// Function to determine and switch power source based on voltage readings
void selectPowerSource(PowerState *mode) {
  double batteryVolt = getBatteryVoltage();
  double panelVolt = getPanelVoltage();

  switch (*mode) {
    case POWER_BATTERY:
      digitalWrite(MOSFET_PANEL, LOW);
      digitalWrite(MOSFET_BATTERY, HIGH);
      if (panelVolt >= 2.5 && batteryVolt > panelVolt) { // will use the battery
        *mode = POWER_PANEL;
      } else if (batteryVolt < panelVolt) { // this means that in all cases of battery output is less than panel, switch to chargin.
        *mode = POWER_CHARGING;
      }
      break;
    case POWER_PANEL: // state in which the panel powers arduino
      digitalWrite(MOSFET_BATTERY, LOW);
      digitalWrite(MOSFET_PANEL, HIGH);
      if (panelVolt < 2.5 && batteryVolt > panelVolt) {
        *mode = POWER_BATTERY; // go into power batter state ;if panel putput is less than 2.5 and battery outputer is more than panel output
      } else if (batteryVolt < panelVolt) {
        *mode = POWER_CHARGING; // changes the state(case)
      }
      break;
    case POWER_CHARGING:
      digitalWrite(MOSFET_BATTERY, LOW);
      digitalWrite(MOSFET_PANEL, HIGH);
      digitalWrite(CHARGE_CONTROL, HIGH);
      if (batteryVolt > 4.9 && panelVolt >= 2.5) { // if the solar poanel 
        *mode = POWER_PANEL;
      } else if (batteryVolt > 4.9 && panelVolt < 2.5) {
        *mode = POWER_BATTERY;
      }
      break;
  }
}

// Function to display charging animation
void showChargingAnimation(int frame) {
    oledDisplay.clearDisplay();
    oledDisplay.setCursor(8, 10);
    oledDisplay.drawBitmap(0, 0, screen[frame], DISPLAY_WIDTH, DISPLAY_HEIGHT, WHITE);
    oledDisplay.print("CHARGING");
    oledDisplay.display();
    delay(100);
}

// Function to update OLED display based on power source
void updateOLED(PowerState *mode) {
  oledDisplay.setTextColor(SSD1306_WHITE);
  oledDisplay.setTextSize(1);
  oledDisplay.setCursor(8, 10);
  if (*mode == POWER_BATTERY) {
    oledDisplay.drawBitmap(0, 0, screen[1], DISPLAY_WIDTH, DISPLAY_HEIGHT, WHITE);
    oledDisplay.print("BATTERY");
  } else if (*mode == POWER_PANEL) {
    oledDisplay.drawBitmap(0, 0, screen[0], DISPLAY_WIDTH, DISPLAY_HEIGHT, WHITE);
    oledDisplay.print("SOLAR PANEL");
  } else {
    showChargingAnimation(4);
    showChargingAnimation(3);
    showChargingAnimation(2);
    showChargingAnimation(1);
    delay(300);
  }
}

// Setup function to initialize hardware and OLED display
void setup() {
  Serial.begin(9600);
  if (!oledDisplay.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 initialization failed"));
    for (;;);
  }

  oledDisplay.display();
  delay(2000);
  oledDisplay.clearDisplay();

  // Set pin modes
  pinMode(BATTERY_SENSOR, INPUT);
  pinMode(PANEL_SENSOR, INPUT);
  pinMode(CHARGE_CONTROL, OUTPUT);
  pinMode(MOSFET_BATTERY, OUTPUT);
  pinMode(MOSFET_PANEL, OUTPUT);
}

// Main loop to handle power source selection and OLED updates
void loop() {
  oledDisplay.clearDisplay();
  selectPowerSource(powerMode);
  updateOLED(powerMode);
  oledDisplay.display();
}
