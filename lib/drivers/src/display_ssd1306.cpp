#include "display_ssd1306.hpp"
#include "helpers.hpp"

#ifdef WITH_DISPLAY_SSD1306

#include "SPI.h"
#include "Wire.h"
Adafruit_SSD1306 *display = NULL;

int display_ssd1306(program *_p) {
	unsigned int pid = _p->pid;
	int cmd_var = find_number("SSD1306_CMD", pid);
	if (cmd_var == 0) {
		error_msg("SSD1306_CMD not defined", pid);
		return -1;
	}

	if (cmd_var == 1) {
		int screen_width = find_number("SSD1306_SCREEN_WIDTH", pid);
		int screen_height = find_number("SSD1306_SCREEN_HEIGHT", pid);
		char *connection = find_string("SSD1306_CONNECTION", pid);
		if (screen_width == 0) {
			error_msg("SSD1306_SCREEN_WIDTH is not defined", pid);
			return -1;
		}
		if (screen_height == 0) {
			error_msg("SSD1306_SCREEN_HEIGHT is not defined", pid);
			return -1;
		}
		if (connection == NULL) {
			error_msg("SSD1306_CONNECTION is not defined", pid);
			return -1;
		}

		if (strcmp(connection, "I2C") == 0) {
			display = new Adafruit_SSD1306(screen_width, screen_height, &Wire, -1);
			return 0;
		}
		if (strcmp(connection, "SPI") == 0) {
			int pin_mosi = find_number("OLED_MOSI", pid);
			int pin_clk = find_number("OLED_CLK", pid);
			int pin_dc = find_number("OLED_DC", pid);
			int pin_cs = find_number("OLED_CS", pid);
			int pin_reset = find_number("OLED_RESET", pid);
			if (pin_mosi > 0) {
				display = new Adafruit_SSD1306(screen_width, screen_height, pin_mosi, pin_clk, pin_dc, pin_reset, pin_cs);
			} else {
				display = new Adafruit_SSD1306(screen_width, screen_height, &SPI, pin_dc, pin_reset, pin_cs);
			}
			return 0;
		}
	}
	if (display == NULL) {
		error_msg("SSD1306 is not initialized yet", pid);
		return -1;
	}
	if (cmd_var == 2) {
		int address = find_number("SSD1306_ADDRESS", pid);
		if (address == 0) {
			error_msg("SSD1306_ADDRESS is not defined", pid);
			return -1;
		}
		display->begin(SSD1306_SWITCHCAPVCC, uint8_t(address));
		return 0;
	}
	if (cmd_var == 3) {
		display->clearDisplay();
		return 0;
	}
	if (cmd_var == 4) {
		int txt_size = find_number("SSD1306_TEXT_SIZE", pid);
		display->setTextSize(uint8_t(txt_size));
		return 0;
	}
	if (cmd_var == 5) {
		int text_color = find_number("SSD1306_COLOR", pid);
		display->setTextColor(uint16_t(text_color));
		return 0;
	}
	if (cmd_var == 6) {
		int cx = find_number("SSD1306_X", pid);
		int cy = find_number("SSD1306_Y", pid);
		display->setCursor(int16_t(cx), int16_t(cy));
		return 0;
	}
	if (cmd_var == 7) {
		char *text = find_string("SSD1306_TEXT", pid);
		display->print(text);
		return 0;
	}
	if (cmd_var == 8) {
		char *text = find_string("SSD1306_TEXT", pid);
		display->println(text);
		return 0;
	}
	if (cmd_var == 9) {
		double n = find_double("SSD1306_NUMBER", pid);
		display->print(n);
		return 0;
	}
	if (cmd_var == 10) {
		double n = find_double("SSD1306_NUMBER", pid);
		display->println(n);
		return 0;
	}
	if (cmd_var == 11) {
		display->display();
		return 0;
	}
	if (cmd_var == 12) {
		int x = find_number("SSD1306_X", pid);
		int y = find_number("SSD1306_Y", pid);
		int tx = find_number("SSD1306_TX", pid);
		int ty = find_number("SSD1306_TY", pid);
		int c = find_number("SSD1306_COLOR", pid);
		display->drawLine(int16_t(x), int16_t(y), int16_t(tx), int16_t(ty), uint16_t(c));
		return 0;
	}
	if (cmd_var == 13) {
		int x = find_number("SSD1306_X", pid);
		int y = find_number("SSD1306_Y", pid);
		int w = find_number("SSD1306_W", pid);
		int h = find_number("SSD1306_H", pid);
		int c = find_number("SSD1306_COLOR", pid);
		display->drawRect(int16_t(x), int16_t(y), int16_t(w), int16_t(h), uint16_t(c));
		return 0;
	}
	if (cmd_var == 14) {
		int x = find_number("SSD1306_X", pid);
		int y = find_number("SSD1306_Y", pid);
		int w = find_number("SSD1306_W", pid);
		int h = find_number("SSD1306_H", pid);
		int c = find_number("SSD1306_COLOR", pid);
		display->fillRect(int16_t(x), int16_t(y), int16_t(w), int16_t(h), uint16_t(c));
		return 0;
	}
	if (cmd_var == 15) {
		int x = find_number("SSD1306_X", pid);
		int y = find_number("SSD1306_Y", pid);
		int radius = find_number("SSD1306_RADIUS", pid);
		int c = find_number("SSD1306_COLOR", pid);
		display->drawCircle(int16_t(x), int16_t(y), int16_t(radius), uint16_t(c));
		return 0;
	}
	if (cmd_var == 16) {
		int x = find_number("SSD1306_X", pid);
		int y = find_number("SSD1306_Y", pid);
		int radius = find_number("SSD1306_RADIUS", pid);
		int c = find_number("SSD1306_COLOR", pid);
		display->fillCircle(int16_t(x), int16_t(y), int16_t(radius), uint16_t(c));
		return 0;
	}
	if (cmd_var == 17) {
		int x = find_number("SSD1306_X", pid);
		int y = find_number("SSD1306_Y", pid);
		int w = find_number("SSD1306_W", pid);
		int h = find_number("SSD1306_H", pid);
		int c = find_number("SSD1306_COLOR", pid);
		int radius = find_number("SSD1306_RADIUS", pid);
		display->drawRoundRect(int16_t(x), int16_t(y), int16_t(w), int16_t(h), int16_t(radius), uint16_t(c));
		return 0;
	}
	if (cmd_var == 18) {
		int x = find_number("SSD1306_X", pid);
		int y = find_number("SSD1306_Y", pid);
		int w = find_number("SSD1306_W", pid);
		int h = find_number("SSD1306_H", pid);
		int c = find_number("SSD1306_COLOR", pid);
		int radius = find_number("SSD1306_RADIUS", pid);
		display->fillRoundRect(int16_t(x), int16_t(y), int16_t(w), int16_t(h), int16_t(radius), uint16_t(c));
		return 0;
	}
	if (cmd_var == 19) {
		int x = find_number("SSD1306_X", pid);
		int y = find_number("SSD1306_Y", pid);
		int c = find_number("SSD1306_COLOR", pid);
		display->drawPixel(int16_t(x), int16_t(y), uint16_t(c));
		return 0;
	}
	if (cmd_var == 99) {
		free(display);
		display = NULL;
		return 0;
	}
	return 0;
}

#endif
