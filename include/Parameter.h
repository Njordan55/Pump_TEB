/**
 * @file Parameter.h
 * @brief Parameter Definitions Header
 * @remark Central definition of parameters used in the project
 * @author Nkwamou Jordan
 * @date 23.10.2025
 */

#include "Pins.h"
#include "KeysDebounce.h"
#include "BusIn.h"

 #ifndef PARAMETER_H
 #define PARAMETER_H

 // Basic Pump timing definitions (in milliseconds) for different liquids
// LIQUID_A and etc and (...) are placeholders; replace with actual liquid names as needed
// The Pumping times is adjusted according to the time needed to pump 100 ml of each liquid
#define PUMP_TIME_ALCOHOL 500 // Pumping time for alcohol in milliseconds
#define PUMP_TIME_LIQUID_A 1500 // Pumping time for (...) in milliseconds
#define PUMP_TIME_LIQUID_B 2000 // Pumping time for (...) in milliseconds

const uint8_t pumpPin [] = { PUMP_A, PUMP_B, PUMP_C };
const size_t pumpPinCount = sizeof(pumpPin) / sizeof(pumpPin[0]);
uint8_t counter = 0;
uint8_t lastCounter = -1;

//Switches/Buttons configuration, Debounce setup and Variables for button states
//
// Create a BusIn for option buttons (example: 4 option buttons)
const uint8_t optionPins[] = { BUTTON_OPT_1, BUTTON_OPT_2, BUTTON_OPT_3, 
                                BUTTON_OPT_4, BUTTON_STOP };
const size_t optionPinsCount = sizeof(optionPins) / sizeof(optionPins[0]);
// usePullup = false because you said you use external resistors
// BusIn expects a small integer count; cast to uint8_t
BusIn keys(optionPins, (uint8_t)optionPinsCount, true);
// Create a debounce object that reads the BusIn mask
KeysDebounce buttonDebounce(&keys, 80);
uint8_t newInput = 0;
uint8_t previousInput = 0;
uint8_t buttonPushed = 0;
// Button flag
bool buttonFlag = false;

// Pumping option control
uint8_t pumpingOption = 0; // 0 = no pumping, 1 = option 1, 2 = option 2, etc.

// Factor to adjust pumping time for alcohol based on selected option
// 0 = no alcohol, 1 = low, 2 = medium, 3 = high
uint8_t factor = 0;
// Timers for each drink type (0 = Alcohol, 1 = Liquid A, 2 = Liquid B)
uint64_t pumpTiming[3] = {0, 0, 0};
const size_t timeCounter = sizeof(pumpTiming) / sizeof(pumpTiming[0]);
// Timing variables for pump timing control
//
uint64_t now = 0;
uint64_t pumptimer = 0;

#endif // PARAMETER_H