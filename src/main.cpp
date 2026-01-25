#include "Parameter.h"

static void outputPinsState();
static void outputPinsModus();

static void setIndicatorLED(uint8_t option);
static void allLEDsOff();

static void pump_on(uint8_t pin);
static void pump_off(uint8_t pin);

static void updatePumpTiming();
static void timer_to_zero();
static void emergencyStopAndReset();

void setup() {
  outputPinsModus();
  outputPinsState();
  keys.begin();
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

  // Select option (only when idle)
  if (buttonFlag && (pumpingOption == 0)) {
    // Only option buttons 1..4 (exclude STOP)
    for (uint8_t i = 0; i < 4; i++) {
      if (buttonPushed & (1 << i)) {
        pumpingOption = i + 1; // 1..4
        break;
      }
    }

    if (pumpingOption != 0) {
      updatePumpTiming();
      setIndicatorLED(pumpingOption);
    } else {
      // No valid option was pressed
      buttonFlag = false;
    }
  }

  // Run pump sequence
  if (pumpingOption != 0) {

    // Skip pumps with 0ms runtime
    while (counter < timeCounter && pumpTiming[counter] == 0) {
      counter++;
      lastCounter = -1;
    }

    if (counter < timeCounter) {
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
  }

  // Done?
  if (counter >= timeCounter) {
    emergencyStopAndReset(); // turns off pumps + resets state + LEDs off
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
  if (option >= 1 && option <= 4) {
    digitalWrite(ledPin[option - 1], HIGH);
  }
}

// ------------------------- Pump helpers -------------------------

static void pump_on(uint8_t pin) {
  digitalWrite(pin, HIGH);
}

static void pump_off(uint8_t pin) {
  digitalWrite(pin, LOW);
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
  pinMode(PUMP_D, OUTPUT);
}

static void outputPinsState() {
  allLEDsOff();

  pump_off(PUMP_A);
  pump_off(PUMP_B);
  pump_off(PUMP_C);
  pump_off(PUMP_D);
}

// ------------------------- Timing logic -------------------------

static void updatePumpTiming() {

  // Default: always dispense Liquid A and Liquid B
  pumpTiming[0] = PUMP_TIME_LIQUID_A;  // Pumping time for Liquid A in milliseconds
  pumpTiming[1] = PUMP_TIME_LIQUID_B;  // Pumping time for Liquid B in milliseconds

  // Default: no alcohol
  pumpTiming[2] = 0; // Pumping time for alcohol in milliseconds
  pumpTiming[3] = 0; // Pumping time for strong alcohol in milliseconds

  switch (pumpingOption) {

    case 1: // Drink 1. non alcoholic drink
      // Only Liquid A and Liquid B
      break;

    case 2: // Drink 2. light alcoholic drink
      pumpTiming[2] = PUMP_TIME_ALCOHOL; // small amount of alcohol
      break;

    case 3: // Drink 3. medium alcoholic drink
      pumpTiming[2] = PUMP_TIME_ALCOHOL * 2; // more alcohol (longer runtime)
      break;

    case 4: // Drink 4. strong alcoholic drink
      pumpTiming[2] = PUMP_TIME_ALCOHOL * 2;     // normal alcohol
      pumpTiming[3] = PUMP_TIME_ALCOHOL_STRONG;  // additional strong alcohol
      break;

    default:
      break;
  }
}


static void timer_to_zero() {
  pumptimer = 0;
  for (size_t i = 0; i < timeCounter; i++) {
    pumpTiming[i] = 0;
  }
}

static void emergencyStopAndReset() {
  // Turn off all pumps
  pump_off(PUMP_A);
  pump_off(PUMP_B);
  pump_off(PUMP_C);
  pump_off(PUMP_D);

  // Reset state machine
  pumpingOption = 0;
  counter = 0;
  lastCounter = -1;
  buttonFlag = false;

  timer_to_zero();
  allLEDsOff();
}
