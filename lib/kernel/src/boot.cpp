#include "boot.hpp"
#include "constants.hpp"
#include "eeprom.hpp"
#include "globals.hpp"
#include "kprint.hpp"
#include "program.hpp"
#include "tasks.hpp"
#include <Arduino.h>

extern KernelGlobals KGlobals;
extern program programs[MAX_PROGRAM_COUNT];

int minik_boot() {
	int epc = eeprom_program_count();
	if (epc > 0) {
		kprint("EEPROM programs found.\n");
		kprint("Loading EEPROM in 5 seconds.\n");
		kprint("Hit any key from Serial to abort auto-load\n");
		bool abort = false;
		unsigned long start = millis();
		while (true) {
			if (Serial.available()) {
				abort = true;
				break;
			}
			if (millis() - start > 5000) {
				break;
			}
			delay(200);
#ifdef BOARD_ESP32
			vTaskDelay(1);
#endif
		}
		if (abort) {
			return 1;
		}
		char buffer[MAX_LINE_LENGTH] = {0};
		for (unsigned int i = 0; i < epc; i++) {
			programs[i].pid = KGlobals.get_next_pid();
			programs[i].status_code = PROGRAM_COMPILING;
			programs[i].start_time = 0;
			programs[i].end_time = 0;

			while (true) {
#ifdef BOARD_ESP32
				vTaskDelay(1);
#endif
				memset(buffer, 0, MAX_LINE_LENGTH);
				int read = eeprom_read_line(buffer);
				kprint("eeprom: ");
				kprint(buffer);
				kprint("\n");
				if (read == 0) {
					programs[i].status_code = PROGRAM_RUNNING;
					break;
				}
				if (read == -1) {
					programs[i].pid = 0;
					programs[i].status_code = PROGRAM_FREE;
					break;
				}
				programs[i].compile(buffer);
			}
		}
	}
	return 0;
}
