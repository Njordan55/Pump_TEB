/**
 * @file Pump.h
 * @brief Pump control class header
 */

#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

class Pump {
public:
  Pump(uint8_t pumpPin, uint8_t ledPin = 255, bool activeHigh = true);
  void begin();
  void set(bool on);
  void toggle();
  bool isOn() const;
  void startFor(unsigned long durationMs);
  void update();
  void cancelTimer();
  unsigned long remainingMillis() const;
private:
  uint8_t m_pumpPin;
  uint8_t m_ledPin;
  bool m_activeHigh;
  bool m_state;
  unsigned long m_endTime;
  void applyState();
};

// Convenience helpers
static inline void pumping(uint8_t pumpPin, bool on) { digitalWrite(pumpPin, on ? HIGH : LOW); }
static inline void pumping(Pump &pumpObj, bool on) { pumpObj.set(on); }
static inline void pumping(Pump &pumpObj, unsigned long durationMs) { pumpObj.startFor(durationMs); }

#endif // PUMP_H
