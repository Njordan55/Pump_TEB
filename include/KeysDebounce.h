/**
 * @file KeysDebounce.h
 * @brief Header file for key debounce functionality
 * @details This class provides a debounce mechanism for key inputs,
 * ensuring that only stable key presses are registered.
  * @author Nkwamou Jordan ,Adrian Maurer
 */

#ifndef KEYS_DEBOUNCE_H
#define KEYS_DEBOUNCE_H

#include <Arduino.h>
#include "BusIn.h"

class KeysDebounce {
private:
  // Pointer to the BusIn object
  BusIn *m_busIn;
  // Debounce wait time in milliseconds
  int m_waitTime;
  uint8_t m_previousInput;
  uint64_t m_lastChangeTime;
public:
  /**
 * Constructor to initialize the debounce with a BusIn object and wait time
 * @param bus Pointer to BusIn object
 * @param waitTime Debounce wait time in milliseconds
 */
  KeysDebounce(BusIn *bus, int waitTime);
  /**
     * Read the debounced key state
     * @return Debounced key state
     */
  uint8_t read();

  /**
     * Conversion operator to uint8_t
     * @return Debounced key state
     */
  operator uint8_t();
};

#endif  // KEYS_DEBOUNCE_H
