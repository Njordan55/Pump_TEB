#include "Pins.h"
#include "KeysDebounce.h"
#include "BusIn.h"

// Create a BusIn for option buttons (example: 4 option buttons)
const uint8_t optionPins[] = { BUTTON_OPT_1, BUTTON_OPT_2, BUTTON_OPT_3, BUTTON_OPT_4, BUTTON_STOP };
const size_t counter = sizeof(optionPins) / sizeof(optionPins[0]);
// usePullup = false because you said you use external resistors
// BusIn expects a small integer count; cast to uint8_t
BusIn keys(optionPins, (uint8_t)counter, false);
// Create a debounce object that reads the BusIn mask
KeysDebounce buttonDebounce(&keys, 80);
uint8_t newInput = 0;
uint8_t previousInput = 0;
uint8_t buttonPushed = 0;
// Track pump/LED state and toggle on each button press
bool pumpState = false;

// pointer to the currently selected timing array for pumps
uint32_t *pump_timer = nullptr; // will point to one of the pumpTiming_* arrays
uint32_t pumpTimer_stop[] = {0, 0, 0, 0};
uint32_t pumpTiming_no_alcohol[] =     {0, 1000, 1500, 2000};
uint32_t pumpTiming_light_alcohol[] =  {500, 1000, 1500, 2000};
uint32_t pumpTiming_medium_alcohol[] = {1000, 1000, 1500, 2000};
uint32_t pumpTiming_strong_alcohol[] = {2000, 1000, 1500, 2000};
uint32_t totalTime = 0;

uint32_t now = 0;
uint32_t targetTime_pump[] = {0, 0, 0, 0}; // // Target timer for {PUMP_A, PUMP_B, PUMP_C, PUMP_D}
bool riched[] = {false, false, false, false};

void buttonthreat(uint8_t button);
void timerUpdate();
void timeReached();

void setup() {
  pinMode(PUMP_A, OUTPUT);
  pinMode(PUMP_B, OUTPUT);
  pinMode(PUMP_C, OUTPUT);
  pinMode(PUMP_D, OUTPUT);
  pinMode(RED, OUTPUT);
  // Configure input pins for the key bus (keys.begin() sets INPUT or INPUT_PULLUP
  // depending on how the BusIn was constructed).
  keys.begin();

  digitalWrite(RED, LOW);
  digitalWrite(PUMP_A, LOW); // Turn off pump (assuming HIGH is on)
  digitalWrite(PUMP_B, LOW); // Turn off pump (assuming HIGH is on)
  digitalWrite(PUMP_C, LOW); // Turn off pump (assuming HIGH is on)
  digitalWrite(PUMP_D, LOW); // Turn off pump (assuming HIGH is on)
}

void loop() {
  // Use the debounced read (returns a bitmask for the bus)
  newInput = buttonDebounce.read();
  now = millis();

  // Detect rising edges for the bus: bits that went from 0 -> 1
  uint8_t rising = (~previousInput) & newInput;
  buttonPushed = rising; // mask of buttons newly pressed
  buttonthreat(buttonPushed);
  timerUpdate();
  timeReached();

  // Drive outputs based on pumpState
  digitalWrite(PUMP_A, pumpState ? HIGH : LOW);
  digitalWrite(PUMP_B, pumpState ? HIGH : LOW);
  digitalWrite(PUMP_C, pumpState ? HIGH : LOW);
  digitalWrite(PUMP_D, pumpState ? HIGH : LOW);

  previousInput = newInput;
}

void buttonthreat(uint8_t button){
  switch(button){
    case 1:
      pump_timer = pumpTiming_no_alcohol;
    break;
    case 2:
      pump_timer = pumpTiming_light_alcohol;
    break;
    case 4:
      pump_timer = pumpTiming_medium_alcohol;
    break;
    case 8:
      pump_timer = pumpTiming_strong_alcohol;
    break;
    case 16:
      pump_timer = pumpTimer_stop;
    break;
    default:
    break;
  }
}

void timerUpdate(){
  if (pump_timer == nullptr) return;
  for (int i = 0; i < 4; i++){
    targetTime_pump[i] = pump_timer[i] + now;
  }
}

void timeReached(){
  for (int i = 0; i < 4; i++){
    riched[i] = (now >= targetTime_pump[i]);
  }
}