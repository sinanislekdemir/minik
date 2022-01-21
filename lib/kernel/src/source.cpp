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
	delay(5000);
	char eeprom_defined = EEPROM.read(0);
	Serial.print("EEPROM Flag: ");
	Serial.println(eeprom_defined);
	if (eeprom_defined == EEPROM_DEFINED) {
		Serial.println("Source code found in EEPROM");
		char *size = (char *)malloc(4);
		size[0] = EEPROM.read(1);
		size[1] = EEPROM.read(2);
		size[2] = EEPROM.read(3);
		size[3] = EEPROM.read(4);
		int code_size_d = 0;
		memcpy(&code_size_d, size, 4);
		free(size);

		this->source = (char *)malloc(code_size_d + 1);
		memset(this->source, 0, code_size_d + 1);

		for (unsigned int i = 0; i < code_size_d; i++) {
			this->source[i] = EEPROM.read(i + 5);
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
		Serial.println("Store source code in eeprom");
#ifdef BOARD_ESP32
		Serial.print("Eeprom size: ");
		Serial.println(4096);
		EEPROM.begin(4096);
#endif
		for (unsigned int i = 0; i < 4096; i++) {
			EEPROM.write(i, 0);
		}
		int scl = strlen(this->source);
		EEPROM.write(0, EEPROM_DEFINED);
		char *size = (char *)malloc(4);
		memcpy(size, &scl + 1, 4);
		EEPROM.write(1, size[0]);
		EEPROM.write(2, size[1]);
		EEPROM.write(3, size[2]);
		EEPROM.write(4, size[3]);
		for (unsigned int i = 0; i < scl; i++) {
			EEPROM.write(i + 5, this->source[i]);
		}
		Serial.print("Wrote ");
		Serial.print(scl);
		Serial.println(" bytes to EEPROM");
#ifdef BOARD_ESP32
		EEPROM.end();
#endif
		return;
	}
	if (strcmp(this->buffer, "#ignore") == 0) {
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
