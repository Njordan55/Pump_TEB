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
bool pumpState[4] = {false, false, false, false};

// pointer to the currently selected timing array for pumps
uint32_t *pump_timer = nullptr; // will point to one of the pumpTiming_* arrays
uint32_t pumpTimer_stop[] = {0, 0, 0, 0};
uint32_t ledTime = 0;
int current_option = 0;
uint32_t pumping_factor[4] = {0, 1, 2, 3}; // factor to adjust pumping times {no alcohol, light, medium, strong}
uint32_t pumpTiming_no_alcohol[] =     {pumping_factor[0] * PUMP_TIME_ALCOHOL, PUMP_TIME_LIQUID_A, PUMP_TIME_LIQUID_B, PUMP_TIME_LIQUID_C};
uint32_t pumpTiming_light_alcohol[] =  {pumping_factor[1] * PUMP_TIME_ALCOHOL, PUMP_TIME_LIQUID_A, PUMP_TIME_LIQUID_B, PUMP_TIME_LIQUID_C};
uint32_t pumpTiming_medium_alcohol[] = {pumping_factor[2] * PUMP_TIME_ALCOHOL, PUMP_TIME_LIQUID_A, PUMP_TIME_LIQUID_B, PUMP_TIME_LIQUID_C};
uint32_t pumpTiming_strong_alcohol[] = {pumping_factor[3] * PUMP_TIME_ALCOHOL, PUMP_TIME_LIQUID_A, PUMP_TIME_LIQUID_B, PUMP_TIME_LIQUID_C};
uint32_t totalTime = 0;
uint32_t ledFreq[] = {0, 1000, 500, 100, 0}; // LED status for {no alcohol, light, medium, strong, stop}

uint32_t now = 0;
uint32_t targetTime_pump[] = {0, 0, 0, 0}; // // Target timer for {PUMP_A, PUMP_B, PUMP_C, PUMP_D}
uint32_t targetTime_led = 0;
bool riched[] = {false, false, false, false};

void buttonUse(uint8_t button);
void timerUpdate();
bool timeReached_led();
void timeReached_pump();


void stop_All_Pumps();
void status_Led(uint8_t option_Drink);
void pumpState_onLed();
void setPumpState(int option);

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
  buttonPushed = (~previousInput) & newInput;
  if (buttonPushed != 0){
    buttonUse(buttonPushed);
    timerUpdate();
  }

  if (current_option != 0){
    timeReached_pump();
  }

 digitalWrite(RED, timeReached_led()); // Turn off LED by default
  
  // Drive outputs based on pumpState
  digitalWrite(PUMP_A, pumpState[0] ? HIGH : LOW);
  digitalWrite(PUMP_B, pumpState[1] ? HIGH : LOW);
  digitalWrite(PUMP_C, pumpState[2] ? HIGH : LOW);
  digitalWrite(PUMP_D, pumpState[3] ? HIGH : LOW);

  previousInput = newInput;
}

void buttonUse(uint8_t button){
  switch(button){
    case 1:
      pump_timer = pumpTiming_no_alcohol;
      ledTime = ledFreq[0];
      current_option = 1;
      setPumpState(current_option);
    break;
    case 2:
      pump_timer = pumpTiming_light_alcohol;
      ledTime = ledFreq[1];
      current_option = 2;
      setPumpState(current_option);
    break;
    case 4:
      pump_timer = pumpTiming_medium_alcohol;
      ledTime = ledFreq[2];
      current_option = 3;
      setPumpState(current_option);
    break;
    case 8:
      pump_timer = pumpTiming_strong_alcohol;
      ledTime = ledFreq[3];
      current_option = 4;
      setPumpState(current_option);
    break;
    case 16:
      pump_timer = pumpTimer_stop;
      ledTime = ledFreq[4];
      current_option = 0;
      stop_All_Pumps();
    break;
    default:
    break;
  }
}

void setPumpState(int option){
  switch(option){
    case 1:
      pumpState[0] = false; // Start Pump A
      pumpState[1] = true; // Start Pump B
      pumpState[2] = true; // Start Pump C
      pumpState[3] = true; // Start Pump D
    break;
    case 2:
      pumpState[0] = true; // Start Pump A
      pumpState[1] = true; // Start Pump B
      pumpState[2] = true; // Start Pump C
      pumpState[3] = true; // Start Pump D
    break;
    case 3:
      pumpState[0] = true; // Start Pump A
      pumpState[1] = true; // Start Pump B
      pumpState[2] = true; // Start Pump C
      pumpState[3] = true; // Start Pump D
    break;
    case 4:
      pumpState[0] = true; // Start Pump A
      pumpState[1] = true; // Start Pump B
      pumpState[2] = true; // Start Pump C
      pumpState[3] = true; // Start Pump D
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

void timeReached_pump(){
  for (int i = 0; i < 4; i++){
    if (riched[i] == false && now >= targetTime_pump[i]){
      riched[i] = true;
      // Stop the pump
      switch(i){
        case 0:
          pumpState[0] = false;
        break;
        case 1:
          pumpState[1] = false;
        break;
        case 2:
          pumpState[2] = false;
        break;
        case 3:
          pumpState[3] = false;
        break;
        default:
        break;
      }
    }
  }

  // Check if all pumps have reached their target time
  if (riched[0] && riched[1] && riched[2] && riched[3]){
    // Reset for next operation
    for (int i = 0; i < 4; i++){
      riched[i] = false;
    }
    pump_timer = nullptr; // Clear the pump timer pointer
  }
}

bool timeReached_led(){
  if (now >= targetTime_led){
    targetTime_led = now + ledTime; // update next target time
    return true;
  }else{
    return false;
  }
}


void stop_All_Pumps(){
  for (int i = 0; i < 4; i++){
    targetTime_pump[i] = now; // set all target times to now to stop pumps
  }
}

void status_Led(uint8_t option_Drink){
  switch(option_Drink){
    case 0:
      digitalWrite(RED, LOW); // LED off
    break;
    case 1:
      digitalWrite(RED, HIGH); // LED on
    break;
    case 2:
      // Blink LED
      if ((now / 500) % 2 == 0){
        digitalWrite(RED, HIGH);
      } else {
        digitalWrite(RED, LOW);
      }
    break;
    default:
      digitalWrite(RED, LOW); // LED off
    break;
  }
}

void pumpState_onLed(uint32_t blinky){
  if (blinky == 0){
    digitalWrite(RED, LOW); // LED off
  } else {
    // Blink LED
    if ((now / blinky) % 2 == 0){
      digitalWrite(RED, HIGH);
    } else {
      digitalWrite(RED, LOW);
    }
  }
}