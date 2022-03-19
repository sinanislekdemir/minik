#include "term_mem.hpp"
#include "eeprom.hpp"
#include "kprint.hpp"
#include "memory.hpp"
#include "status.hpp"

int term_mem(const char *io_buffer) {
	if (strcmp(io_buffer, "memdump") == 0) {
		char tmp[8] = {0};
		for (unsigned int i = 0; i < MAX_MEM; i++) {
			if (i % 16 == 0 && i > 0) {
				kprint("\n");
			}
			char c = read_area_char(i);
			itoa(c, tmp, HEX);
			if (strlen(tmp) > 2) {
				memset(tmp + 2, 0, 6);
			}
			if (strlen(tmp) == 1) {
				kprint(" ");
			}

			kprint(tmp);
			kprint(" ");

			memset(tmp, 0, 8);
		}
		kprint("\n");
	}
	if (strcmp(io_buffer, "free") == 0) {
		char tmp[16] = {0};
		int ram = free_ram();
		itoa(ram, tmp, 10);
		kprint("Free ram size: ");
		kprint(tmp);
		kprint("\n");
	}
	if (strcmp(io_buffer, "clear eeprom") == 0) {
		kprint("Cleaning eeprom: ");
		eeprom_clean();
		kprint("done\n");
	}
	if (strcmp(io_buffer, "size eeprom") == 0) {
		char buffer[MAX_LINE_LENGTH] = {0};
		kprint("EEPROM Size: ");
		itoa(eeprom_free_area(), buffer, DEC);
		kprint(buffer);
		memset(buffer, 0, MAX_LINE_LENGTH);
		kprint(" / ");
		itoa(EEPROM_SIZE, buffer, DEC);
		kprint(buffer);
		kprint("\n");
	}
	return 0;
}
