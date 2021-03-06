#ifndef _drivers_display_hpp
#define _drivers_display_hpp

#ifdef WITH_DISPLAY_SSD1306
#define DISPLAY_ADDRESS 10
#define DISPLAY_DATA_ADDRESS 20
#define DISPLAY_OLED_MOSI 50
#define DISPLAY_OLED_CLK 60
#define DISPLAY_OLED_DC 70
#define DISPLAY_OLED_CS 80
#define DISPLAY_OLED_RESET 90
#include "program.hpp"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int display_ssd1306(program *_p);
#endif

#endif
