#include "dos.hpp"
#include "kprint.hpp"

#ifdef WITH_SDCARD
#include <SD.h>
#include <SPI.h>
char active_directory[MAX_PATH_SIZE] = {0};
#endif

bool check_mount() {
	if (strlen(active_directory) == 0) {
		kprint("SDCard is not mounted\n");
		kprint("  did you forget to 'mount' sd card?\n");
		return false;
	}
	return true;
}

int dos_check(const char *io_buffer) {
#ifdef WITH_SDCARD
	if (strcmp(io_buffer, "mount") == 0) {
		bool res = SD.begin();
		if (!res) {
			kprint("Failed to mound sdcard\n");
		} else {
			strcat(active_directory, "/");
			kprint("SDCard mounted\n");
		}
		SD.end();
		return 0;
	}

	if (strncmp(io_buffer, "rm ", 3) == 0) {
		if (!check_mount()) {
			return 0;
		}
		SD.begin();
		char new_path[MAX_PATH_SIZE] = {0};
		char path_name[32] = {0};
		strcpy(path_name, io_buffer + 3);
		bool check = SD.remove(path_name);
		if (!check) {
			kprint("can not remove [");
			kprint(path_name);
			kprint("]\n");
		}
		SD.end();
	}
	if (strncmp(io_buffer, "rmdir ", 6) == 0) {
		if (!check_mount()) {
			return 0;
		}
		SD.begin();
		char new_path[MAX_PATH_SIZE] = {0};
		char path_name[32] = {0};
		strcpy(path_name, io_buffer + 3);
		bool check = SD.rmdir(path_name);
		if (!check) {
			kprint("can not remove [");
			kprint(path_name);
			kprint("]\n");
		}
		SD.end();
	}
	if (strcmp(io_buffer, "mkdir") == 0) {
		// TODO
	}
	if (strcmp(io_buffer, "ls") == 0) {
		if (!check_mount()) {
			return 0;
		}
		SD.begin();
		File d = SD.open(active_directory);
		if (!d) {
			kprint("Directory ");
			kprint(active_directory);
			kprint(" does not exists\nDropping back to root /");
			memset(active_directory, 0, MAX_PATH_SIZE);
			strcat(active_directory, "/");
		} else {
			if (!d.isDirectory()) {
				kprint("Path ");
				kprint(active_directory);
				kprint(" is not a directory\nDropping back to root /");
				memset(active_directory, 0, MAX_PATH_SIZE);
				strcat(active_directory, "/");
			} else {
				int count = 0;
				File f = d.openNextFile();
				while (f) {
					count++;
					if (f.isDirectory()) {
						kprint("[");
						kprint(f.name());
						kprint("]\n");
					} else {
						char size_bytes[16] = {0};
						itoa(f.size(), size_bytes, DEC);
						kprint(f.name());
						kprint(":");
						kprint(size_bytes);
						kprint(" bytes\n");
					}
					f.close();
					f = d.openNextFile();
				}
			}
		}
		SD.end();
	}
	if (strncmp(io_buffer, "cd ", 3) == 0) {
		if (!check_mount()) {
			return 0;
		}
		SD.begin();
		char new_path[MAX_PATH_SIZE] = {0};
		char path_name[32] = {0};
		strcpy(path_name, io_buffer + 3);
		if (path_name[0] == '/') {
			memset(new_path, 0, MAX_PATH_SIZE);
			strcat(new_path, path_name);

		} else {
			strcat(new_path, active_directory);

			if (active_directory[strlen(active_directory) - 1] != '/') {
				strcat(new_path, "/");
			}

			strcat(new_path, path_name);
		}
		File f = SD.open(new_path);
		if (!f) {
			kprint("Directory [");
			kprint(new_path);
			kprint("] does not exists");
		} else {
			if (!f.isDirectory()) {
				kprint("Path [");
				kprint(new_path);
				kprint("] is not a directory");
			} else {
				memset(active_directory, 0, MAX_PATH_SIZE);
				strcat(active_directory, new_path);
			}
			f.close();
		}
		SD.end();
	}
	if (strncmp(io_buffer, "cat ", 4) == 0) {
		if (!check_mount()) {
			return 0;
		}
		SD.begin();
		char new_path[MAX_PATH_SIZE] = {0};
		char path_name[32] = {0};
		strcpy(path_name, io_buffer + 4);
		if (path_name[0] == '/') {
			memset(new_path, 0, MAX_PATH_SIZE);
			strcat(new_path, path_name);
		} else {
			strcat(new_path, active_directory);

			if (active_directory[strlen(active_directory) - 1] != '/') {
				strcat(new_path, "/");
			}

			strcat(new_path, path_name);
		}
		File f = SD.open(new_path);
		if (!f) {
			kprint("File [");
			kprint(new_path);
			kprint("] does not exists");
		} else {
			char tmp[17] = {0};
			while (1) {
				size_t b = f.readBytes(tmp, 16);
				if (b == 0) {
					break;
				}
				kprint(tmp);
				memset(tmp, 0, 17);
			}
			kprint("\n");
			f.close();
		}
	}
#endif
	return 0;
}
