#include "display_ssd1306.hpp"
#include "helpers.hpp"

#ifdef WITH_DISPLAY_SSD1306

#include "SPI.h"
#include "Wire.h"
Adafruit_SSD1306 display = NULL;

int display_ssd1306(program *_p) {
	unsigned int pid = _p->pid;
	int cmd_var = int(read_area_double(DISPLAY_ADDRESS));
	if (cmd_var == 0) {
		error_msg("SSD1306_CMD not defined", pid);
		return -1;
	}

	if (cmd_var == 1) {
		char connection[4] = {0};
		int screen_width = int(read_area_double(DISPLAY_DATA_ADDRESS));
		int screen_height = int(read_area_double(DISPLAY_DATA_ADDRESS + 4));
		read_area_str(DISPLAY_DATA_ADDRESS + 8, 4, connection);
		if (screen_width == 0) {
			error_msg("SSD1306_SCREEN_WIDTH is not defined", pid);
			return -1;
		}
		if (screen_height == 0) {
			error_msg("SSD1306_SCREEN_HEIGHT is not defined", pid);
			return -1;
		}
		if (strcmp(connection, "I2C") == 0) {
			display = new Adafruit_SSD1306(screen_width, screen_height, &Wire, -1);
			return 0;
		}
		if (strcmp(connection, "SPI") == 0) {
			int pin_mosi = int(read_area_double(DISPLAY_OLED_MOSI));
			int pin_clk = int(read_area_double(DISPLAY_OLED_CLK));
			int pin_dc = int(read_area_double(DISPLAY_OLED_DC));
			int pin_cs = int(read_area_double(DISPLAY_OLED_CS));
			int pin_reset = int(read_area_double(DISPLAY_OLED_RESET));
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
		int address = int(read_area_double(DISPLAY_DATA_ADDRESS));
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
		int txt_size = int(read_area_double(DISPLAY_DATA_ADDRESS));
		display->setTextSize(uint8_t(txt_size));
		return 0;
	}
	if (cmd_var == 5) {
		int text_color = int(read_area_double(DISPLAY_DATA_ADDRESS));
		display->setTextColor(uint16_t(text_color));
		return 0;
	}
	if (cmd_var == 6) {
		int cx = int(read_area_double(DISPLAY_DATA_ADDRESS));
		int cy = int(read_area_double(DISPLAY_DATA_ADDRESS + 4));
		display->setCursor(int16_t(cx), int16_t(cy));
		return 0;
	}
	if (cmd_var == 7) {
		char text[MAX_LINE_LENGTH] = {0};
		read_area_str(DISPLAY_DATA_ADDRESS, MAX_LINE_LENGTH, text);
		display->print(text);
		return 0;
	}
	if (cmd_var == 8) {
		char text[MAX_LINE_LENGTH] = {0};
		read_area_str(DISPLAY_DATA_ADDRESS, MAX_LINE_LENGTH, text);
		display->println(text);
		return 0;
	}
	if (cmd_var == 9) {
		double n = read_area_double(DISPLAY_DATA_ADDRESS);
		display->print(n);
		return 0;
	}
	if (cmd_var == 10) {
		double n = read_area_double(DISPLAY_DATA_ADDRESS);
		display->println(n);
		return 0;
	}
	if (cmd_var == 11) {
		display->display();
		return 0;
	}
	if (cmd_var == 12) {
		int x = int(read_area_double(DISPLAY_DATA_ADDRESS));
		int y = int(read_area_double(DISPLAY_DATA_ADDRESS + 4));
		int tx = int(read_area_double(DISPLAY_DATA_ADDRESS + 8));
		int ty = int(read_area_double(DISPLAY_DATA_ADDRESS + 12));
		int c = int(read_area_double(DISPLAY_DATA_ADDRESS + 16));
		display->drawLine(int16_t(x), int16_t(y), int16_t(tx), int16_t(ty), uint16_t(c));
		return 0;
	}
	if (cmd_var == 13) {
		int x = int(read_area_double(DISPLAY_DATA_ADDRESS));
		int y = int(read_area_double(DISPLAY_DATA_ADDRESS + 4));
		int w = int(read_area_double(DISPLAY_DATA_ADDRESS + 8));
		int h = int(read_area_double(DISPLAY_DATA_ADDRESS + 12));
		int c = int(read_area_double(DISPLAY_DATA_ADDRESS + 16));
		display->drawRect(int16_t(x), int16_t(y), int16_t(w), int16_t(h), uint16_t(c));
		return 0;
	}
	if (cmd_var == 14) {
		int x = int(read_area_double(DISPLAY_DATA_ADDRESS));
		int y = int(read_area_double(DISPLAY_DATA_ADDRESS + 4));
		int w = int(read_area_double(DISPLAY_DATA_ADDRESS + 8));
		int h = int(read_area_double(DISPLAY_DATA_ADDRESS + 12));
		int c = int(read_area_double(DISPLAY_DATA_ADDRESS + 16));
		display->fillRect(int16_t(x), int16_t(y), int16_t(w), int16_t(h), uint16_t(c));
		return 0;
	}
	if (cmd_var == 15) {
		int x = int(read_area_double(DISPLAY_DATA_ADDRESS));
		int y = int(read_area_double(DISPLAY_DATA_ADDRESS + 4));
		int radius = int(read_area_double(DISPLAY_DATA_ADDRESS + 8));
		int c = int(read_area_double(DISPLAY_DATA_ADDRESS + 12));
		display->drawCircle(int16_t(x), int16_t(y), int16_t(radius), uint16_t(c));
		return 0;
	}
	if (cmd_var == 16) {
		int x = int(read_area_double(DISPLAY_DATA_ADDRESS));
		int y = int(read_area_double(DISPLAY_DATA_ADDRESS + 4));
		int radius = int(read_area_double(DISPLAY_DATA_ADDRESS + 8));
		int c = int(read_area_double(DISPLAY_DATA_ADDRESS + 12));
		display->fillCircle(int16_t(x), int16_t(y), int16_t(radius), uint16_t(c));
		return 0;
	}
	if (cmd_var == 17) {
		int x = int(read_area_double(DISPLAY_DATA_ADDRESS));
		int y = int(read_area_double(DISPLAY_DATA_ADDRESS + 4));
		int w = int(read_area_double(DISPLAY_DATA_ADDRESS + 8));
		int h = int(read_area_double(DISPLAY_DATA_ADDRESS + 12));
		int c = int(read_area_double(DISPLAY_DATA_ADDRESS + 16));
		int radius = int(read_area_double(DISPLAY_DATA_ADDRESS + 20));
		display->drawRoundRect(int16_t(x), int16_t(y), int16_t(w), int16_t(h), int16_t(radius), uint16_t(c));
		return 0;
	}
	if (cmd_var == 18) {
		int x = int(read_area_double(DISPLAY_DATA_ADDRESS));
		int y = int(read_area_double(DISPLAY_DATA_ADDRESS + 4));
		int w = int(read_area_double(DISPLAY_DATA_ADDRESS + 8));
		int h = int(read_area_double(DISPLAY_DATA_ADDRESS + 12));
		int c = int(read_area_double(DISPLAY_DATA_ADDRESS + 16));
		int radius = int(read_area_double(DISPLAY_DATA_ADDRESS + 20));
		display->fillRoundRect(int16_t(x), int16_t(y), int16_t(w), int16_t(h), int16_t(radius), uint16_t(c));
		return 0;
	}
	if (cmd_var == 19) {
		int x = int(read_area_double(DISPLAY_DATA_ADDRESS));
		int y = int(read_area_double(DISPLAY_DATA_ADDRESS + 4));
		int c = int(read_area_double(DISPLAY_DATA_ADDRESS + 8));

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
