#ifndef _drivers_display_hpp
#define _drivers_display_hpp

#ifdef WITH_DISPLAY_SSD1306
#include "program.hpp"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int display_ssd1306(program *_p);
#endif

#endif
