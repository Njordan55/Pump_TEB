#include "Parameter.h"

void outputPinsState();
void outputPinsModus();
void LED_togle(uint8_t option);
void pump_togle(uint8_t pumpPin);
// Recompute derived timing values when 'factor' changes
void updatePumpTiming();
void timer_to_zero();


void setup() {
  // Set pin modes for output pins
  outputPinsModus();

  // Initialize output pins to known state
  outputPinsState();

  // Configure input pins for the key bus (keys.begin() sets INPUT or INPUT_PULLUP
  // depending on how the BusIn was constructed).
  keys.begin();
}

void loop() {
  // Use the debounced read (returns a bitmask for the bus)
  newInput = buttonDebounce.read();
  now = millis();

  // Detect rising edges for the bus: bits that went from 0 -> 1
  buttonPushed = (~previousInput) & newInput;
  if (buttonPushed && !buttonFlag){
    buttonFlag = true;
  }
  //  pumptimer = now + PUMP_TIME_LIQUID_B;
  //  digitalWrite(PUMP_A, HIGH);

  if (buttonFlag && (pumpingOption == 0)){
    for (uint8_t i = 0; i < (counter-1); i++){
      if(buttonPushed & (1 << i)){
        pumpingOption = i + 1;
        break;
      }
    }
    factor = pumpingOption - 1; // 0 = no alcohol, 1 = low, 2 = medium, 3 = high
    updatePumpTiming();         // refresh timing values based on new factor
    LED_togle(pumpingOption);
  }

  if (pumpingOption != 0){
    if (counter != lastCounter){
      pumptimer = now + pumpTiming[counter];
      lastCounter = counter;
      switch (counter){
        case 0:
          pump_togle(pumpPin[counter]);
        break;
        case 1:
          pump_togle(pumpPin[counter]);
        break;
        case 2:
          pump_togle(pumpPin[counter]);
        break;
      }
    }
    if (now >= pumptimer){
      switch(counter){
        case 0:
          pump_togle(pumpPin[counter]);
        break;
        case 1:
          pump_togle(pumpPin[counter]);
        break;
        case 2:
          pump_togle(pumpPin[counter]);
        break;
      }
      counter++;
    }
  }
  if (counter >= timeCounter){
    pumpingOption = 0;
    counter = 0;
    buttonFlag = false;
    lastCounter = -1;
    timer_to_zero();
  }


//  if (now >= pumptimer){
//  digitalWrite(PUMP_A, LOW);
//  }

  previousInput = newInput;
}

void LED_togle(uint8_t option){
  bool currentState = false;
  for (uint8_t i = 0; i < option; i++){
    currentState = digitalRead(ledPin[i]);
    digitalWrite(ledPin[i] , !currentState);
  }
}

void pump_togle(uint8_t pumpPin){
  bool currentState = digitalRead(pumpPin);
  digitalWrite(pumpPin, !currentState);
}

void outputPinsModus(){
    // Initial PinMode configurations for LEDs
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);

  // Initial PinMode configurations for PUMOs
  pinMode(PUMP_A, OUTPUT);
  pinMode(PUMP_B, OUTPUT);
  pinMode(PUMP_C, OUTPUT);
}
void outputPinsState(){
  // Set initial states for LEDs (all off)
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);

  // Set initial states for PUMPs (all off)
  digitalWrite(PUMP_A, LOW); // Turn off pump (assuming HIGH is on)
  digitalWrite(PUMP_B, LOW); // Turn off pump (assuming HIGH is on)
  digitalWrite(PUMP_C, LOW); // Turn off pump (assuming HIGH is on)
}

// Recompute derived timing values when 'factor' changes
void updatePumpTiming(){
  pumpTiming[0] = PUMP_TIME_ALCOHOL * factor; // Alcohol time adjusted by factor
  pumpTiming[1] = PUMP_TIME_LIQUID_A;         // Liquid A time
  pumpTiming[2] = PUMP_TIME_LIQUID_B;         // Liquid B time
}

void timer_to_zero(){
  pumptimer = 0;
  pumpTiming[0] = 0;
  pumpTiming[1] = 0;
  pumpTiming[2] = 0;
}