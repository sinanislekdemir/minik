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

variable root_variable = {(char *)"", (char *)"", 0, 0, 0, false, NULL};
variable HIGHV = {(char *)"HIGH", dtoc(HIGH), sizeof(double), 0, TYPE_NUM, false, NULL};
variable LOWV = {(char *)"LOW", dtoc(LOW), sizeof(double), 0, TYPE_NUM, false, NULL};
variable RISINGV = {(char *)"RISING", dtoc(RISING), sizeof(double), 0, TYPE_NUM, false, NULL};
variable FALLINGV = {(char *)"FALLING", dtoc(FALLING), sizeof(double), 0, TYPE_NUM, false, NULL};
#ifdef BOARD_ESP32
variable ONLOWV = {(char *)"ONLOW", dtoc(ONLOW), sizeof(double), 0, TYPE_NUM, false, NULL};
variable ONHIGHV = {(char *)"ONHIGH", dtoc(ONHIGH), sizeof(double), 0, TYPE_NUM, false, NULL};
variable IPULLDOWN = {(char *)"INPUT_PULLDOWN", dtoc(INPUT_PULLDOWN), sizeof(double), 0, TYPE_NUM, false, NULL};
#endif
variable PIV = {(char *)"PI", dtoc(PI), sizeof(double), 0, TYPE_NUM, false, NULL};

variable INPUTV = {(char *)"INPUT", dtoc(INPUT), sizeof(double), 0, TYPE_NUM, false, NULL};
variable OUTPUTV = {(char *)"OUTPUT", dtoc(OUTPUT), sizeof(double), 0, TYPE_NUM, false, NULL};
variable IPULLUP = {(char *)"INPUT_PULLUP", dtoc(INPUT_PULLUP), sizeof(double), 0, TYPE_NUM, false, NULL};

variable HPIV = {(char *)"HALF_PI", dtoc(HALF_PI), sizeof(double), 0, TYPE_NUM, false, NULL};
variable TWOPIV = {(char *)"TWO_PI", dtoc(TWO_PI), sizeof(double), 0, TYPE_NUM, false, NULL};
variable DTR = {(char *)"DEG_TO_RAD", dtoc(DEG_TO_RAD), sizeof(double), 0, TYPE_NUM, false, NULL};
variable RTD = {(char *)"RAD_TO_DEG", dtoc(RAD_TO_DEG), sizeof(double), 0, TYPE_NUM, false, NULL};
variable EULERV = {(char *)"EULER", dtoc(EULER), sizeof(double), 0, TYPE_NUM, false, NULL};

variable *find_variable(const char *name, unsigned int pid) {
	variable *node = &root_variable;
	if (strcmp(name, "HIGH") == 0) {
		return &HIGHV;
	}
	if (strcmp(name, "LOW") == 0) {
		return &LOWV;
	}
	if (strcmp(name, "RISING") == 0) {
		return &RISINGV;
	}
	if (strcmp(name, "FALLING") == 0) {
		return &FALLINGV;
	}
	if (strcmp(name, "INPUT") == 0) {
		return &INPUTV;
	}
	if (strcmp(name, "OUTPUT") == 0) {
		return &OUTPUTV;
	}
	if (strcmp(name, "INPUT_PULLUP") == 0) {
		return &IPULLUP;
	}
#ifdef BOARD_ESP32
	if (strcmp(name, "INPUT_PULLDOWN") == 0) {
		return &IPULLDOWN;
	}
	if (strcmp(name, "ONLOW") == 0) {
		return &ONLOWV;
	}
	if (strcmp(name, "ONHIGH") == 0) {
		return &ONHIGHV;
	}
#endif
	if (strcmp(name, "PI") == 0) {
		return &PIV;
	}
	if (strcmp(name, "HALF_PI") == 0) {
		return &HPIV;
	}
	if (strcmp(name, "TWO_PI") == 0) {
		return &TWOPIV;
	}
	if (strcmp(name, "DEG_TO_RAD") == 0) {
		return &DTR;
	}
	if (strcmp(name, "RAD_TO_DEG") == 0) {
		return &RTD;
	}
	if (strcmp(name, "EULER") == 0) {
		return &EULERV;
	}

	while (true) {
		if (node->name != NULL && strcmp(node->name, name) == 0 && pid == node->pid && node->deleted == false) {
			if (node->type == TYPE_VARIABLE && node->data != NULL) {
				return find_variable(node->data, pid);
			}
			return node;
		}
		node = node->next;
		if (node == NULL) {
			break;
		}
	}

	return NULL;
}

void free_program(unsigned int pid) {
	if (pid == 0) {
		// 0 is reserved pid;
		return;
	}

	variable *head = &root_variable;
	variable *tmp;
	while (head != NULL) {
		tmp = head;
		head = head->next;
		if (tmp->pid == pid) {
			if (tmp->name != NULL)
				free(tmp->name);
			if (tmp->data != NULL)
				free(tmp->data);
			free(tmp);
		}
	}
}

void mem_dump() {
	variable *node = &root_variable;
	while (node != NULL) {
		Serial.print("N: ");
		Serial.print(node->name);
		Serial.print(" T: ");
		Serial.print(node->type);
		Serial.print(" S: ");
		Serial.print(node->datasize);
		Serial.print("bytes PID: ");
		Serial.println(node->pid);
		node = node->next;
	}
}

void free_variable(const char *name, unsigned int pid) {
	/**
	   Free variables allocated data and name,
	   and mark the entry as deleted for future use.
	   But this doesn't actually remove the item totally from the linked
	   list. You need some sort of defrag.
	 */
	variable *node = &root_variable;
	while (true) {
		if (node->name != NULL && strcmp(node->name, name) == 0 && pid == node->pid) {
			free(node->data);
			free(node->name);
			node->datasize = 0;
			node->pid = 0;
			node->deleted = true;
			break;
		}
		node = node->next;
		if (node == NULL) {
			break;
		}
	}
}

void new_variable(variable *v) {
	/**
	   Add the variable to the linked list.
	 */
	variable *node = &root_variable;
	v->deleted = false;
	while (true) {
		if (node->next == NULL) {
			node->next = v;
			break;
		}
		node = node->next;
		if (node == NULL) {
			// TODO: Raise errro
			break;
		}
	}
}

void new_number(char *name, double value, unsigned int pid) {
	variable *res = find_variable(name, pid);
	if (res == NULL) {
		res = (variable *)malloc(sizeof(variable));
		res->next = NULL;
		res->data = dtoc(value);
		res->type = TYPE_NUM;
		res->datasize = sizeof(double);
		res->pid = pid;
		res->deleted = false;
		res->name = (char *)malloc(strlen(name) + 1);
		memset(res->name, 0, strlen(name) + 1);
		strcpy(res->name, name);
		new_variable(res);
		return;
	}
	if (res->data != NULL) {
		free(res->data);
	}
	res->data = dtoc(value);
	res->type = TYPE_NUM;
	res->datasize = sizeof(double);
	res->deleted = false;
}

void new_string(char *name, char *value, int size, unsigned int pid) {
	variable *res = find_variable(name, pid);
	if (res == NULL) {
		res = (variable *)malloc(sizeof(variable));
		res->next = NULL;
		res->data = (char *)malloc(size);
		memset(res->data, 0, size);
		memcpy(res->data, (const char *)value, size);
		res->type = TYPE_STR;
		res->datasize = size;
		res->pid = pid;
		res->deleted = false;
		res->name = (char *)malloc(strlen(name) + 1);
		memset(res->name, 0, strlen(name) + 1);
		strcpy(res->name, name);
		new_variable(res);
		return;
	}
	if (res->data != NULL) {
		free(res->data);
	}
	res->data = (char *)malloc(size);
	memset(res->data, 0, size);
	memcpy(res->data, (const char *)value, size);
	res->datasize = size;
	res->deleted = false;
}

void error_msg(const char *msg, unsigned int pid) { new_string((char *)"ERR_EXCEPTION", (char *)msg, strlen(msg), pid); }

void defrag_variables() {}
