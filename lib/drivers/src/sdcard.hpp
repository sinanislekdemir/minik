#ifndef _drivers_sdcard_hpp
#define _drivers_sdcard_hpp
#define WITH_SDCARD
#ifdef WITH_SDCARD

#define SDCARD_ADDRESS 10
#define SDCARD_DATA_ADDRESS 20

#ifdef BOARD_ATMEGA
#include <SD.h>
#define FILE_APPEND O_APPEND
#endif

#include "program.hpp"

int sdcard(program *_p);

#endif
#endif
