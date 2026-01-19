#include "Pins.h"
#include "KeysDebounce.h"
#include "BusIn.h"

// Basic Pump timing definitions (in milliseconds) for different liquids
// LIQUID_A and etc and (...) are placeholders; replace with actual liquid names as needed
// The Pumping times is adjusted according to the time needed to pump 100 ml of each liquid
#define PUMP_TIME_ALCOHOL 500 // Pumping time for alcohol in milliseconds
#define PUMP_TIME_LIQUID_A 1500 // Pumping time for (...) in milliseconds
#define PUMP_TIME_LIQUID_B 2000 // Pumping time for (...) in milliseconds
#define PUMP_TIME_LIQUID_C 2500 // Pumping time for (...) in milliseconds

// Create a BusIn for option buttons (example: 4 option buttons)
const uint8_t optionPins[] = { BUTTON_OPT_1, BUTTON_OPT_2, BUTTON_OPT_3, BUTTON_OPT_4, BUTTON_STOP };
const size_t counter = sizeof(optionPins);
// usePullup = false because you said you use external resistors
// BusIn expects a small integer count; cast to uint8_t
BusIn keys(optionPins, (uint8_t)counter, true);

// Create a debounce object that reads the BusIn mask
KeysDebounce buttonDebounce(&keys, 80);
uint8_t newInput = 0;
uint8_t previousInput = 0;
uint8_t buttonPushed = 0;

uint32_t now = 0;
uint32_t pumptimer = 0;

void LED_togle(uint8_t ledPin);
void pump_togle(uint8_t pumpPin);

void setup() {
  // Initial PinMode configurations for LEDs
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);

  // Initial PinMode configurations for PUMOs
  pinMode(PUMP_A, OUTPUT);
  pinMode(PUMP_B, OUTPUT);
  pinMode(PUMP_C, OUTPUT);

  // Configure input pins for the key bus (keys.begin() sets INPUT or INPUT_PULLUP
  // depending on how the BusIn was constructed).
  keys.begin();

  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);

  digitalWrite(PUMP_A, LOW); // Turn off pump (assuming HIGH is on)
  digitalWrite(PUMP_B, LOW); // Turn off pump (assuming HIGH is on)
  digitalWrite(PUMP_C, LOW); // Turn off pump (assuming HIGH is on)
}

void loop() {
  // Use the debounced read (returns a bitmask for the bus)
  newInput = buttonDebounce.read();
  now = millis();

  // Detect rising edges for the bus: bits that went from 0 -> 1
  buttonPushed = (~previousInput) & newInput;
  if (buttonPushed){
    switch(buttonPushed){
      case 0x01: // BUTTON_OPT_1 pressed
        LED_togle(GREEN);
        pump_togle(PUMP_A);
        break;
      case 0x02: // BUTTON_OPT_2 pressed
        LED_togle(YELLOW);
        pump_togle(PUMP_B);
        break;
      case 0x04: // BUTTON_OPT_3 pressed
        LED_togle(BLUE);
        pump_togle(PUMP_C);
        break;
      case 0x08: // BUTTON_OPT_4 pressed
        LED_togle(RED);
        break;
        default:
        break;
    }
  //  pumptimer = now + PUMP_TIME_LIQUID_B;
  //  digitalWrite(PUMP_A, HIGH);
  }

//  if (now >= pumptimer){
//  digitalWrite(PUMP_A, LOW);
//  }

  previousInput = newInput;
}

void LED_togle(uint8_t ledPin){
  bool currentState = digitalRead(ledPin);
  digitalWrite(ledPin, !currentState);
}

void pump_togle(uint8_t pumpPin){
  bool currentState = digitalRead(pumpPin);
  digitalWrite(pumpPin, !currentState);
}
void timePumping(){

}