/*
  BusIn.cpp - file for handling multiple
  digital input pins as a single bus.
  Created on: 23.10.2025
  Author: Nkwamou Jordan
  Released into the public domain.
*/

#include "BusIn.h"

BusIn::BusIn(const uint8_t *pins, uint8_t count, bool usePullup)
    : m_count(0), m_usePullup(usePullup) {
  if (count > 8) count = 8;
  m_count = count;
  for (uint8_t i = 0; i < m_count; ++i) {
    m_pins[i] = pins[i];
  }
}

void BusIn::begin() {
  for (uint8_t i = 0; i < m_count; ++i) {
    pinMode(m_pins[i], m_usePullup ? INPUT_PULLUP : INPUT);
  }
}

uint8_t BusIn::read() const {
  uint8_t mask = 0;
  for (uint8_t i = 0; i < m_count; ++i) {
    uint8_t v = digitalRead(m_pins[i]);
    if (v) mask |= (1 << i);
  }
  return mask;
}

uint8_t BusIn::readPin(uint8_t index) const {
  if (index >= m_count) return 0;
  return digitalRead(m_pins[index]);
}