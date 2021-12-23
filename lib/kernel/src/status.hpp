/**
   Signal state is a LED state to indicate current status of the board.

   Note to myself
   millis()
   Returns the number of milliseconds passed since the Arduino board began
   running the current program. This number will overflow (go back to zero),
   after approximately 50 days.
 */

#ifndef _signal_hpp
#define _signal_hpp

#ifdef BOARD_ESP32
#define STATUS_PIN 25 // 25 For Heltec Board
#endif
#ifdef BOARD_ATMEGA
#define STATUS_PIN 13
#endif

#define BOOT 0b11111111
#define READY 0b00001111
#define SERIAL_PORT_OPENED 0b11111110
#define STOP 0b10000000

#define ERROR 0b01010101
#define ERROR_UNKNOWN 0b00000001

#include "daemon.hpp"
#include <Arduino.h>

class StatusEngine : public daemon {
private:
  int _status;
  unsigned long _last_milliseconds;
  unsigned long _diff_milliseconds;
  int _status_pin;
  int _pin_state;

public:
  StatusEngine();
  void change_status_pin(int pin);
  void set_status(int status);
  int process();
};

int free_ram();
#endif
