/**
 * @file Pins.h
 * @brief Hardware Pin Mapping Header
 * @remark Central definition of pin numbers
 * @author Nkwamou Jordan
 * @date 23.10.2025
 */

#ifndef PINS_H
#define PINS_H

// --- Led signal for alcoholic degree for the drinks  ---
#define GREEN 4    // Led fofr non alcoholic drink
#define YELLOW 3   // Led for light alcoholic drink
// #define BLUE 15     // Led for medium alcoholic drink
#define RED 12
// --- Pump pin ---
#define PUMP_A 13
#define PUMP_B 12
#define PUMP_C 11
#define PUMP_D 10

// --- Buttons to define the drinks alcoholic degree ---
#define BUTTON_OPT_1 6  // Drink 1. mixture selection button for non alcoholic drink
#define BUTTON_OPT_2 7  // Drink 2. mixture selection button for light alcoholic drink
#define BUTTON_OPT_3 8  // Drink 3. mixture selection button for medium alcoholic drink
#define BUTTON_OPT_4 9  // Drink 4. mixture selection button for strong alcoholic drink
// #define BUTTON_START 5  // Start button to begin mixing after Option chosed
#define BUTTON_STOP 5   // Stop button any process and come back to initial (chosing)


#endif  // PINS_H
