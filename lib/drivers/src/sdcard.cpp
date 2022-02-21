#include "sdcard.hpp"

#ifdef WITH_SDCARD

#include "tasks.hpp"
#include <SD.h>
#include <SPI.h>

uint8_t ss_pin = 0;
extern int kernel_next_pid;

int sdcard(program *_p) {
	unsigned int pid = _p->pid;
	int cmd_var = int(read_area_double(SDCARD_ADDRESS));
	if (cmd_var == 0) {
		error_msg("SD_CMD not defined", pid);
		return -1;
	}
	if (cmd_var == 1) {
		int _ss_pin = int(read_area_double(SDCARD_DATA_ADDRESS));
		if (_ss_pin == 0) {
			ss_pin = 5; // based on ESP32 defaults
		} else {
			ss_pin = uint8_t(_ss_pin);
		}
		if (!SD.begin(ss_pin)) {
			write_area(SDCARD_DATA_ADDRESS, double(0));
		} else {
			write_area(SDCARD_DATA_ADDRESS, double(1));
#ifdef BOARD_ESP32
			uint8_t card_type = SD.cardType();
			double card_size = double(SD.cardSize() / (1024 * 1024));
			write_area(SDCARD_DATA_ADDRESS + 10, double(card_type));
			write_area(SDCARD_DATA_ADDRESS + 20, card_size);
#endif
		}
		SD.end();
		return 0;
	}
	if (ss_pin > 0) {
		SD.begin(ss_pin);
	}
	if (cmd_var == 2) {
		char dir[MAX_LINE_LENGTH] = {0};
		read_area_str(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH, dir);
		File d = SD.open(dir);
		if (!d) {
			error_msg("SD_DIR does not exist", pid);
			SD.end();
			return -1;
		}
		if (!d.isDirectory()) {
			d.close();
			SD.end();
			error_msg("SD_DIR is not a directory", pid);
			return -1;
		}
		int count = 0;
		File file = d.openNextFile();
		while (file) {
			count++;
			file.close();
			file = d.openNextFile();
		}
		d.close();
		file.close();
		SD.end();
		free_area(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH + 1);
		write_area(SDCARD_DATA_ADDRESS, double(count));
		return 0;
	}
	if (cmd_var == 3) {
		char dir[MAX_LINE_LENGTH] = {0};
		read_area_str(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH, dir);
		File d = SD.open(dir);
		if (!d) {
			error_msg("SD_DIR does not exist", pid);
			SD.end();
			return -1;
		}
		if (!d.isDirectory()) {
			d.close();
			error_msg("SD_DIR is not a directory", pid);
			SD.end();
			return -1;
		}
		int index = 0;
		int findex = int(read_area_double(SDCARD_DATA_ADDRESS + 132));
		File file = d.openNextFile();
		free_area(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH);
		while (file) {
			if (index == findex) {
				const char *fname = file.name();
				write_area(SDCARD_DATA_ADDRESS, (char *)fname, MAX_LINE_LENGTH);
				write_area(SDCARD_DATA_ADDRESS + 132, double(file.size()));
				if (file.isDirectory()) {
					write_area(SDCARD_DATA_ADDRESS + 136, double(1));
				} else {
					write_area(SDCARD_DATA_ADDRESS + 136, double(0));
				}
				file.close();
				d.close();
				SD.end();
				return 0;
			}
			index++;
			file.close();
			file = d.openNextFile();
		}
		file.close();
		d.close();
		SD.end();
		error_msg("SD_INDEX does not exist", pid);
		return -1;
	}
	if (cmd_var == 4) {
		char dir[MAX_LINE_LENGTH] = {0};
		read_area_str(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH, dir);
		free_area(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH);
		if (SD.mkdir(dir)) {
			write_area(SDCARD_DATA_ADDRESS, double(1));
		} else {
			write_area(SDCARD_DATA_ADDRESS, double(0));
		}
		SD.end();
		return 0;
	}
	if (cmd_var == 5) {
		char dir[MAX_LINE_LENGTH] = {0};
		read_area_str(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH, dir);
		File d = SD.open(dir);
		if (!d) {
			error_msg("SD_DIR does not exist", pid);
			SD.end();
			return -1;
		}
		if (!d.isDirectory()) {
			d.close();
			error_msg("SD_DIR is not a directory", pid);
			SD.end();
			return -1;
		}
		if (SD.rmdir(dir)) {
			write_area(SDCARD_DATA_ADDRESS, double(1));
		} else {
			write_area(SDCARD_DATA_ADDRESS, double(0));
		}
		SD.end();
		return 0;
	}
#ifdef BOARD_ESP32
	if (cmd_var == 6) {
		char old_name[MAX_LINE_LENGTH] = {0};
		char new_name[MAX_LINE_LENGTH] = {0};
		read_area_str(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH, old_name);
		read_area_str(SDCARD_DATA_ADDRESS + 132, MAX_LINE_LENGTH, new_name);
		free_area(SDCARD_DATA_ADDRESS, 132 + MAX_LINE_LENGTH);
		if (SD.rename(old_name, new_name)) {
			write_area(SDCARD_DATA_ADDRESS, double(1));
		} else {
			write_area(SDCARD_DATA_ADDRESS, double(0));
		}
		SD.end();
		return 0;
	}
#endif
	if (cmd_var == 7) {
		char filename[MAX_LINE_LENGTH] = {0};
		read_area_str(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH, filename);
		if (SD.remove(filename)) {
			write_area(SDCARD_DATA_ADDRESS, double(1));
		} else {
			write_area(SDCARD_DATA_ADDRESS, double(0));
		}
		SD.end();
		return 0;
	}
	if (cmd_var == 8) {
		char filename[MAX_LINE_LENGTH] = {0};
		read_area_str(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH, filename);
		int seek = int(read_area_double(SDCARD_DATA_ADDRESS + 132));
		int size = int(read_area_double(SDCARD_DATA_ADDRESS + 136));
		File file = SD.open(filename);
		if (!file) {
			error_msg("SD_FILENAME could not be opened", pid);
			SD.end();
			return -1;
		}
		if (size > MAX_LINE_LENGTH) {
			size = MAX_LINE_LENGTH;
		}
		char buffer[MAX_LINE_LENGTH] = {0};
		file.seek(uint32_t(seek));
		file.readBytes(buffer, size);
		free_area(SDCARD_DATA_ADDRESS, 140);
		write_area(SDCARD_DATA_ADDRESS, buffer, MAX_LINE_LENGTH);
		file.close();
		SD.end();
		return 0;
	}
	if (cmd_var == 9) {
		char filename[MAX_LINE_LENGTH] = {0};
		read_area_str(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH, filename);
		char buffer[MAX_LINE_LENGTH] = {0};
		read_area_str(SDCARD_DATA_ADDRESS + 132, MAX_LINE_LENGTH, buffer);
		File file = SD.open(filename, FILE_WRITE);
		if (!file) {
			error_msg("SD_FILENAME could not be opened", pid);
			SD.end();
			return -1;
		}
		free_area(SDCARD_DATA_ADDRESS, 132 + MAX_LINE_LENGTH);
		if (file.print(buffer)) {
			write_area(SDCARD_DATA_ADDRESS, double(1));
		} else {
			write_area(SDCARD_DATA_ADDRESS, double(0));
		}
		file.close();
		SD.end();
		return 0;
	}
	if (cmd_var == 10) {
		char filename[MAX_LINE_LENGTH] = {0};
		read_area_str(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH, filename);
		char buffer[MAX_LINE_LENGTH] = {0};
		read_area_str(SDCARD_DATA_ADDRESS + 132, MAX_LINE_LENGTH, buffer);
		File file = SD.open(filename, FILE_APPEND);
		if (!file) {
			error_msg("SD_FILENAME could not be opened", pid);
			SD.end();
			return -1;
		}
		free_area(SDCARD_DATA_ADDRESS, 132 + MAX_LINE_LENGTH);
		if (file.print(buffer)) {
			write_area(SDCARD_DATA_ADDRESS, double(1));
		} else {
			write_area(SDCARD_DATA_ADDRESS, double(0));
		}
		file.close();
		SD.end();
		return 0;
	}
	if (cmd_var == 11) {
		char filename[MAX_LINE_LENGTH] = {0};
		read_area_str(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH, filename);
		File file = SD.open(filename);
		free_area(SDCARD_DATA_ADDRESS, MAX_LINE_LENGTH);
		write_area(SDCARD_DATA_ADDRESS, double(file.size()));
		file.close();
		SD.end();
		return 0;
	}
	if (cmd_var == 12) {
		// TODO: Create program load here;
		return 0;
	}
	return 0;
}

#endif
