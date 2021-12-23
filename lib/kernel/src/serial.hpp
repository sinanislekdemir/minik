/**
   Slightly better Serial input controls
 */
#ifndef _serial_hpp
#define _serial_hpp

#include "status.hpp"
#include <Arduino.h>

int serial_getline(unsigned int buffer_size, char *back);
char *serial_get_multiline(unsigned int buffer_size);

#endif
