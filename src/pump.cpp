/**
 * @file pump.cpp
 * @brief Implementation for Pump class (with non-blocking timer)
 */

/**
 * @file pump.cpp
 * @brief Implementation for Pump class (with non-blocking timer)
 */

#include "Pump.h"

Pump::Pump(uint8_t pumpPin, uint8_t ledPin, bool activeHigh)
    : m_pumpPin(pumpPin), m_ledPin(ledPin), m_activeHigh(activeHigh), m_state(false), m_endTime(0) {}

void Pump::begin() {
  pinMode(m_pumpPin, OUTPUT);
  if (m_ledPin != 255) {
    pinMode(m_ledPin, OUTPUT);
  }
  applyState();
}

void Pump::set(bool on) {
  m_state = on;
  // cancel any timer when explicitly setting state
  m_endTime = 0;
  applyState();
}

void Pump::toggle() {
  m_state = !m_state;
  // toggling cancels any pending timer
  m_endTime = 0;
  applyState();
}

bool Pump::isOn() const {
  return m_state;
}

void Pump::startFor(unsigned long durationMs) {
  if (durationMs == 0) return;
  m_state = true;
  m_endTime = millis() + durationMs;
  applyState();
}

void Pump::update() {
  if (m_endTime == 0) return;
  unsigned long now = millis();
  // handle millis() wrap-around using subtraction
  if ((long)(now - m_endTime) >= 0) {
    m_state = false;
    m_endTime = 0;
    applyState();
  }
}

void Pump::cancelTimer() {
  m_endTime = 0;
}

unsigned long Pump::remainingMillis() const {
  if (m_endTime == 0) return 0;
  unsigned long now = millis();
  if ((long)(m_endTime - now) <= 0) return 0;
  return m_endTime - now;
}

void Pump::applyState() {
  uint8_t outputVal = m_state ? (m_activeHigh ? HIGH : LOW) : (m_activeHigh ? LOW : HIGH);
  digitalWrite(m_pumpPin, outputVal);
  if (m_ledPin != 255) {
    digitalWrite(m_ledPin, outputVal);
  }
}
