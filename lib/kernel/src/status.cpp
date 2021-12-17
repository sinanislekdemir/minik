#include "status.hpp"

int _status = 0;
unsigned long _last_milliseconds = 0;
unsigned long _diff_milliseconds = 0;
int _status_pin = STATUS_PIN;
int _pin_state = LOW;

void set_status(int status) { _status = status; }

void change_status_pin(int pin) {
  // Turn off the lights for the old pin;
  pinMode(_status_pin, OUTPUT);
  digitalWrite(_status_pin, LOW);
  // change the pin;
  _status_pin = pin;
  pinMode(_status_pin, OUTPUT);
  digitalWrite(_status_pin, _pin_state);
}

void process_status() {
  unsigned long now = millis();
  if (_last_milliseconds == 0) {
    _last_milliseconds = now;
  }
  _diff_milliseconds = now - _last_milliseconds;
  if (_diff_milliseconds >= 1000) {
    _diff_milliseconds = 0;
    _last_milliseconds = now;
  }
  int step = _diff_milliseconds / 125;
  int val = (_status >> step) & 1;
  if (val) {
    _pin_state = HIGH;
  } else {
    _pin_state = LOW;
  }
  digitalWrite(_status_pin, _pin_state);
}

void breath() { process_status(); }

int free_ram() {
#ifdef BOARD_ATMEGA
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
#endif

#ifdef BOARD_ESP32
  return ESP.getFreeHeap();
#endif
  return 0;
}
