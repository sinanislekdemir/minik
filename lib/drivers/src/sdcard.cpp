#include "sdcard.hpp"

#ifdef WITH_SDCARD

#include <SD.h>
#include <SPI.h>

uint8_t ss_pin = 0;

int sdcard(program *_p) {
	unsigned int pid = _p->pid;
	int cmd_var = find_number("SD_CMD", pid);
	if (cmd_var == 0) {
		error_msg("SD_CMD not defined", pid);
		return -1;
	}
	if (cmd_var == 1) {
		int _ss_pin = find_number("SD_SS_PIN", pid);
		if (_ss_pin == 0) {
			ss_pin = 5; // based on ESP32 defaults
		} else {
			ss_pin = uint8_t(_ss_pin);
		}
		if (!SD.begin(ss_pin)) {
			new_number((char *)"SD_MOUNT", 0, pid);
		} else {
			new_number((char *)"SD_MOUNT", 1, pid);
#ifdef BOARD_ESP32
			uint8_t card_type = SD.cardType();
			double card_size = double(SD.cardSize() / (1024 * 1024));
			new_number((char *)"SD_CARDTYPE", double(card_type), pid);
			new_number((char *)"SD_CARDSIZE", card_size, pid);
#endif
		}
		SD.end();
		return 0;
	}
	if (ss_pin > 0) {
		SD.begin(ss_pin);
	}
	if (cmd_var == 2) {
		char *dir = find_string("SD_DIR", pid);
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
		new_number((char *)"SD_FILECOUNT", double(count), pid);
		return 0;
	}
	if (cmd_var == 3) {
		char *dir = find_string("SD_DIR", pid);
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
		int findex = find_number("SD_INDEX", pid);
		File file = d.openNextFile();
		while (file) {
			if (index == findex) {
				const char *fname = file.name();
				new_string((char *)"SD_FILENAME", (char *)fname, pid);
				new_number((char *)"SD_FILESIZE", double(file.size()), pid);
				if (file.isDirectory()) {
					new_number((char *)"SD_ISDIR", 1, pid);
				} else {
					new_number((char *)"SD_ISDIR", 0, pid);
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
		char *dir = find_string("SD_DIR", pid);
		if (SD.mkdir(dir)) {
			new_number((char *)"SD_SUCCESS", 1, pid);
		} else {
			new_number((char *)"SD_SUCCESS", 0, pid);
		}
		SD.end();
		return 0;
	}
	if (cmd_var == 5) {
		char *dir = find_string("SD_DIR", pid);
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
			new_number((char *)"SD_SUCCESS", 1, pid);
		} else {
			new_number((char *)"SD_SUCCESS", 0, pid);
		}
		SD.end();
		return 0;
	}
#ifdef BOARD_ESP32
	if (cmd_var == 6) {
		char *old_name = find_string("SD_FILENAME", pid);
		char *new_name = find_string("SD_FILENAME_NEW", pid);
		if (old_name == NULL) {
			error_msg("SD_FILENAME is not defined", pid);
			SD.end();
			return -1;
		}
		if (new_name == NULL) {
			error_msg("SD_FILENAME_NEW is not defined", pid);
			SD.end();
			return -1;
		}
		if (SD.rename(old_name, new_name)) {
			new_number((char *)"SD_SUCCESS", 1, pid);
		} else {
			new_number((char *)"SD_SUCCESS", 0, pid);
		}
		SD.end();
		return 0;
	}
#endif
	if (cmd_var == 7) {
		char *filename = find_string("SD_FILENAME", pid);
		if (filename == NULL) {
			error_msg("SD_FILENAME is not defined", pid);
			SD.end();
			return -1;
		}
		if (SD.remove(filename)) {
			new_number((char *)"SD_SUCCESS", 1, pid);
		} else {
			new_number((char *)"SD_SUCCESS", 0, pid);
		}
		SD.end();
		return 0;
	}
	if (cmd_var == 8) {
		char *filename = find_string("SD_FILENAME", pid);
		int seek = find_number("SD_SEEK", pid);
		int size = find_number("SD_SIZE", pid);
		if (filename == NULL) {
			error_msg("SD_FILENAME is not defined", pid);
			SD.end();
			return -1;
		}
		File file = SD.open(filename);
		if (!file) {
			error_msg("SD_FILENAME could not be opened", pid);
			SD.end();
			return -1;
		}
		char *buffer = (char *)malloc(size + 1);
		memset(buffer, 0, size + 1);
		file.seek(uint32_t(seek));
		file.readBytes(buffer, size);
		new_string((char *)"SD_BUFFER", buffer, pid);
		free(buffer);
		file.close();
		SD.end();
		return 0;
	}
	if (cmd_var == 9) {
		char *filename = find_string("SD_FILENAME", pid);
		char *buffer = find_string("SD_BUFFER", pid);
		if (filename == NULL) {
			error_msg("SD_FILENAME is not defined", pid);
			SD.end();
			return -1;
		}
		if (buffer == NULL) {
			error_msg("SD_BUFFER is not defined", pid);
			SD.end();
			return -1;
		}
		File file = SD.open(filename, FILE_WRITE);
		if (!file) {
			error_msg("SD_FILENAME could not be opened", pid);
			SD.end();
			return -1;
		}
		if (file.print(buffer)) {
			new_number((char *)"SD_SUCCESS", 1, pid);
		} else {
			new_number((char *)"SD_SUCCESS", 0, pid);
		}
		file.close();
		SD.end();
		return 0;
	}
	if (cmd_var == 10) {
		char *filename = find_string("SD_FILENAME", pid);
		char *buffer = find_string("SD_BUFFER", pid);
		if (filename == NULL) {
			error_msg("SD_FILENAME is not defined", pid);
			SD.end();
			return -1;
		}
		if (buffer == NULL) {
			error_msg("SD_BUFFER is not defined", pid);
			SD.end();
			return -1;
		}
		File file = SD.open(filename, FILE_APPEND);
		if (!file) {
			error_msg("SD_FILENAME could not be opened", pid);
			SD.end();
			return -1;
		}
		if (file.print(buffer)) {
			new_number((char *)"SD_SUCCESS", 1, pid);
		} else {
			new_number((char *)"SD_SUCCESS", 0, pid);
		}
		file.close();
		SD.end();
		return 0;
	}
	if (cmd_var == 11) {
		char *filename = find_string("SD_FILENAME", pid);
		File file = SD.open(filename);
		new_number((char *)"SD_SIZE", double(file.size()), pid);
		file.close();
		SD.end();
		return 0;
	}
	return 0;
}

#endif
