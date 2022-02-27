#ifndef _drivers_bluetooth_hpp
#define _drivers_bluetooth_hpp

#ifdef BOARD_ESP32
#include "program.hpp"

#ifdef WITH_BLUETOOTH_SERIAL

#define BT_SERIAL_ADDRESS 10
#define BT_DATA_ADDRESS 20

#ifndef BLUETOOTH_SERIAL_NAME
#define BLUETOOTH_NAME "MinikESP32"
#else
#define BLUETOOTH_NAME XSTR(BLUETOOTH_SERIAL_NAME)
#endif
int bluetooth_serial(program *p);
#endif

#ifdef WITH_BLUETOOTH
int bluetooth(program *p);
#endif

#endif

int init_bt_serial();

#endif
