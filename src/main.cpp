#include "Parameter.h"

static void outputPinsState();
static void outputPinsModus();

static void setIndicatorLED(uint8_t option);
static void allLEDsOff();

static void pump_on(uint8_t pin);
static void pump_off(uint8_t pin);
static void pumping (uint8_t n_o_pumps);

static void updatePumpTiming();
static void timer_to_zero();
static void emergencyStopAndReset();

void setup() {
  outputPinsModus();
  outputPinsState();
  now = millis();
  n_o_pumps = (uint8_t)timeCounter;
  memcpy(pumpTiming, pumpTimeStarts, sizeof(pumpTiming));
  keys.begin();
  while (now < PUMP_FILLING_TIME * timeCounter) {
    pumping(n_o_pumps);
    now = millis();
  }
}

void loop() {
  newInput = buttonDebounce.read();
  now = millis();

  // Rising edges: 0 -> 1
  buttonPushed = (~previousInput) & newInput;

  // STOP has highest priority (it's the LAST pin in optionPins[])
  const uint8_t STOP_BIT = (uint8_t)(optionPinsCount - 1);
  if (buttonPushed & (1 << STOP_BIT)) {
    emergencyStopAndReset();
    previousInput = newInput;
    return;
  }

  if (buttonPushed && !buttonFlag) {
    buttonFlag = true;
  }

  // Select option (only when button and option idle)
  if (buttonFlag && (pumpingOption == 0)) {
    // Only option buttons 1..4 (exclude STOP) with priority to the lowest
    // option index
    for (uint8_t i = 0; i < 4; i++) {
      if (buttonPushed & (1 << i)) {
        pumpingOption = i + 1; // 1..4
        break;
      }
    }
      updatePumpTiming();
      setIndicatorLED(pumpingOption);
  }

  // Run pump sequence
  if (pumpingOption != 0) {

    // Skip pumps with 0ms runtime
    if (pumpTiming[timeCounter] == 0) {
      n_o_pumps--;
    }else{
      n_o_pumps = (uint8_t)timeCounter;
    }
    pumping(n_o_pumps);
  }

  previousInput = newInput;
}

// ------------------------- LED helpers -------------------------

static void allLEDsOff() {
  for (size_t i = 0; i < ledPinCount; i++) {
    digitalWrite(ledPin[i], LOW);
  }
}

static void setIndicatorLED(uint8_t option) {
  // Show only ONE LED depending on option
  allLEDsOff();
  for (uint8_t i =0; i < option; i++ ) {
    digitalWrite(ledPin[i], HIGH);
  }
}

// ------------------------- Pump helpers -------------------------

static void pump_on(uint8_t pin) {
  digitalWrite(pin, HIGH);
}

static void pump_off(uint8_t pin) {
  digitalWrite(pin, LOW);
}

// ------------------------- Timing logic -------------------------

static void updatePumpTiming() {

  // Default: always dispense Liquid A and Liquid B
  pumpTiming[0] = PUMP_TIME_LIQUID_A;  // Pumping time for Liquid A in milliseconds
  pumpTiming[1] = PUMP_TIME_LIQUID_B;  // Pumping time for Liquid B in milliseconds

  switch (pumpingOption) {

    case 1: // Drink 1. non alcoholic drink
      factor = TIMER_FACTOR * 0; // no alcohol
      break;

    case 2: // Drink 2. light alcoholic drink
      factor = TIMER_FACTOR * 1; // light alcoholic
      break;

    case 3: // Drink 3. medium alcoholic drink
      factor = TIMER_FACTOR * 2; // medium alcoholic
      break;

    case 4: // Drink 4. strong alcoholic drink
        factor = TIMER_FACTOR * 3; // strong alcoholic
      break;

    default:
      break;
  }

  pumpTiming[2] = factor * PUMP_TIME_ALCOHOL;
}


static void timer_to_zero() {
  pumptimer = 0;
  for (size_t i = 0; i < timeCounter; i++) {
    pumpTiming[i] = 0;
  }
}

static void pumping (uint8_t n_o_pumps) {
  if (counter < n_o_pumps) {
    // Start the step once
    if ((int8_t)counter != lastCounter) {
      pumptimer = now + pumpTiming[counter];
      lastCounter = (int8_t)counter;
      pump_on(pumpPin[counter]);
    }
    
    // Finish step
    if (now >= pumptimer) {
      pump_off(pumpPin[counter]);
      counter++;
      lastCounter = -1; // force next step start
    }
  }

  // Done?
  if (counter >= n_o_pumps) {
    emergencyStopAndReset(); // turns off pumps + resets state + LEDs off
  }
}

// ------------------------- Emergency Stop -------------------------

static void emergencyStopAndReset() {
  // Turn off all pumps
  pump_off(PUMP_A);
  pump_off(PUMP_B);
  pump_off(PUMP_C);

  // Reset state machine
  pumpingOption = 0;
  counter = 0;
  lastCounter = -1;
  buttonFlag = false;
  factor = 0;

  timer_to_zero();
  allLEDsOff();
}

// ------------------------- IO setup -------------------------

static void outputPinsModus() {
  // LEDs
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);

  // Pumps
  pinMode(PUMP_A, OUTPUT);
  pinMode(PUMP_B, OUTPUT);
  pinMode(PUMP_C, OUTPUT);
}

static void outputPinsState() {
  allLEDsOff();

  pump_off(PUMP_A);
  pump_off(PUMP_B);
  pump_off(PUMP_C);
}