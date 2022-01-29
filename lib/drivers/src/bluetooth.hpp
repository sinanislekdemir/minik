#ifndef _drivers_bluetooth_hpp
#define _drivers_bluetooth_hpp

#ifdef BOARD_ESP32
#include "program.hpp"

#ifdef WITH_BLUETOOTH_SERIAL
int bluetooth_serial(program *_p);
#endif

#ifdef WITH_BLUETOOTH
int bluetooth(program *_p);
#endif

#endif
#endif
