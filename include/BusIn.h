/*
  BusIn.h - Library for handling multiple digital input pins as a single bus.
  Created on: 23.10.2025
  *@author Nkwamou Jordan, Adrian Maurer
 */

#ifndef BUSIN_H
#define BUSIN_H

#include <Arduino.h>

class BusIn {
public:
  /**
   * Construct a BusIn object
   * @param pins Pointer to array of pin numbers
   * @param count Number of pins (max 8)
   * @param usePullup If true, configure pins as INPUT_PULLUP
   */
  BusIn(const uint8_t *pins, uint8_t count, bool usePullup = true);

  /**
   * Initialize pin modes. Call in setup() or from your caller.
   */
  void begin();

  /**
   * Read the bus and return a bitmask (bit0 = first pin)
   */
  uint8_t read() const;

  /**
   * Read single pin by index (0..count-1)
   */
  uint8_t readPin(uint8_t index) const;

  /**
   * Number of pins managed
   */
  uint8_t count() const { return m_count; }

private:
  uint8_t m_pins[8];
  uint8_t m_count;
  bool m_usePullup;
};

#endif // BUSIN_H
