#ifndef _drivers_sdcard_hpp
#define _drivers_sdcard_hpp
#ifdef WITH_SDCARD

#ifdef BOARD_ATMEGA
#include <SD.h>
#define FILE_APPEND O_APPEND
#endif

#include "program.hpp"

int sdcard(program *_p);

#endif
#endif
