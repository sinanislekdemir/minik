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

svariable string_variables[MAX_STR] = {0};
dvariable double_variables[MAX_NUM] = {0};

void update_number(short index, double value) {
	if (index >= MAX_NUM) {
		return;
	}
	double_variables[index].val = value;
}

void update_string(short index, char *value) {
	if (index > MAX_STR) {
		return;
	}
	memset(string_variables[index].data, 0, MAX_LINE_LENGTH);
	strcat(string_variables[index].data, value);
}

double get_d(short index) {
	if (index >= MAX_NUM) {
		return 0;
	}
	return double_variables[index].val;
}

void get_s(short index, char *back) {
	memset(back, 0, MAX_LINE_LENGTH);
	if (index >= MAX_STR) {
		return;
	}
	memcpy(back, string_variables[index].data, MAX_LINE_LENGTH);
}

int init_mem() {
	for (unsigned short i = 0; i < MAX_NUM; i++) {
		double_variables[i].free = true;
	}
	for (unsigned short i = 0; i < MAX_STR; i++) {
		string_variables[i].free = true;
	}
}

void free_program(char pid) {
	if (pid == 0) {
		// 0 is reserved pid;
		return;
	}
	for (unsigned short i = 0; i < MAX_STR; i++) {
		if (string_variables[i].pid == pid) {
			string_variables[i].free = true;
			memset(string_variables[i].data, 0, MAX_LINE_LENGTH);
			string_variables[i].pid = 0;
		}
	}

	for (unsigned short i = 0; i < MAX_NUM; i++) {
		if (double_variables[i].pid == pid) {
			double_variables[i].free = true;
			double_variables[i].pid = 0;
			double_variables[i].val = 0;
		}
	}
}

void mem_dump() {
	Serial.println("String allocation:");
	for (unsigned short i = 0; i < MAX_STR; i++) {
		if (string_variables[i].free) {
			Serial.print(0);
		} else {
			Serial.print(1);
		}
		if (i % 40 == 0) {
			Serial.print('\n');
		}
	}
	Serial.println("\nDouble allocation:");
	for (unsigned short i = 0; i < MAX_NUM; i++) {
		if (double_variables[i].free) {
			Serial.print(0);
		} else {
			Serial.print(1);
		}
		if (i % 40 == 0) {
			Serial.print('\n');
		}
	}
}

void free_variable(short index, char pid, char type) {
	if (type == TYPE_NUM) {
		double_variables[index].free = true;
		double_variables[index].pid = 0;
		double_variables[index].val = 0;
		return;
	}
	if (type == TYPE_STR) {
		string_variables[index].free = true;
		string_variables[index].pid = 0;
		memset(string_variables[index].data, 0, MAX_LINE_LENGTH);
	}
}

void new_variable(svariable v) {
	/**
	   Add the variable to the linked list.
	 */
	for (unsigned short i = 0; i < MAX_STR; i++) {
		if (string_variables[i].free) {
			memcpy(string_variables[i].data, v.data, MAX_LINE_LENGTH);
			string_variables[i].free = false;
			string_variables[i].pid = v.pid;
			return;
		}
	}
}

int new_number(double value, char pid) {
	for (unsigned short i = 0; i < MAX_NUM; i++) {
		if (double_variables[i].free) {
			double_variables[i].pid = pid;
			double_variables[i].free = false;
			double_variables[i].val = value;
			return i;
		}
	}
	return -1;
}

int new_string(char *value, char pid) {
	for (unsigned short i = 0; i < MAX_STR; i++) {
		if (string_variables[i].free) {
			memset(string_variables[i].data, 0, MAX_LINE_LENGTH);
			strcat(string_variables[i].data, value);
			string_variables[i].pid = pid;
			string_variables[i].free = false;
			return i;
		}
	}
	return -1;
}

void error_msg(const char *msg, char pid) { new_string((char *)msg, pid); }
