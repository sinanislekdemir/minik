/**
   Slightly better Serial input controls
 */
#ifndef _serial_hpp
#define _serial_hpp

#define MAX_LINE_LENGTH 128
#define NL "\n"

#include "status.hpp"
#include <Arduino.h>

int serial_getline(unsigned int buffer_size, char *back);
char *serial_get_multiline(unsigned int buffer_size);

#endif
