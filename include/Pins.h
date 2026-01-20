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
#define GREEN 7    // Led fofr non alcoholic drink
#define YELLOW 8   // Led for light alcoholic drink
#define BLUE 9    // Led for medium alcoholic drink
#define RED 10     // Led for strong alcoholic drink

// --- Pump pin ---
#define PUMP_A 11
#define PUMP_B 12
#define PUMP_C 13

// --- Buttons to define the drinks alcoholic degree ---
#define BUTTON_OPT_1 6  // Drink 1. mixture selection button for non alcoholic drink
#define BUTTON_OPT_2 5  // Drink 2. mixture selection button for light alcoholic drink
#define BUTTON_OPT_3 4  // Drink 3. mixture selection button for medium alcoholic drink
#define BUTTON_OPT_4 3  // Drink 4. mixture selection button for strong alcoholic drink
#define BUTTON_STOP 2   // Stop button any process and come back to initial (chosing)


#endif  // PINS_H
