#include "source.hpp"
#include "program.hpp"
#include "status.hpp"
#include "tasks.hpp"
#include <EEPROM.h>

extern int serial_lock;
extern int kernel_next_pid;
bool eeprom;

SourceEngine::SourceEngine() {

#ifdef BOARD_ESP32
	EEPROM.begin(4096);
#endif
	char eeprom_defined = EEPROM.read(0);
	Serial.print("EEPROM Flag: ");
	Serial.println(eeprom_defined);
	if (eeprom_defined == EEPROM_DEFINED) {
		Serial.println("Source code found in EEPROM");
		unsigned int code_size_d = 0;
		for (unsigned int i = 0; i < EEPROM.length(); i++) {
			if (EEPROM.read(i) == 0) {
				code_size_d = i;
				break;
			}
		}

		Serial.print("  source code size: ");
		Serial.print(code_size_d);
		Serial.println(" bytes");

		this->source = (char *)malloc(code_size_d + 1);
		memset(this->source, 0, code_size_d + 1);

		for (unsigned int i = 0; i < code_size_d; i++) {
			this->source[i] = EEPROM.read(i + 1);
		}

		Serial.println(this->source);
		this->create_task();
	}
#ifdef BOARD_ESP32
	EEPROM.end();
#endif

	this->serial_open = false;
	this->boud_rate = 9600;
	this->buffer_cursor = 0;
	this->buffer = (char *)malloc(BUFFER_SIZE);
	memset(this->buffer, 0, BUFFER_SIZE);
	this->source = (char *)malloc(1);
	memset(this->source, 0, 1);
}

void SourceEngine::create_task() {
	program *prog = new program(kernel_next_pid++);
	prog->source = this->source;
	prog->compile();
	add_task(prog, 1);
}

void SourceEngine::append_to_source() {
	Serial.println(this->buffer);
	if (strcmp(this->buffer, ".") == 0) {
		this->create_task();

		this->buffer_cursor = 0;
		memset(this->buffer, 0, BUFFER_SIZE);

		this->source = (char *)malloc(1);
		memset(this->source, 0, 1);

		Serial.println(MSG_READY_TO_RECEIVE);
		serial_lock = NO_LOCK;
		return;
	}
	if (strcmp(this->buffer, "eeprom") == 0) {
#ifdef BOARD_ESP32
		EEPROM.begin(EEPROM.length());
#endif
		unsigned int scl = strlen(this->source);
		if (scl >= EEPROM.length()) {
			Serial.println("Source code is too big to store in EEPROM");
			return;
		}
		for (unsigned int i = 0; i < EEPROM.length(); i++) {
			EEPROM.write(i, 0);
		}

		EEPROM.write(0, EEPROM_DEFINED);
		for (unsigned int i = 0; i < scl; i++) {
			EEPROM.write(i + 1, this->source[i]);
		}
		Serial.print("Wrote ");
		Serial.print(scl);
		Serial.println(" bytes to EEPROM");
#ifdef BOARD_ESP32
		EEPROM.end();
#endif
		return;
	}
	if (strcmp(this->buffer, "#source-code") == 0) {
		this->buffer_cursor = 0;
		memset(this->buffer, 0, BUFFER_SIZE);
		Serial.println(MSG_READY_TO_RECEIVE);
		return;
	}
	if (strlen(this->buffer) < 2) {
		this->buffer_cursor = 0;
		memset(this->buffer, 0, BUFFER_SIZE);
		return;
	}
	int new_size = strlen(this->source) + strlen(this->buffer) + 2;
	char *temp = (char *)malloc(new_size);
	memset(temp, 0, new_size);
	strcpy(temp, this->source);
	strcat(temp, this->buffer);
	strcat(temp, "\n");

	free(this->source);
	this->source = temp;

	memset(this->buffer, 0, BUFFER_SIZE);
	this->buffer_cursor = 0;
}

int SourceEngine::process() {
#ifdef BOARD_ESP32
	if (Serial.available() && !this->serial_open) {
		this->serial_open = true;
		Serial.println(MSG_READY_TO_RECEIVE);
	}
#endif
#ifdef BOARD_ATMEGA
	if (Serial && !this->serial_open) {
		this->serial_open = true;
		Serial.println(MSG_READY_TO_RECEIVE);
	}
#endif
	if (!this->serial_open)
		return 1;
	if (serial_lock > KERNEL_LOCK)
		return 1;
	if (!Serial.available())
		return 1;
	if (serial_lock == NO_LOCK) {
		serial_lock = KERNEL_LOCK;
	}

	memset(this->buffer, 0, BUFFER_SIZE);
	Serial.readBytesUntil('\n', this->buffer, BUFFER_SIZE);
	append_to_source();
	serial_lock = -1;
	return 1;
}
