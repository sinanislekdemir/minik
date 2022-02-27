#include "eeprom.hpp"
#include <Arduino.h>
#include <EEPROM.h>

int eeprom_cursor = 0;

int init_eeprom() {
	EEPROM.begin(EEPROM_SIZE);
	int epc = eeprom_program_count();
	if (epc > 0) {
		Serial.print("Found stored programs: ");
		Serial.println(epc);
	}
	return 1;
}

int eeprom_free_area() {
	int c = 0;
	for (unsigned int i = 0; i < EEPROM_SIZE; i++) {
		if (EEPROM.read(i) == 0) {
			c++;
		}
#ifdef BOARD_ESP32
		vTaskDelay(1);
#endif
	}
	return c;
}

int eeprom_clean() {
	for (unsigned int i = 0; i < EEPROM_SIZE; i++) {
		if (EEPROM.read(i) == 0) {
			break;
		}
		EEPROM.write(i, 0);
		EEPROM.commit();
		Serial.print(i);
		Serial.print("/");
		Serial.println(EEPROM_SIZE);
#ifdef BOARD_ESP32
		vTaskDelay(1);
#endif
	}
}

int eeprom_program_count() {
	int c = 0;
	for (unsigned int i = 0; i < EEPROM_SIZE; i++) {
		if (EEPROM.read(i) == STX) {
			c++;
		}
#ifdef BOARD_ESP32
		vTaskDelay(1);
#endif
	}
	return c;
}

int eeprom_write_line(const char *buffer) {
	int pos = -1;
	for (unsigned int i = 0; i < EEPROM_SIZE; i++) {
		if (EEPROM.read(i) == 0) {
			pos = i;
			break;
		}
	}
	if (pos == -1) {
		return -1;
	}
	for (unsigned int i = 0; i < strlen(buffer); i++) {
		EEPROM.write(pos + i, buffer[i]);
		EEPROM.commit();
	}
	EEPROM.write(pos + strlen(buffer), '\n');
	EEPROM.commit();
	return 0;
}

int eeprom_read_line(char *back) {
	int cursor = 0;
	int ecursor = 0;
	for (unsigned int i = eeprom_cursor; i < EEPROM_SIZE; i++) {
		uint8_t c = EEPROM.read(i);
		if (c == 0) {
			return -1;
		}
		if (c == STX) {
			continue;
		}
		if (c == ETX) {
			eeprom_cursor = i + 1;
			return 0;
		}
		if (c > 31 && c < 127) {
			back[cursor++] = c;
		}
		if (c == '\n') {
			ecursor = i;
			break;
		}
	}
	eeprom_cursor = ecursor + 1;
	return cursor;
}

int eeprom_start_program() {
	for (unsigned int i = 0; i < EEPROM_SIZE; i++) {
		if (EEPROM.read(i) == 0) {
			EEPROM.write(i, STX);
                        EEPROM.commit();
			return 0;
		}
	}
	return -1;
}

int eeprom_end_program() {
	for (unsigned int i = 0; i < EEPROM_SIZE; i++) {
		if (EEPROM.read(i) == 0) {
			EEPROM.write(i, ETX);
			EEPROM.commit();
			return 0;
		}
	}
	return -1;
}
