#ifndef _drivers_bluetooth_hpp
#define _drivers_bluetooth_hpp

#ifdef BOARD_ESP32
#include "program.hpp"

#ifdef WITH_BLUETOOTH_SERIAL

#ifndef BLUETOOTH_SERIAL_NAME
#define BLUETOOTH_NAME "MinikESP32"
#else
#define BLUETOOTH_NAME XSTR(BLUETOOTH_SERIAL_NAME)
#endif
int bluetooth_serial(program *_p);
#endif

#ifdef WITH_BLUETOOTH
int bluetooth(program *_p);
#endif

#endif

int init_bt_serial();

#endif
