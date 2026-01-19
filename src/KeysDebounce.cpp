/**
 * @file KeysDebounce.cpp
 * @brief Implementation file for KeysDebounce class
 * @author Nkwamou Jordan
 * @date 25.10.2025
 */

#include "KeysDebounce.h"

KeysDebounce::KeysDebounce(BusIn *bus, int waitTime) {
  m_busIn = bus;
  m_waitTime = waitTime;
  m_previousInput = 0;
  m_lastChangeTime = -1;
}

uint8_t KeysDebounce::read(){
  if(m_lastChangeTime >= 0){
    if (millis() < m_lastChangeTime + m_waitTime){
      return m_previousInput;
    }
    m_lastChangeTime = -1;
  }

  uint8_t newInputValue = m_busIn->read();
  if(newInputValue != m_previousInput){
    m_lastChangeTime = millis();
    m_previousInput = newInputValue;
  }
  return newInputValue;
}


KeysDebounce::operator uint8_t() {
  return read();
}