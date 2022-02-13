/**
   We are storing variables at global in a linked list.
   With linked list, we are avoiding memory swaps while
   extending arrays. Keep the memory requirements at minimum.
   Also all memory is shared and accessible.
 */
#include "memory.hpp"
#include "constants.hpp"
#include "helpers.hpp"
#include <Arduino.h>

char _memory_area[MAX_MEM] = {0};
_protected _protected_memory[64] = {0};
char _last_err[MAX_LINE_LENGTH] = {0};

int init_mem() {
	for (unsigned short i = 0; i < 64; i++) {
		_protected_memory[i].pid = -1;
	}
}

void free_program(char pid) {
	if (pid == 0) {
		// 0 is reserved pid;
		return;
	}
	for (unsigned short i = 0; i < 64; i++) {
		if (_protected_memory[i].pid == pid) {
			_protected_memory[i].pid = -1;
		}
	}
}

void free_area(unsigned int index, unsigned int size) { memset(_memory_area + index, 0, size); }

int write_area(unsigned int index, char *data) {
	memcpy(_memory_area + index, data, strlen(data));
	return 0;
}

int write_area(unsigned int index, int data) {
	memcpy(_memory_area + index, &data, sizeof(int));
	return 0;
}

int write_area(unsigned int index, double data) {
	memcpy(_memory_area + index, &data, sizeof(double));
	return 0;
}

int write_area(unsigned int index, long data) {
	memcpy(_memory_area + index, &data, sizeof(double));
	return 0;
}

int write_area(unsigned int index, char data) {
	_memory_area[index] = data;
	return 0;
}

int write_area(unsigned int index, char *data, unsigned int size) {
	memcpy(_memory_area + index, data, size);
	return 0;
}

int append_area(unsigned int index, char data) {
	for (unsigned int i = index; i < MAX_MEM; i++) {
		if (_memory_area[i] == 0) {
			_memory_area[i] = data;
			return 0;
		}
	}
	return -1;
}

int read_area_str(unsigned int index, unsigned int size, char *back) {
	if (size == 0) {
		unsigned int tmp = 0;
		for (unsigned int i = index; i < MAX_MEM; i++) {
			if (_memory_area[i] != 0) {
				tmp++;
			} else {
				break;
			}
		}
		size = tmp;
	}
	memcpy(back, _memory_area + index, size);
	return size;
}

double read_area_double(unsigned int index) {
	double result;
	memcpy(&result, _memory_area + index, sizeof(double));
	return result;
}

int read_area_int(unsigned int index) {
	int result;
	memcpy(&result, _memory_area + index, sizeof(int));
	return result;
}

long read_area_long(unsigned int index) {
	long result;
	memcpy(&result, _memory_area + index, sizeof(long));
	return result;
}

char read_area_char(unsigned int index) { return _memory_area[index]; }

void error_msg(const char *msg, char pid) {
	memset(_last_err, 0, MAX_LINE_LENGTH);
	strcat(_last_err, msg);
}
