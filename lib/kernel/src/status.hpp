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

#define STATUS_PIN 25 // 25 For Heltec Board

#define BOOT 0b11111111
#define READY 0b00001111
#define SERIAL_PORT_OPENED 0b11111110
#define STOP 0b10000000

#define ERROR 0b01010101
#define ERROR_UNKNOWN 0b00000001

#include <Arduino.h>

void set_status(int status);
void process_status();
void change_status_pin(int pin);
void breath();

#ifdef BOARD_ATMEGA
int free_ram();
#endif
#endif
