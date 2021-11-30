/**
   Slightly better Serial input controls
 */
#ifndef _serial_hpp
#define _serial_hpp

#define MAX_LINE_LENGTH 128
#define NL "\n"

#include <Arduino.h>
#include "status.hpp"

char *serial_getline(int buffer_size);
char *serial_get_multiline(int buffer_size);

#endif
